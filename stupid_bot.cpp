#include "stupid_bot.h"

#include "snake.h"
#include "playground.h"

namespace Slither {

namespace {

inline QPointF next(const Snake *s, qreal dt)
{
    return QPointF{s->position() + (s->direction() * s->speed() * dt).toPointF()}; // emitting the moving code for the snake to find out where
                                                                                   // it will be after s->move(dt)
}

}

StupidBot::StupidBot(Snake *parent)
    : Bot(parent)
{ }

void StupidBot::act(qreal duration)
{
    if(wouldChrash(duration * 1.1)) {
        qInfo() << "I (" << this << ") chrash if I move again: " << position() << QVector2D(position()).length() << duration;
        changeDestination();
    }
    m_snake->setDestination(position() + m_snake->destination());
}

bool StupidBot::wouldChrash(qreal dt)
{ return !playground()->checkBounds(next(m_snake, dt)); }

void StupidBot::changeDestination()
{
    bool toLeft = QRandomGenerator::global()->bounded(1);
    m_snake->setDestination(m_snake->position() +
                            (m_snake->destination() + QPointF(
                                 qCos(toLeft ? -135 : 135), qSin(toLeft ? -135 : 135)
                                 )));
        // I'm sorry for this code(that it is bad formated); if you can make it better readable, just do it!
}

} // namespace Slither
