#include "tablemodel.h"

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent)
{
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rows;
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return columns;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    QVariant variant;
    if(indexValid(index))
    {
        if(role == AliveRole)
            variant = cells.at(index.row()).at(index.column()).alive;
        else if(role == SelectedRole)
            variant = cells.at(index.row()).at(index.column()).selected;
    }
    return variant;
}

void TableModel::select(int index, bool select)
{
    qDebug() << Q_FUNC_INFO << index << select;
    int row= index % rowCount();
    int column = index / rowCount();
    cells[row][column].alive = select;
    emit dataChanged(this->index(row, column), this->index(row, column));
}

void TableModel::clear()
{
    beginResetModel();
    for(int row = 0; row < rows; ++row)
    {
        for(int column = 0; column < columns; ++column)
        {
            cells[row][column].alive = false;
        }
    }
    endResetModel();
}

void TableModel::reset()
{
    qDebug() << Q_FUNC_INFO;
    clear();
    randomize();
}

void TableModel::init(int rows, int columns)
{
    qDebug() << Q_FUNC_INFO << rows << columns;
    beginResetModel();
    cells.clear();
    for(int row = 0; row < rows; ++row)
    {
        QList<Cell> currentRow;
        for(int column = 0; column < columns; ++column)
        {
            Cell cell;
            currentRow.append(cell);
        }
        cells.append(currentRow);
    }
    endResetModel();
}

void TableModel::randomize()
{
    qDebug() << Q_FUNC_INFO;
    beginResetModel();
    QRandomGenerator gen;
    gen.seed(QDateTime::currentMSecsSinceEpoch());
    for(int row = 0; row < rows; ++row)
    {
        for(int column = 0; column < columns; ++column)
        {
            cells[row][column].alive = gen.generate() % 2;
        }
    }

    endResetModel();
}

void TableModel::update()
{
    qDebug() << Q_FUNC_INFO;
    for(int row = 0; row < rows; ++row)
    {
        for(int column = 0; column < columns; ++column)
        {
            int aliveNeigbours = countAliveNeighbours(row, column);
            bool alive = cells[row][column].alive;
            if(alive)
            {
                cells[row][column].future = aliveNeigbours == 2 | aliveNeigbours == 3;
            }
            else
                cells[row][column].future = aliveNeigbours == 3;
        }
    }
    for(int row = 0; row < rows; ++row)
    {
        for(int column = 0; column < columns; ++column)
        {
            cells[row][column].alive = cells[row][column].future;
        }
    }
    //emit data changed signal for entire table, since individual update signals
    //will cause a lot of overhead leading to bad UI performance.
    emit dataChanged(this->index(0, 0), this->index(rowCount() - 1, columnCount() - 1));
}


bool TableModel::indexValid(const QModelIndex &index) const
{
//    qDebug() << Q_FUNC_INFO << index;
    if(cells.size() && index.row() < cells.size())
    {
        return index.column() < cells.first().size();
    }
    return true;
}

bool TableModel::isAlive(int row, int column)
{
    if(row < 0 || row >= cells.size())
        return false;
    else
    {
        if(column < 0 || column >= cells.first().size())
        {
            return false;
        }
        else
        {
            return cells[row][column].alive;
        }
    }
}

int TableModel::countAliveNeighbours(int row, int column)
{
//    qDebug() << Q_FUNC_INFO;
    int aliveNeighbours = 0;

    aliveNeighbours += isAlive(row - 1, column - 1);
    aliveNeighbours += isAlive(row - 1, column);
    aliveNeighbours += isAlive(row - 1, column + 1);
    aliveNeighbours += isAlive(row, column - 1);
    aliveNeighbours += isAlive(row, column + 1);
    aliveNeighbours += isAlive(row + 1, column - 1);
    aliveNeighbours += isAlive(row + 1, column);
    aliveNeighbours += isAlive(row + 1, column + 1);

    return aliveNeighbours;
}

QHash<int, QByteArray> TableModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[AliveRole] = "alive";
    roles[SelectedRole] = "selected";
    return roles;
}
