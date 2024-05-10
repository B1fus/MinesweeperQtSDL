#pragma once
#include "MinesweeperWidget.hpp"
#include <QtWidgets>
#include <QtCore>

class MinesweeperSettingsWidget : public QWidget{
private:
    MinesweeperWidget* _minesweeperWid;
    QLineEdit* _minesEdit;

private slots:
    void _applySettings();
    
public:
    MinesweeperSettingsWidget(MinesweeperWidget* msWidget);
};

class GameMainWindow : public QMainWindow{
private:
    MinesweeperWidget* _minesweeperWid;
    QLabel* _minesCountLabel;
    QLabel* _timerLabel;
    QMenu* _gameMenu;
    QElapsedTimer _roundTimer;

    QAction* _openSettingWindowAct;
    MinesweeperSettingsWidget* _settingWindow;
public:
    GameMainWindow();
private slots:
    void _handleResetGame();
    void _openSettingWindow();
    void _timerLoop();
};