#ifndef REDPLAYER_H
#define REDPLAYER_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QGraphicsItem>
#include <QKeyEvent>

class Player: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Player(QGraphicsItem* parent = nullptr);
    Player(QGraphicsItem* parent, int _ID);

    int health;
    int score;

    void keyPressEvent(QKeyEvent* event);
    void setPosition(int x, int y, int angle);

private:
    int ID;
    int angle;
    int idLastKeyPressed;
    bool canShot;
    bool canMove;

    bool checkCollision();
    void resetPosition(int key);
    QTimer *moveTimer;
    QTimer *bulletTimer;

private slots:
    void slotBulletTimer();
    void slotMoveTimer();

signals:
    void createBullet(int player, int lastkey, int x, int y);
    void sendBullet(int player, int lastkey, int x, int y);
    void sendMove(int playerID, qreal x, qreal y, int lastKey);

};

#endif // REDPLAYER_H
