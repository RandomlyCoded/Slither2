#ifndef SLITHER_PLAYGROUND_H
#define SLITHER_PLAYGROUND_H

#include "leaderboard.h"
#include "datamodel.h"
#include "snake.h"

#include <QAbstractListModel>
#include <QColor>
#include <QObject>
#include <QPointF>
#include <QTimer>

namespace Slither {

class Bot;
class Snake;
class Leaderboard;
class ChunkHandler;
class Chunk;

namespace Tests
{

class ChunkTest;

} // needs forward declaration to add a friend

struct EnergyPearl
{
    qreal amount;
    QPointF position;
    QColor color;

    Q_GADGET
    Q_PROPERTY(qreal amount MEMBER amount CONSTANT FINAL)
    Q_PROPERTY(QPointF position MEMBER position CONSTANT FINAL)
    Q_PROPERTY(QColor color MEMBER color CONSTANT FINAL)
};

class EnergyPearlListModel : public DataListModel<EnergyPearl>
{
    Q_OBJECT;

public:
    using DataListModel::DataListModel;
    EnergyPearlListModel(EnergyPearlListModel &other) { m_rows = other.m_rows; }
};

class SnakeListModel : public DataListModel<Snake *>
{
    Q_OBJECT;

public:
    using DataListModel::DataListModel;
    SnakeListModel(SnakeListModel &other) { m_rows = other.m_rows; }
};

class Playground : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal size READ size NOTIFY sizeChanged FINAL)
//    Q_PROPERTY(Slither::EnergyPearlListModel *energyPearls READ energyPearls NOTIFY energyPearlsChanged FINAL)
    Q_PROPERTY(QList<Slither::Snake *> snakes READ snakes NOTIFY snakesChanged FINAL)

    Q_PROPERTY(int snakeCount READ snakeCount NOTIFY snakesChanged FINAL)
    Q_PROPERTY(int pearlAmount READ pearlAmount NOTIFY energyPearlsChanged FINAL)
    Q_PROPERTY(int totalSnakesSize READ totalSnakesSize NOTIFY totalSnakesSizeChanged FINAL)

    Q_PROPERTY(Leaderboard *leaderboard READ leaderboard NOTIFY leaderboardChanged FINAL)

    Q_PROPERTY(qreal previousMspt READ previousMspt NOTIFY previousMsptChanged FINAL)

    Q_PROPERTY(QList<Chunk *> chunks READ chunks NOTIFY chunksUpdated FINAL)
    Q_PROPERTY(int chunksPerRow READ chunksPerRow FINAL)

public:
    Playground(QObject *parent = nullptr);

    qreal size() const noexcept { return m_size; }

    QList<Snake*> snakes() const { return m_snakes; };

    bool checkCrash(Snake *toCheck);

    int snakeCount() const { return m_snakes.count(); }

    int pearlAmount() const { return EnergyPearlCap; }

    int totalSnakesSize() const;

    constexpr static qreal boostSpeed  = 10;
    constexpr static qreal normalSpeed =  5;

    Leaderboard *leaderboard() const;

    qreal previousMspt() const;

    QList<Chunk *> &chunks() const;

    int chunksPerRow() const;

public slots:
    void initialize(qreal size);

    bool checkBounds(QPointF position) const;
    bool checkBounds(Snake *snake) const;

    qreal consumeNearbyPearls(QPointF position, const Snake *eater = nullptr);

    void addSnake(Snake *snake);
    void killSnake(Snake *snake);
    void spawnSnake();

    EnergyPearl spawnPearl() const;
    static EnergyPearl createPearl(QPointF position = {0, 0}, qreal amount = 1, QColor color = "#7f7f7f");

    void addPearl(const EnergyPearl &p);

    QColor rgbFromHue(int hue) { return QColor::fromHsv(hue, 100, 100); }

    void save();
    void load();
    void load(QString filename);

    void tick(qreal dt);

    int indexOfSnake(Slither::Snake *snake) { return m_snakes.indexOf(snake); }

    QList<QPointF> scaleToZoom(QList<QPointF> positions, int zoom)
    { QList<QPointF> ret; for(const auto &pos: positions) ret += (pos * zoom); return ret; }

signals:
    void sizeChanged(qreal size);
    void snakesChanged(QList<Slither::Snake *> snakes);

    void colorChanged();

    void energyPearlsChanged();

    void totalSnakesSizeChanged();

    void leaderboardChanged();

    void previousMsptChanged();

    void chunksUpdated();

private:
    void energyBoost();
    void initializeChunkGrid();
    void setSize(qreal newSize) { m_size = newSize; }

    int countEnergyPearls();

    Playground(qreal size, QObject *parent = nullptr);

    qreal m_size = 100;

    QTimer *m_newSnakeTimer = new QTimer{this};
    QTimer *m_energyTimer = new QTimer{this};
    QTimer *m_tickTimer = new QTimer{this};

    QPointer<Leaderboard> m_leaderboard;
    QList<Snake*> m_snakes;
    ChunkHandler *const m_chunkHandler;

    int m_maximumEnergy;
    int m_maxSnakeAmt = 12; // 1 for debug, normally 12 works fine

    qreal m_lastMspt;

    constexpr static int EnergyPearlCap = 4000; // how many energy pearls can exist untill we do not add more from dying snakes or sprinting

    friend class Tests::ChunkTest;
};


} // namespace Slither

Q_DECLARE_METATYPE(QList<Slither::EnergyPearl>)
Q_DECLARE_METATYPE(QList<Slither::Snake>)

#endif // SLITHER_PLAYGROUND_H
