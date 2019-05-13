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
    int HP;
    bool inScene;

    void getDamage();
    void restoreHP();
    int getHP() {return HP;}
};
#endif // WOODEN_BOX_H
