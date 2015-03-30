#include "HighLevelParam.h"

#include <QApplication>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <iostream>
#include <fstream>
#include <QtGui>
#include <QMenu>
#include <QLabel>

HighLevelParam::HighLevelParam(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(createHLParametersGroup(), 0, 0);
    grid->addWidget(createKickOffGroup(), 0, 1);
    setLayout(grid);

    setWindowTitle(tr("Group Boxes"));
    resize(300, 250);
}

HighLevelParam::~HighLevelParam()
{

}

QGroupBox *HighLevelParam::createHLParametersGroup()
{
    QGroupBox *HighLevelParameters = new QGroupBox(tr("HighLevelParameters"));

    fixPlan = new QCheckBox(tr("Fix Plan"));
    fixPlan->setChecked(false);
    commLess = new QCheckBox(tr("Communication-Less"));
    commLess->setChecked(false);
    QLabel *numOfPlayersLabel = new QLabel(tr("Number Of Players : "));
    numOfPlayers = new QSpinBox;
    QLabel *formationVersionLabel = new QLabel(tr("Formation Version   : "));
    formationVersion = new QSpinBox;

    Apply = new QPushButton(tr("&Apply"));
    Apply->setShortcut(QApplication::translate("SetParameters", "Ctrl+A", 0));

    QHBoxLayout *vbox1 = new QHBoxLayout;
    vbox1->addWidget(numOfPlayersLabel);
    vbox1->addWidget(numOfPlayers);
    vbox1->addStretch(1);

    QHBoxLayout *vbox2 = new QHBoxLayout;
    vbox2->addWidget(formationVersionLabel);
    vbox2->addWidget(formationVersion);
    vbox2->addStretch(1);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(fixPlan);
    vbox->addWidget(commLess);
    vbox->addLayout(vbox1);
    vbox->addLayout(vbox2);
    vbox->addWidget(Apply);
    vbox->addStretch(1);
    HighLevelParameters->setLayout(vbox);

    connect(fixPlan, SIGNAL(clicked(bool)), this, SLOT(chaneFixPlanFlag(bool)));
    connect(commLess, SIGNAL(clicked(bool)), this, SLOT(chaneCommLessFlag(bool)));

    return HighLevelParameters;
}

QGroupBox *HighLevelParam::createKickOffGroup()
{
    QGroupBox *kickOff = new QGroupBox(tr("KickOff"));
    kickOff->setCheckable(true);
    kickOff->setChecked(false);

    QRadioButton *automat = new QRadioButton(tr("Auto"));
    QRadioButton *toFreeSpace = new QRadioButton(tr("to Free Space"));
    QRadioButton *dribbling = new QRadioButton(tr("Dribbling"));
    QRadioButton *toTeamMate = new QRadioButton(tr("to TeamMate"));
    QRadioButton *fix = new QRadioButton(tr("Fix    :  "));
    QLabel       *X_label = new QLabel(tr("X : "));
    fix_x = new QSpinBox;
    QLabel       *Y_label = new QLabel(tr("Y : "));
    fix_y = new QSpinBox;
    fix_x->setEnabled(false);
    fix_y->setEnabled(false);
    automat->setChecked(true);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addStretch(1);
    hbox->addWidget(X_label);
    hbox->addWidget(fix_x);
    hbox->addWidget(Y_label);
    hbox->addWidget(fix_y);
    hbox->addStretch(4);

    QVBoxLayout *vbox3 = new QVBoxLayout;
    vbox3->addWidget(fix);
    vbox3->addLayout(hbox);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(automat);
    vbox->addWidget(toFreeSpace);
    vbox->addWidget(dribbling);
    vbox->addWidget(toTeamMate);
    vbox->addLayout(vbox3);
    vbox->addStretch(1);
    kickOff->setLayout(vbox);

    connect(fix, SIGNAL(clicked()), this, SLOT(setEnableXandY()));
    connect(automat, SIGNAL(clicked()), this, SLOT(setDisableXandY()));
    connect(toFreeSpace, SIGNAL(clicked()), this, SLOT(setDisableXandY()));
    connect(dribbling, SIGNAL(clicked()), this, SLOT(setDisableXandY()));
    connect(toTeamMate, SIGNAL(clicked()), this, SLOT(setDisableXandY()));
    connect(Apply, SIGNAL(clicked()), this, SLOT(applyChanges()));

    return kickOff;
}

void HighLevelParam::setEnableXandY()
{
    fix_x->setEnabled(true);
    fix_y->setEnabled(true);
}

void HighLevelParam::setDisableXandY()
{
    fix_x->setEnabled(false);
    fix_y->setEnabled(false);
}

void HighLevelParam::chaneFixPlanFlag(bool flag)
{
    fixPlanFlag = flag;
    std::cout << flag << std::endl;
}

void HighLevelParam::chaneCommLessFlag(bool flag)
{
    commLessFlag = flag;
}

void HighLevelParam::applyChanges()
{
    QString address = "../behaviorHLParameters.cfg";
    std::string stradd = address.toStdString();
    std::cout << "Writing file on " << stradd<< std::endl;

    std::ofstream file(address.toStdString().c_str(), std::ios::out | std::ios::trunc);
    if (!file)
    {
        std::cerr << "could not write file..." << std::endl;
        return;
    }

    std::string strFix = "false",
                strComm = "false";

    if (fixPlanFlag == true)
        strFix = "true";
    else
        strFix = "false";

    if (commLessFlag == true)
        strComm = "true";
    else
        strComm = "false";

    file << "fixPlan = " << strFix << std::endl
        << "commless = " << strComm << std::endl
        << "numOfPlayers = " << numOfPlayers->value() << std::endl
        << "formationVersion = " << formationVersion->value() << std::endl;
}
