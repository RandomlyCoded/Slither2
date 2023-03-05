#include "eating_bot.h"
#include "snake.h"

namespace Slither
{

EatingBot::EatingBot(Snake *controlled)
    : Bot(controlled)
{}

void EatingBot::act(qreal dt)
{
    if(QVector2D(m_snake->position()).distanceToPoint(QVector2D(m_snake->destination())) <= m_snake->size()) {
            // a distance < size means the snake ate the food. so we need to find a new one:
        m_snake->setDestination(findNextFood().position);
    }
}

} // namespace Slither
