#ifndef BULLET_H
#define BULLET_H


#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>
#include <wooden_box.h>

class Bullet : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Bullet(int directionKey, int x, int y, int shooterID, QGraphicsItem* parent = 0);
public slots:
    void move();
signals:
    void deleteBullet(Bullet* bullet_);
    void redPlayerGetDamage(Bullet* bullet_);
    void bluePlayerGetDamage(Bullet* bullet_);
    void boxGetDamage(Bullet* bullet_);
    void woodenBoxGetDamage(Bullet* bullet_, QGraphicsItem* woodenBox);

private:
    void setPosition();
    int _directionKey;
    int whoShoot;
    int angle;
    int tX;
    int tY;
};
#endif // BULLET_H
