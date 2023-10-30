#include "neuralnet.h"

#include <QDebug>
#include <QFile>
#include <QRandomGenerator>

namespace
{

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

inline constexpr qreal sigmoid(qreal x)
{
    return 1. / (1 + std::pow(M_E, -x));
}

} // namespace

NeuralNet::NeuralNet(int i, int o, int l, int c)
{
    Q_UNUSED(i)
    Q_UNUSED(o)
    Q_UNUSED(l)
    Q_UNUSED(c)

    for(int i = 0; i < 10; ++i) {
        QGenericMatrix<10, 10, qreal> w;

        w *= 0; // set all values to 0

        for(int j = 0; j < 10; ++j) {
            qreal v = QRandomGenerator::global()->generateDouble() * 4 - 1;

            w(0, j) = v;
        }

//        QDebug d = qInfo();
//        printMatrix(w, d);

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

QList<qreal> NeuralNet::decide(QList<qreal> input)
{
    QGenericMatrix<10, 10, qreal> inMat;

    inMat *= 0; // clear all data

    for(int i = 0; i < 10; ++i) {
        if(i >= input.length())
            inMat(0, i) = 1;
        else
        inMat(0, i) = input[i];
    }

    auto res = compiledWeights * inMat;

    auto ret = QList<qreal> {
        (res(0, 0) + res(0, 1)) / 3., // boosting ?
        (res(0, 2) + res(0, 3) + res(0, 4) + res(0, 5)) / 4., // turning 1
        (res(0, 6) + res(0, 7) + res(0, 8) + res(0, 9)) / 4.  // turning 2
    };

#define DEBUG_PRINT_DECISIONS 0

#if DEBUG_PRINT_DECISIONS
    auto deb = qInfo();

    printMatrix(inMat, deb);
    deb << Qt::endl;

    printMatrix(compiledWeights, deb);
    deb << Qt::endl;

    printMatrix(res, deb);
    deb << Qt::endl;

    deb << ret;

    deb.~QDebug();
#endif // DEBUG_PRINT_DECISIONS

    return ret;
}

void NeuralNet::crossover(NeuralNet *other, qreal w)
{
    qInfo() << "cossover";
    for(int i = 0; i < actualWeights.size(); ++i) {
        actualWeights[i] = other->weights()[i] * w + actualWeights[i] * (1 - w);
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

void NeuralNet::assign(NeuralNet *other)
{
    actualWeights = other->weights();

    compileWeights();
}

void NeuralNet::compileWeights()
{
    compiledWeights *= 0; // reset

    // pre set values to 1 so multiplication works
    for(int i = 0; i < 10; ++i)
        compiledWeights(0, i) = 1;

    // mulitply the matricies to get a "compiled" matrix
    for(auto &w: actualWeights) {
        compiledWeights = w * compiledWeights;

        // apply the sigmoid function to each weight
        for(int i = 0; i < 10; ++i)
            compiledWeights(0, i) = sigmoid(compiledWeights(0, 1));
    }
}
