#include "stupid_bot.h"

#include "snake.h"
#include "playground.h"

namespace Slither {

namespace {

inline QPointF next(const Snake *s, qreal dt)
{
    return QPointF{s->position() + (s->direction().normalized() * s->speed() * dt).toPointF()}; // emitting the moving code for the snake to find out where
                                                                                   // it will be after s->move(dt)
}

}

StupidBot::StupidBot(Snake *parent)
    : Bot(parent)
{ }

void StupidBot::act(qreal duration)
{
    if(wouldChrash(duration * 1.1)) {
        changeDestination();
        return;
    }
    m_snake->setDestination(position() + m_snake->direction().toPointF());
}

bool StupidBot::wouldChrash(qreal dt)
{ return !playground()->checkBounds(next(m_snake, dt)); } // avoid crashes into other snakes using findNextSnake

void StupidBot::changeDestination()
{
    qreal angle = QRandomGenerator::global()->generateDouble() * 360;
    m_snake->setDestination(QPointF{cos(angle), sin(angle)} * playground()->size());
}

} // namespace Slither
