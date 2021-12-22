#include "bot.h"
#include "snake.h"

#include <QRandomGenerator>
#include <QDebug>
#include <QObject>

namespace Slither {

void Bot::remove()
{ this->~Bot(); }

Bot::Bot(Snake *controlled)
{
    m_snake = controlled;
    QObject::connect(m_snake, &Snake::died, this, &Bot::remove);
}

QPointF Bot::destination() const
{
    return m_snake->destination();
}

Playground *Bot::playground() const
{
    return m_snake->playground();
}

bool is_inRange(qreal dist, qreal value, qreal range)
{
    return ((dist <= value) && ((dist - range) >= value)) || ((dist >= value) && ((dist + range) <= value));
}

const QPointF Slither::Bot::position() const
{
    return m_snake->position();
}

} // namespace Slither
