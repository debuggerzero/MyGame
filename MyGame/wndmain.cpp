﻿#include "wndmain.h"
#include "ui_wndmain.h"

#include <QDebug>

WndMain::WndMain(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::WndMain)
{
    _ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);

    this->setStyleSheet(Utility::getStyleSheet(":/image/wndmain.qss"));

    this->setAttribute(Qt::WA_DeleteOnClose);

    initWindows();
}

WndMain::~WndMain()
{
    delete _ui;
}

void WndMain::tuopan()
{
    _menu = new QMenu(this);
    _action1 =new QAction(_menu);
    _action1->setText("exit");
    _menu->addAction(_action1);

    _systemTray = new QSystemTrayIcon(this);
    _systemTray->setContextMenu(_menu);

    _systemTray->setIcon(QIcon(":/image/image/logo.png"));
    _systemTray->setToolTip("MyGame");
    disconnect(_systemTray, &QSystemTrayIcon::activated, this, &WndMain::activeTray);
    disconnect(_action1, &QAction::triggered, this,&WndMain::exit);

    connect(_systemTray, &QSystemTrayIcon::activated, this, &WndMain::activeTray);
    connect(_action1, &QAction::triggered, this,&WndMain::exit);
}

void WndMain::on_btnGame_clicked()
{
    _ui->MainWindow->setCurrentWidget(_ui->AllGame);
}

void WndMain::on_btnAchieve_clicked()
{
    _ui->MainWindow->setCurrentWidget(_ui->achieve);
}

void WndMain::on_btnUser_clicked()
{
    _ui->MainWindow->setCurrentWidget(_ui->User);

    _ui->nameList->clear();
    _ui->numberList_2->clear();
    _ui->scoreList_2->clear();
    _ui->gameTimeList->clear();

    _ui->UserNameLabel->setText("UserName：" + Archive::userName);
    _ui->AccountLabel->setText("Account：" + Archive::account);

    _ui->nameList->addItem("2048");
    _ui->nameList->addItem("五子棋");
    _ui->nameList->addItem("贪吃蛇");
    _ui->nameList->addItem("色彩达人");
    _ui->nameList->addItem("国际象棋");

    Archive gobangRecord("五子棋");
    gobangRecord.readGameStorage();
    Archive snakeRecord("贪吃蛇");
    snakeRecord.readGameStorage();
    Archive tzfeRecord("2048");
    tzfeRecord.readGameStorage();
    Archive colorRecord("色彩达人");
    colorRecord.readGameStorage();
    Archive chessRecord("国际象棋");
    chessRecord.readGameStorage();

    _ui->numberList_2->addItem(QString::number(tzfeRecord.getNumber()));
    _ui->numberList_2->addItem(QString::number(gobangRecord.getNumber()));
    _ui->numberList_2->addItem(QString::number(snakeRecord.getNumber()));
    _ui->numberList_2->addItem(QString::number(colorRecord.getNumber()));
    _ui->numberList_2->addItem(QString::number(chessRecord.getNumber()));

    _ui->scoreList_2->addItem(QString::number(tzfeRecord.getBestScore()));
    _ui->scoreList_2->addItem("-");
    _ui->scoreList_2->addItem(QString::number(snakeRecord.getBestScore()));
    _ui->scoreList_2->addItem(QString::number(colorRecord.getBestScore()));
    _ui->scoreList_2->addItem("-");

    _ui->gameTimeList->addItem(QString::number(tzfeRecord.getGameTime() / 1000) + " s");
    _ui->gameTimeList->addItem(QString::number(gobangRecord.getGameTime() / 1000) + " s");
    _ui->gameTimeList->addItem(QString::number(snakeRecord.getGameTime() / 1000) + " s");
    _ui->gameTimeList->addItem(QString::number(colorRecord.getGameTime() / 1000) + " s");
    _ui->gameTimeList->addItem(QString::number(chessRecord.getGameTime() / 1000) + " s");
}

