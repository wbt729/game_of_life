#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QDebug>
#include <QAbstractTableModel>
#include <QRandomGenerator>
#include <QDateTime>

class Cell {
public:
    bool alive = false;
    bool selected = false;
    bool future = false;
};

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

    enum CellRoles {
        AliveRole,
        SelectedRole
    };

public:
    explicit TableModel(QObject *parent = nullptr);
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE void select(int index, bool select);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void reset();

    QImage getImage(int row, int column);
    void init(int rows, int columns);
    void randomize();

public slots:
    void update();

private:
    const int rows = 64;
    const int columns = 64;
    bool indexValid(const QModelIndex &index) const;
    QList<QList<Cell>> cells;
    bool isAlive(int row, int column);
    int countAliveNeighbours(int row, int column);

protected:
    QHash<int, QByteArray> roleNames() const override;
};

#endif // TABLEMODEL_H
