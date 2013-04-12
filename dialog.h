#ifndef dialog_h
#define dialog_h


#include <QDialog>


class Dialog : public QDialog 
{
    Q_OBJECT

public:    
    Dialog() {QDialog();} ;

public slots:
    void reject() { };

};


#endif

