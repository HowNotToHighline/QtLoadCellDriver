#include "mainwindow.h"

#include <QApplication>
#include <src/DataAcquisition/DataProcessor.h>

int main(int argc, char *argv[]) {
    DataProcessor dataProcessor;
    QApplication a(argc, argv);
    MainWindow w(dataProcessor);
    w.show();
    return a.exec();
}
