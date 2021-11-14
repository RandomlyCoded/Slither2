#include "playground.h"

#include "snake.h"
#include "bot.h"

#include <QRandomGenerator>
#include <QVector2D>
#include <QDebug>
#include <QFile>

namespace Slither {

namespace {

}

void Playground::checkCrash(Snake *toCheck)
{
    for(const auto check: m_snakes) {
        if(check == toCheck)
            continue;

        for(const auto checkedTile: check->segments()) {
            if(toCheck->position() == checkedTile) {
                qInfo("YOU CHRASHED");
                killSnake(toCheck);
                return;
            }
        }
    }
}

void Playground::initialize(qreal size)
{
    // update playground size if needed
    if (size == 0)
        size = m_size;
#ifdef QT_DEBUG // Debug: have a smaller playground for faster Debug-gameplay
    size = 100;
#endif
    if (!qFuzzyCompare(m_size, size)) {
        m_size = size;
        emit sizeChanged(m_size);
    }

    // number of pearls we generate depends on the playground's area
    const auto area = M_PI * size * size;
    const auto count = qRound(area / (m_size * 3/4));

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
    m_newSnakeTimer->setInterval(10000); // oder immer wenn eine Stirbt spawnt eine neue? dann müssten wir aber hier einen for-loop einbauen
//    m_newSnakeTimer->start();

    // starting timer for new Energy Pearls
    connect(m_energyTimer, &QTimer::timeout, this, &Playground::energyBoost);
    m_energyTimer->setInterval(1000/30);
    m_energyTimer->start();

    // if snakes changes, the total amount changes
    connect(this, &Playground::snakesChanged, this, &Playground::totalSnakesSizeChanged);
    if(!m_snakes.isEmpty())
        for(const auto sn: m_snakes)
            connect(sn, &Snake::lenghtChanged, this, &Playground::totalSnakesSizeChanged);
}

bool Playground::checkBounds(QPointF position) const
{
    auto vector = QVector2D(position);
    return vector.length() < m_size;
}

bool Playground::checkBounds(Snake *snake) const
{
    auto vector = QVector2D(snake->position());
    return (vector.length() + snake->size()) < m_size;
}

qreal Playground::consumeNearbyPearls(QPointF position, const Snake* eater)
{
    bool hadToAddSnake = false;
    if(eater == nullptr) {
        eater = new Snake{this};
        hadToAddSnake = true;
    }

    qreal amount = 0;

    for (int row = 0; row < m_energyPearls->rowCount(); ++row) {
        const auto index = m_energyPearls->index(row);
        const auto pearl = index.data(EnergyPearlListModel::DataRole).value<EnergyPearl>();

        // check if close enough towards an pearl
        if (QVector2D{pearl.position - position}.length() > (eater->size() * 1.5) + pearl.amount)
            continue;

        amount += pearl.amount;
        m_energyPearls->remove(index);
    }

    if(hadToAddSnake)
        delete eater;
    if(amount)
        emit energyPearlsChanged();
    return amount;
}

void Playground::addSnake(Snake *snake)
{
    if (snake)
        snake->setPlayground(this);
    m_snakes.append(snake);
    connect(snake, &Snake::sizeChanged, this, &Playground::totalSnakesSizeChanged);
    emit snakesChanged(m_snakes);
    qInfo() << m_snakes.count();
}

void Playground::killSnake(Snake *snake)
{
    const int killedSnake = m_snakes.indexOf(snake);
    m_snakes.removeAt(killedSnake);
    snake->die();
    disconnect(snake, &Snake::sizeChanged, this, &Playground::totalSnakesSizeChanged);
//    m_deathSnakes.append(snake);
    emit snakesChanged(m_snakes);
}

void Playground::spawnSnake()
{
    if(m_snakes.length() >= 3)
        return;

    qInfo("adding Snake...");
    addSnake(new Snake);
    m_snakes.last()->spawn({0, 0}, {1, 1});
    emit snakesChanged(m_snakes);
    qInfo() << "SIZE OF \"m_snakes\":" << m_snakes.count();
}

EnergyPearl Playground::spawnPearl() const
{
    const auto rng = QRandomGenerator::global();

    // use polar coordinates to evenly place the pearls
    const auto r = m_size * pow(rng->generateDouble(), 1);
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
    for(const auto sn: m_snakes) {
        if(!checkBounds(sn)) {
            killSnake(sn);
            continue;
        }
        if(sn->useBot())
            sn->bot()->act(dt);
        sn->move(dt);
    }

//    moveDeathSnakes();
}
/*
void Playground::moveDeathSnakes()
{
    for(auto sn: m_deathSnakes)
        for (int i = 1; i < sn->segments().count(); ++i)
            sn->segments()[i] += (QVector2D{sn->segments()[i] - sn->segments()[i - 1]}).toPointF();
}*/

} // namespace Slither

#include "moc_playground.cpp"
