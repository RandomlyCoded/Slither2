#ifndef NEWCOLOR_H
#define NEWCOLOR_H

#include <QWidget>

class NewColor : public QWidget
{
    Q_OBJECT
public:
    explicit NewColor(QWidget *parent = nullptr);

    void setColor(QColor color);
    void setColor(int r, int g, int b);

    void paintEvent(QPaintEvent *);
};

#endif // NEWCOLOR_H
