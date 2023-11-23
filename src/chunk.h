#ifndef CHUNK_H
#define CHUNK_H

#include <QList>
#include <QObject>
#include <QPoint>


namespace Slither
{

class Playground;
class SnakeListModel;
class EnergyPearlListModel;
struct EnergyPearl;
class Snake;

class Chunk : public QObject
{
    Q_OBJECT

public:
    explicit Chunk(QPoint coords);

    Chunk (const Chunk &rhs);
    Chunk (Chunk &&rhs);
    Chunk &operator= (const Chunk &rhs);

    bool maybeAddSnake(Snake *sn);
    bool maybeAddPearl(const EnergyPearl &p);
    qreal consumeNearbyPearls(const Snake *sn);

    bool checkBounds(const QPointF &pos) const;
    bool checkY(const qreal y) const;
    bool checkX(const qreal x) const;

    constexpr static int ChunkSize = 16;

    QPoint coords() const
    { return m_coords; }

    EnergyPearlListModel *energyPearls() const
    { return m_energyPearls; }

private:
    QPoint m_coords;

    SnakeListModel *m_snakes;
    EnergyPearlListModel *m_energyPearls;
};

class ChunkHandler : public QObject
{
    Q_OBJECT

public:
    ChunkHandler(Playground *pg);

    void init(int chunksEachDir);

    auto &chunks() { return m_chunks; }
    Chunk &findChunk(QPointF pos);

    bool tryAdd(EnergyPearl p);
    bool tryAdd(Snake *s);

    EnergyPearlListModel *bufferedPearls() { return m_bufferedPearls; }
    void updateBufferedPearls();

private:
    void reset();

    QList<QList<Chunk>> m_chunks;

    Playground *m_playground;

    int offset;

    EnergyPearlListModel *m_bufferedPearls;
};

} // namespace Slither

#endif // CHUNK_H
