#include "playground.h"

#include "../Botdefs/bot.h"

#include "snake.h"
#include "leaderboard.h"
#include "chunk.h"

#include <QRandomGenerator>
#include <QVector2D>
#include <QDebug>
#include <QFile>
#include <QVector2D>

namespace Slither {

namespace {

qreal bounded(QRandomGenerator *rng, qreal lowest, qreal highest)
{
    return rng->bounded(highest - lowest) + lowest;
}

QPointF get(const Playground *pg)
{
    auto rng = QRandomGenerator::global();
    auto r = pg->size() * bounded(rng, .1, .8);
    auto phi = 2 * M_PI * rng->bounded(1.);

    return QPointF{r * cos(phi), r * sin(phi)};
}

} // namespace

Playground::Playground(QObject *parent)
    : Playground(100, parent)
{
    initialize(0);
}

Playground::Playground(qreal size, QObject *parent)
    : QObject(parent)
    , m_snakes({})
    , m_leaderboard(new Leaderboard(this))
    , m_chunkHandler(new ChunkHandler{this})
{}

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

    initializeChunkGrid();

    m_maximumEnergy = count;

    // let's create new pearls
    for (int i = 0; i < count; ++i) {
        auto pearl = spawnPearl();

        m_chunkHandler->tryAdd(pearl);
    }

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

    // start timer for ticking the playground
    connect(m_tickTimer, &QTimer::timeout, [this] {this->tick(0.1); });
    m_tickTimer->setInterval(5);
    m_tickTimer->start();

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

    return m_chunkHandler->findChunk(position)->consumeNearbyPearls(eater);
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
        addPearl(p);
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

EnergyPearl Playground::createPearl(QPointF position, qreal amount, QColor color)
{
    EnergyPearl pearl;
    pearl.amount = amount;
    pearl.color = color;
    pearl.position = position;
    return pearl;
}

void Playground::addPearl(const EnergyPearl &p)
{
    m_chunkHandler->findChunk(p.position)->maybeAddPearl(p);
}

void Playground::energyBoost()
{
    const auto pearlAmount = countEnergyPearls();

    const auto toSpawn = m_maximumEnergy - pearlAmount;

    for (int i = 0; i < toSpawn; ++i)
        addPearl(spawnPearl());

    emit energyPearlsChanged();
}

void Playground::initializeChunkGrid()
{
    const int chunkAmt = qCeil(m_size / Chunk::ChunkSize);

    qInfo() << "initializing" << chunkAmt << "chunks";

    m_chunkHandler->init(chunkAmt);
}

int Playground::countEnergyPearls()
{
    int count = 0;

    for(const auto c: m_chunkHandler->chunks())
        count += c->energyPearls()->rowCount();

    return count;
}

void Playground::tick(qreal dt)
{
    auto clock = std::chrono::high_resolution_clock();
    auto start = clock.now();

    for (const auto sn: m_snakes) {
        if (!checkBounds(sn)) {
            killSnake(sn);
            continue;
        }

        if (sn->useBot()) {
            auto b = sn->bot();
            if (!b)
                continue;
            b->act(dt);
        }

        sn->move(dt);
        checkCrash(sn);
    }

    leaderboard()->reload();
    emit leaderboardChanged();

   emit chunksUpdated();

    auto end = clock.now();
    std::chrono::nanoseconds _diff = (end - start);
    qreal diff = _diff.count() * 1. / (std::nano::den / std::milli::den); // nano -> ms

    m_lastMspt = diff;
    emit previousMsptChanged();
}

Leaderboard *Playground::leaderboard() const
{
    return m_leaderboard;
}

qreal Playground::previousMspt() const
{
    return m_lastMspt;
}

QList<Chunk *> &Playground::chunks() const
{
    return m_chunkHandler->chunks();
}

int Playground::chunksPerRow() const
{
    return m_chunkHandler->chunks().size();
}

} // namespace Slither

#include "chunk.h"
#include "leaderboard.h"
#include "leaderboard.h"
#include "moc_playground.cpp"
