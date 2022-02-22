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

void MainWindow::on_startButton_clicked() {
    if(started) {
        ui->startButton->setEnabled(false);
        _dataProcessor.Stop();
    } else {
        ui->startButton->setText("Stop");
        started = true;
        // TODO: Actually use info from UI
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

void MainWindow::stopped() {
    started = false;
    ui->startButton->setEnabled(true);
    ui->startButton->setText("Start");
}
