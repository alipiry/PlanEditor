#ifndef TABWIDGET_H
#define TABWIDGET_H

#include "SetParameters/SetParameters.h"
#include "HLParameters/HighLevelParam.h"
#include "Formation/mainwindow.h"

#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QString>

class SetParameters;
class HighLevelParam;
class QFileInfo;
class QTabWidget;

class TabWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit TabWidget(QWidget *parent = 0);
    ~TabWidget();
    
private:
    QMenu *fileMenu;
    QTabWidget *tabWidget;
    QPushButton *exitButton;
};

class SetParametersTab : public QWidget
{
    Q_OBJECT

public:
    SetParametersTab(QWidget *parent = 0);

    SetParameters *theSetParameters;
};

class FormationTab : public QWidget
{
    Q_OBJECT

public:
    FormationTab(QWidget *parent = 0);

    MainWindow *theFormation;

};

class HighLevelTab : public QWidget
{
    Q_OBJECT

public:
    HighLevelTab(QWidget *parent = 0);

    HighLevelParam *theHighLevelParam;

};

#endif // TABWIDGET_H