void WndMain::on_btnHist_clicked()
{
    _ui->MainWindow->setCurrentWidget(_ui->History);
}

void WndMain::on_btnInstal_clicked()
{
    tuopan();

    _ui->MainWindow->setCurrentWidget(_ui->Instal);
}

void WndMain::on_btnDwi_clicked()
{
    this->showMinimized();

    tuopan();
    _systemTray->show();

    this->hide();
}

void WndMain::on_btnClose_clicked()
{
    this->close();
}

void WndMain::contextMenuEvent(QContextMenuEvent *event)
{
    _openAction->disconnect();
    _records->disconnect();
    _achievements->disconnect();

    if (_ui->TZFE->underMouse()) {
        connect(_openAction, &QAction::triggered, this, &WndMain::openTZFE);
        connect(_records, &QAction::triggered, this, [=](){
            _ui->MainWindow->setCurrentWidget(_ui->History);
            showList("2048");
        });
        connect(_achievements, &QAction::triggered, this, [=](){
            _ui->MainWindow->setCurrentWidget(_ui->achieve);
            _ui->achieveWidget->setCurrentWidget(_ui->tzfeAchieve);
        });
        _rightClieckMenu->exec(event->globalPos());
    }
    else if (_ui->Snake->underMouse()) {
        connect(_openAction, &QAction::triggered, this, &WndMain::openSnake);
        connect(_records, &QAction::triggered, this, [=](){
            _ui->MainWindow->setCurrentWidget(_ui->History);
            showList("贪吃蛇");
        });
        connect(_achievements, &QAction::triggered, this, [=](){
            _ui->MainWindow->setCurrentWidget(_ui->achieve);
            _ui->achieveWidget->setCurrentWidget(_ui->snakeAchieve);
        });
        _rightClieckMenu->exec(event->globalPos());
    }
    else if (_ui->Gobang->underMouse()) {
        connect(_openAction, &QAction::triggered, this, &WndMain::openGobang);
        connect(_records, &QAction::triggered, this, [=](){
            _ui->MainWindow->setCurrentWidget(_ui->History);
            showList("五子棋");
        });
        connect(_achievements, &QAction::triggered, this, [=](){
            _ui->MainWindow->setCurrentWidget(_ui->achieve);
            _ui->achieveWidget->setCurrentWidget(_ui->gobangAchieve);
        });
        _rightClieckMenu->exec(event->globalPos());
    }
    else if (_ui->color->underMouse()) {
        connect(_openAction, &QAction::triggered, this, &WndMain::openColor);
        connect(_records, &QAction::triggered, this, [=](){
            _ui->MainWindow->setCurrentWidget(_ui->History);
            showList("色彩达人");
        });
        connect(_achievements, &QAction::triggered, this, [=](){
            _ui->MainWindow->setCurrentWidget(_ui->achieve);
            _ui->achieveWidget->setCurrentWidget(_ui->colorAchieve);
        });
        _rightClieckMenu->exec(event->globalPos());
    }
    else if (_ui->comingsoom->underMouse()) {
        connect(_openAction, &QAction::triggered, this, &WndMain::openChess);
        connect(_records, &QAction::triggered, this, [=](){
            _ui->MainWindow->setCurrentWidget(_ui->History);
            showList("国际象棋");
        });
        connect(_achievements, &QAction::triggered, this, [=](){
            _ui->MainWindow->setCurrentWidget(_ui->achieve);
            _ui->achieveWidget->setCurrentWidget(_ui->colorAchieve);
        });
        _rightClieckMenu->exec(event->globalPos());
    }
}

