#include "net2.h"
#include "aibot.h"
#include "snake.h"

#include <QPointF>
#include <QVector2D>

namespace Slither
{

namespace
{

std::unique_ptr<NeuralNet> s_globalNet = nullptr;

}

Net2::Net2(AiBot *s)
    : m_bot(s)

    // This net looks as the following:
    /* I.x
     * I.y
     * nextFood.x
     * nextFood.y
     * nextSnakeSeg.x
     * nextSnakeSeg.y
     * nextBorder.x
     * nextBorder.y
     */
{
    *m_net = *globalNet();
    m_net->mutate(MUTATION_RATE());
}

void Net2::act(qreal dt)
{
    // get all the input
    auto input = QList<float>(8);

    QPointF food = m_bot->findNextFood().position;
    QPointF snake = m_bot->findNextSnakeSegment();
    QPointF border = m_bot->findBorder();

    input[0] = m_bot->position().x();
    input[1] = m_bot->position().y();

    input[2] = food.x();
    input[3] = food.y();

    input[4] = snake.x();
    input[5] = snake.y();

    input[6] = border.x();
    input[7] = border.y();

    // let the net decide
    auto output = m_net->output(input);

    // tell the snake what it wants based on the output
    m_bot->m_snake->setDestination(QPointF(output[0], output[1]) * m_bot->playground()->size());
    // multiply it by the size, because we get only -1 .. 1

    m_bot->m_snake->setBoosting(output[2] > 0);
//    qInfo() << input << "->" << output;
}

void Net2::mutate(NeuralNet *n)
{
    *s_globalNet = *n;
}

NeuralNet *Net2::globalNet()
{
    if (Q_UNLIKELY(!s_globalNet))
        s_globalNet.reset(new NeuralNet(8, 10, 3, 2));

    return s_globalNet.get();
}

}
