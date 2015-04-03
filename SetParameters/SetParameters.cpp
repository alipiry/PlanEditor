#include "SetParameters.h"
#include "ParametersTreeModel.h"

#include <QApplication>
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <QtGui>
#include <QLabel>
#include <QLineEdit>

SetParameters::SetParameters(QWidget *parent) :
    QWidget(parent)
{

#ifdef Q_WS_MAEMO_5
    // Alternating row colors look bad on Maemo
    view->setAlternatingRowColors(false);
#endif

    QLabel *searchLabel = new QLabel(tr("Search:"));
    searchLine = new QLineEdit;

    findParameters = "";

    insertRowButton    = new QPushButton(tr("Insert&Row"));
    insertColumnButton = new QPushButton(tr("Insert&Column"));
    removeRowButton    = new QPushButton(tr("Remove&Row"));
    removeColumnButton = new QPushButton(tr("Remove&Column"));
    insertChildButton  = new QPushButton(tr("&InsertChild"));
    findButton = new QPushButton(tr("&Find"));
    loadButton = new QPushButton(tr("&Load..."));
    loadButton->setToolTip(tr("Load contacts from a file"));
    saveButton = new QPushButton(tr("&Save..."));
    saveButton->setToolTip(tr("Save contacts to a file"));

    insertRowButton->setShortcut(QApplication::translate("SetParameters", "Ctrl+Shift+R", 0));
    insertColumnButton->setShortcut(QApplication::translate("SetParameters", "Ctrl+Shift+C", 0));
    removeRowButton->setShortcut(QApplication::translate("SetParameters", "Ctrl+R", 0));
    removeColumnButton->setShortcut(QApplication::translate("SetParameters", "Ctrl+C", 0));
    insertChildButton->setShortcut(QApplication::translate("SetParameters", "Ctrl+I", 0));
    findButton->setShortcut(QApplication::translate("SetParameters", "Ctrl+F", 0));
    loadButton->setShortcut(QApplication::translate("SetParameters", "Ctrl+L", 0));
    saveButton->setShortcut(QApplication::translate("SetParameters", "Ctrl+S", 0));

    insertRowButton->setEnabled(false);
    insertColumnButton->setEnabled(false);
    removeRowButton->setEnabled(false);
    removeColumnButton->setEnabled(false);
    insertChildButton->setEnabled(false);
    findButton->setEnabled(false);
    saveButton->setEnabled(false);

    QStringList headers;
    headers << tr("Title") << tr("Description");

    view = new QTreeView;
//    view->setObjectName(QStringLiteral("view"));       ????????
    view->setAlternatingRowColors(true);
    view->setSelectionBehavior(QAbstractItemView::SelectItems);
    view->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    view->setAnimated(false);
    view->setAllColumnsShowFocus(true);

    QVBoxLayout *buttonLayout1 = new QVBoxLayout;
    buttonLayout1->addWidget(insertRowButton);
    buttonLayout1->addWidget(insertColumnButton);
    buttonLayout1->addWidget(removeRowButton);
    buttonLayout1->addWidget(removeColumnButton);
    buttonLayout1->addWidget(insertChildButton);
    buttonLayout1->addWidget(loadButton);
    buttonLayout1->addWidget(saveButton);
    buttonLayout1->addStretch();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(searchLabel, 0, 0);
    mainLayout->addWidget(view, 1, 1);
    mainLayout->addWidget(searchLine, 0, 1);
    mainLayout->addWidget(findButton, 0, 2);
    mainLayout->addLayout(buttonLayout1, 1, 2);

    setLayout(mainLayout);
    setWindowTitle(tr("Set ANY Parameters That You Like :D"));

    connect(findButton, SIGNAL(clicked()), this, SLOT(findClicked()));
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveToFile()));
    connect(insertRowButton, SIGNAL(clicked()), this, SLOT(insertRow()));
    connect(insertColumnButton, SIGNAL(clicked()), this, SLOT(insertColumn()));
    connect(removeRowButton, SIGNAL(clicked()), this, SLOT(removeRow()));
    connect(removeColumnButton, SIGNAL(clicked()), this, SLOT(removeColumn()));
    connect(insertChildButton, SIGNAL(clicked()), this, SLOT(insertChild()));
}

