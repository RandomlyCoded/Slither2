#include "neuralnet.h"

#include <QFile>
#include <QRandomGenerator>

NeuralNet::NeuralNet(int i, int o, int l, int c)
{
    Q_UNUSED(i)
    Q_UNUSED(o)
    Q_UNUSED(l)
    Q_UNUSED(c)

    for(int i = 0; i < 10; ++i) {
        QGenericMatrix<10, 1, qreal> w;

        for(int j = 0; j < 10; ++j) {
            qreal v = QRandomGenerator::global()->generateDouble() * 4 - 1;

            w(0, j) = v;
        }

        qInfo() << w;

        actualWeights.append(w);
    }

    qInfo();

    compileWeights();
}

void NeuralNet::mutate(qreal mr)
{
    int layer = QRandomGenerator::global()->bounded(actualWeights.length());
    int idx = QRandomGenerator::global()->bounded(10);

    qInfo() << "mutating" << layer << idx;

     qreal off = actualWeights[layer](0, idx) + QRandomGenerator::global()->generateDouble() * mr - 0.5 * mr;

     if(off < -2)
         off = -2;
     if(off > 2)
         off = 2;

     actualWeights[layer](0, idx) = off;

    compileWeights();
}

template<int N, int M>
void printMatrix(QGenericMatrix<N, M, qreal> m, QDebug &dbg, int fw = 15)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "QGenericMatrix<" << N << ", " << M
        << ", " << QMetaType::fromType<qreal>().name()
        << ">(" << Qt::endl << qSetFieldWidth(fw);
    for (int row = 0; row < M; ++row) {
        for (int col = 0; col < N; ++col)
            dbg << m(row, col);
        dbg << Qt::endl;
    }
    dbg << qSetFieldWidth(0) << ')';
}

QList<qreal> NeuralNet::decide(QList<qreal> input)
{
    static QGenericMatrix<1, 10, qreal> shrink {
        new qreal[] {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1
        }
    };

    auto inMat = QGenericMatrix<1, 10, qreal>(input.data());

    auto res = inMat * compiledWeights;

    auto d = (res * shrink).data();

    auto ret = QList<qreal> {
        d[0] * 1/3. + d[1] * 1/3. + d[2] * 1/3.,
        d[3] * 1/3. + d[4] * 1/3. + d[5] * 1/3.,
        d[6] * 1/3. + d[7] * 1/3. + d[8] * 1/3.,
    };

#define DEBUG_PRINT_DECISIONS 0

#if DEBUG_PRINT_DECISIONS
    auto dbg = qInfo();
    printMatrix(res, dbg);
    printMatrix(res * shrink, dbg);

    dbg << input << "->" << ret << Qt::endl;
#endif // DEBUG_PRINT_DECISIONS

    return ret;
}

void NeuralNet::crossover(NeuralNet *other)
{
    qInfo() << "cossover";
    for(int i = 0; i < actualWeights.size(); ++i) {
        actualWeights[i] = other->weights()[i] * 0.25 + actualWeights[i] * 0.75;
    }

    compileWeights();
}

void NeuralNet::save(QString filename)
{
    auto f = new QFile(filename);

    if(!f->open(QFile::ReadWrite)) {
        qWarning() << filename << ": " << f->errorString();
    }

    QDataStream ds(f);

    for(auto w: actualWeights)
        ds << w;

    f->close();
}

void NeuralNet::load(QString filename)
{
    auto f = new QFile(filename);

    if(!f->open(QFile::ReadOnly)) {
        qWarning() << filename << ": " << f->errorString();
    }

    QDataStream ds(f);

    for(int i = 0; i < 10; ++i)
        ds >> actualWeights[i];

    compileWeights();
}

void NeuralNet::compileWeights()
{
    auto compileData = QGenericMatrix<10, 10, qreal>();

    for(auto &w: actualWeights) {
        auto tmp = QGenericMatrix<10, 10, qreal>();
        for(int i = 0; i < 10; ++i)
            tmp(i, i) = w(0, i);

        compileData = compileData * tmp;
    }

    for(int i = 0; i < 10; ++i)
        compiledWeights(0, i) = compileData(i, i);
}
