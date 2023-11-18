#include "bot.h"
#include "../src/snake.h"

#include <QRandomGenerator>
#include <QDebug>
#include <QObject>

namespace Slither {

Bot::Bot(Snake *controlled)
{
    m_snake = controlled;
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

QPointF Slither::Bot::position() const
{
    return m_snake->position();
}

Snake *Bot::snake()
{
    return m_snake;
}

EnergyPearl Bot::findNextFood()
{
    auto energyPearls = playground()->energyPearls();
    qreal dist = qInf();
    auto pearl = energyPearls->at(0);

    for(int i = 0; i < energyPearls->rowCount(); i++) {
        if(QVector2D(energyPearls->at(i).position - position()).length() < dist) {
            pearl = energyPearls->at(i);
            dist = QVector2D(energyPearls->at(i).position - position()).length();
        }
    }

//    qInfo() << pearl.position << position();

    return pearl;
}

QPointF Bot::findNextSnakeSegment()
{
    QPointF rel = playground()->snakes()[0]->position();

    for(auto sn: playground()->snakes()) {
        if(sn == snake())
            continue;
        for(auto seg: sn->segments())
            if(QVector2D(seg).distanceToPoint(QVector2D(snake()->position())) < QVector2D(rel).length())
                rel = seg - snake()->position();
    }
    return rel + snake()->position();
}

QPointF Bot::findBorder()
{
    qreal angle = atan2(snake()->position().y(), snake()->position().x());

    return QPointF(cos(angle), sin(angle)) * playground()->size();
}

} // namespace Slither
