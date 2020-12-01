#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include "tablemodel.h"

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int rowCount READ getRowCount WRITE setRowCount NOTIFY geometryChanged)
    Q_PROPERTY(int columnCount READ getColumnCount WRITE setColumnCount NOTIFY geometryChanged)
    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged)

public:
    explicit Controller(QObject *parent = nullptr);
    TableModel* getTableModel() { return tableModel; }

    Q_INVOKABLE int getRowCount() { return rowCount; }
    Q_INVOKABLE int getColumnCount() { return columnCount; }
    Q_INVOKABLE void setRowCount(int count);
    Q_INVOKABLE void setColumnCount(int count);
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE bool isRunning() { return running; }

signals:
    void geometryChanged();
    void runningChanged();

private:
    TableModel* tableModel = nullptr;
    const int defaultRowCount = 64;
    const int defaultColumnCount = 64;
    const int updateInterval = 100;

    int rowCount;
    int columnCount;

    void initTable();

    QTimer timer;
    bool running;
};

#endif // CONTROLLER_H
