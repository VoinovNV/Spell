#include "mainwindow.hpp"
#include <QtWidgets/QApplication>
#include <QException>
#include <iostream>
int main(int argc, char *argv[])
{
    try{
        QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
    }
    catch(QException a){std::cout<<a.what();}

}
