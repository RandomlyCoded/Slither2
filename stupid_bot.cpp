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

QVector2D relativePosition(const Snake *s)
{
    return QVector2D(s->destination() - s->position());
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
    m_snake->setDestination(QVector2D(position() + destination()).toPointF());
}

bool StupidBot::wouldChrash(qreal dt)
{ return !playground()->checkBounds(next(m_snake, dt)); }

namespace {

int get()
{
    auto a = QRandomGenerator::global()->bounded(2);
    auto b = 1 - (a * 2);
    qInfo() << a << b;
    return b;
}

}
void StupidBot::changeDestination()
{
    qreal angle = QRandomGenerator::global()->generateDouble() * get();
    m_snake->setDestination(QPointF{cos(angle), sin(angle)});
}

} // namespace Slither
