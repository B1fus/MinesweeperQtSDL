#include "../include/GameMainWindow.hpp"

MinesweeperSettingsWidget::MinesweeperSettingsWidget(MinesweeperWidget* msWidget)
    : _minesweeperWid(msWidget)
{
    QVBoxLayout* layout = new QVBoxLayout();
    this->setLayout(layout);

    QWidget* minesWid = new QWidget();
    QHBoxLayout* minesLay = new QHBoxLayout();
    minesWid->setLayout(minesLay);
        QLabel* minesLab = new QLabel("Mines: ");
        _minesEdit = new QLineEdit();
        _minesEdit->setValidator(new QIntValidator(this));
        _minesEdit->setText(QString::number(_minesweeperWid->map.getCountMines()));
    minesLay->addWidget(minesLab);
    minesLay->addWidget(_minesEdit);
    layout->addWidget(minesWid);

    QWidget* widthWid = new QWidget();
    QHBoxLayout* widthLay = new QHBoxLayout();
    widthWid->setLayout(widthLay);
        QLabel* widthLab = new QLabel("Width: ");
        _mapWidthEdit = new QLineEdit();
        _mapWidthEdit->setValidator(new QIntValidator(this));
        _mapWidthEdit->setText(QString::number(_minesweeperWid->map.getMapSize().x));
    widthLay->addWidget(widthLab);
    widthLay->addWidget(_mapWidthEdit);
    layout->addWidget(widthWid);

    QWidget* heightWid = new QWidget();
    QHBoxLayout* heightLay = new QHBoxLayout();
    heightWid->setLayout(heightLay);
        QLabel* heightLab = new QLabel("Height: ");
        _mapHeightEdit = new QLineEdit();
        _mapHeightEdit->setValidator(new QIntValidator(this));
        _mapHeightEdit->setText(QString::number(_minesweeperWid->map.getMapSize().y));
    heightLay->addWidget(heightLab);
    heightLay->addWidget(_mapHeightEdit);
    layout->addWidget(heightWid);

    QPushButton* applyButton = new QPushButton("Apply");
    connect(applyButton, &QPushButton::released, this, &MinesweeperSettingsWidget::_applySettings);
    layout->addWidget(applyButton);

}

void MinesweeperSettingsWidget::_applySettings(){
    int widthMap = _mapWidthEdit->text().toInt();
    if(widthMap<=0) _mapWidthEdit->setText("1");
    widthMap = _mapWidthEdit->text().toInt();

    int heightMap = _mapHeightEdit->text().toInt();
    if(heightMap<=0) _mapHeightEdit->setText("1");
    heightMap = _mapHeightEdit->text().toInt();

    _minesweeperWid->map.setMapSize(widthMap, heightMap);

    int minesCount = _minesEdit->text().toInt();
    if(minesCount > (_minesweeperWid->map.getMapSize().x * _minesweeperWid->map.getMapSize().y) / 2)
        _minesEdit->setText(QString::number((_minesweeperWid->map.getMapSize().x * _minesweeperWid->map.getMapSize().y) / 2));
    if(minesCount <= 0)
        _minesEdit->setText(QString::number(1));
    minesCount = _minesEdit->text().toInt();
    _minesweeperWid->map.setCountMines(minesCount);

    _minesweeperWid->resetGame();
}