void WndMain::showList(QString gameName)
{
    Archive archive(gameName);
    QVector<struct GameTime> records = archive.getRecords();
    QStringList numberLists;
    QStringList scoreLists;
    QStringList startTimeLists;
    QStringList endTimeLists;
    for (int i = 0; i < records.length(); i++) {
        struct GameTime record = records.at(i);
        numberLists.append(QString::number(record.number));
        scoreLists.append(QString::number(record.bestScore));
        startTimeLists.append(record.startTime);
        endTimeLists.append(record.endTime);
    }
    _ui->numberList->clear();
    _ui->numberList->addItems(numberLists);
    _ui->scoreList->clear();
    _ui->scoreList->addItems(scoreLists);
    _ui->startTimeList->clear();
    _ui->startTimeList->addItems(startTimeLists);
    _ui->endTimeList->clear();
    _ui->endTimeList->addItems(endTimeLists);
}

void WndMain::openTZFE()
{
    if (!_isOpen) {
        GameStartWindows *gameWidget = new GameStartWindows();
        connect(gameWidget, &GameStartWindows::windowsClose, this, [=](){
            this->show();
            _isOpen = false;
        });
        gameWidget->show();
        this->hide();
    }
}

void WndMain::openGobang()
{
    if (!_isOpen) {
        GStartWindow *gameWidget = new GStartWindow();
        connect(gameWidget, &GStartWindow::windowsClose, this, [=](){
            this->show();
            _isOpen = false;
        });
        gameWidget->show();
        this->hide();
    }
}

void WndMain::openSnake()
{
    if (!_isOpen) {
        GStartWidght *gameWidget = new GStartWidght();
        connect(gameWidget, &GStartWidght::windowsClose, this, [=](){
            this->show();
            _isOpen = false;
        });
        gameWidget->show();
        this->hide();
    }
}

void WndMain::openColor()
{
    if (!_isOpen) {
        Widget *gameWidget = new Widget();
        connect(gameWidget, &Widget::windowsClose, this, [=](){
            this->show();
            _isOpen = false;
        });
        gameWidget->show();
        this->hide();
    }
}

void WndMain::openChess()
{
    if (!_isOpen) {
        GameWndStrat *gameWidget = new GameWndStrat();
        connect(gameWidget, &GameWndStrat::windowsClose, this, [=](){
            this->show();
            _isOpen = false;
        });
        gameWidget->show();
        this->hide();
    }
}

void WndMain::activeTray(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick){
        this->show();
        this->showNormal();
        delete _systemTray;
    }
}

void WndMain::exit()
{
    this->hide();
}

