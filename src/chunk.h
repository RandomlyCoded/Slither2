#ifndef CHUNK_H
#define CHUNK_H

#include "playground.h"

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

    Q_PROPERTY(QPoint coords READ coords CONSTANT FINAL)
    Q_PROPERTY(EnergyPearlListModel *energyPearls READ energyPearls NOTIFY energyPearlsChanged FINAL)

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

    QPoint coords() const
    { return m_coords; }

    EnergyPearlListModel *energyPearls() const
    { return m_energyPearls; }

    constexpr static int ChunkSize = 16;

public slots:
    int chunkSize() const { return ChunkSize; }

signals:
    void energyPearlsChanged();

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
    Chunk *findChunk(QPointF pos);

    bool tryAdd(EnergyPearl p);
    bool tryAdd(Snake *s);

private:
    void reset();

    QList<Chunk *> m_chunks;

    Playground *m_playground;

    int offset;
    int chunksPerRow;
};

} // namespace Slither

Q_DECLARE_METATYPE(QList<Slither::Chunk *>)

#endif // CHUNK_H
