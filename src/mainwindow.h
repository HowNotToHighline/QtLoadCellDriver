#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <src/DataAcquisition/DataProcessor.h>
#include "CallbackInterface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public CallbackInterface {
Q_OBJECT

public:
    explicit MainWindow(DataProcessor &dataProcessor);

    void stopped() override;

    ~MainWindow() override;

private slots:
    void on_connectButton_clicked();

    void on_startButton_clicked();

    void on_tareButton_clicked();

    void on_calibrationButton_clicked();

private:
    Ui::MainWindow *ui;
    DataProcessor &_dataProcessor;
    bool started = false;
    bool connected = false;
};

#endif // MAINWINDOW_H
