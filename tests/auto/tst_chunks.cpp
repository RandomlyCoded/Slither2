#include <QTest>

#include "../../src/chunk.h"
#include "../../src/playground.h"
#include "../../src/snake.h" // needs include due to Playground having a Snake * Q_PROPERTY

using namespace std::chrono_literals;

namespace Slither::Tests {

class ChunkTest : public QObject
{
    Q_OBJECT

private slots:
    void chunks_data()
    {
        QTest::addColumn<ChunkHandler *>("handler");
        QTest::addColumn<QPointF>("point");
        QTest::addColumn<QPoint>("chunkPos");

        auto pg = new Playground(Chunk::ChunkSize * 2, this);

        pg->initializeChunkGrid();

        auto handler = pg->m_chunkHandler;

        QTest::addRow("0/0") << handler << QPointF {0, 0} << QPoint {0, 0};
        QTest::addRow("1/1") << handler << QPointF {Chunk::ChunkSize + 0.1, Chunk::ChunkSize + 0.2} << QPoint {1, 1};
        QTest::addRow("-1/0") << handler << QPointF {-0.3, 0.4} << QPoint {-1, 0};
    }

    void chunks()
    {
        QFETCH(ChunkHandler *, handler);
        QFETCH(QPointF, point);
        QFETCH(QPoint, chunkPos);

        qInfo() << point << chunkPos;

        QCOMPARE(handler->findChunk(point)->coords(), chunkPos);
    }
};

} // namespace Slither::Tests

QTEST_MAIN(Slither::Tests::ChunkTest)

#include "tst_chunks.moc"
