#include "ParametersTreeModel.h"

#include <QtGui>

#include "ParametersTreeItem.h"
#include <iostream>
#include <fstream>
#include <QFileDialog>

ParametersTreeModel::ParametersTreeModel(const QStringList &headers, const QString &data,
                     QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;

    rootItem = new ParametersTreeItem(rootData);
    setupModelData(data.split(QString("\n")), rootItem);
}

ParametersTreeModel::~ParametersTreeModel()
{
    delete rootItem;
}

int ParametersTreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}

QVariant ParametersTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    ParametersTreeItem *item = getItem(index);

    return item->data(index.column());
}

Qt::ItemFlags ParametersTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

ParametersTreeItem *ParametersTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        ParametersTreeItem *item = static_cast<ParametersTreeItem*>(index.internalPointer());
        if (item) return item;
    }
    return rootItem;
}

QVariant ParametersTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex ParametersTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    ParametersTreeItem *parentItem = getItem(parent);

    ParametersTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool ParametersTreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool ParametersTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    ParametersTreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

QModelIndex ParametersTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    ParametersTreeItem *childItem = getItem(index);
    ParametersTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool ParametersTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool ParametersTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    ParametersTreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int ParametersTreeModel::rowCount(const QModelIndex &parent) const
{
    ParametersTreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool ParametersTreeModel::setData(const QModelIndex &index, const QVariant &value,
                        int role)
{
    if (role != Qt::EditRole)
        return false;

    ParametersTreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool ParametersTreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void ParametersTreeModel::setupModelData(const QStringList &lines, ParametersTreeItem *parent)
{
    QList<ParametersTreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].mid(position, 1) != " ")
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            ParametersTreeItem *parent = parents.last();
            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
        }

        number++;
    }
}

void ParametersTreeModel::Save(QString fileName)
{
    std::cout << "Saving file to " << fileName.toStdString() << std::endl;
    std::ofstream file(fileName.toStdString().c_str(), std::ios::out | std::ios::trunc);

    QVariant a;
    ParametersTreeItem *treeItemData = rootItem;

    for (int i=0; i<=treeItemData->childCount(); i++) {
        QModelIndex indexA = index(i, 0, QModelIndex());
        a = index(i, 0, QModelIndex()).data();
        file << a.toString().toStdString()<<"\n";

        int j = 0;
        if (index(j, 0, indexA).isValid())
            while(index(j, 0, indexA).data().isValid()) {
                a = index(j, 0, indexA).data();
                if (a.toString().toStdString() == index(0, 0, QModelIndex()).data().toString().toStdString())
                    break;
                file << "  "<<a.toString().toStdString()<<"\n";
                QModelIndex indexB = index(j, 0, indexA);

                int k=0;
                if (index(k, 0, indexB).isValid())
                    while(index(k, 0, indexB).data().isValid()) {
                        a = index(k, 0, indexB).data();
                        if (a.toString().toStdString() == index(0, 0, QModelIndex()).data().toString().toStdString())
                            break;
                        std::cout << a.toString().toStdString() << std::endl;
                        file <<"    "<<a.toString().toStdString()<<"\n";
                        k++;
                    }
                j++;
            }
    }
}
