#include "editor.h" // Source file for functions where Data streams are needed.

#include <QFile>
#include <QDebug>
#include <QProcess>

namespace Editor {

void Data::save()
{
    QFile saveFile{"../../../data/Skin.slData"};

    if (!saveFile.open(QFile::WriteOnly)) {
        qWarning() << saveFile.fileName() << ": " << saveFile.errorString();
        return;
    }

    QTextStream saveStream{&saveFile};
    for(const auto &color: m_skin) {
        QString save = color.name(QColor::HexRgb);
        saveStream << save << Qt::endl;
    }
    qInfo() << "finished saving skin:" << saveFile.fileName();
}

void Data::getColor(int *r, int *g, int *b) // doesn't work :.( ...newColor.exe chrashes...
{
    QFile *loadFile = new QFile{"../colors.tmpSlData"};

    if (!loadFile->open(QFile::ReadOnly)) {
        qWarning() << loadFile->fileName() << ": " << loadFile->errorString();
        return;
    }

    QTextStream *read = new QTextStream{loadFile};
    QList<QString> *hexRGBIn = new QList<QString>;

    for(int i = 0; i < 3; i++) {
        QString str = read->readLine(4);
        if(str.length() == 3)
            hexRGBIn->append(str);
        else if(str.length() == 2)
            hexRGBIn->append(QString::fromStdString("0") + str);
        else if(str.length() == 1)
            hexRGBIn->append(QString::fromStdString("00") + str);
        else if(str.length() == 0) {
            hexRGBIn->append(QString::fromStdString("000") + str);
            qWarning() << "color" << i << "has lenght 0. Set color to \"000\"";
        }
    }
    qInfo() << "hexRGBIn:" << *hexRGBIn;

    // convert into 3 hexadecimal strings
    QString strR = hexRGBIn->first();
    hexRGBIn->removeFirst();
    QString strG = hexRGBIn->first();
    hexRGBIn->removeFirst();
    QString strB = hexRGBIn->first();
    hexRGBIn->removeFirst();

    delete hexRGBIn;

    qInfo() << "r:" << strR << ", g:" << strG << ", b:" << strB;

    *r = strR.toInt();
    *g = strG.toInt();
    *b = strG.toInt();

    loadFile->close();
    delete loadFile;
}

void Data::reloadColors()
{
    int r = 0; // 123
    int b = 0; // 231
    int g = 0; // 132

    getColor(&r, &g, &b);

    qInfo() << "r:" << r << "g:" << g << "b:" << b;


    QColor color{r, g, b};
    if(color.isValid())
        m_qmlColors.append(color);
    else {
        qWarning() << "new color " << color.name(QColor::HexRgb) << " is invalid";
        return;
    }

    emit qmlColorsChanged(qmlColors());

    m_colorAmount += 1; // die Farbe ist immer gleich
    emit colorAmountChanged(colorAmount());
}

void Data::loadSkin()
{
    qInfo() << "loading Skin";

    QFile loadFile{"../../../data/Skin.slData"};
    if(!loadFile.open(QFile::ReadOnly))
        qWarning() << loadFile.fileName() << ":" << loadFile.errorString();
    QTextStream loadStream{&loadFile};

    qInfo() << loadStream.readAll();
}

void Data::getPlayerName(QString name)
{
    QFile nameFile{"../../../data/name.txt"};
    if(name == "")
        return;
    if(!nameFile.open(QFile::WriteOnly))
        qWarning() << nameFile.fileName() << ":" << nameFile.errorString();
    QTextStream nameStream{&nameFile};
    nameFile.reset();

    nameStream << name;
    nameFile.close();
    qInfo() << "set playername:" << name;
}

void Data::loadPendant()
{
    qInfo() << "try to load pendant...";

    QProcess pendantLoad;

    QString pendantLoadPath = "../../pendantLoad/bin/debug/pendantLoad.exe";
    QStringList pendantLoadArgs;

    pendantLoad.start(pendantLoadPath, pendantLoadArgs, QProcess::ExistingOnly);
}

} // namespace Editor
