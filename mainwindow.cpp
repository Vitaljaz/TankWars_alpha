#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "redplayer.h"

#include "box.h"
#include "wooden_box.h"
#include "bullet.h"
#include "server.h"
#include "clientsocket.h"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QList>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(820, 620);
    this->setFixedSize(820, 620);
    ui->graphicsView->setFocus();
    playerID = 0;
    isReadyBluePlayer = false;
    isReadyRedPlayer  = false;
    client = new clientsocket();
    server = new Server();
    CreateMap();
    connectionController(0);
    connect(redPlayer, SIGNAL(createBullet(int, int,int,int)), this, SLOT(createBullet(int, int,int,int)));
    connect(bluePlayer, SIGNAL(createBullet(int, int,int,int)), this, SLOT(createBullet(int, int,int,int)));
    connect(redPlayer, SIGNAL(sendBullet(int,int,int,int)), this, SLOT(sendBullet(int,int,int,int)));
    connect(bluePlayer, SIGNAL(sendBullet(int,int,int,int)), this, SLOT(sendBullet(int,int,int,int)));
    connect(server, SIGNAL(connectPlayer()), this, SLOT(connectPlayer()));
    connect(client, SIGNAL(createBullet(int,int,int,int)), this, SLOT(createBullet(int,int,int,int)));
    connect(client, SIGNAL(setReadyPlayer(int)), this, SLOT(setReadyPlayer(int)));
    connect(client, SIGNAL(setMove(int,int,int,int)), this, SLOT(setMove(int,int,int,int)));
    connect(redPlayer, SIGNAL(sendMove(int,qreal,qreal,int)), this, SLOT(sendMove(int,qreal,qreal,int)));
    connect(bluePlayer, SIGNAL(sendMove(int,qreal,qreal,int)), this, SLOT(sendMove(int,qreal,qreal,int)));
    qDebug() <<"Map created!";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreateMap(){

    ui->btnExit->hide();
    ui->btnRestart->hide();

    // create scene
    QString mapPath = ":/map_1_3.png";
    QImage mapImage(mapPath);
    mainScene = new QGraphicsScene();
    QGraphicsPixmapItem* mapBackground = new QGraphicsPixmapItem(QPixmap::fromImage(mapImage));
    mapBackground->setOffset(0, 0);
    mainScene->addItem(mapBackground);
    ui->graphicsView->setScene(mainScene);

    QImage heartImage(":/hp.png");
    healthScene = new QGraphicsScene();
    QGraphicsPixmapItem* heartBackground = new QGraphicsPixmapItem(QPixmap::fromImage(heartImage));
    heartBackground->setOffset(0, 0);
    healthScene->addItem(heartBackground);

    redPlayer = new RedPlayer(nullptr, 0);
    mainScene->addItem(redPlayer);
    redPlayer->setPos(20, 500);
    spawnRedPlayer();

    bluePlayer = new RedPlayer(nullptr, 1);
    mainScene->addItem(bluePlayer);
    bluePlayer->setPos(510, 20);
    spawnBluePlayer();

    setHP4RedPlayer(5);
    setHP4BluePlayer(5);

    // add boxes
    box* box_1 = new box();
    mainScene->addItem(box_1);
    box_1->setPos(100, 500);
    box* box_2 = new box();
    mainScene->addItem(box_2);
    box_2->setPos(100, 410);
    box* box_3 = new box();
    mainScene->addItem(box_3);
    box_3->setPos(400, 10);
    box* box_4 = new box();
    mainScene->addItem(box_4);
    box_4->setPos(400, 100);

    // add wooden boxes


    wooden_box* wbox_1 = new wooden_box();
    mainScene->addItem(wbox_1);
    wbox_1->setPos(120, 120);
    woodenBoxList.push_back(wbox_1);

    wooden_box* wbox_2 = new wooden_box();
    mainScene->addItem(wbox_2);
    wbox_2->setPos(120, 200);
    woodenBoxList.push_back(wbox_2);

    wooden_box* wbox_3 = new wooden_box();
    mainScene->addItem(wbox_3);
    wbox_3->setPos(200, 120);
    woodenBoxList.push_back(wbox_3);

    wooden_box* wbox_4 = new wooden_box();
    mainScene->addItem(wbox_4);
    wbox_4->setPos(400, 400);
    woodenBoxList.push_back(wbox_4);

    wooden_box* wbox_5 = new wooden_box();
    mainScene->addItem(wbox_5);
    wbox_5->setPos(400, 320);
    woodenBoxList.push_back(wbox_5);

    wooden_box* wbox_6 = new wooden_box();
    mainScene->addItem(wbox_6);
    wbox_6->setPos(320, 400);
    woodenBoxList.push_back(wbox_6);

    // add walls
    mainScene->addRect(0, 600, 600, 10, QPen(Qt::NoPen), QBrush(Qt::darkGray));
    mainScene->addRect(0, 0, 10, 600, QPen(Qt::NoPen), QBrush(Qt::darkGray));
    mainScene->addRect(600, 0, 10, 610, QPen(Qt::NoPen), QBrush(Qt::darkGray));
    mainScene->addRect(0, 0, 600, 10, QPen(Qt::NoPen), QBrush(Qt::darkGray));

    // end
    ui->graphicsView->show();
}

