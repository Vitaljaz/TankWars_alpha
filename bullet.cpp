#include "bullet.h"

#include <QTimer>
#include <QList>
#include <QThread>

#include "mainwindow.h"
#include "box.h"
#include "player.h"
#include "wooden_box.h"

Bullet::Bullet(int directionKey, int x, int y, int shooterID, QGraphicsItem *parent) :QObject(), QGraphicsPixmapItem(parent)
{
    _directionKey = directionKey;
    angle = 0;
    whoShoot = shooterID;
    tX = x;
    tY = y;
    setPixmap(QPixmap(":/bullet.png"));
    setTransformOriginPoint(6, 13);
    setRotation(angle);
    setPosition();
    QTimer* bulletTimer = new QTimer(this);
    connect(bulletTimer, SIGNAL(timeout()), this, SLOT(move()));
    bulletTimer->start(45);
}

void Bullet::move()
{
    QList<QGraphicsItem*> colliding_items = collidingItems();

    for (int i = 0; i<colliding_items.size(); ++i)
    {
        if (typeid(*(colliding_items[i])) == typeid(Player))
        {
            if (whoShoot == 0){
                emit bluePlayerGetDamage(this);
                delete this;
                return;
            }
            else {
                emit redPlayerGetDamage(this);
                delete this;
                return;
            }
        }
        else if (typeid(*(colliding_items[i])) == typeid(box))
        {
            emit boxGetDamage(this);
            delete this;
            return;
        }
        else if (typeid(*(colliding_items[i])) == typeid(wooden_box)){
            emit woodenBoxGetDamage(this, colliding_items[i]);
            delete this;
            return;
        }
    }

    if (_directionKey == 1)
    {
        this->setPos(x(), y() - 20);
        if (this->pos().y() < 0)
        {
            emit deleteBullet(this);
            delete this;
        }
    }

    if (_directionKey == 2)
    {
        this->setPos(x(), y() + 20);
        if (this->pos().y() > 590)
        {
            emit deleteBullet(this);
            delete this;
        }
    }

    if (_directionKey == 3)
    {
        this->setPos(x() + 20, y());
        if (this->pos().x() > 590)
        {
            emit deleteBullet(this);
            delete this;
        }
    }

    if (_directionKey == 4)
    {
        this->setPos(x() - 20, y());
        if (this->pos().x() < 5)
        {
            emit deleteBullet(this);
            delete this;
        }
    }
}

void Bullet::setPosition()
{
    if (_directionKey == 1)
    {
        if (angle != 0)
        {
            angle = 0;
            setRotation(angle);
        }
        this->setPos(tX + 30, tY - 32);
    }

    if (_directionKey == 2)
    {
        if (angle != 180)
        {
            angle = 180;
            setRotation(angle);
        }
        this->setPos(tX + 30, tY + 80);
    }

    if (_directionKey == 3)
    {
        if (angle != 90)
        {
            angle = 90;
            setRotation(angle);
        }
        this->setPos(tX + 90, tY + 22);
    }

    if(_directionKey == 4)
    {
        if (angle != -90)
        {
            angle = -90;
            setRotation(angle);
        }
        this->setPos(tX - 30, tY + 24);
    }
}
