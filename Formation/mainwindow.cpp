#include "ui_mainwindow.h"
#include "mainwindow.h"

#include <QFileSystemModel>
#include <qmessagebox.h>
#include <QFileDialog>
#include <QSplitter>
#include <fstream>
#include <cmath>

#include <iostream>

unsigned int VoronoiParticle::_aiID = 0;
bool MouseClicked=false;

#ifdef FAST_PERFORMANCE
#  define FOR(iterator, initial, cond) \
    for (int iterator=initial; cond; iterator+=2)
#else
#  define FOR(iterator, initial, cond) \
    for (int iterator=0; cond; iterator++)
#endif

void Monitor::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() != Qt::LeftButton) return;
    MouseClicked = true;

    sX = ev->x();
    sY = ev->y();
}

void Monitor::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() != Qt::LeftButton) return;

    emit addParticle(sX, sY, ev->x(), ev->y());
    MouseClicked = false;
}

void Monitor::mouseMoveEvent(QMouseEvent *ev)
{
    if (MouseClicked)
        emit mMove(sX, sY, ev->x(), ev->y());
    emit cordinatePointer(ev->x()*2, ev->y()*2);
}

//void Monitor::mousePressEvent(QMouseEvent *ev)
//{
//    if (ev->button() != Qt::LeftButton) return;

//    sX = ev->x();
//    sY = ev->y();
//}

//void Monitor::mouseReleaseEvent(QMouseEvent *ev)
//{
//    if (ev->button() != Qt::LeftButton) return;
//    emit addParticle(sX, sY, ev->x(), ev->y());
//}

//void Monitor::mouseMoveEvent(QMouseEvent *ev)
//{
//    emit mMove(sX, sY, ev->x(), ev->y());
//}

void MainWindow::getMouseMove(int x1, int y1, int x2, int y2)
{
    if (abs(x1-x2) < SNAP_MAX && abs(y1-y2) < SNAP_MAX) return;

    double theta = atan2(y2-y1 , x2-x1);

    qpn->setPen(QPen(Qt::black, 2));
    qpn->drawLine(x2 - cos(theta)*10, y2 - sin(theta)*10, x1 + cos(theta)*10, y1 + sin(theta)*10);
    qpn->drawLine(x2 - cos(theta)*10, y2 - sin(theta)*10, x2 - cos(theta+0.2)*30, y2 - sin(theta+0.2)*30);
    qpn->drawLine(x2 - cos(theta)*10, y2 - sin(theta)*10, x2 - cos(theta-0.2)*30, y2 - sin(theta-0.2)*30);
    refreshUI();
    qpn->setPen(QPen(Qt::darkGreen, 2));
    qpn->drawLine(x2 - cos(theta)*10, y2 - sin(theta)*10, x1 + cos(theta)*10, y1 + sin(theta)*10);
    qpn->drawLine(x2 - cos(theta)*10, y2 - sin(theta)*10, x2 - cos(theta+0.2)*30, y2 - sin(theta+0.2)*30);
    qpn->drawLine(x2 - cos(theta)*10, y2 - sin(theta)*10, x2 - cos(theta-0.2)*30, y2 - sin(theta-0.2)*30);
}

void MainWindow::getParticle(int x, int y, int cx, int cy)
{
    on_update_clicked();
    hasSaved = false;

    x = x-550/2;
    y = 400/2-y;
    cx = cx-550/2;
    cy = 400/2-cy;

    if (abs(x-cx) < SNAP_MAX && abs(y-cy) < SNAP_MAX)
        addParticle(VoronoiParticle(x, y));
    else if (!particles.size())
        addParticle(VoronoiParticle(x, y, cx, cy));
    else
    {
        float d;
        unsigned int nid = nearBy(cx, cy, d);
        const VoronoiParticle& p = particles[nid];

        if (d < SNAP_MAX)
            addParticle(VoronoiParticle(x, y, p));
        else
            addParticle(VoronoiParticle(x, y, cx, cy));

    }

    drawField();
    drawParticles();
    refreshUI();
}

void MainWindow::setLabel(int x, int y)
{
    x = x-550;
    y = 400-y;

    ui->label_6->setText(QString("%1, %2").arg(x).arg(y));
}