SetParameters::~SetParameters()
{

}

void SetParameters::insertChild()
{
    QModelIndex index = view->selectionModel()->currentIndex();
    QAbstractItemModel *model = view->model();

    if (model->columnCount(index) == 0) {
        if (!model->insertColumn(0, index))
            return;
    }

    if (!model->insertRow(0, index))
        return;

    for (int column = 0; column < model->columnCount(index); ++column) {
        QModelIndex child = model->index(0, column, index);
        model->setData(child, QVariant("[No data]"), Qt::EditRole);
        if (!model->headerData(column, Qt::Horizontal).isValid())
            model->setHeaderData(column, Qt::Horizontal, QVariant("[No header]"),
                                 Qt::EditRole);
    }

    view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                            QItemSelectionModel::ClearAndSelect);
}

bool SetParameters::insertColumn(const QModelIndex &parent)
{
    QAbstractItemModel *model = view->model();
    int column = view->selectionModel()->currentIndex().column();

    // Insert a column in the parent item.
    bool changed = model->insertColumn(column + 1, parent);
    if (changed)
        model->setHeaderData(column + 1, Qt::Horizontal, QVariant("[No header]"),
                             Qt::EditRole);

    return changed;
}

void SetParameters::insertRow()
{
    QModelIndex index = view->selectionModel()->currentIndex();
    QAbstractItemModel *model = view->model();

    if (!model->insertRow(index.row()+1, index.parent()))
        return;

    for (int column = 0; column < model->columnCount(index.parent()); ++column) {
        QModelIndex child = model->index(index.row()+1, column, index.parent());
        model->setData(child, QVariant("[No data]"), Qt::EditRole);
    }
}

bool SetParameters::removeColumn(const QModelIndex &parent)
{
    QAbstractItemModel *model = view->model();
    int column = view->selectionModel()->currentIndex().column();

    // Insert columns in each child of the parent item.
    bool changed = model->removeColumn(column, parent);

    return changed;
}

void SetParameters::removeRow()
{
    QModelIndex index = view->selectionModel()->currentIndex();
    QAbstractItemModel *model = view->model();
}

void SetParameters::findClicked()
{

}

void SetParameters::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Config-File"), "",
        tr("Config-File *.cfg (*.cfg)"));

    std::cout << "Loading file from " << fileName.toStdString() << std::endl;

    QStringList headers;
    headers << tr("Title") << tr("Description");

    QFile file(tr("%1").arg(fileName));
    file.open(QIODevice::ReadOnly);
    model = new ParametersTreeModel(headers, file.readAll());
    file.close();

    view->setModel(model);
    for (int column = 0; column < model->columnCount(); ++column)
        view->resizeColumnToContents(column);

    if (fileName.toStdString() != "") {
        insertRowButton->setEnabled(true);
        insertColumnButton->setEnabled(true);
        removeRowButton->setEnabled(true);
        removeColumnButton->setEnabled(true);
        insertChildButton->setEnabled(true);
        findButton->setEnabled(true);
        saveButton->setEnabled(true);
    }
    else {
        insertRowButton->setEnabled(false);
        insertColumnButton->setEnabled(false);
        removeRowButton->setEnabled(false);
        removeColumnButton->setEnabled(false);
        insertChildButton->setEnabled(false);
        findButton->setEnabled(false);
        saveButton->setEnabled(false);
    }
}

void SetParameters::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("SetParameters"), "",
        tr("Config-File *.cfg(*.cfg);;All Files (*)"));

    model->Save(fileName);
}
