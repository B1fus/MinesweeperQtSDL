#include <iostream>
#include <QtWidgets>
#include <QtCore>
#include <SDL2/SDL.h>
#include "../include/MinesweeperWidget.hpp"
#include "../include/GameMainWindow.hpp"

int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    QApplication app(argc,argv);
    // QMainWindow *Wnd = new QMainWindow;
    // Wnd->resize(810,610);  // for fill up the QMGL, menu and toolbars
    // Wnd->setWindowTitle("QMathGL sample");
    // Wnd->show();

    // MinesweeperWidget* msWidget = new MinesweeperWidget(Wnd);

    // QLayout* layout = new QGridLayout;
    // layout->addWidget(msWidget);
    // layout->setAlignment(Qt::AlignLeft);
    // msWidget->setLayout(layout);

    // QWidget* centrWid = new QWidget;
    // centrWid->setLayout(layout);

    // Wnd->setCentralWidget(centrWid);
    
    GameMainWindow* mainWindow = new GameMainWindow();

    return app.exec();
}