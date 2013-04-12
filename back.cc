#include <back.h>
#include "dialog.h"
#include <QApplication>
#include <QPainter>

#define PORT 3000
#define IMAGE "lockscreen.png"

#define SIXTY 1
#define MINUTES 15 


Back::Back(int argc, char ** argv) 
: QApplication::QApplication( argc, argv) 
{
    dialog.setWindowFlags( Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    dialog.setWindowState(Qt::WindowFullScreen);
    dialog.setStyleSheet("background-image: url("IMAGE")");

    interval = MINUTES; 
    minutesLeft = interval;

    tray = new QSystemTrayIcon();
    tray->setIcon(QIcon(renderText(minutesLeft)));
    tray->show();


    actionTime15 = new QAction("15", &menu);
    actionTime15->setCheckable(true);
    actionTime20 = new QAction("20", &menu);
    actionTime20->setCheckable(true);
    actionTime40 = new QAction("40", &menu);
    actionTime40->setCheckable(true);
    actionTime40->setChecked(true);

    connect(actionTime15, SIGNAL(triggered()), this, SLOT(setTime15()));  
    connect(actionTime20, SIGNAL(triggered()), this, SLOT(setTime20()));  
    connect(actionTime40, SIGNAL(triggered()), this, SLOT(setTime40()));  

    menu.addAction(actionTime15);  
    menu.addAction(actionTime20);  
    menu.addAction(actionTime40);  
    tray->setContextMenu(&menu); 


    timer.start( SIXTY * 1000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(time()));

    connect(&server, SIGNAL(newConnection()), this, SLOT(net()));
    server.listen(QHostAddress::Any, PORT);
}

void Back::lock()
{  
    if ( ! dialog.isVisible() )
    {
        dialog.setWindowFlags( Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
        dialog.setWindowState(Qt::WindowFullScreen);
        dialog.show();
    }
}

void Back::time()
{
    timer.start( SIXTY * 1000);
    minutesLeft--;

    if (minutesLeft <= 0)
    {
        minutesLeft = 0;
        lock();
    }

    tray->setIcon(QIcon(renderText(minutesLeft)));
}

void Back::unlock()
{  
    minutesLeft = interval;
    tray->setIcon(QIcon(renderText(minutesLeft)));
    dialog.hide();
}

void Back::net()
{
    server.close();
    server.listen(QHostAddress::Any, PORT);
    unlock();
}

void Back::setTime15()
{
    actionTime15->setChecked(true);
    actionTime20->setChecked(false);
    actionTime40->setChecked(false);
    interval=15;
    unlock();
}

void Back::setTime20()
{
    actionTime15->setChecked(false);
    actionTime20->setChecked(true);
    actionTime40->setChecked(false);
    interval=20;
    unlock();
}

void Back::setTime40()
{
    actionTime15->setChecked(false);
    actionTime20->setChecked(false);
    actionTime40->setChecked(true);
    interval=40;
    unlock();
}

QPixmap Back::renderText(int minutes)
{
    QPixmap pixmap = QPixmap(22,22);
   
    if (minutes < 3)
        pixmap.fill( Qt::red );
    else
        pixmap.fill( Qt::white );

    QPainter painter( &pixmap );
    painter.setPen( Qt::black );

    QRect rect = QRect(0, 0, 21, 21);
    painter.drawText( rect, Qt::AlignCenter, QString::number(minutesLeft));
    painter.drawRect( rect );

    return pixmap;
}
