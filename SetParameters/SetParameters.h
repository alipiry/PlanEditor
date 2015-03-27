#ifndef SETPARAMETERS_H
#define SETPARAMETERS_H

#include "ParametersTreeModel.h"

#include <QTreeView>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QModelIndex>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

class QAction;
class QTreeView;
class QWidget;

class SetParameters : public QWidget
{
    Q_OBJECT
    
public:
    explicit SetParameters(QWidget *parent = 0);
    ~SetParameters();

    ParametersTreeModel *model;

    QPushButton *insertRowButton;
    QPushButton *insertColumnButton;
    QPushButton *removeRowButton;
    QPushButton *removeColumnButton;
    QPushButton *insertChildButton;
    QPushButton *exitButton;
    QPushButton *findButton;
    QPushButton *loadButton;
    QPushButton *saveButton;
    QLineEdit   *searchLine;
    QVBoxLayout *vboxLayout;
    QTreeView   *view;
    QString     findParameters;

private slots:
    void insertChild();
    bool insertColumn(const QModelIndex &parent = QModelIndex());
    void insertRow();
    bool removeColumn(const QModelIndex &parent = QModelIndex());
    void removeRow();
    void loadFromFile();
    void saveToFile();
    void findClicked();

};

#endif // SETPARAMETERS_H
