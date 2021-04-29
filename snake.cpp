#include "playground.h"
#include "snake.h"

#include <QDebug>
#include <QVector2D>

namespace Slither {

void Snake::setDestination(QPointF destination)
{
    if (m_destination == destination)
        return;

    m_destination = destination;
    emit destinationChanged(m_destination);
}

void Snake::setSpeed(qreal speed)
{
    if (qFuzzyCompare(m_speed, speed))
        return;

    m_speed = speed;
    emit speedChanged(m_speed);
}

Snake::Snake(QObject *parent)
    : QObject{parent}
{
    connect(this, &Snake::destinationChanged, this, &Snake::directionChanged);
    connect(this, &Snake::positionChanged, this, &Snake::directionChanged);
}

QVector2D Snake::direction() const
{
    return QVector2D{destination() - position()}.normalized();
}

void Snake::setPlayground(Playground *playground)
{
    m_playground = playground;
}

void Snake::die()
{
    qInfo("so you died");
    m_segments.clear();
}

void Snake::spawn(QPointF position, QPointF destination)
{
    const auto dp = direction();
    m_destination = destination;
    m_segments.clear();

    for (int i = 0; i < 5; ++i)
        m_segments.append(position - (dp * i).toPointF());

    emit destinationChanged(m_destination);
    emit positionChanged();

    emit segmentsChanged(m_segments);
}

void Snake::move(qreal dt)
{
    if (m_segments.isEmpty())
        return;

    const auto dp = direction();

    if (m_playground) {
        if (const auto energy = m_playground->consumeNearbyPearls(position()); energy > 0) {
            for (m_load += energy; m_load > 0; --m_load)
                m_segments.append(m_segments.last() + dp.toPointF());

/*            if(m_segments.count() > 600) {
                int amount = m_segments.count();
                QList<QPointF> tmp = m_segments;
                m_segments.clear();

                loadCounter++;
                amount /= snakeSize;

                for(int i = 0; i < amount; i++) {
                    m_segments.append(tmp.first());
                    tmp.removeFirst();
                }

                tmp.clear();
            }*/

//                const int SIZE = m_segments.count();
//                m_segments.clear();
//                QList<QPointF> tmp = m_segments;
//                for(int i = 0; i < SIZE; i++) {
//                    m_segments.append(QPointF(tmp[i]));
//                }
//                emit segmentsChanged(m_segments);
//            }

            emit loadChanged(m_load);
        }
    }

    const auto motion = dp * speed() * dt;
    m_segments[0] += motion.toPointF();

// die Schlange zerplatzt ... wemm die Schlange stirbt?
//    for (int i = 1; i < m_balls.count(); ++i)
//        m_balls[i] += (QVector2D{m_balls[i] - m_balls[i - 1]} * dt).toPointF();

    for (int i = 1; i < m_segments.count(); ++i)
        m_segments[i] += (QVector2D{m_segments[i - 1] - m_segments[i]} * speed() * dt).toPointF();

    emit positionChanged();
    emit segmentsChanged(m_segments);

    playground()->checkCrash(this);
}

} // namespace Slither

#include "moc_snake.cpp"
