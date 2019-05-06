#ifndef WOODEN_BOX_H
#define WOODEN_BOX_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

class wooden_box : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    wooden_box(QGraphicsItem* parent = nullptr);
    void getDamage();
    int getHP() {return HP;}
    int HP;
};
#endif // WOODEN_BOX_H
