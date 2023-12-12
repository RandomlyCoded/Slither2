#include "chunk.h"

#include "snake.h"

namespace Slither {

Chunk::Chunk(QPoint coords)
{
    m_coords = coords;
    m_snakes = new SnakeListModel;
    m_energyPearls = new EnergyPearlListModel;
}

Chunk::Chunk(const Chunk &rhs)
{
    m_coords = rhs.m_coords;
    m_snakes = rhs.m_snakes;
    m_energyPearls = rhs.m_energyPearls;
}

Chunk::Chunk(Chunk &&rhs)
    : m_coords(rhs.m_coords)
    , m_snakes(rhs.m_snakes)
    , m_energyPearls(rhs.m_energyPearls)
{}

Chunk &Chunk::operator=(const Chunk &rhs)
{
    m_coords = rhs.m_coords;
    m_snakes = rhs.m_snakes;
    m_energyPearls = rhs.m_energyPearls;

    return *this;
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

bool Chunk::maybeAddPearl(const EnergyPearl &p)
{
    if(checkBounds(p.position)) {
        m_energyPearls->add(p);
        return true;
    }

    return false;
}

qreal Chunk::consumeNearbyPearls(const Snake *sn)
{
    qreal amount = 0;

    QList<EnergyPearl> newPearls;

    for (int row = 0; row < m_energyPearls->rowCount(); ++row) {
        const auto index = m_energyPearls->index(row);
        auto pearl = index.data(EnergyPearlListModel::DataRole).value<EnergyPearl>();

        // check if close enough to a pearl, skip pearl if yes
        if (!(QVector2D{pearl.position - sn->position()}.length() > (sn->size() * 1.5) + pearl.amount))
            continue;

        amount += pearl.amount;

        newPearls.append(std::move(pearl));
    }

    m_energyPearls->reset(std::move(newPearls));

//    emit energyPearlsChanged();

    return amount;
}

bool Chunk::checkBounds(const QPointF &pos) const
{
    return checkY(pos.y()) && checkX(pos.x());
}

bool Chunk::checkY(const qreal y) const
{
    auto lowerBound = (m_coords.y() * ChunkSize);
    bool lowerOk = y >= lowerBound;

    auto upperBound = lowerBound + ChunkSize;
    bool upperOk = y < upperBound;

    return lowerOk && upperOk;
}

bool Chunk::checkX(const qreal x) const
{
    return x >= (m_coords.x() * ChunkSize) && x < ((m_coords.x() * ChunkSize) + ChunkSize);
}

ChunkHandler::ChunkHandler(Playground *pg)
    : m_playground (pg)
{
    offset = m_playground->size() / Chunk::ChunkSize;
}

void ChunkHandler::init(int chunksEachDir)
{
    reset();

    offset = chunksEachDir;

    chunksPerRow = chunksEachDir * 2;

    for(int y = -chunksEachDir; y <= chunksEachDir; ++y)
        for(int x = -chunksEachDir; x <= chunksEachDir; ++x)
            m_chunks.append(new Chunk {QPoint{x, y}});
}

Chunk *ChunkHandler::findChunk(QPointF pos)
{
    for(auto c: m_chunks) {
        if(c->checkBounds(pos))
            return c;
    }

    // did not find chunk
    return nullptr;
}

bool ChunkHandler::tryAdd(EnergyPearl p)
{
    return findChunk(p.position)->maybeAddPearl(p);
}

bool ChunkHandler::tryAdd(Snake *s)
{
    return findChunk(s->position())->maybeAddSnake(s);
}

//bool ChunkHandler::exits(int cx, int cy) const
//{
//    const auto x = cx + offset;
//    const auto y = cy + offset;
//    return (x > 0 && x < m_chunks[0].size())
//           && (y > 0 && y < m_chunks.size());
//}

void ChunkHandler::reset()
{
    m_chunks.clear();
}

} // namespace Slither
