#ifndef TABWIDGET_H
#define TABWIDGET_H

#include "SetParameters/SetParameters.h"
#include "Formation/mainwindow.h"

#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QString>

class SetParameters;
class Formation;
class QFileInfo;
class QTabWidget;

//namespace Ui {
//class TabWidget;
//}

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
#endif // TABWIDGET_H
