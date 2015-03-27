#ifndef PARAMETERSTREEITEM_H
#define PARAMETERSTREEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

class ParametersTreeItem
{
public:
    ParametersTreeItem(const QVector<QVariant> &data, ParametersTreeItem *parent = 0);
    ~ParametersTreeItem();

    ParametersTreeItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    ParametersTreeItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

private:
    QList<ParametersTreeItem*> childItems;
    QVector<QVariant> itemData;
    ParametersTreeItem *parentItem;
};

#endif // PARAMETERSTREEITEM_H
