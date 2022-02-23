#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(DataProcessor &dataProcessor)
        : QMainWindow(nullptr),
          ui(new Ui::MainWindow),
          _dataProcessor(dataProcessor) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_connectButton_clicked() {
    if(connected) {
        _dataProcessor.DisConnect();
        connected = false;
        ui->connectButton->setText("Connect");
        ui->startButton->setEnabled(false);
        ui->tareButton->setEnabled(false);
    } else {
        _dataProcessor.Connect();
        connected = true;
        ui->connectButton->setText("Disconnect");
        ui->startButton->setEnabled(true);
        ui->tareButton->setEnabled(true);
    }
}

void MainWindow::on_startButton_clicked() {
    if(started) {
        ui->startButton->setEnabled(false);
        _dataProcessor.Stop();
    } else {
        ui->startButton->setText("Stop");
        ui->tareButton->setEnabled(false);
        started = true;
        // TODO: Use load cell config from UI
        _dataProcessor.Start({
                                     nullptr,
                                     ui->sampleRate->value(),
                                     static_cast<float>(ui->triggerForce->value()),
                                     static_cast<float>(ui->stopForce->value()),
                                     static_cast<float>(ui->preRecordingTime->value()),
                                     static_cast<float>(ui->postRecordingTime->value()),
                                     ui->measurementName->text().toStdString(),
                             },
                             this
        );
    }
}

void MainWindow::on_tareButton_clicked() {
    _dataProcessor.Tare();
}

void MainWindow::on_calibrationButton_clicked() {
    _dataProcessor.Calibrate(static_cast<float>(ui->calibrationForce->value()));
}

void MainWindow::stopped() {
    started = false;
    ui->startButton->setEnabled(true);
    ui->tareButton->setEnabled(true);
    ui->startButton->setText("Start");
}
