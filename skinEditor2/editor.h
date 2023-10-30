#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include <string>
#include <QString>
#include <QColor>
#include <QDebug>
#include <QVector3D>

namespace Editor {

class Data : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QColor> skin READ skin NOTIFY skinChanged FINAL)
    Q_PROPERTY(QList<QColor> qmlColors READ qmlColors  NOTIFY qmlColorsChanged FINAL)
    Q_PROPERTY(int colorAmount READ colorAmount NOTIFY colorAmountChanged FINAL)
    Q_PROPERTY(int skinLenght READ skinLenght CONSTANT FINAL)

    Q_PROPERTY(int animationInterval READ animationInterval NOTIFY animationIntervalChanged FINAL)

public:
    Data();
    QList<QColor> qmlColors() const { return m_qmlColors; }
    int colorAmount() const { return m_colorAmount; }

    QList<QColor> skin() const { return m_skin; }
    int skinLenght() const { return skinSize; }
    int animationInterval() const { return m_animationInterval; }

public slots:
    void setColor(int time, QColor color);
    void save();
    void startGame(QStringList args = {""});
    void createColor();
    void loadSkin();

    void getPlayerName(QString name);
    void loadPendant();

    void setAnimationInterval(int newInterval);

    int getInt(QString txt);
    QVector3D colorGridPosition(int i);

signals:
    void skinChanged(QList<QColor> skin);

    void qmlColorsChanged(QList<QColor> possibleColors);
    void colorAmountChanged(int colorAmount);

    void animationIntervalChanged();

private:
    const int skinSize;
    QColor possible[3] = {QColor{0x00, 0x00ff, 0x00}, QColor{0x00, 0x00, 0x00}, QColor{0x00, 0x00, 0x00ff}};
    bool started = false;
    QList<QColor> m_skin;

    void reloadColors();

    QList<QColor> m_qmlColors;
    int m_colorAmount;
    void resetColors();
    void getColor(int *r, int *g, int *b);
    int m_animationInterval;
};


} // namespace Editor

#endif // EDITOR_H