void MainWindow::addParticle(const VoronoiParticle& p)
{
    ui->b_id->setValue(particles.size());
    ui->b_x->setValue(p.xraw()*2);
    ui->b_y->setValue(p.yraw()*2);
    ui->b_cx->setValue(p.cxraw()*2);
    ui->b_cy->setValue(p.cyraw()*2);
    ui->b_name->setText("");
    ui->NumOfSup->setValue(p.num());
    ui->b_par->setValue(p.id());

    particles.push_back(p);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    SNAP_MAX(30),
    addr("empty"),
    hasSaved(true)
{
    ui->setupUi(this);
    setMouseTracking(true);

    dirModel = new QFileSystemModel;
    dirModel->setRootPath("~/");
    QStringList filters;
    filters << "*.cfg";
    dirModel->setNameFilters(filters);
    dirModel->setNameFilterDisables(false);
    ui->listView->setModel(dirModel);
    ui->listView->setRootIndex(dirModel->index(QDir::currentPath()));

    monitor = new Monitor(ui->centralWidget);
    monitor->setObjectName(QString::fromUtf8("monitor"));
    monitor->setGeometry(QRect(10/2, 10/2, 1100/2, 800/2));
    monitor->setFrameShape(QFrame::Box);
    connect(monitor, SIGNAL(addParticle(int,int,int,int)), this, SLOT(getParticle(int,int,int,int)));
    connect(monitor, SIGNAL(mMove(int,int,int,int)), this, SLOT(getMouseMove(int,int,int,int)));
    connect(monitor, SIGNAL(cordinatePointer(int,int)), this, SLOT(setLabel(int,int)));

    qmg = new QImage(1100/2, 800/2, QImage::Format_RGB888);
    qpn = new QPainter(qmg);

    ui->createNew->setShortcut(QApplication::translate("SetParameters", "Ctrl+N", 0));
    ui->save->setShortcut(QApplication::translate("SetParameters", "Ctrl+Shift+S", 0));
    ui->simpleSave->setShortcut(QApplication::translate("SetParameters", "Ctrl+S", 0));
    ui->load->setShortcut(QApplication::translate("SetParameters", "Ctrl+L", 0));
    ui->clear->setShortcut(QApplication::translate("SetParameters", "Ctrl+R", 0));
    ui->update->setShortcut(QApplication::translate("SetParameters","Ctrl+D", 0));
    ui->mirrorX->setShortcut(QApplication::translate("SetParameters", "Ctrl+X", 0));
    ui->mirrorY->setShortcut(QApplication::translate("SetParameters", "Ctrl+Y", 0));


    drawField();
    refreshUI();
}

MainWindow::~MainWindow()
{
    delete qpn;
    delete qmg;
    delete ui;
}

void MainWindow::drawField()
{
    //-- Carpet
    qpn->setBrush(Qt::darkGreen);
    qpn->drawRect(0/2, 0/2, 1100/2, 800/2);

    //-- Goals
    qpn->setPen(Qt::yellow);
    qpn->setBrush(QBrush(Qt::yellow, Qt::Dense6Pattern));
    qpn->drawRect(50/2, 325/2, 50/2, 150/2);
    qpn->setBrush(QBrush(Qt::blue, Qt::Dense6Pattern));
    qpn->drawRect(1000/2, 325/2, 50/2, 150/2);

    //-- Lines
    qpn->setBrush(Qt::NoBrush);
    qpn->setPen(QPen(Qt::white, 5/2));
    qpn->drawRect(100/2, 100/2, 900/2, 600/2);
    qpn->drawRect(100/2, 290/2, 60/2, 220/2); //-- Penalty Box
    qpn->drawRect(940/2, 290/2, 60/2, 220/2); //-- Penalty Box
    qpn->drawLine(550/2, 100/2, 550/2, 700/2); //-- Half Line
    qpn->drawEllipse(QPoint(550/2, 400/2), 75/2, 75/2); //-- Circle
    qpn->drawEllipse(QPoint(230/2, 400/2), 5/2, 5/2); //-- Cross
    qpn->drawEllipse(QPoint(870/2, 400/2), 5/2, 5/2); //-- Cross

    //-- Self Goal Posts
    qpn->setBrush(Qt::yellow);
    qpn->setPen(Qt::yellow);
    qpn->drawEllipse(QPoint(100/2, 325/2), 5/2, 5/2);
    qpn->drawEllipse(QPoint(100/2, 475/2), 5/2, 5/2);

    //-- Opp. Goal Posts
    qpn->setBrush(Qt::yellow);
    qpn->setPen(Qt::yellow);
    qpn->drawEllipse(QPoint(1000/2, 325/2), 5/2, 5/2);
    qpn->drawEllipse(QPoint(1000/2, 475/2), 5/2, 5/2);
}

void MainWindow::drawParticles()
{
    if (!particles.size()) return;

    //-- Draw Centers
    qpn->setBrush(Qt::darkBlue);
    qpn->setPen(QPen(Qt::blue, 3));
    for (std::vector<VoronoiParticle>::const_iterator p=particles.begin(); p<particles.end(); p++)
        if (p->isMoved())
            qpn->drawPoint(p->cx(), p->cy());

    //-- Draw Arrows
    qpn->setBrush(Qt::NoBrush);
    qpn->setPen(QPen(Qt::black, 2));

    for (std::vector<VoronoiParticle>::const_iterator p=particles.begin(); p<particles.end(); p++)
        if (p->isMoved())
        {
            double theta = atan2(p->cy()-p->y() , p->cx()-p->x());

            qpn->drawLine(p->cx() - cos(theta)*10, p->cy() - sin(theta)*10, p->x() + cos(theta)*10, p->y() + sin(theta)*10);
            qpn->drawLine(p->cx() - cos(theta)*10, p->cy() - sin(theta)*10, p->cx() - cos(theta+0.2)*30, p->cy() - sin(theta+0.2)*30);
            qpn->drawLine(p->cx() - cos(theta)*10, p->cy() - sin(theta)*10, p->cx() - cos(theta-0.2)*30, p->cy() - sin(theta-0.2)*30);
        }

    //-- Draw Particles
    qpn->setBrush(Qt::darkRed);
    qpn->setPen(QPen(Qt::red, 5));
    for (std::vector<VoronoiParticle>::const_iterator p=particles.begin(); p<particles.end(); p++)
        qpn->drawPoint(p->x(), p->y());

    //-- Draw texts
    qpn->setBrush(Qt::NoBrush);
    qpn->setPen(QPen(Qt::black));
    for (std::vector<VoronoiParticle>::const_iterator p=particles.begin(); p<particles.end(); p++)
        if (p->_name != "")
            qpn->drawText(p->x(), p->y()+15, QString::fromStdString(p->_name));

    //-- Draw ID Numbers
    qpn->setBrush(Qt::NoBrush);
    qpn->setPen(QPen(Qt::black));
    for (std::vector<VoronoiParticle>::const_iterator p=particles.begin(); p<particles.end(); p++)
            qpn->drawText(p->x()-15, p->y()+15, QString::fromStdString(tr("%1").arg(p->_id).toStdString()) + ".");

    //-- Draw Borders
    qpn->setPen(QPen(Qt::green, 1));
    FOR (x, 0, x<qmg->width())
//    for (int x=0; x<qmg->width(); x++)
    {
        unsigned int lastID = 0;
        FOR (y, 0, y<qmg->height())
//        for (int y=0; y<qmg->height(); y++)
        {
            unsigned int id = nearestID(x, y);
            if (lastID == id)
                continue;

            qpn->drawPoint(x, y);
            lastID = id;
        }
    }

    FOR (y, 1, y<qmg->height()-2)
//    for (int y=1; y<qmg->height(); y++)
    {
        unsigned int lastID = 0;
        FOR (x, 1, x<qmg->width()-2)
//        for (int x=1; x<qmg->width(); x++)
        {
            unsigned int id = nearestID(x, y);
            if (lastID == id)
                continue;

            qpn->drawPoint(x, y);
            lastID = id;
        }
    }
}

unsigned int MainWindow::nearestID(int x, int y)
{
    float distance = 9999999999;
    unsigned int id = -1;
    for (std::vector<VoronoiParticle>::const_iterator p=particles.begin(); p<particles.end(); p++)
    {
        const float d = (x-p->x())*(x-p->x()) + (y-p->y())*(y-p->y());

        if (distance > d)
        {
            distance = d;
            id = p->id();
        }
    }

    return id;
}

unsigned int MainWindow::nearBy(int x, int y, float& d)
{
    float distance = 9999999999;
    unsigned int id = -1;
    for (unsigned int i=0; i<particles.size(); i++)
    {
        const VoronoiParticle& p = particles[i];
        const float d1 = sqrt((x-p.xraw())*(x-p.xraw()) + (y-p.yraw())*(y-p.yraw()));
        const float d2 = sqrt((x-p.cxraw())*(x-p.cxraw()) + (y-p.cyraw())*(y-p.cyraw()));

        if (distance > d1)
        {
            distance = d1;
            id = i;
        }

        if (distance > d2)
        {
            distance = d2;
            id = i;
        }
    }

    d = distance;
    return id;
}

void MainWindow::refreshUI()
{
    monitor->setPixmap(QPixmap::fromImage(*qmg));
}

void MainWindow::on_b_id_editingFinished()
{
    if ((unsigned)ui->b_id->value() >= particles.size()) return;

    const VoronoiParticle& p = particles[ui->b_id->value()];
    ui->b_x->setValue(p.xraw()*2);
    ui->b_y->setValue(p.yraw()*2);
    ui->b_cx->setValue(p.cxraw()*2);
    ui->b_cy->setValue(p.cyraw()*2);
    ui->b_name->setText(QString::fromStdString(p._name));
    ui->NumOfSup->setValue(p.num());
    ui->b_par->setValue(p.id());
}

void MainWindow::on_update_clicked()
{
    if ((unsigned)ui->b_id->value() >= particles.size()) return;
    VoronoiParticle& p = particles[ui->b_id->value()];

    if ((unsigned)ui->b_par->value() < particles.size() && p.id() != (unsigned)ui->b_par->value())
    {
        p = VoronoiParticle(ui->b_x->value(), ui->b_y->value(), particles[ui->b_par->value()]);
        ui->b_cx->setValue(p.cxraw());
        ui->b_cy->setValue(p.cyraw());
    }
    else
    {
        p._x = ui->b_x->value()/2;
        p._y = ui->b_y->value()/2;
        p._cx = ui->b_cx->value()/2;
        p._cy = ui->b_cy->value()/2;
    }
    p._name = ui->b_name->text().toStdString();
    p._NumOfSup = ui->NumOfSup->value();

    drawField();
    drawParticles();
    refreshUI();
}

void MainWindow::on_clear_clicked()
{
    if (!particles.size()) return;

    if (QMessageBox::warning(this, "Warn", "Are you sure you want to clear all points?", QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        particles.clear();
        drawField();
        drawParticles();
        refreshUI();

        hasSaved = true;
    }
}

void MainWindow::on_mirrorX_clicked()
{
    hasSaved = false;

    const unsigned s = particles.size();
    for (unsigned i=0; i<s; i++)
    {
        VoronoiParticle np = particles[i];
        np._x *= -1;
        np._cx *= -1;
        np._name = (np._name=="")?"":(np._name+"_mx");
        np._id += s;

        particles.push_back(np);
    }

    drawField();
    drawParticles();
    refreshUI();
}

void MainWindow::on_mirrorY_clicked()
{
    hasSaved = false;

    const unsigned s = particles.size();
    for (unsigned i=0; i<s; i++)
    {
        VoronoiParticle np = particles[i];
        np._y *= -1;
        np._cy *= -1;
        np._name = (np._name=="")?"":(np._name+"_my");
        np._id += s;

        particles.push_back(np);
    }

    drawField();
    drawParticles();
    refreshUI();
}

void MainWindow::on_simpleSave_clicked()
{
    if (addr != "empty")
        saveConfig(addr);
    else
        on_save_clicked();
}

void MainWindow::saveConfig(const std::string& add)
{
    std::ofstream file(add.c_str(), std::ios::out | std::ios::trunc);
    if (!file)
    {
        std::cerr << "could not write file..." << std::endl;
        return;
    }

    file << "# region, x, y [, cx, cy] [:name]" << std::endl;
    for (std::vector<VoronoiParticle>::const_iterator p=particles.begin(); p<particles.end(); p++)
    {
        file << p->id() << ", " << p->xraw()*20.f << ", " << p->yraw()*20.f<<", "<<p->num();
        if (p->isMoved())
            file << ", " << p->cxraw()*20.f << ", " << p->cyraw()*20.f;

        if (p->_name != "")
            file << " :" << p->_name;

        file << std::endl;
    }
}

void MainWindow::loadConfig(const std::string& add)
{
    class CFGReader {
    public:
        int id, x, y, cx, cy, num;
        std::string s;
        bool textMode;
        bool isOk;
        char varCounter;

        void pushAValue(char i, int value)
        {
            switch (i)
            {
            case 0:
                id = value;
                break;
            case 1:
                x = value;
                break;
            case 2:
                y = value;
                break;
            case 3:
                num = value;
                break;
            case 4:
                cx = value;
                break;
            case 5:
                cy = value;
            }
        }

        void processLine(const char* str)
        {
            if (!str) return;

            isOk = false;
            s = "";
            id = x = y = cx = cy = num = 0;
            textMode = false;

            varCounter=0;
            int varValue=0;
            bool neg = false;

            unsigned int i;

            for (i=0; str[i] != '\0'; i++)
            {
                const char c = str[i];

                if (c == '#' || c==';') //-- A Comment
                    break;

                if (c == ' ')
                    continue;

                if (c == ':')
                {
                    textMode = true;
                    break;
                }

                if (c == ',') //-- Push An other
                {
                    if (varCounter < 5)
                        pushAValue(varCounter, neg?(-1*varValue):varValue);
                    varValue = 0;
                    varCounter++;
                    neg = false;
                    continue;
                }

                if (c == '-')
                {
                    neg = true;
                    continue;
                }

                varValue = varValue*10 + (c - 48);
            }

            if (varValue != 0)
            {
                pushAValue(varCounter, neg?(-1*varValue):varValue);
            }

            if (textMode)
            {
                for (i++; str[i] != '\0'; i++)
                {
                    s += str[i];
                }
            }

            if (varCounter < 4)
            {
                cx = x;
                cy = y;
            }

            if (varCounter < 2)
                isOk = false;
            else
                isOk = true;
        }
    } cfgReader;

    std::ifstream file(add.c_str(), std::ios::in);
    if (!file) { std::cerr << "File not found..." << std::endl; return; }

    char sz[255];
    while (!file.eof())
    {
        file.getline(sz, 255);
        cfgReader.processLine(sz);
        if (!cfgReader.isOk) continue;

        VoronoiParticle p = VoronoiParticle(0, 0, "");
        p._x = cfgReader.x/20.f;
        p._y = cfgReader.y/20.f;
        p._cx = cfgReader.cx/20.f;
        p._cy = cfgReader.cy/20.f;
        p._id = cfgReader.id;
        p._NumOfSup = cfgReader.num;
        p._name = cfgReader.s;

        std::cout << p.id() << ") " << p.xraw() << ", " << p.yraw() <<", "<<p.num()<< std::endl;

        particles.push_back(p);
    }
}

void MainWindow::on_save_clicked()
{
    on_update_clicked();
    QString address = QFileDialog::getSaveFileName(this,
        tr("SetParameters"), "",
        tr("Config-File *.cfg(*.cfg);;All Files (*)"));
    std::string stradd = address.toStdString();
    std::cout << "Writing file on " << stradd<< std::endl;
    saveConfig(stradd);
    hasSaved = true;
}

void MainWindow::on_load_clicked()
{
    if (!hasSaved && QMessageBox::warning(this,
        "Warn", "You have un-saved points,\nBy reloading, current points will be lost,\nAre you sure you want to reload?",
             QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
        return;

    hasSaved = true;
    particles.clear();
    for (int i=0; i<=VoronoiParticle::_aiID; i++)
        ui->comboBox->removeItem(0);

    VoronoiParticle::_aiID = 0;

    QString address = QFileDialog::getOpenFileName(this,
        tr("Formation"), "",
        tr("Config-File *.cfg(*.cfg);;All Files (*)"));

    addr = address.toStdString();
    std::cout << "Loading file from " << address.toStdString() << std::endl;
    loadConfig(address.toStdString());

    drawField();
    drawParticles();
    refreshUI();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if ((unsigned)index >= particles.size()) return;

    const VoronoiParticle& p = particles[index];
    ui->b_id->setValue(index);
    ui->b_x->setValue(p.xraw()*2);
    ui->b_y->setValue(p.yraw()*2);
    ui->b_cx->setValue(p.cxraw()*2);
    ui->b_cy->setValue(p.cyraw()*2);
    ui->b_name->setText(QString::fromStdString(p._name));
    ui->NumOfSup->setValue(p.num());
    ui->b_par->setValue(p.id()+1);
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QString strPath = dirModel->fileInfo(index).absoluteFilePath();
    if (!strPath.endsWith(".cfg")) return;

    if (!hasSaved && QMessageBox::warning(this,
         "Warn", "You have un-saved points,\nBy reloading, current points will be lost,\nAre you sure you want to reload?",
             QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
        return;

    hasSaved = true;
    particles.clear();

    for (int i=0; i<=VoronoiParticle::_aiID; i++)
        ui->comboBox->removeItem(0);

    VoronoiParticle::_aiID = 0;

    std::cout << "Loading file from " << strPath.toStdString() << std::endl;
    addr = strPath.toStdString();
    loadConfig(strPath.toStdString());

    drawField();
    drawParticles();
    refreshUI();
    drawParticles();
}

void MainWindow::on_createNew_clicked()
{
    hasSaved = true;
    particles.clear();

    for (int i=0; i<=VoronoiParticle::_aiID; i++)
        ui->comboBox->removeItem(0);

    VoronoiParticle::_aiID = 0;
    addr = "empty";

    drawField();
    drawParticles();
    refreshUI();
}
