#include "playground.h"

#include "snake.h"

#include <QRandomGenerator>
#include <QVector2D>

#include <cmath>

#include <iostream>

namespace Slither {

namespace {

}

void Playground::checkCrash(Snake *toCheck)
{
    for(const auto *check: m_snakes) {
        std::cout << check;
        if(check == toCheck) {
            qInfo("that was only you...");
            continue;
        }

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

    if (!qFuzzyCompare(m_size, size)) {
        m_size = size;
        emit sizeChanged(m_size);
    }

    // number of pearls we generate depends on the playground's area
    const auto area = M_PI * size * size;
    const auto count = qRound(area / 750);

    // let's create new pearls
    QList<EnergyPearl> pearls;
    pearls.reserve(count);

    for (int i = 0; i < count; ++i)
        pearls += spawnPearl();

    m_energyPearls->reset(std::move(pearls));

    // starting timer for new Snakes
//    connect(m_newSnakeTimer, &QTimer::timeout, this, &Playground::spawnSnake);
//    m_newSnakeTimer->setInterval(1000); // oder immer wenn eine Stirbt spawnt eine neue?
//    m_newSnakeTimer->start();
}

bool Playground::checkBounds(QPointF position) const
{
    return false;
}

qreal Playground::consumeNearbyPearls(QPointF position)
{
    qreal amount = 0;

    for (int row = 0; row < m_energyPearls->rowCount(); ++row) {
        const auto index = m_energyPearls->index(row);
        const auto pearl = index.data(EnergyPearlListModel::DataRole).value<EnergyPearl>();

        // check if close enough towards an pearl
        if (QVector2D{pearl.position - position}.length() > 0.5 + pearl.amount)
            continue;

        amount += pearl.amount;

        m_energyPearls->setData(index, QVariant::fromValue(spawnPearl()));
    }

    return amount;
}

void Playground::addSnake(Snake *snake)
{
    qInfo("add new Snake");
    if (snake)
        snake->setPlayground(this);
//    m_snakes.append(snake);
}

void Playground::killSnake(Snake *snake)
{
    snake->die();
    const int killedSnake = m_snakes.indexOf(snake);
    m_snakes.removeAt(killedSnake);
}

void Playground::spawnSnake()
{
    qInfo("adding Snake... nee...wird doch nicht ausgeführt...");
//    addSnake(new Snake);
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

} // namespace Slither
