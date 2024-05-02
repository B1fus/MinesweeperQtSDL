#include <iostream>
#include <QtWidgets>
#include <QtCore>
#include <SDL2/SDL.h>
#include "../include/MinesweeperWidget.hpp"

int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    QApplication a(argc,argv);
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
    
    QWidget* sdlWidgetContainer = new QWidget();
    MinesweeperWidget* sdlWidget = new MinesweeperWidget(sdlWidgetContainer);
    auto *gridLayout = new QGridLayout();
    gridLayout->addWidget(sdlWidget, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    sdlWidgetContainer->setLayout(gridLayout);

    QPushButton* button = new QPushButton("Button");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(sdlWidgetContainer);
    layout->addWidget(button);

    QWidget widget;
    widget.setLayout(layout);
    widget.resize(640, 480);
    widget.show();

    return a.exec();
}