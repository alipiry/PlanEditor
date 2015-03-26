#include "ParametersTreeItem.h"

#include <QStringList>
#include <iostream>

//using namespace std;

ParametersTreeItem::ParametersTreeItem(const QVector<QVariant> &data, ParametersTreeItem *parent)
{
    parentItem = parent;
    itemData = data;
}

ParametersTreeItem::~ParametersTreeItem()
{
    qDeleteAll(childItems);
}

ParametersTreeItem *ParametersTreeItem::child(int number)
{
    return childItems.value(number);
}

int ParametersTreeItem::childCount() const
{
    return childItems.count();
}

int ParametersTreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<ParametersTreeItem*>(this));

    return 0;
}

int ParametersTreeItem::columnCount() const
{
    return itemData.count();
}

QVariant ParametersTreeItem::data(int column) const
{
    return itemData.value(column);
}

bool ParametersTreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        ParametersTreeItem *item = new ParametersTreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool ParametersTreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (ParametersTreeItem *child, childItems)
        child->insertColumns(position, columns);

    return true;
}

ParametersTreeItem *ParametersTreeItem::parent()
{
    return parentItem;
}

bool ParametersTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool ParametersTreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (ParametersTreeItem *child, childItems)
        child->removeColumns(position, columns);

    return true;
}

bool ParametersTreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}
