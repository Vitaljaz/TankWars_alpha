#ifndef BOX_H
#define BOX_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

class box : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    box(QGraphicsItem* parent = nullptr);
};
#endif // BOX_H
