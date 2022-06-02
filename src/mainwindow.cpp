#include <QSettings>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <string>
#include "./DataAcquisition/devices/LoadCellDriverFactory.h"

MainWindow::MainWindow(DataProcessor &dataProcessor)
        : QMainWindow(nullptr),
          ui(new Ui::MainWindow),
          _dataProcessor(dataProcessor) {
    ui->setupUi(this);

    printf("Loading loadcell drivers\n");
    QSettings settings("configs/drivers.ini", QSettings::IniFormat);
    QStringList drivers = settings.childGroups();
    for (const QString &driverName: drivers) {
        // FIXME: Nicer way to print QString's
        printf("Driver loaded: %s\n", driverName.toStdString().c_str());

        settings.beginGroup(driverName);
        QString driver = settings.value("driver", "").toString();
        if (driver.isNull()) throw std::runtime_error("No driver specified in ini config!");

        settings.beginGroup(driver);
        std::map<std::string, std::string> options;
        for (const auto &optionKey: settings.allKeys()) {
            options.insert(std::pair<std::string, std::string>(optionKey.toStdString(), settings.value(
                    optionKey).value<QString>().toStdString()));
        }
        settings.endGroup();

        if (!LoadCellDriverFactory::isValidConfig(driver.toStdString(), options)) {
            throw std::runtime_error("Invalid config for driver!");
        }
        // TODO: Add option to dropdown list

        settings.endGroup();
    }
    printf("Done loading loadcell drivers\n");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_connectButton_clicked() {
    if (connected) {
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
    if (started) {
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
