#include "HighLevelParam.h"

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QApplication>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <iostream>
#include <QTextEdit>
#include <fstream>
#include <QtGui>
#include <QMenu>
#include <QLabel>

HighLevelParam::HighLevelParam(QWidget *parent)
    : QWidget(parent),
      commLessFlag(false),
      fixPlanFlag(false),
      kickOffFlag(0)
{
    Apply = new QPushButton(tr("&Apply"));
    Apply->setShortcut(QApplication::translate("SetParameters", "Ctrl+A", 0));
    Apply->setFixedHeight(40);
    Apply->setFixedWidth(200);

    text = new QTextEdit;
    text->setFixedHeight(450);
    text->setFixedWidth(560);

    QHBoxLayout *vb = new QHBoxLayout;
    vb->addStretch(1);
    vb->addWidget(Apply);
    vb->addStretch(1);

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(createHLParametersGroup(), 0, 0);
    grid->addWidget(createKickOffGroup(), 1, 0);
    grid->addWidget(text, 0, 1);
    grid->addWidget(Apply, 2, 2);
    setLayout(grid);

    QString address = "../behaviorHLParameters.cfg";
    std::string stradd = address.toStdString();
    std::cout << "Loading file from " << stradd<< std::endl;
    loadConfig(stradd);

    setWindowTitle(tr("High Level Strategy"));
    resize(300, 250);
}

HighLevelParam::~HighLevelParam()
{

}

QGroupBox *HighLevelParam::createHLParametersGroup()
{
    QGroupBox *HighLevelParameters = new QGroupBox(tr("HighLevelParameters"));

    fixPlan          = new QCheckBox(tr("Fix Plan"));
    fixPlan->setChecked(false);
    commLess         = new QCheckBox(tr("Communication-Less"));
    commLess->setChecked(false);
    QLabel *numOfPlayersLabel     = new QLabel(tr("Number Of Players : "));
    numOfPlayers     = new QSpinBox;
    QLabel *formationVersionLabel = new QLabel(tr("Formation Version   : "));
    formationVersion = new QSpinBox;

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
    vbox->addStretch(1);
    HighLevelParameters->setLayout(vbox);

    HighLevelParameters->setFixedHeight(500);

    connect(fixPlan, SIGNAL(clicked(bool)), this, SLOT(chaneFixPlanFlag(bool)));
    connect(commLess, SIGNAL(clicked(bool)), this, SLOT(chaneCommLessFlag(bool)));

    return HighLevelParameters;
}

QGroupBox *HighLevelParam::createKickOffGroup()
{
    QGroupBox *kickOff = new QGroupBox(tr("KickOff"));

    automat = new QRadioButton(tr("Auto"));
    automat->setChecked(true);

    toFreeSpace = new QRadioButton(tr("to Free Space"));
    dribbling   = new QRadioButton(tr("Dribbling"));
    toTeamMate  = new QRadioButton(tr("to TeamMate"));
    fix                       = new QRadioButton(tr("Fix    :  "));
    QLabel       *X_label     = new QLabel(tr("X : "));
    QLabel       *Y_label     = new QLabel(tr("  Y : "));

    fix_x = new QSpinBox;
    fix_x->setMaximum(9999);
    fix_x->setMinimum(0);
    fix_y = new QSpinBox;
    fix_y->setMaximum(9999);
    fix_y->setMinimum(0);
    fix_x->setEnabled(false);
    fix_y->setEnabled(false);


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

    kickOff->setFixedHeight(600);

    connect(fix, SIGNAL(clicked()), this, SLOT(setFix()));
    connect(automat, SIGNAL(clicked()), this, SLOT(setAutomat()));
    connect(toFreeSpace, SIGNAL(clicked()), this, SLOT(setToFreeSpace()));
    connect(dribbling, SIGNAL(clicked()), this, SLOT(setDribbling()));
    connect(toTeamMate, SIGNAL(clicked()), this, SLOT(setToTeamMate()));
    connect(Apply, SIGNAL(clicked()), this, SLOT(applyChanges()));

    return kickOff;
}

void HighLevelParam::setFix()
{
    kickOffFlag = 4;
    fix_x->setEnabled(true);
    fix_y->setEnabled(true);

}

void HighLevelParam::setAutomat()
{
    kickOffFlag = 0;
    fix_x->setEnabled(false);
    fix_y->setEnabled(false);
    automat->setEnabled(true);
    toFreeSpace->setEnabled(true);
    dribbling->setEnabled(true);
    toTeamMate->setEnabled(true);
}

void HighLevelParam::setToFreeSpace()
{
    kickOffFlag = 1;
    fix_x->setEnabled(false);
    fix_y->setEnabled(false);
    automat->setEnabled(true);
    toFreeSpace->setEnabled(true);
    dribbling->setEnabled(true);
    toTeamMate->setEnabled(true);
}

void HighLevelParam::setDribbling()
{
    kickOffFlag = 2;
    fix_x->setEnabled(false);
    fix_y->setEnabled(false);
    automat->setEnabled(true);
    toFreeSpace->setEnabled(true);
    dribbling->setEnabled(true);
    toTeamMate->setEnabled(true);
}

