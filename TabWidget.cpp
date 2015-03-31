#include "TabWidget.h"

#include <QFile>
#include <QtGui>
#include <QMenuBar>
#include <QTreeView>
#include <QFileDialog>
#include <QVBoxLayout>

TabWidget::TabWidget(QWidget *parent) :
    QWidget(parent)
{
    tabWidget = new QTabWidget;
    tabWidget->addTab(new HighLevelTab(), tr("HighLevel"));
    tabWidget->addTab(new FormationTab(), tr("Formation"));
    tabWidget->addTab(new SetParametersTab(), tr("SetParameters"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);

    setWindowTitle(tr("Strategy  -----> Keep Calm and Edit Parameters :D"));
}

TabWidget::~TabWidget()
{

}

SetParametersTab::SetParametersTab(QWidget *parent)
{
    theSetParameters = new SetParameters;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(theSetParameters);
    setLayout(mainLayout);
}

FormationTab::FormationTab(QWidget *parent)
{
    theFormation = new MainWindow;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(theFormation);
    setLayout(mainLayout);
}

HighLevelTab::HighLevelTab(QWidget *parent)
{
    theHighLevelParam = new HighLevelParam;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(theHighLevelParam);
    setLayout(mainLayout);
}
