#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QLabel>

#include <redplayer.h>
#include <bullet.h>
#include <wooden_box.h>
#include <server.h>
#include <clientsocket.h>


namespace Ui {
class MainWindow;
}

class clientsocket;
class Server;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QGraphicsScene* mainScene;
    QGraphicsScene* healthScene;
    RedPlayer* redPlayer;
    RedPlayer* bluePlayer;
    clientsocket* client;
    Server* server;
    QList <wooden_box*> woodenBoxList;
    QGraphicsPixmapItem* mapBackground_black;
    QGraphicsPixmapItem* waiting_room;

    void CreateMap();
    void connectionController(int status);

    int playerID;
    bool isReadyRedPlayer;
    bool isReadyBluePlayer;


signals:

public slots:


private slots:
    void sendBullet(int player, int lastkey, int x, int y);
    void createBullet(int player, int idLastKey, int x, int y);
    void deleteBullet(Bullet* bullet_);
    void bluePlayerGetDamage(Bullet* bullet_);
    void redPlayerGetDamage(Bullet* bullet_);
    void boxGetDamage(Bullet* bullet_);
    void on_btnRestart_clicked();
    void on_btnExit_clicked();
    void on_btnConnect_clicked();
    void connectPlayer();
    void woodenBoxGetDamage(Bullet* bullet_, QGraphicsItem* woodenBox);
    void on_btnReady_clicked();
    void on_btnNewGame_clicked();
    void setReadyPlayer(int playerID_);
    void sendMove(int playerID, qreal x, qreal y, int angle);
    void setMove(int playerID, int x, int y, int angle);

private:
    void setHP4RedPlayer(int hp);
    void setHP4BluePlayer(int hp);
    void spawnRedPlayer();
    void spawnBluePlayer();
    void bluePlayerWin();
    void redPlayerWin();
    void endGame();
    void setControll();
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