void HighLevelParam::setToTeamMate()
{
    kickOffFlag = 3;
    fix_x->setEnabled(false);
    fix_y->setEnabled(false);
    automat->setEnabled(true);
    toFreeSpace->setEnabled(true);
    dribbling->setEnabled(true);
    toTeamMate->setEnabled(true);
}

void HighLevelParam::chaneFixPlanFlag(bool flag)
{
    fixPlanFlag = flag;

    if (flag == true) {
        fix_x->setEnabled(true);
        fix_y->setEnabled(true);
        fix->toggle();
        setFix();
        automat->setEnabled(false);
        toFreeSpace->setEnabled(false);
        dribbling->setEnabled(false);
        toTeamMate->setEnabled(false);
    }

    else {
        fix_x->setEnabled(false);
        fix_y->setEnabled(false);
        automat->toggle();
        setAutomat();
        automat->setEnabled(true);
        toFreeSpace->setEnabled(true);
        dribbling->setEnabled(true);
        toTeamMate->setEnabled(true);
    }
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

    if (commLessFlag == true)
        strComm = "true";

    file << "/**" << std::endl
         << " *  Parameters for strategy/planning of the game." << std::endl
         << " *  High Level Behavior 2015" << std::endl
         << " */" << std::endl << std::endl << "FixPlan = {" << std::endl
         << "  fix = "<<strFix <<";"
         << "           // a fix planning/strategy is set on robots based on following parameters"<< std::endl
         << "  commless = " << strComm <<";" << "          // state of the communication"<< std::endl
         << "  numOfPlayers = " << numOfPlayers->value() << ";"<< "          // number of players to coordinate minus the goalkeeper"
         << std::endl
         << "  formationVersion = " << formationVersion->value() <<";"<< "      // version of the formation" << std::endl
         << "};" << std::endl << std::endl << "KickOff = {" <<std::endl << "  kickOffType = "<<kickOffFlag <<";" <<std::endl
         << "  kickOffFixTarget = {x = " << fix_x->value()
         << "; y = "<< fix_y->value() <<";};" << std::endl<<"};";

    loadConfig(address.toStdString());
}

void HighLevelParam::loadConfig(const std::string& add)
{
    std::ifstream file(add.c_str(), std::ios::in);
    if (!file) { std::cerr << "File not found..." << std::endl; return; }

    QString setText = "";
    char sz[255];

    while (!file.eof())
    {
        file.getline(sz, 255);
        setText += sz;
        setText += '\n';
        text->setText(setText);
        processLine(sz);
    }
}

void HighLevelParam::processLine(const char* str)
{
    if (!str) return;
    QString s = "";
    QString w = "";
    bool isValue = false;
    bool neg = false;

    unsigned int i;

    for (i=0; str[i] != ';'; i++)
    {
        if (str[i] == '#' || (str[i] == '/' && str[i+1] == '/') ||
                (str[i] == '/' && str[i+1] == '*') || str[i] == '*')
            break;

        if (str[i] == '\0')
            break;

        if (str[i] == ' ' || str[i] == '\t')
            continue;

        if (str[i] == '}' || str[i] == ';' || str[i] == '{')
            i++;

        if (str[i] == '=' && str[i+2] != '{')
        {
            isValue = true;
            continue;
        }

        if (str[i] == '-')
        {
            neg = true;
            continue;
        }

        if (isValue)
        {
            if (str[i] == '{')
                break;

            for (i; str[i] != ';'; i++)
                s += str[i];

            isValue = false;
        }

        if (w == "fix")
        {
            if (s == "false")
                fixPlan->setChecked(false);
            else {
                fixPlan->setChecked(true);
                fix->toggle();
                fix_x->setEnabled(true);
                fix_y->setEnabled(true);
                automat->setEnabled(false);
                toFreeSpace->setEnabled(false);
                dribbling->setEnabled(false);
                toTeamMate->setEnabled(false);
            }
        }

        if (w == "commless")
        {
            if (s == "false")
                commLess->setChecked(false);
            else
                commLess->setChecked(true);
        }

        if (w == "kickOffType")
            switch (s.toInt()) {
            case 0:
                automat->setEnabled(true);
                automat->toggle();
                break;
            case 1:
                toFreeSpace->setEnabled(true);
                toFreeSpace->toggle();
                break;
            case 2:
                dribbling->setEnabled(true);
                dribbling->toggle();
                break;
            case 3:
                toTeamMate->setEnabled(true);
                toTeamMate->toggle();
                break;
            case 4:
                fix->setEnabled(true);
                fix->toggle();
                break;
            default:
                break;
            }

        if (w == "numOfPlayers")
            numOfPlayers->setValue(s.toInt());

        if (w == "formationVersion")
            formationVersion->setValue(s.toInt());

        if (w == "kickOffFixTarget")
            w = "";

        if (w == "=x") {
            fix_x->setValue(s.toInt());
            w = "";
            s = "";
        }

        if (w == "y") {
            fix_y->setValue(s.toInt());
            w = "";
            s = "";
        }

        if (str[i] == ';')
            i = i+2;

        w += str[i];
    }
}
