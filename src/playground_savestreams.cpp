#include "playground.h"
#include "snake.h"

#include "../Botdefs/aibot.h"
#include "../Botdefs/neuralnet.h"

#include <QDebug>
#include <QFile>

namespace Slither {

QDataStream &operator<<(QDataStream &ds, Snake::ImportantData &d)
{
    ds << d.segC;
    //we can't print the segments as a list due to how QList<T> is written :( so in a for-loop

    for(auto p: d.segs)
        ds << p;

    ds << d.dest << d.botType;

    return ds;
}

QDataStream &operator>>(QDataStream &ds, Snake::ImportantData &d)
{
    ds >> d.segC;

    d.segs.clear();
    d.segs.reserve(d.segC);
    QPointF seg;
    for(int i = 0; i < d.segC; i++) {
        ds >> seg;
        d.segs.append(seg);
    }

    ds >> d.dest;

    ds >> d.botType;

    return ds;
}

QDataStream &operator<<(QDataStream &ds, EnergyPearl &p)
{
    ds << p.position;
    ds << p.amount;
    ds << p.color;

    return ds;
}

QDataStream &operator>>(QDataStream &ds, EnergyPearl &p)
{
    ds >> p.position;
    ds >> p.amount;
    ds >> p.color;

    return ds;
}

QDataStream &operator<<(QDataStream &ds, NeuralNet *n)
{
    return ds;
}

QDataStream &operator>>(QDataStream &ds, NeuralNet *n)
{
    return ds;
}

void Playground::save()
{
    QFile saveFile{"data/game.slgame", this};

    if(!saveFile.open(QFile::WriteOnly)) {
        qWarning() << saveFile.fileName() << ":" << saveFile.errorString();
        return;
    }

    QDataStream stream{&saveFile};

//    QTextStream stream{&saveFile};


    // write the size of the playground(8 bytes, a 64-bit-integer)
    stream << m_size;
    // now write the snakes:
    // first of all the amount of snakes
    stream << m_snakes.count();

    Snake::ImportantData sD;
    for(auto *sn: m_snakes) {
        sD = Snake::ImportantData(sn); // so we are up-to-date
        stream << sD;
    }

    // now the enery pearls
    // write the amount
    stream << m_energyPearls->rowCount();
    for(auto &p: *m_energyPearls)
        stream << p;

    /*
     *    8 bytes playground size
     *
     * +snakes: (at least 41 bytes each)
     *    8 bytes size(means amount of segments)
     *    1 byte  botType
     *  +16 bytes destination
     *  +16 bytes segments (for each segment)
     *
     * +energy pearls: (40 bytes each)
     *   16 bytes position
     *  + 8 bytes value
     *  +16 Bytes color
     */
}

void Playground::load()
{
    load("data/game.slgame");
}

void Playground::load(QString filename)
{
    if(filename.isEmpty())
        return;
    QFile saveFile{filename, this};

    if(!saveFile.open(QFile::ReadOnly)) {
        qWarning() << saveFile.fileName() << ":" << saveFile.errorString();
        return;
    }

    QDataStream stream{&saveFile};

    m_snakes.clear();
    m_energyPearls->reset();

    stream >> m_size;

    qsizetype snakeAmt;
    stream >> snakeAmt;

    Snake::ImportantData sD;
    for(int i = 0; i < snakeAmt; i++) {
        stream >> sD;
        Snake * s = new Snake(sD);
        addSnake(s);
    }

    qsizetype ePAmt;
    stream >> ePAmt;

    for(int i = 0; i < ePAmt; i++) {
        EnergyPearl p;
        stream >> p;
        m_energyPearls->add(p);
    }

    emit snakesChanged(m_snakes);
    emit energyPearlsChanged();
    emit sizeChanged(m_size);
}

} // namespace Slither
