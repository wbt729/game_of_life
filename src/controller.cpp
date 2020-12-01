#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    rowCount = defaultRowCount;
    columnCount = defaultColumnCount;
    running = false;

    this->tableModel = new TableModel(this);
    initTable();

    timer.setInterval(updateInterval);
    connect(&timer, &QTimer::timeout, tableModel, &TableModel::update);
}

void Controller::setRowCount(int count)
{
    qDebug() << Q_FUNC_INFO << count;
    this->columnCount = count;
}

void Controller::setColumnCount(int count)
{
    qDebug() << Q_FUNC_INFO << count;
    this->rowCount = count;
}

void Controller::start()
{
    qDebug() << Q_FUNC_INFO;
    timer.start();
    running = true;
    emit runningChanged();
}

void Controller::stop()
{
    qDebug() << Q_FUNC_INFO;
    timer.stop();
    running = false;
    emit runningChanged();
}

void Controller::initTable()
{
    qDebug() << Q_FUNC_INFO;
    tableModel->init(rowCount, columnCount);
    tableModel->randomize();
}
