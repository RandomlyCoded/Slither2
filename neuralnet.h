#ifndef NEURALNET_H
#define NEURALNET_H

#include <QGenericMatrix>

class NeuralNet
{
public:
    NeuralNet(int i, int o, int l, int c);;

    void mutate(qreal mr);

    QList<qreal> decide(QList<qreal> input);

    void crossover(NeuralNet *other);

    auto weights() { return actualWeights; }

    void save(QString filename);
    void load(QString filename);

private:
    QList<QGenericMatrix<10, 10, qreal>> actualWeights;
    QGenericMatrix<10, 10, qreal> compiledWeights;

    void compileWeights();
};

#endif // NEURALNET_H