GameMainWindow::GameMainWindow():QMainWindow() {
    QWidget* sdlWidgetContainer = new QWidget();
    //QWidget* sdlWidget = new QWidget();
    _minesweeperWid = new MinesweeperWidget(sdlWidgetContainer);
    auto *gridLayout = new QGridLayout();
    gridLayout->addWidget(_minesweeperWid, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);
    sdlWidgetContainer->setLayout(gridLayout);


    QHBoxLayout* layout = new QHBoxLayout;
    QVBoxLayout* layoutRight = new QVBoxLayout;
    layoutRight->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(sdlWidgetContainer);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QWidget* widgetContaiterRight = new QWidget();
    widgetContaiterRight->setLayout(layoutRight);

        QLabel* minesLabel = new QLabel("Mines: ");
        _minesCountLabel = new QLabel("10");
        QHBoxLayout* layoutMines = new QHBoxLayout();
        QWidget* widgetMinesLabels = new QWidget();
        widgetMinesLabels->setLayout(layoutMines);
        layoutMines->addWidget(minesLabel);
        layoutMines->addWidget(_minesCountLabel);

    layoutMines->setContentsMargins(0, 0, 0, 0);
    layoutRight->addWidget(widgetMinesLabels);
    
        QLabel* timeLabel = new QLabel("Time: ");
        _timerLabel = new QLabel("00:00:00");
        QHBoxLayout* layoutTime = new QHBoxLayout();
        QWidget* widgetTimeLabels = new QWidget();
        widgetTimeLabels->setLayout(layoutTime);
        layoutTime->addWidget(timeLabel);
        layoutTime->addWidget(_timerLabel);

    layoutTime->setContentsMargins(0, 0, 0, 0);
    layoutRight->addWidget(widgetTimeLabels);
    QPushButton* button1 = new QPushButton("Restart");
    connect(button1, &QPushButton::released, this, &GameMainWindow::_handleResetGame);
    layoutRight->addWidget(button1);
    QPushButton* button2 = new QPushButton("Button");
    layoutRight->addWidget(button2);
    layout->addStretch(1);
    layout->addWidget(widgetContaiterRight);
    layout->addStretch(2);
    
    QWidget* widget = new QWidget(this);
    this->setCentralWidget(widget);
    widget->setLayout(layout);
    widget->resize(640, 480);
    this->show();

    _settingWindow = new MinesweeperSettingsWidget(_minesweeperWid);
    this->resize(200,100);

    //creating actions
    _openSettingWindowAct = new QAction("Settings", this);

    _gameMenu = menuBar()->addMenu("Menu");
    _gameMenu->addAction(_openSettingWindowAct);
    connect(_openSettingWindowAct, &QAction::triggered, this, &GameMainWindow::_openSettingWindow);

    //connect and create ui things
    connect(_minesweeperWid, &MinesweeperWidget::gameCodeChanged, this, &GameMainWindow::_gameCodeChanged);
    _updateTimerForRoundTimer = new QTimer(this);
    connect(_updateTimerForRoundTimer, &QTimer::timeout, this, &GameMainWindow::_timerLoop);
    
    connect(_minesweeperWid, &MinesweeperWidget::flagToggeled, this, &GameMainWindow::_updateCountMines);
    _updateCountMines();
}

void GameMainWindow::_handleResetGame(){
    _roundTimer.restart();
    _minesweeperWid->resetGame();
    _updateCountMines();
}

void GameMainWindow::_openSettingWindow(){
    _settingWindow->show();
}

void GameMainWindow::_timerLoop(){
    qint64 tEl = _roundTimer.elapsed() / 1000;
    QString str = QString("%1:%2:%3").arg(  tEl / 3600       , 2, 10, QChar('0'))
                                     .arg( (tEl % 3600) / 60 , 2, 10, QChar('0'))
                                     .arg( (tEl % 60)        , 2, 10, QChar('0'));
    _timerLabel->setText(str);
}

void GameMainWindow::_gameCodeChanged(uint8_t gameCode){
    switch (gameCode)
    {
    case 0:
        _roundTimer.restart();
        _timerLoop();
        _updateTimerForRoundTimer->stop();
        break;
    case 1: //game started, first click
        _roundTimer.restart();
        _updateTimerForRoundTimer->start(400);
        break;
    case 2: //game lose
        _updateTimerForRoundTimer->stop();
        break;
    case 3: //game win
        _updateTimerForRoundTimer->stop();
        break;
    default:
        break;
    }
}

void GameMainWindow::_updateCountMines(){
    _minesCountLabel->setText(QString::number(
        static_cast<int>(_minesweeperWid->map.getCountMines()) - static_cast<int>(_minesweeperWid->map.getCountFlags())
        ));
}