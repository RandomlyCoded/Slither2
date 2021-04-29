#ifndef SLITHER_SNAKE_H
#define SLITHER_SNAKE_H

#include <QObject>
#include <QPointF>
#include <QPointer>

namespace Slither {

class Playground;

class Snake : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QPointF> segments READ segments NOTIFY segmentsChanged FINAL)
    Q_PROPERTY(QPointF destination READ destination WRITE setDestination NOTIFY destinationChanged FINAL)
    Q_PROPERTY(QVector2D direction READ direction NOTIFY directionChanged FINAL)
    Q_PROPERTY(qreal speed READ speed WRITE setSpeed NOTIFY speedChanged FINAL)
    Q_PROPERTY(bool canBoost READ canBoost NOTIFY canBoostChanged FINAL)
    Q_PROPERTY(qreal load READ load NOTIFY loadChanged FINAL)

public:
    explicit Snake(QObject *parent = {});

    QList<QPointF> segments() const { return m_segments; }
    QPointF destination() const { return m_destination; }
    QPointF position() const { return (!m_segments.isEmpty() ? m_segments[0] : QPointF{}); }
    QVector2D direction() const;

    qreal speed() const { return m_speed; }
    bool canBoost() const { return m_segments.count() > 10; }
    qreal load() const { return m_load; }

    void setPlayground(Playground *playground);
    Playground *playground() const { return m_playground; }

    void die();

public slots:
    void spawn(QPointF position, QPointF destination);
    void move(qreal dt);

    void setDestination(QPointF destination);
    void setSpeed(qreal speed);

signals:
    void segmentsChanged(QList<QPointF> segments);
    void destinationChanged(QPointF destination);
    void positionChanged();
    void directionChanged();
    void speedChanged(qreal speed);
    void canBoostChanged(bool canBoost);
    void loadChanged(qreal load);

private:
/*    int loadCounter = 1;
    int snakeSize = m_segments.count() / loadCounter;*/
    QPointer<Playground> m_playground;
    QList<QPointF> m_segments = {};
    QPointF m_destination;
    qreal m_speed = 5;
    qreal m_load = 0;
    bool m_canBoost;
};

} // namespace Slither

#endif // SLITHER_SNAKE_H