void MainWindow::connectionController(int status)
{
    if (status == 0){
        QImage waitingroomImage(":/waiting_room.png");
        waiting_room = new QGraphicsPixmapItem(QPixmap::fromImage(waitingroomImage));
        waiting_room->setOffset(5, 5);
        mainScene->addItem(waiting_room);
    }
    if (status == 1){
        mainScene->removeItem(waiting_room);
        status = 3;
    }
}

void MainWindow::sendBullet(int player, int lastkey, int x, int y)
{
    client->sendBullet(player, x, y, lastkey);
}

void MainWindow::setReadyPlayer(int playerID_)
{
    if (playerID_ == 0){
        isReadyRedPlayer = true;
        ui->r_gameStatus->setText("Game status: ready");
        connectionController(1);
    }
    else {
        isReadyBluePlayer = true;
        ui->b_gameStatus->setText("Game status: ready");
        connectionController(1);
    }
    if (isReadyRedPlayer == true && isReadyBluePlayer == true){
        connectionController(1);
    }
}

void MainWindow::sendMove(int playerID, qreal x, qreal y, int angle)
{
    client->sendMove(playerID, x, y, angle);
}

void MainWindow::setMove(int playerID, int x, int y, int angle)
{
    if (playerID == 0){
        redPlayer->setPosition(x, y, angle);
    }
    else {
        bluePlayer->setPosition(x, y, angle);
    }
}

void MainWindow::createBullet(int player, int idLastKey, int x, int y)
{
    Bullet* _bullet = new Bullet(idLastKey, x, y, playerID);
    connect(_bullet, SIGNAL(deleteBullet(Bullet*)), this, SLOT(deleteBullet(Bullet*)));
    connect(_bullet, SIGNAL(bluePlayerGetDamage(Bullet*)), this, SLOT(bluePlayerGetDamage(Bullet*)));
    connect(_bullet, SIGNAL(redPlayerGetDamage(Bullet*)), this, SLOT(redPlayerGetDamage(Bullet*)));
    connect(_bullet, SIGNAL(boxGetDamage(Bullet*)), this, SLOT(boxGetDamage(Bullet*)));
    connect(_bullet, SIGNAL(woodenBoxGetDamage(Bullet*, QGraphicsItem*)), this, SLOT(woodenBoxGetDamage(Bullet*,QGraphicsItem*)));
    mainScene->addItem(_bullet);
}

void MainWindow::deleteBullet(Bullet *bullet_)
{
    mainScene->removeItem(bullet_);
}

