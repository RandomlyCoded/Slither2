#ifndef SLITHER_SNAKE_H
#define SLITHER_SNAKE_H

#include "bot.h"
#include "playground.h"

#include <QColor>
#include <QObject>
#include <QPointF>
#include <QTextStream>
#include <QVector2D>
#include <QDebug>
#include <QPointer>

namespace Slither {

class Playground;

class Snake : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QPointF> segments READ segments NOTIFY segmentsChanged FINAL)
    Q_PROPERTY(QPointF lastSegment READ lastSegment NOTIFY segmentsChanged FINAL)
    Q_PROPERTY(QPointF position READ position NOTIFY positionChanged FINAL)
    Q_PROPERTY(QPointF destination READ destination WRITE setDestination NOTIFY destinationChanged FINAL)
    Q_PROPERTY(QVector2D direction READ direction NOTIFY directionChanged FINAL)

    Q_PROPERTY(qreal speed READ speed WRITE setSpeed NOTIFY speedChanged FINAL)
    Q_PROPERTY(bool canBoost READ canBoost NOTIFY canBoostChanged FINAL)

    Q_PROPERTY(qreal load READ load NOTIFY loadChanged FINAL)
    Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged FINAL)
    Q_PROPERTY(QString length READ lengthInfo NOTIFY lengthChanged FINAL)
    Q_PROPERTY(QList<QColor> skin READ skin WRITE setSkin NOTIFY skinChanged FINAL)

    Q_PROPERTY(Bot bot READ bot /*NOTIFY botChanged*/ FINAL)
    Q_PROPERTY(bool useBot READ useBot WRITE setUseBot NOTIFY botUsingChanged FINAL)

    Q_PROPERTY(bool isAlive READ isAlive NOTIFY isAliveChanged FINAL)
    Q_PROPERTY(QString positionInfo READ positionInfo NOTIFY positionChanged FINAL)

    Q_PROPERTY(bool boosting READ boosting WRITE setBoosting NOTIFY boostingChanged FINAL)

    Q_PROPERTY(QPointF direction_point READ dir_p NOTIFY directionChanged FINAL)
    Q_PROPERTY(QString name READ name CONSTANT FINAL)

public:
    struct ImportantData {
        ImportantData(Snake *s)
            : segC(s->segments().count()), segs(s->segments()), dest(s->destination()), botType(s->botType()), s(s) {}
        ImportantData() {}
        int segC;
        QList<QPointF> segs;
        QPointF dest;
        uchar botType;
        Snake *s;
    };

    explicit Snake(QObject *parent = nullptr);
    Snake(Snake &other);
    Snake(ImportantData &d);

    bool operator <(Snake &other) { return length() < other.length(); }
    bool operator >(Snake &other) { return other < *this; }                 // a < b means b < a
    bool operator<=(Snake &other) { return !(*this > other); }              // a <= b means !(b < a)
    bool operator>=(Snake &other) { return !(*this < other); }              // a >= b means !(b > a)
    bool operator==(Snake &other) { return length() == other.length(); }

    QList<QPointF> segments() const { return m_segments; }
    QPointF destination() const { return m_destination; }
    QPointF position() const { return (!m_segments.isEmpty() ? m_segments[0] : QPointF{}); }
    QVector2D direction() const { return QVector2D{destination() - position()}.normalized(); }

    qreal speed() const { return m_speed; }
    bool canBoost() const { return m_segments.count() > 10; }
    qreal load() const { return m_load; }

    void setPlayground(Playground *playground) { m_playground = playground; }
    Playground *playground() const { return m_playground.get(); }

    bool useBot() const { return m_useBot; }
    Bot *bot() const { return m_bot.get(); }

    QList<EnergyPearl> die();

    int size() const { return m_size; }
    qreal length() const { return m_length + m_load; }
    QString lengthInfo() const;

    bool isAlive() const { return m_isAlive; }


    Snake &operator=(Snake &other) {
        Snake &sn(other);
        return sn;
    }
    QString positionInfo();

    const QList<QColor> &skin() const { return m_skin; }
    void setSkin(const QList<QColor> &newSkin)
    {
        if (m_skin == newSkin)
            return;
        m_skin = newSkin;
        emit skinChanged();
    }

    bool boosting() const { return m_boosting; }
    void setBoosting(bool newBoosting);

    void setUseBot(bool newUseBot);

    static void playDieSound();

    QPointF dir_p() const { return direction().toPointF(); }

    QPointF lastSegment() const { return m_segments.last(); }

    QString name() { return "Snake#kp"; }

    using BotType = Bot::Type;
    BotType botType() { return m_bot->type(); }

public slots:
    void spawn(QPointF position, QPointF destination);
    void move(qreal dt);

    void setDestination(QPointF destination);
    void setSpeed(qreal speed);

    void setSize(int size);

    void changeBotUsing() { m_useBot = !m_useBot; }

    QColor skinAt(int index) { return m_skin[index % m_skin.length()]; }
    QColor skinLast() { return m_skin.last(); }

    void setBotType(Slither::Snake::BotType type); // bruh sry but clang :)

signals:
    void died();
    void segmentsChanged(QList<QPointF> segments);
    void destinationChanged(QPointF destination);
    void positionChanged();
    void directionChanged();
    void speedChanged(qreal speed);
    void canBoostChanged(bool canBoost);
    void loadChanged(qreal load);

    void sizeChanged(int size);
    void lengthChanged();

    void botUsingChanged();

    void isAliveChanged();

    void skinChanged();

    void boostingChanged();

    void lastSegmentChanged();

private:
    QPointer<Slither::Bot> m_bot;
    int m_size;
    bool m_isAlive = true;
    QColor m_color;
    QColor m_speedColor;

    QPointer<Slither::Playground> m_playground;
    QList<QPointF> m_segments = {};
    QPointF m_destination;
    QPointF m_requestedDestination = {};

    int m_length;

    friend Playground;

    qreal m_speed = 5;
    qreal m_load = 0;

    bool m_useBot = true;

    QList<QColor> m_skin;
    bool m_boosting;

    void init();
    void setDest();
};

} //namespace Slither

template<class Stream>
Stream &operator<<(Stream &s, Slither::Snake &sn);

#endif // SLITHER_SNAKE_H
