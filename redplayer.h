#ifndef REDPLAYER_H
#define REDPLAYER_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QGraphicsItem>
#include <QKeyEvent>

class RedPlayer: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    RedPlayer(QGraphicsItem* parent = nullptr);
    RedPlayer(QGraphicsItem* parent, int _ID);
    void keyPressEvent(QKeyEvent* event);

    void setPosition(int x, int y, int angle);

    int health;
    int score;

public slots:


private slots:
    void slotBulletTimer();
    void slotMoveTimer();

signals:
    void createBullet(int player, int lastkey, int x, int y);
    void sendBullet(int player, int lastkey, int x, int y);
    void sendMove(int playerID, qreal x, qreal y, int lastKey);

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
};

#endif // REDPLAYER_H
