#ifndef CHUNK_H
#define CHUNK_H

#include <QList>
#include <QPoint>


namespace Slither
{

class SnakeListModel;
class EnergyPearlListModel;
struct EnergyPearl;
class Snake;

class Chunk
{
public:
    explicit Chunk(QPoint coords);

    bool maybeAddSnake(Snake *sn);
    bool maybeAddPearl(EnergyPearl &p);
    qreal consumeNearbyPearls(const Snake *sn);

    bool checkBounds(const QPointF &pos);

    constexpr static int ChunkSize = 16;

private:
    QPoint m_coords;

    SnakeListModel *m_snakes;
    EnergyPearlListModel *m_energyPearls;
};

class ChunkHandler
{
public:
    void init(int chunksEachDir);

    Chunk &chunkAt(int x, int y) { return chunk(x / Chunk::ChunkSize, y / Chunk::ChunkSize); }
    Chunk &chunk(int cx, int cy);

private:
    void reset();

    QList<QList<Chunk>> m_chunks;
};

} // namespace Slither

#endif // CHUNK_H
