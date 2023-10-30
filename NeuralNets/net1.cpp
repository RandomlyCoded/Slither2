#include "net1.h"
#include "aibot.h"
#include "snake.h"

#include <QVector2D>

namespace Slither
{

namespace
{

std::unique_ptr<NeuralNet> s_globalNet = nullptr;

}

Net1::Net1(AiBot *s)
    : m_bot(s)
    // this net looks as the following:
    /* -> angle to next food                  | hidden * 2    | angle to move ->
     * -> distance to next food               |    *          | boost? (< 0 = no, > 0 = yes) ->
     * -> angle to next snake part(not me)    |   10          |
     * -> distance to next snake part(not me) |               |
     * -> angle to next border                |               |
     * -> distance to next border             |               |
     */

{
    *m_net = *globalNet();
    m_net->mutate(MUTATION_RATE());
}

void Net1::act(qreal dt)
{
    // get all the input
    auto input = QList<float>(6);

    QPointF food = m_bot->findNextFood();
    QPointF snake = m_bot->findNextSnakeSegment();
    QPointF border = m_bot->findBorder();

    input[0] = 180 * atan2(food.y() - m_bot->position().y(), food.x() - m_bot->position().x()) / M_PI; // this should give a range beetween -180 and 180
    input[1] = QVector2D(food - m_bot->position()).length();

    input[2] = 180 * atan2(snake.y() - m_bot->position().y(), snake.x() - m_bot->position().x()) / M_PI; // this should give a range beetween -180 and 180
    input[3] = QVector2D(snake - m_bot->position()).length();

    input[4] = 180 * atan2(border.y() - m_bot->position().y(), border.x() - m_bot->position().x()) / M_PI; // this should give a range beetween -180 and 180
    input[5] = QVector2D(border - m_bot->position()).length();

    // let the net decide
    auto output = m_net->output(input);

    // tell the snake what it wants based on the output
    m_bot->m_snake->setDestination(m_bot->position() + QPointF(cos(output[0]), sin(output[0])));
    m_bot->m_snake->setBoosting(output[1] > 0);
}

void Net1::mutate(NeuralNet *n)
{
    *s_globalNet = *n;
}

NeuralNet *Net1::globalNet()
{
    if(Q_UNLIKELY(!s_globalNet))
        s_globalNet.reset(new NeuralNet(6, 10, 2, 2));
    return s_globalNet.get();
}

} // namespace Slither
