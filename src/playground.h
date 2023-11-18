#ifndef SLITHER_PLAYGROUND_H
#define SLITHER_PLAYGROUND_H

#include "leaderboard.h"
#include "datamodel.h"
#include "chunk.h"

#include <QAbstractListModel>
#include <QColor>
#include <QObject>
#include <QPointF>
#include <QTimer>

namespace Slither {

class Bot;
class Snake;
class Leaderboard;

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
    Q_PROPERTY(Slither::EnergyPearlListModel *energyPearls READ energyPearls NOTIFY energyPearlsChanged FINAL)
    Q_PROPERTY(QList<Slither::Snake *> snakes READ snakes NOTIFY snakesChanged FINAL)

    Q_PROPERTY(int snakeCount READ snakeCount NOTIFY snakesChanged FINAL)
    Q_PROPERTY(int pearlAmount READ pearlAmount NOTIFY energyPearlsChanged FINAL)
    Q_PROPERTY(int totalSnakesSize READ totalSnakesSize NOTIFY totalSnakesSizeChanged FINAL)

    Q_PROPERTY(bool masshacks READ masshacks NOTIFY masshacksChanged FINAL)

    Q_PROPERTY(Leaderboard *leaderboard READ leaderboard NOTIFY leaderboardChanged FINAL)

    Q_PROPERTY(qreal tickDelay READ tickDelay NOTIFY tickDelayChanged FINAL)

public:
    Playground(QObject *parent = nullptr);
    qreal size() const noexcept { return m_size; }
    EnergyPearlListModel *energyPearls() const { return m_energyPearls; }
    QList<Snake*> snakes() const { return m_snakes; };

    bool checkCrash(Snake *toCheck);

    int snakeCount() const { return m_snakes.count(); }

    int pearlAmount() const { return m_energyPearls->rowCount(); }

    int totalSnakesSize() const;

    constexpr const static qreal boostSpeed  = 10;
    constexpr const static qreal normalSpeed =  5;

    bool checkBounds(Snake *snake) const;

    bool masshacks() const { return m_masshacksActive; }

    Leaderboard *leaderboard() const;

    qreal tickDelay() const;

public slots:
    void tp()
    {
        for(auto &p: *m_energyPearls)
            p.position = QPointF(0, 0);
    }
    void switch_masshacks() { m_masshacksActive ^= 1; }
    void initialize(qreal size);

    bool checkBounds(QPointF position) const;
    qreal consumeNearbyPearls(QPointF position, const Snake *eater = nullptr);

    void addSnake(Snake *snake);
    void killSnake(Snake *snake);
    void spawnSnake();
    void deleteSnake(Snake *snake);

    EnergyPearl spawnPearl() const;
    EnergyPearl addPearl(QPointF position = {0, 0}, qreal amount = 1, QColor color = "#7f7f7f",
                         bool autoadd = false) const;

    QColor color(int H) { return QColor::fromHsv(H, 100, 100); }

    void save();
    void load();
    void load(QString filename);

    void moveSnakes(qreal dt);

    int indexOfSnake(Slither::Snake *snake) { return m_snakes.indexOf(snake); }

    QList<QPointF> zoomOut(QList<QPointF> positions, int zoom)
    { QList<QPointF> ret; for(const auto &pos: positions) ret += (pos * zoom); return ret; }

signals:
    void sizeChanged(qreal size);
    void snakesChanged(QList<Slither::Snake *> snakes);

    void colorChanged();

    void energyPearlsChanged();

    void totalSnakesSizeChanged();

    void masshacksChanged();

    void leaderboardChanged();

    void tickDelayChanged();

private:
    void energyBoost();
    void initializeChunkGrid();

    qreal m_size = 100;

    QTimer *m_newSnakeTimer = new QTimer{this};
    QTimer *m_energyTimer = new QTimer{this};
    QTimer *m_tickTimer = new QTimer{this};

    QPointer<Leaderboard> m_leaderboard;
    QList<Snake*> m_snakes;
    EnergyPearlListModel *const m_energyPearls = new EnergyPearlListModel{this};
    ChunkHandler *const m_chunkHandler = new ChunkHandler{};

    int m_maximumEnergy;
    int m_maxSnakeAmt = 12; // 1 for debug, normally 12 works fine
    int m_pearlAmount;
    int m_snakeCount;

    qreal m_tickDelay;

    const int m_maxEnergyCount = 4000; // how many energy pearls can exist untill we do not add more from dying snakes or sprinting

    bool m_masshacksActive = 0;
};


} // namespace Slither

Q_DECLARE_METATYPE(QList<Slither::EnergyPearl>)
Q_DECLARE_METATYPE(QList<Slither::Snake>)

#endif // SLITHER_PLAYGROUND_H
