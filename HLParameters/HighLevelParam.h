#ifndef HighLevelParam_H
#define HighLevelParam_H

#include <QRadioButton>
#include <QPushButton>
#include <QGroupBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QWidget>

class QGroupBox;

class HighLevelParam : public QWidget
{
    Q_OBJECT
    
public:
    explicit HighLevelParam(QWidget *parent = 0);
    ~HighLevelParam();

    bool commLessFlag = false;
    bool fixPlanFlag  = false;

    QPushButton *Apply;
    QCheckBox   *fixPlan;
    QCheckBox   *commLess;
    QSpinBox    *numOfPlayers;
    QSpinBox    *formationVersion;
    QSpinBox    *KickOffX;
    QSpinBox    *KickOffY;
    QRadioButton*fix;

private slots:
    void setEnableXandY();
    void setDisableXandY();
    void applyChanges();
    void chaneFixPlanFlag(bool);
    void chaneCommLessFlag(bool);

signals:
    void checked(bool);

private:
    QGroupBox *createHLParametersGroup();
    QGroupBox *createKickOffGroup();

    QSpinBox *fix_x;
    QSpinBox *fix_y;
    bool enableFlag;
};

#endif // HighLevelParam_H


