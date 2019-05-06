#include "wooden_box.h"

wooden_box::wooden_box(QGraphicsItem *parent): QObject(), QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/wood_box.png"));
    HP = 2;
}

void wooden_box::getDamage()
{
    setPixmap(QPixmap(":/wooden_box_damage.png"));
    this->HP--;
}
