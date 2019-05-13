#include "player.h"
#include "box.h"
#include "mainwindow.h"
#include "wooden_box.h"
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QDebug>
#include <QTimer>

Player::Player(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{

}

Player::Player(QGraphicsItem *parent, int _ID)
{
    if (_ID == 0){
        ID = 0;
        angle = 180;
        health = 4;
        score = 0;
        idLastKeyPressed = 0;
        setPixmap(QPixmap(":/red_tank.png"));
        setTransformOriginPoint(36, 36);
        setRotation(angle);

        canShot = false;
        bulletTimer = new QTimer();
        connect(bulletTimer, &QTimer::timeout, this, &Player::slotBulletTimer);
        bulletTimer->start(1000/2);
    }
    else {
        ID = 1;
        angle = 0;
        health = 4;
        score = 0;
        idLastKeyPressed = 0;
        setPixmap(QPixmap(":/blue_tank.png"));
        setTransformOriginPoint(36, 36);
        setRotation(angle);

        canShot = false;
        bulletTimer = new QTimer();
        connect(bulletTimer, &QTimer::timeout, this, &Player::slotBulletTimer);
        bulletTimer->start(1000/2);
    }
}

// idLastKeyPressed:
// 0 - none, 1 - key_up, 2 - key_down, 3 - key_right, 4 - key_left

void Player::keyPressEvent(QKeyEvent* event){
        if(event->key() == Qt::Key_Up){
            emit sendMove(ID, this->pos().x(), this->pos().y() - 5, 180);
            if (this->y() - 5 > 0){
                this->setPos(x(), y() - 5);
                idLastKeyPressed = 1;
                if (checkCollision()){
                    resetPosition(idLastKeyPressed);
                    //return;
                }
                if (angle != 180){
                    angle = 180;
                    setRotation(angle);
                }
             }
        }

        if (event->key() == Qt::Key_Down){
            emit sendMove(ID, this->pos().x(), this->pos().y() + 5, 0);
            if (this->y() + 5 < 533){
                this->setPos(x(), y() + 5);
                idLastKeyPressed = 2;
                if (checkCollision()){
                    resetPosition(idLastKeyPressed);
                    //return;
                }
                if (angle != 0){
                    angle = 0;
                    setRotation(angle);
                }
                canMove = false;
        }
        }

        if (event->key() == Qt::Key_Right){
            emit sendMove(ID, this->pos().x() + 5, this->pos().y(), -90);
            if (this->x() + 5 < 533){
                this->setPos(x() + 5, y());
                idLastKeyPressed = 3;
                if (checkCollision()){
                    resetPosition(idLastKeyPressed);
                    //return;
                }
                if (angle != -90){
                    angle = -90;
                    setRotation(angle);
                }
                canMove = false;
            }
        }

        if (event->key() == Qt::Key_Left){
            emit sendMove(ID, this->pos().x() - 5, this->pos().y(), 90);
            if(this->x() - 5 > 0){
                this->setPos(x() - 5, y());
                idLastKeyPressed = 4;
                if (checkCollision()){
                    resetPosition(idLastKeyPressed);
                    //return;
                }
                if (angle != 90){
                    angle = 90;
                    setRotation(angle);
                }
                canMove = false;
            }
        }

        if (event->key() == Qt::Key_Space){
           if (canShot){
               emit sendBullet(ID, idLastKeyPressed, this->x(), this->y());
               emit createBullet(ID, idLastKeyPressed, this->x(), this->y());
               canShot = false;
           }
        }
}

void Player::setPosition(int x, int y, int angle)
{
    this->setPos((qreal) x, (qreal) y);
    this->setRotation(angle);
    this->angle = angle;
}

void Player::slotBulletTimer()
{
    if (canShot){
        canShot = false;
    }
    else {
        canShot = true;
    }
}

void Player::slotMoveTimer()
{
    if (canMove) {
        canMove = false;
    } else {
        canMove = true;
    }
}

bool Player::checkCollision()
{
    bool isCollision = false;
    QList<QGraphicsItem*> colliding_items = collidingItems();
    for (int i = 0; i < colliding_items.size(); i++){
        if (typeid(*(colliding_items[i])) == typeid(box))
            isCollision = true;
        if (typeid(*(colliding_items[i])) == typeid(Player))
            isCollision = true;
        if (typeid(*(colliding_items[i])) == typeid(wooden_box))
            isCollision = true;
    }
    return isCollision;
}

void Player::resetPosition(int key)
{
    if (key == 1){
        this->setPos(x(), y() + 6);
    }
    else if (key == 2){
        this->setPos(x(), y() - 6);
    }
    else if (key == 3){
        this->setPos(x() - 6, y());
    }
    else if (key == 4){
        this->setPos(x() + 6, y());
    }
}
