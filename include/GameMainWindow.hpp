#pragma once
#include "MinesweeperWidget.hpp"
#include <QtWidgets>
#include <QtCore>

class MinesweeperSettingsWidget : public QWidget{
private:
    MinesweeperWidget* _minesweeperWid;
    QLineEdit* _minesEdit;
    QLineEdit* _mapWidthEdit;
    QLineEdit* _mapHeightEdit;

private slots:
    void _applySettings();
    void _cancelSettings();
    
public:
    MinesweeperSettingsWidget(MinesweeperWidget* msWidget);
    virtual ~MinesweeperSettingsWidget() = default;
};

class GameMainWindow : public QMainWindow{
private:
    MinesweeperWidget* _minesweeperWid;
    QLabel* _minesCountLabel;
    QLabel* _timerLabel;
    QMenu* _gameMenu;
    QElapsedTimer _roundTimer;
    QTimer* _updateTimerForRoundTimer;

    QAction* _openSettingWindowAct;
    MinesweeperSettingsWidget* _settingWindow;
public:
    GameMainWindow();
private slots:
    void _handleResetGame();
    void _openSettingWindow();
    void _timerLoop();
    void _gameCodeChanged(uint8_t gameCode);
    void _updateCountMines();
};