void WndMain::initWindows()
{
    _isOpen = false;

    _ui->headPortraitLabel->setPixmap(Archive::headPath);

    _ui->MainWindow->setCurrentWidget(_ui->AllGame);

    //===================================================================================
    //游戏界面按钮事件

    //2048游戏按钮
    connect(_ui->TZFE, &QPushButton::clicked, this, &WndMain::openTZFE);

    //五子棋游戏按钮
    connect(_ui->Gobang, &QPushButton::clicked, this, &WndMain::openGobang);

    //贪吃蛇游戏按钮
    connect(_ui->Snake, &QPushButton::clicked, this, &WndMain::openSnake);

    //color游戏按钮
    connect(_ui->color, &QPushButton::clicked, this, &WndMain::openColor);

    //chess游戏按钮
    connect(_ui->comingsoom, &QPushButton::clicked, this, &WndMain::openChess);

    _rightClieckMenu = new QMenu(this);
    //设置菜单样式
    /**************************************/
    //创建菜单项
    _openAction = new QAction("打开",this);
    _achievements = new QAction("查看成就",this);
    _records = new QAction("查看记录",this);

    //将创建好的菜单项加入到菜单中
    _rightClieckMenu->addAction(_openAction);
    _rightClieckMenu->addAction(_achievements);
    _rightClieckMenu->addAction(_records);

    _openAction->setIcon(QIcon(":/image/image/gameIcon.png"));
    _achievements->setIcon(QIcon(":/image/image/achiIcon.png"));
    _records->setIcon(QIcon(":/image/image/histIcon.png"));
    //关联菜单项按钮和对应的槽函数
    //====================================================================================

    //====================================================================================
    //记录界面按钮事件======================================================================
    //记录
    connect(_ui->tzfeRecordButton, &QPushButton::clicked, this, [=](){
        showList("2048");
    });

    connect(_ui->snakeRecordButton, &QPushButton::clicked, this, [=](){
        showList("贪吃蛇");
    });

    connect(_ui->gobangRecordButton, &QPushButton::clicked, this, [=](){
        showList("五子棋");
    });

    connect(_ui->colorRecordButton, &QPushButton::clicked, this, [=](){
        showList("色彩达人");
    });

    connect(_ui->chessRecordButton, &QPushButton::clicked, this, [=](){
        showList("国际象棋");
    });
    //====================================================================================

    //====================================================================================
    //成就界面按钮事件
    connect(_ui->gobangRecordButton_2, &QPushButton::clicked, this, [=](){
        _ui->achieveWidget->setCurrentWidget(_ui->gobangAchieve);
    });

    connect(_ui->tzfeRecordButton_2, &QPushButton::clicked, this, [=](){
        _ui->achieveWidget->setCurrentWidget(_ui->tzfeAchieve);
    });

    connect(_ui->snakeRecordButton_2, &QPushButton::clicked, this, [=](){
        _ui->achieveWidget->setCurrentWidget(_ui->snakeAchieve);
    });

    connect(_ui->colorRecordButton_2, &QPushButton::clicked, this, [=](){
        _ui->achieveWidget->setCurrentWidget(_ui->colorAchieve);
    });

    connect(_ui->chessRecordButton_2, &QPushButton::clicked, this, [=](){
        _ui->achieveWidget->setCurrentWidget(_ui->chessAchieve);
    });
    //====================================================================================

    //====================================================================================
    //设置按钮事件
    connect(_ui->btnchead, &QPushButton::clicked, this, [=](){
        QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("TextFiles(*.png, *.jpg)"));
        if (path != "") {
            Archive::headPath = path;
            _ui->headPortraitLabel->setPixmap(Archive::headPath);
            QMessageBox::about(this, "提示", "头像上传成功...");
            Archive archive;
            archive.writeUserMessage();
        }
        else {
            QMessageBox::about(this, "提示", "未选择任何图片...");
        }
    });

    connect(_ui->btncnickname, &QPushButton::clicked, this, [=](){
        bool ok;
        QString name = QInputDialog::getText(this, "更改用户名", "请输入新的用户名：", QLineEdit::Normal, "", &ok);
        if (ok) {
            if (name.length() > 5) {
                QMessageBox::about(this, "提示", "用户名过长(小于5)...");
            }
            else {
                Archive::userName = name;
                _ui->UserNameLabel->setText(name);
                QMessageBox::about(this, "提示", "用户名设置成功...");
                Archive archive;
                archive.writeUserMessage();
            }
        }
    });

    connect(_ui->btncpassword, &QPushButton::clicked, this, [=](){
        bool ok;
        QString password = QInputDialog::getText(this, "更改密码", "请输入新的密码：", QLineEdit::Password, "", &ok);
        if (ok) {
            if (password.length() > 15) {
                QMessageBox::about(this, "提示", "密码过长(小于15)...");
            }
            else {
                Archive::password = password;
                QMessageBox::about(this, "提示", "密码修改成功...");
                Archive archive;
                archive.writeUserMessage();
            }
        }
    });

    connect(_ui->btnlogout, &QPushButton::clicked, this, [=](){
        QString Path = "C:\\MyGame\\user\\" + Archive::account;
        QDir dir(Path);
        int result = QMessageBox::question(this, "提示", "是否注销账户，如果是将退出程序，并注销...");
        if (result == QMessageBox::Yes){
            dir.removeRecursively();
            this->close();
        }
    });
    //=========================================================================================================
}
