#include "chunk.h"

#include "snake.h"
#include "playground.h"

namespace Slither {

Chunk::Chunk(QPoint coords)
{
    m_coords = coords;
    m_snakes = new SnakeListModel;
    m_energyPearls = new EnergyPearlListModel;
}

bool Chunk::maybeAddSnake(Snake *sn)
{
    const auto chunkCoords = sn->position() / ChunkSize;

    if(checkBounds(chunkCoords)) {
        m_snakes->add(sn);
        return true;
    }

    return false;
}

bool Chunk::maybeAddPearl(EnergyPearl &p)
{
    const auto chunkCoords = p.position / ChunkSize;

    if(checkBounds(chunkCoords)) {
        m_energyPearls->add(p);
        return true;
    }

    return false;
}

qreal Chunk::consumeNearbyPearls(const Snake *sn)
{
    Q_ASSERT_X(false, Q_FUNC_INFO, "not implemented yet");

    return -1;
}

bool Chunk::checkBounds(const QPointF &pos)
{
    return (pos.x() > m_coords.x() && pos.x() < m_coords.x() + ChunkSize)
           && (pos.y() > m_coords.y() && pos.y() < m_coords.y() + ChunkSize);
}

void ChunkHandler::init(int chunksEachDir)
{
    reset();
    for(int y = 0; y < chunksEachDir; ++y) {
        QList<Chunk> row;

        for(int x = 0; x < chunksEachDir; ++x)
            row.emplaceBack(QPoint{x, y});

        m_chunks.append(row);
    }
}

Chunk &ChunkHandler::chunk(int cx, int cy)
{
    return m_chunks[cy][cx];
}

void ChunkHandler::reset()
{
    m_chunks.clear();
}

} // namespace Slither