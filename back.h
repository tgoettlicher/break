#ifndef back_h
#define back_h


#include <QApplication>
#include "dialog.h"
#include <QObject>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QSystemTrayIcon>
#include <QtNetwork/QTcpServer>


//class Back : public QObject, public QApplication 
class Back : public QApplication 
{
    Q_OBJECT

public:    
    Back(int argc, char** argv);

private:
    Dialog dialog;
    QTcpServer server;
    QTimer timer;
    QMenu menu;
    QSystemTrayIcon *tray;
    int minutesLeft;

    QAction* actionTime15;
    QAction* actionTime20;
    QAction* actionTime40;

    int interval;


    QPixmap renderText(int minutes);

public slots:
    void lock();
    void unlock();
    void net();
    void time();
    
    void setTime15();
    void setTime20();
    void setTime40();

};


#endif

