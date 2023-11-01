#include "snake.h"

#include "playground.h"
#include "Bots"

#include <QDebug>
#include <QVector2D>
#include <QRandomGenerator>
//#include <QtMultimedia/QMediaPlayer>

#ifdef QT_DEBUG
#define START_AT_SIZE    0 // starting as a bigger snake. ONLY AVAILABLE IN DEBUG MODE
#endif

#define MASSHACKS_ACTIVE 0 // set it to 1 for activating mass hacks; then you will grow every time you move. Bots are also growing.

namespace Slither {

namespace
{

auto rng = QRandomGenerator::global();

int snakeCount = 0;

} // namespace

void Snake::setDestination(QPointF destination)
{
    m_requestedDestination = destination;
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

void Snake::setBotType(BotType type)
{
    qInfo() << Q_FUNC_INFO << type;
    delete m_bot;
    switch(type) {
    case BotType::Bot_Stupid:
        m_bot = new StupidBot(this);
        break;
    case BotType::Bot_Eating:
        m_bot = new EatingBot(this);
        break;
    case BotType::Bot_Killing:
        m_bot = new KillingBot(this);
        break;
    case BotType::Bot_NoBot:
        m_bot = new FollowMouseBot(this);
        break;
    case BotType::Bot_Intelligent:
        m_bot = new AiBot(this);
        break;
    default:
        m_bot = new StupidBot(this);
    }
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

void Snake::init()
{
    connect(this, &Snake::destinationChanged, this, &Snake::directionChanged);
    connect(this, &Snake::positionChanged, this, &Snake::directionChanged);
    m_skin = {Qt::red, Qt::green, Qt::blue};
}

Snake::Snake(QObject *parent)
    : QObject{parent}
    , m_bot(new AiBot{this})
    , m_size(1)
    , m_isAlive(false)
    , m_segments({QPointF(0, 0)})
    , snakeIdx(snakeCount++)
{
    init();
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

    m_length = other.length();
    m_useBot = other.useBot();

    init();

    m_skin = other.skin();
}

Snake::Snake(ImportantData &d)
{
    setBotType(BotType(d.botType));
    init();
    m_segments = d.segs;
    m_destination = d.dest;
    m_size = 1;
    m_load = 0;
    m_length = 0;
    snakeIdx = snakeCount++;
}

QList<EnergyPearl> Snake::die()
{
    QList<EnergyPearl> pearls;
    for(int i = 0; i < m_segments.count(); i += m_size) {
        const QPointF &seg = m_segments[i];
        QColor clr = skinAt(i);
        qreal r = clr.red()   + rng->generateDouble(); if(r > 255) r = 255;
        qreal g = clr.blue()  + rng->generateDouble(); if(g > 255) g = 255;
        qreal b = clr.green() + rng->generateDouble(); if(b > 255) b = 255;
        pearls.append(EnergyPearl{qreal(m_size), seg + QPointF(rng->generateDouble(), rng->generateDouble()), QColor(r, g, b)});
    }

    m_isAlive = false;
    emit isAliveChanged();
    emit destinationChanged(m_destination);
    emit positionChanged();

    playDieSound();
    emit died();

    return pearls;
}

QString Snake::lengthInfo() const
{
    QString str;
    str = QString::fromStdString(std::to_string(length()));

    std::string s = str.toStdString();

    auto index = s.find_first_of(".");

    s = s.substr(0, index + 2);
    return QString::fromStdString(s);
}

void Snake::spawn(QPointF position, QPointF destination)
{
    m_segments = {};
    m_size = 1;
    m_isAlive = true;
//    const auto dp = direction();
    m_destination = destination;
    m_segments.clear();

    for (int i = 0; i < 5
     #if START_AT_SIZE
         + 400
     #endif
         ; ++i)
        m_segments.append(/*position - (dp * i).toPointF()*/position - destination);

    m_length = m_segments.count();
    emit destinationChanged(m_destination);
    emit positionChanged();

    emit segmentsChanged(m_segments);
    emit lengthChanged();

    emit isAliveChanged();
}

QVector2D min(QVector2D a, QVector2D b)
{
    return a.length() < b.length() ? a : b;
}

void Snake::setDest()
{
    const auto rotationSpeed = 0.2 / m_size;
    // get the direction and the requested direction as normalized vector
    const auto dp = direction();
    const auto reqDp = QVector2D{m_requestedDestination - m_segments[0]}.normalized();

    // get the angle of each vector
    const auto ang = atan2(dp.y(), dp.x()) + M_PI;
    const auto reqAng = atan2(reqDp.y(), reqDp.x()) + M_PI;

    // get the difference of both angles
    const auto _diff = reqAng - ang;
    qreal diff;

    if(qFuzzyIsNull(_diff)) // if the rotation is ~ 0, return
        return;
    if(_diff < 0)
        diff = -rotationSpeed;
    else
        diff = rotationSpeed;

    if(qAbs(_diff) > M_PI) // rotation > 180°
        diff *= -1;

    if(qAbs(diff) > qAbs(_diff)) // the difference is less then the standard rotation, so rotate the difference
        diff = _diff;

    diff += ang - M_PI; // add the angle(normal, so -M_PI) to get the actual direction

    // set the destination
    m_destination = position() + QPointF(cos(diff), sin(diff));

    emit destinationChanged(m_destination);
}

void Snake::move(qreal dt) // steht "dt" für "duration"? bitte LESBARE Namen verwenden
{
    if(!m_isAlive)
        return;
    if (m_segments.isEmpty())
        return;

    if(m_destination == position())
        m_requestedDestination = {0, 0}; // stop snakes from not moving at all

    auto dp = direction();
    setDest();

    if (m_playground) {
        if (const auto energy = m_playground->consumeNearbyPearls(position(), this); energy > 0) {
            for (m_load += energy; m_load > 0; --m_load)
                m_segments.append(m_segments.last());

            if((length() / m_size) > 200)
                emit sizeChanged(++m_size);

            emit segmentsChanged(m_segments);
            emit lengthChanged();
        }
    }

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

    m_length = m_segments.count();
    emit lengthChanged();
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
/*    QMediaPlayer player;
    player.setAudioRole(QAudio::MusicRole);
    player.setMuted(false);

    QMediaContent c{nullptr, QUrl{"../../data/snakeDies.m4a"}}; // README: vllt aus qrc laden?

    player.setMedia(c);
    player.play();
    */
}

} // namespace Slither

template<class Stream>
Stream &operator <<(Stream &s, Slither::Snake &sn)
{
    s << sn.size() << sn.name();
    return s;
}

#include "moc_snake.cpp"
