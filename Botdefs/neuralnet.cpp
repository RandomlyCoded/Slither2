#include "neuralnet.h"

#include <QDebug>
#include <QFile>
#include <QRandomGenerator>

#define DEBUG_PRINT_DECISIONS 0

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
    return 1. / (1 + std::pow(M_E, -x)) * 2 - 1;
}

inline qreal rng()
{
    return QRandomGenerator::global()->generateDouble() * 4 - 2;
}

} // namespace

NeuralNet::NeuralNet(int i, int o, int l, int c)
{
    Q_UNUSED(i)
    Q_UNUSED(o)
    Q_UNUSED(l)
    Q_UNUSED(c)

    QGenericMatrix<10, 11, qreal> l1;


    // generate the weights
    for(int j = 0; j < 10; ++j) {
        qreal v = rng();

        // fill the column with the weights
        for(int y = 0; y < 9; ++y)
            l1(y, j) = v;
    }

    // add the bias into the last row
    for(int j = 0; j < 10; ++j) {
        qreal v = rng();

        l1(9, j) = v;
    }

    actualWeights.append(l1); // add the input as the first "layer"

    // generate the remaining layers
    for(int i = 0; i < 10; ++i) {
        QGenericMatrix<10, 11, qreal> w;

        // generate the weights
        for(int j = 0; j < 10; ++j) {
            qreal v = rng();

            // fill the column with the weights
            for(int y = 0; j < 10; ++j)
                l1(y, j) = v;
        }

        // add the bias into the last row
        for(int j = 0; j < 10; ++j) {
            qreal v = rng();

            l1(10, j) = v;
        }

        actualWeights.append(w); // add the layer
    }

    // finally, we need a "layer" with 3 weights to shrink the result into the final shape

    QGenericMatrix<10, 11, qreal> out;

    out *= 0; // remove all the 1's of the identity

    // generate the weights
    for(int j = 0; j < 3; ++j) {
        qreal v = rng();

        // fill the column with the weights
        for(int y = 0; y < 10; ++y)
            out(y, j) = v;
    }

    // add the bias into the last row
    for(int j = 0; j < 3; ++j) {
        qreal v = rng();

        out(10, j) = v;
    }

    actualWeights.append(out); // add the input as the first "layer"

    compileWeights();
}

void NeuralNet::mutate(qreal mr)
{
    const int amt = QRandomGenerator::global()->bounded(actualWeights.length());

    for(int i = 0; i < amt; ++i) {
        int layer = QRandomGenerator::global()->bounded(actualWeights.length());
        int idx = QRandomGenerator::global()->bounded(10);
        bool bias = QRandomGenerator::global()->bounded(1); // 0 or 1

//        qInfo() << "mutating" << layer << idx;

        qreal off = actualWeights[layer](bias ? 10 : 0, idx) + rng() * mr;

        if(off < -2)
            off = -2;
        if(off > 2)
            off = 2;

        if(bias)
            actualWeights[layer](10, idx) = off;

        else {
            // we need to change an entire column
            for(int i = 0; i < 10; ++i)
                actualWeights[layer](i, idx) = off;
        }

        compileWeights();
    }
}

QList<qreal> NeuralNet::decide(QList<qreal> input)
{
    QGenericMatrix<11, 1, qreal> inMat;

    inMat *= 0; // clear all data

#if DEBUG_PRINT_DECISIONS
    auto deb = qInfo();
#endif // DEBUG_PRINT_DECISIONS

    // move the input data to the matrix
    for(int i = 0; i < 10; ++i) {
        if(i >= input.length())
            inMat(0, i) = 0;
        else
        inMat(0, i) = input[i];
    }

    inMat(0, 10) = 1; // bias multiplier

#if DEBUG_PRINT_DECISIONS
    printMatrix(inMat, deb);
    deb << Qt::endl;
#endif // DEBUG_PRINT_DECISIONS

    for(auto w: actualWeights) {
        auto res = inMat * w;

#if DEBUG_PRINT_DECISIONS
        printMatrix(res, deb);
        deb << Qt::endl;
#endif // DEBUG_PRINT_DECISIONS

        for(int i = 0; i < 10; ++i) // the bias at 0/10 is kept
            inMat(0, i) = sigmoid(res(0, i)); // copy the values and apply the sigmoid function
    }

    auto d = inMat.data();

    QList<qreal> ret(d, d + 3);

//    qInfo() << input << ret;

#if DEBUG_PRINT_DECISIONS
    deb << ret;
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
    /*
     * I changed the architecture of the nets, now I dont really know how to pre multiply them
     */

    compiledWeights *= 0; // reset

    // pre set values to 1 so multiplication works
    for(int i = 0; i < 10; ++i)
        compiledWeights(0, i) = 1;

    // mulitply the matricies to get a "compiled" matrix
    for(auto &w: actualWeights) {
//        compiledWeights = w * compiledWeights;

        // apply the sigmoid function to each weight
        for(int i = 0; i < 10; ++i)
            compiledWeights(0, i) = sigmoid(compiledWeights(0, 1));
    }
}
