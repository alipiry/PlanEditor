#ifndef PARAMETERSTREEMODEL_H
#define PARAMETERSTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class ParametersTreeItem;

class ParametersTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ParametersTreeModel(const QStringList &headers, const QString &data,
              QObject *parent = 0);
    ~ParametersTreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole);

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex());
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex());
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex());
    void Save(QString fileName);

private:
    void setupModelData(const QStringList &lines, ParametersTreeItem *parent);
    ParametersTreeItem *getItem(const QModelIndex &index) const;

    ParametersTreeItem *rootItem;
    
};

#endif // PARAMETERSTREEMODEL_H