void MainWindow::bluePlayerGetDamage(Bullet* bullet_)
{
    bluePlayer->health--;
    QPixmap pixmap(":/hp_2.png");
    if (bluePlayer->health == 0){
        ui->b_hp1->setPixmap(pixmap);
        redPlayerWin();
    }
    if (bluePlayer->health == 4){
        ui->b_hp5->setPixmap(pixmap);
    }
    if (bluePlayer->health == 3){
        ui->b_hp4->setPixmap(pixmap);
    }
    if (bluePlayer->health == 2){
        ui->b_hp3->setPixmap(pixmap);
    }
    if (bluePlayer->health == 1){
        ui->b_hp2->setPixmap(pixmap);
    }
    mainScene->removeItem(bullet_);
}

void MainWindow::redPlayerGetDamage(Bullet* bullet_)
{
    redPlayer->health--;
    QPixmap pixmap(":/hp_2.png");
    if (redPlayer->health == 0){
        ui->r_hp1->setPixmap(pixmap);
        bluePlayerWin();
    }
    if (redPlayer->health == 4){
        ui->r_hp5->setPixmap(pixmap);
    }
    if (redPlayer->health == 3){
        ui->r_hp4->setPixmap(pixmap);
    }
    if (redPlayer->health == 2){
        ui->r_hp3->setPixmap(pixmap);
    }
    if (redPlayer->health == 1){
        ui->r_hp2->setPixmap(pixmap);
    }
    mainScene->removeItem(bullet_);
}

void MainWindow::boxGetDamage(Bullet* bullet_)
{
    mainScene->removeItem(bullet_);
}

void MainWindow::setHP4RedPlayer(int hp)
{
    redPlayer->health = hp;
    QPixmap pixmap(":/hp_1.png");
    if (hp == 5){
        ui->r_hp1->setPixmap(pixmap);
        ui->r_hp2->setPixmap(pixmap);
        ui->r_hp3->setPixmap(pixmap);
        ui->r_hp4->setPixmap(pixmap);
        ui->r_hp5->setPixmap(pixmap);
    }
}

void MainWindow::setHP4BluePlayer(int hp)
{
    bluePlayer->health = hp;
    QPixmap pixmap(":/hp_1.png");
    if (hp == 5){
        ui->b_hp1->setPixmap(pixmap);
        ui->b_hp2->setPixmap(pixmap);
        ui->b_hp3->setPixmap(pixmap);
        ui->b_hp4->setPixmap(pixmap);
        ui->b_hp5->setPixmap(pixmap);
    }
}

void MainWindow::spawnRedPlayer()
{
    redPlayer->setRotation(180);
    redPlayer->show();
    redPlayer->setPos(20, 500);
    ui->graphicsView->setFocus();
}

void MainWindow::spawnBluePlayer()
{
    bluePlayer->setRotation(0);
    bluePlayer->show();
    bluePlayer->setPos(510, 20);
    ui->graphicsView->setFocus();
}

void MainWindow::bluePlayerWin()
{
    if (bluePlayer->score < 4){
    QImage mapImage_black(":/blue_tank_win.png");
    mapBackground_black = new QGraphicsPixmapItem(QPixmap::fromImage(mapImage_black));
    mapBackground_black->setOffset(0, 0);
    mainScene->addItem(mapBackground_black);

    ui->btnExit->show();
    ui->btnRestart->show();

    bluePlayer->hide();
    redPlayer->hide();

    bluePlayer->score++;
    ui->bScore->setText(QString::number(bluePlayer->score));

    }
    else {
        endGame();
    }
}

void MainWindow::redPlayerWin()
{
    if (redPlayer->score < 4){
    QImage mapImage_black(":/red_tank_win.png");
    mapBackground_black = new QGraphicsPixmapItem(QPixmap::fromImage(mapImage_black));
    mapBackground_black->setOffset(0, 0);
    mainScene->addItem(mapBackground_black);

    ui->btnExit->show();
    ui->btnRestart->show();

    bluePlayer->hide();
    redPlayer->hide();

    redPlayer->score++;
    ui->rScore->setText(QString::number(redPlayer->score));
    }
    else {
        endGame();
    }
}

