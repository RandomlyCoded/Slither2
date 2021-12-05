#include "playground.h"

#include <QFile>

namespace Slither {

QTextStream &operator <<(QTextStream &ts, QList<Slither::Snake*> snakes)
{
    return ts;
}
QTextStream &operator >>(QTextStream &ts, QList<Slither::Snake*> snakes)
{
    return ts;
}

QTextStream &operator <<(QTextStream &ts, EnergyPearlListModel *epL)
{
    return ts;
}
QTextStream &operator >>(QTextStream &ts, EnergyPearlListModel *epL)
{
    return ts;
}

void Playground::save()
{
    qInfo() << "start saving";
    QFile saveFile{"../../data/game.slgame", this};

    if(!saveFile.open(QFile::WriteOnly)) {
        qWarning() << saveFile.fileName() << ":" << saveFile.errorString();
        return;
    }

    QTextStream stream{&saveFile};
    stream << m_energyPearls << Qt::endl;
    stream << m_snakes;
    stream << m_size;
}

void Playground::load()
{
    load("../../data/game.slgame");
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

    QTextStream stream{&saveFile};

    stream >> m_size;
    stream >> m_energyPearls;
    m_snakes.clear();
    stream >> m_snakes;

    emit snakesChanged(m_snakes);
    emit energyPearlsChanged();
    emit sizeChanged(m_size);
}

} // namespace Slither
