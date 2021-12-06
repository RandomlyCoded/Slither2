#include "loader.h"

#include <QFile>
#include <QDebug>
#include <QTextStream>

namespace PendantLoad {

Loader::Loader(QObject *parent) : QObject(parent)
{
    m_possiblePendants.append("Burger");
    m_possiblePendants.append("Cherry");
    m_possiblePendants.append("Ice-Creme");
    m_possiblePendants.append("Minecraft");
    m_possiblePendants.append("The Legend of Zelda: Ocarina of Time");
    m_possiblePendants.append("Pommes Frites");
    m_possiblePendants.append("Triforce");
    m_possiblePendants.append("Zelda");
    m_possiblePendants.append("Userdefined");
}

void Loader::getPendant(QString path)
{
    QString __path = "../../../../data/pendant.slData";
    QFile file{__path};
    file.remove();
    if(!file.open(QFile::ReadWrite)) {
        qWarning() << file.fileName() << ":" << file.errorString();
        return;
    }
    QTextStream stream{&file};

    QString write;
    write = path;
    if(path == "The Legend of Zelda: Ocarina of Time")
        write = "OoT";
    else if(path == "Pommes Frites")
        write = "PommesFrites";
    write += ".svg";
    stream << write;

    qInfo() << stream.readAll();
    file.close();
    emit previewChanged();
}

int Loader::count()
{
    return m_possiblePendants.count();
}

QUrl Loader::getUrl(QString pathInput)
{
    return QUrl{pathInput};
}

QString Loader::getPossiblePendant(int at)
{
    if(at >= count())
        return "";
    return m_possiblePendants.at(at);
}

QString Loader::preview() const
{
    QFile f{"../../../../data/pendant.slData"};
    if(!f.open(QFile::ReadOnly)) {
        qWarning() << f.fileName() << ":" << f.errorString();
        return "";
    }

    QTextStream s{&f};
    return "pendants/" + f.readAll();
}

} // namespace PendantLoad
