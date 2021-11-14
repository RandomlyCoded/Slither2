#include "snake.h"

#include "playground.h"
#include "bot.h"

#include <QDebug>
#include <QVector2D>
#include <QRandomGenerator>
//#include <QtMultimedia/QMediaPlayer>

#define MASSHACKS_ACTIVE 0 // set it to 1 for activating mass hacks; then you will grow every time you move. Bots are also growing.

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

void Snake::setSize(int size)
{
    if (m_size == size)
        return;
    m_size = size;

    emit sizeChanged(m_size);
}

QString Snake::positionInfo()
{
    int _X = position().x() * 1000;
    int _Y = position().y() * 1000;

    qreal x = (qreal) _X / 1000;
    qreal y = (qreal) _Y / 1000;

    QString str = QString::fromStdString(std::to_string(x));
    str += ", ";
    str += QString::fromStdString(std::to_string(y));

    return str;
}

Snake::Snake(QObject *parent)
    : QObject{parent}
    , m_bot(new Bot{this})
    , m_size(1)
    , m_isAlive(false)
{
    connect(this, &Snake::destinationChanged, this, &Snake::directionChanged);
    connect(this, &Snake::positionChanged, this, &Snake::directionChanged);
    m_skin = {Qt::red, Qt::green, Qt::blue};
}

Snake::Snake(Snake &other)
    : Snake()
{
    m_destination = other.destination();
    m_size = other.size();
    m_isAlive = other.m_isAlive;
    m_color = other.m_color;
    m_speedColor = other.m_speedColor;

    m_playground = other.playground();
    m_segments = other.m_segments;

    m_speed = other.speed();
    m_load = other.load();

    m_lenght = other.lenght();
    m_useBot = other.useBot();
    m_position = other.position();

    m_skin = other.skin();
}

void Snake::die()
{
    const auto rng = QRandomGenerator::global();
    for(const auto seg: m_segments) {
        int index = m_segments.indexOf(seg);
        if(index < 0)
            continue;
        QColor clr = skinAt(index);
        qreal r = clr.red()   + rng->generateDouble(); if(r > 255.) r = 255;
        qreal g = clr.blue()  + rng->generateDouble(); if(g > 255.) g = 255.;
        qreal b = clr.green() + rng->generateDouble(); if(b > 255.) b = 255.;
        playground()->addPearl(seg + QPointF(rng->generateDouble(), rng->generateDouble()), m_size, QColor(r, g, b), true);
    }

    qInfo("you died");

//  die Schlange zerplatzt ... wenn die Schlange stirbt?

    for(int i = 0; i < m_segments.count(); i++) {
        const auto angle = rng->bounded(360);
        const auto r = playground()->size();
        m_segments[i] = QPointF{r * cos(angle), r * sin(angle)};
    }

    m_isAlive = false;
    emit destinationChanged(m_destination);
    emit positionChanged();

    playDieSound();
}

QString Snake::lenghtInfo() const
{
    QString str;
    str = QString::fromStdString(std::to_string(lenght()));

    std::string s = str.toStdString();

    auto index = s.find_first_of(".");

    s = s.substr(0, index + 2);
    return QString::fromStdString(s);
}

void Snake::spawn(QPointF position, QPointF destination)
{
    m_size = 1;
    m_isAlive = true;
    const auto dp = direction();
    m_destination = destination;
    m_segments.clear();

    for (int i = 0; i < 5; ++i)
        m_segments.append(position - (dp * i).toPointF());

    m_lenght = m_segments.count();
    emit destinationChanged(m_destination);
    emit positionChanged();

    emit segmentsChanged(m_segments);
    emit lenghtChanged();
}

void Snake::move(qreal dt) // steht "dt" für "duration"? bitte LESBARE Namen verwenden
{
    if(!m_isAlive)
        return;
    if (m_segments.isEmpty())
        return;

    auto dp = direction();

    if (m_playground) {
        if (const auto energy = m_playground->consumeNearbyPearls(position(), this); energy > 0) {
            for (m_load += energy; m_load > 0; --m_load)
                m_segments.append(m_segments.last());

            if((lenght() / m_size) > 200) {
                emit sizeChanged(++m_size);
//                qInfo() << "lenght > 200;" << lenght() << "reset segments";
//                QList<QPointF> tmp = m_segments;
//                m_segments.clear();
//                int amount = tmp.count();
//                amount /= m_size++;
//                for(int i = 0; i < amount; i++) {
//                    m_segments.append(tmp[i]);
//                }
//                tmp.clear();
            }

            emit segmentsChanged(m_segments);
            emit lenghtChanged();
        }
    }

#if MASSHACKS_ACTIVE
    m_load += m_size; // mass hack
#endif

    const auto motion = dp * speed() * dt;
    m_segments[0] += motion.toPointF()/* * angle*/;
    if(m_speed == m_playground->boostSpeed) {
        if(m_load < 0.2) {
            m_segments.removeLast();
            m_load += 1;
        }
        m_playground->addPearl(m_segments.last(), 0.2, skinAt(m_segments.indexOf(m_segments.last())), true);
        m_load -= 0.2;
    }

    for (int i = 1; i < m_segments.count(); ++i) {
        m_segments[i] += (QVector2D{m_segments[i - 1] - m_segments[i]} * speed() * dt).toPointF();
        if(!m_playground->checkBounds(m_segments[i]) && (m_segments[i] != position()))
            m_segments[i] = m_segments[i - 1];
    }

    emit positionChanged();
    emit segmentsChanged(m_segments);

    m_lenght = m_segments.count();
    emit lenghtChanged();

    playground()->checkCrash(this);
}

void Snake::setBoosting(bool newBoosting)
{
    if(!canBoost())
        return;
    if (m_boosting == newBoosting)
        return;
    m_boosting = newBoosting;

    if(m_boosting)
        m_speed = Playground::boostSpeed;
    else
        m_speed = Playground::normalSpeed;


    emit boostingChanged();
    emit speedChanged(m_speed);
}

void Snake::setUseBot(bool newUseBot)
{
    if (m_useBot == newUseBot)
        return;
    m_useBot = newUseBot;
    emit botUsingChanged();
}

void Snake::playDieSound()
{
    qInfo() << "playing \"die-sound\"";
/*    QMediaPlayer player;
    player.setAudioRole(QAudio::MusicRole);
    player.setMuted(false);

    QMediaContent c{nullptr, QUrl{"../../data/snakeDies.m4a"}}; // README: vllt aus qrc laden?

    player.setMedia(c);
    player.play();
    */
}

} // namespace Slither

#include "moc_snake.cpp"