void MainWindow::endGame()
{
    ui->btnExit->show();
    QImage mapImage_black(":/game_ower.png");
    mapBackground_black = new QGraphicsPixmapItem(QPixmap::fromImage(mapImage_black));
    mapBackground_black->setOffset(0, 0);
    mainScene->addItem(mapBackground_black);
}

void MainWindow::setControll()
{
    if (playerID == 0){
        redPlayer->setFlag(QGraphicsItem::ItemIsFocusable);
        redPlayer->setFocus();
    }
    else {
        bluePlayer->setFlag(QGraphicsItem::ItemIsFocusable);
        bluePlayer->setFocus();
    }
}


void MainWindow::on_btnRestart_clicked()
{
    mainScene->removeItem(mapBackground_black);
    ui->btnExit->hide();
    ui->btnRestart->hide();
    spawnBluePlayer();
    spawnRedPlayer();
    setHP4BluePlayer(5);
    setHP4RedPlayer(5);
}

void MainWindow::on_btnExit_clicked()
{
    delete redPlayer;
    delete bluePlayer;
    delete mainScene;
    delete healthScene;
    QApplication::exit();
}

void MainWindow::on_btnConnect_clicked()
{
    if (client->connectToServer(ui->leHost->text(), ui->lePort_2->text().toInt())){
        ui->btnConnect->setDisabled(true);
        ui->btnNewGame->setDisabled(true);
        ui->btnReady->setEnabled(true);
        ui->btnDisconnect->setEnabled(true);
        ui->leHost->setDisabled(true);
        ui->lePort_2->setDisabled(true);
        playerID = 1;
        setControll();
        ui->b_connectStatus->setText("Connect status: connected");
        ui->b_gameStatus->setText("Game status: not ready");
        ui->r_connectStatus->setText("Connect status: connected");
        ui->r_gameStatus->setText("Game status: not ready");
    }
    else {
        perror("Error with connect to server");
    }
}

void MainWindow::connectPlayer()
{
    ui->b_connectStatus->setText("Connect status: connected");
    ui->b_gameStatus->setText("Game status: not ready");
    ui->btnReady->setEnabled(true);
}

void MainWindow::woodenBoxGetDamage(Bullet *bullet_, QGraphicsItem* woodenBox)
{
    mainScene->removeItem(bullet_);
    for(int i = 0; i < woodenBoxList.size(); ++i){
        if (woodenBox == woodenBoxList[i]){
            woodenBoxList[i]->getDamage();
            if (woodenBoxList[i]->getHP() == 0){
                mainScene->removeItem(woodenBoxList[i]);
                delete woodenBoxList[i];
            }
        }
    }
}

void MainWindow::on_btnReady_clicked()
{
    if (playerID == 0){
        ui->btnReady->setDisabled(true);
        isReadyRedPlayer = true;
        ui->r_gameStatus->setText("Game status: ready");
        client->sendReadyStatus(0);
    } else {
        ui->btnReady->setDisabled(true);
        client->sendReadyStatus(1);
        isReadyBluePlayer = true;
        ui->b_gameStatus->setText("Game status: ready");
    }

}

void MainWindow::on_btnNewGame_clicked()
{
    if (server->createServer(ui->lePort_2->text().toInt())){
        client->connectToServer(ui->leHost->text(), ui->lePort_2->text().toInt());
        ui->btnConnect->setDisabled(true);
        ui->btnNewGame->setDisabled(true);
        ui->btnDisconnect->setEnabled(true);
        ui->leHost->setDisabled(true);
        ui->lePort_2->setDisabled(true);
        playerID = 0;
        setControll();
        ui->r_connectStatus->setText("Connect status: connected");
        ui->r_gameStatus->setText("Game status: not ready");
    }
    else {
        perror("Error with create new server");
    }
}

