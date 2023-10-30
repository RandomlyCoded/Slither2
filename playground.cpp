#include "playground.h"

#include "snake.h"
#include "bot.h"
#include "leaderboard.h"

#include <QRandomGenerator>
#include <QVector2D>
#include <QDebug>
#include <QFile>
#include <QVector2D>

namespace Slither {

namespace {

}

Playground::Playground(QObject *parent)
    : QObject(parent)
    , m_leaderboard(new Leaderboard(this))
{
    connect(this, &Playground::snakesChanged, m_leaderboard, &Leaderboard::reload);
    connect(this, &Playground::totalSnakesSizeChanged, m_leaderboard, &Leaderboard::reload);
    initialize(0);
}

bool Playground::checkCrash(Snake *toCheck)
{
    for(const auto check: m_snakes) {
        if(check == toCheck)
            continue;

        for(const auto checkedTile: check->segments()) {
            if(QVector2D(toCheck->position()).distanceToPoint(QVector2D(checkedTile)) < (toCheck->size() + check->size())) {
                killSnake(toCheck);
                return true;
            }
        }
    }
    return false;
}

int Playground::totalSnakesSize() const
{
    qreal amount = 0;
    for(const auto sn: m_snakes)
        amount += sn->length();
    return amount;
}

void Playground::initialize(qreal size)
{
    // update playground size if needed
    if (size == 0)
        size = m_size;

    if (!qFuzzyCompare(m_size, size)) {
        m_size = size;
        emit sizeChanged(m_size);
    }

    // number of pearls we generate depends on the playground's area
    const auto area = M_PI * size * size;
    const auto count = qRound(area / (m_size * 1/4));

    m_maximumEnergy = count;

    // let's create new pearls
    QList<EnergyPearl> pearls;
    pearls.reserve(count);

    for (int i = 0; i < count; ++i)
        pearls += spawnPearl();

    m_energyPearls->reset(std::move(pearls));

    // init m_snakes
    m_snakes = {};

    // starting timer for new Snakes
    connect(m_newSnakeTimer, &QTimer::timeout, this, &Playground::spawnSnake);
    m_newSnakeTimer->setInterval(1000);
    m_newSnakeTimer->start();

    // starting timer for new Energy Pearls
    connect(m_energyTimer, &QTimer::timeout, this, &Playground::energyBoost);
    m_energyTimer->setInterval(1000/10);
    m_energyTimer->start();

    // if snakes changes, the total amount changes
    connect(this, &Playground::snakesChanged, this, &Playground::totalSnakesSizeChanged);
    if(!m_snakes.isEmpty())
        for(const auto sn: m_snakes)
            connect(sn, &Snake::lengthChanged, this, &Playground::totalSnakesSizeChanged);
}

bool Playground::checkBounds(QPointF position) const
{
    return QVector2D(position).length() < m_size;
}

bool Playground::checkBounds(Snake *snake) const
{ return checkBounds(snake->position()); }

qreal Playground::consumeNearbyPearls(QPointF position, const Snake* eater)
{
    if(!eater) {
        return 0;
    }

    qreal amount = 0;

    // SLOW, maybe chunk system/adding all pearls & snakes to a quad tree first?
    for (int row = 0; row < m_energyPearls->rowCount(); ++row) {
        const auto index = m_energyPearls->index(row);
        const auto pearl = index.data(EnergyPearlListModel::DataRole).value<EnergyPearl>();

        // check if close enough to a pearl
        if (QVector2D{pearl.position - position}.length() > (eater->size() * 1.5) + pearl.amount)
            continue;

        amount += pearl.amount;
        m_energyPearls->remove(index);
    }

    if(amount)
        emit energyPearlsChanged();

    return amount * (m_masshacksActive ? 2 : 1);
}

void Playground::addSnake(Snake *snake)
{
    if (!snake)
        return;
    snake->setPlayground(this);
    m_snakes.append(snake);
    connect(snake, &Snake::sizeChanged, this, &Playground::totalSnakesSizeChanged);
    emit snakesChanged(m_snakes);
}

void Playground::killSnake(Snake *snake)
{
    if(!snake)
        return;
    if(!snake->isAlive())
        return;

    const int killedSnake = m_snakes.indexOf(snake);
    snake->disconnect(this);
    m_snakes.removeAt(killedSnake);
    emit snakesChanged(m_snakes);

    auto deathpearls = snake->die();
    for(auto p: deathpearls)
        m_energyPearls->add(p);
}

namespace {

qreal bounded(QRandomGenerator *rng, qreal lowest, qreal highest)
{
    return rng->bounded(highest - lowest) + lowest;
}

QPointF get(const Playground *pg)
{
    auto rng = QRandomGenerator::global();
    auto r = pg->size() * bounded(rng, .1, .8); // QPointF(0, 0) is error code.
    auto phi = 2 * M_PI * rng->bounded(1.);

    return QPointF{r * cos(phi), r * sin(phi)};
}

}

void Playground::spawnSnake()
{
    if(m_snakes.length() >= m_maxSnakeAmt)
        return;

    auto pos = get(this);
    auto dest = get(this);
    if(!checkBounds(pos)) {
        auto v = QVector2D(pos);
        v.normalize();
        v *= (m_size/4);

        pos = v.toPointF();
    }

    Snake *sn = new Snake(this);
    addSnake(sn); // if its an error its too expensive to generate, add, remove and delete the snake, so do it here.

    m_snakes.last()->spawn(pos, dest);
    emit snakesChanged(m_snakes);
}

void Playground::deleteSnake(Snake *snake)
{
    if(!snake)
        return;
    if(!snake->isAlive())
        return;

    const int killedSnake = m_snakes.indexOf(snake);
    snake->disconnect(this);
    m_snakes.removeAt(killedSnake);
    emit snakesChanged(m_snakes);
    snake->die();
}

EnergyPearl Playground::spawnPearl() const
{
    const auto rng = QRandomGenerator::global();

    // use polar coordinates to evenly place the pearls
    const auto r = m_size * pow(rng->generateDouble(), .5);
    const auto phi = 2 * M_PI * rng->generateDouble();

    // create pearl of random color, amount and position
    EnergyPearl pearl;
    pearl.amount = 1 + rng->generateDouble();
    pearl.color = QColor{rng->bounded(255), rng->bounded(255), rng->bounded(255)};
    pearl.position = QPointF{r * cos(phi), r * sin(phi)};
    return pearl;
}

EnergyPearl Playground::addPearl(QPointF position, qreal amount, QColor color, bool autoadd) const
{
    if(m_energyPearls->rowCount() > m_maxEnergyCount)
        return {};

    EnergyPearl pearl;
    pearl.amount = amount;
    pearl.color = color;
    pearl.position = position;
    if(autoadd)
        m_energyPearls->add(pearl);
    return pearl;
}

void Playground::energyBoost()
{
    const auto pearlAmount = m_energyPearls->rowCount();

    const auto toSpawn = m_maximumEnergy - pearlAmount;

    QList<EnergyPearl> existingPearls;
    existingPearls.reserve(m_energyPearls->rowCount());
    for(int i = 0; i < m_energyPearls->rowCount(); i++)
        existingPearls += m_energyPearls->at(i);

    QList<EnergyPearl> pearls;
    pearls.reserve(toSpawn);

    for (int i = 0; i < toSpawn; ++i)
        pearls += spawnPearl();

    existingPearls += pearls;

    m_energyPearls->reset(std::move(existingPearls));

    emit energyPearlsChanged();
}

void Playground::moveSnakes(qreal dt)
{
    auto clock = std::chrono::high_resolution_clock();
    auto start = clock.now();
    for(const auto sn: m_snakes) {
        if(!checkBounds(sn)) {
            deleteSnake(sn);
            continue;
        }
        if(sn->useBot()) {
            auto b = sn->bot();
            if(!b)
                continue;
            b->act(dt);
        }
        sn->move(dt);
        checkCrash(sn);
    }

    leaderboard()->reload();
    emit leaderboardChanged();

    auto end = clock.now();
    std::chrono::nanoseconds _diff = (end - start);
    qreal diff = _diff.count() * 1. / (std::nano::den / std::milli::den); // nano -> ms
    m_tickDelay = diff;
    /*qInfo() << "start:"   << start.time_since_epoch().count() <<
               "\tend:"   << end.time_since_epoch().count() <<
               "\tcount:" << _diff.count() <<
               "\tdiff:"  << diff << "ms" <<
               "\tdelay:" << m_tickDelay;*/
    emit tickDelayChanged();
}

Leaderboard *Playground::leaderboard() const
{
    return m_leaderboard;
}

qreal Playground::tickDelay() const
{
    return m_tickDelay;
}

} // namespace Slither

#include "leaderboard.h"
#include "leaderboard.h"
#include "moc_playground.cpp"
