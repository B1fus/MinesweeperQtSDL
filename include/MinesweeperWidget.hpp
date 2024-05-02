#pragma once
#include "SDLWidget.hpp"
#include <vector>
#include <string>
#include <SDL2/SDL_image.h>
#include <stack>
#include <iostream>

template <typename T>
struct Point2{
    T x, y;
    Point2() = default;
    Point2(T x, T y) : x(x), y(y) {}
    Point2(const Point2<T>& p){
        x = p.x;
        y = p.y;
    }

    Point2<T> operator/(const T r){
        Point2<T> p(*this);
        p.x/=r;
        p.y/=r;
        return p;
    }
};


struct MapTile{
    enum Type{NotVisited = 10, Flag = 11, VisitedMine = 12,
              Mine = 13
    };
    
    static std::map<int32_t, std::shared_ptr<SDL_Surface>> tileSurfaces;
    static void initTiles(std::string tilesPath, uint32_t tileSize);
    
    Point2<int32_t> pos;
    int32_t type;

    MapTile(Point2<int32_t> pos, int32_t type = Type::NotVisited):pos(pos), type(type){}
    
    void render(SDL_Renderer* renderer, uint32_t tileSize);
};


class Map{
    std::vector<std::vector<MapTile>> _map; //real map
    std::vector<std::vector<MapTile>> _userMap; //map that user will see
    uint32_t _countMines;
    uint32_t _countFlags = 0;
    uint32_t _countOpenedTiles = 0;
    uint32_t _tileSize;
    uint32_t _width, _height;

    int32_t _getCountNearMines(uint32_t x, uint32_t y) const;
    int32_t _recursiveOpenTile(uint32_t x, uint32_t y);
    int32_t _openTile(uint32_t x, uint32_t y);
    void _setRandomMine();

public:
    int32_t openTile(uint32_t x, uint32_t y);
    int32_t toggleFlag(uint32_t x, uint32_t y);

    void renderUserMap(SDL_Renderer* renderer);
    void showAllMinesInUserMap();

    //clears old map and generate new
    void generateRandomMap();

    uint32_t getCountMines() const;
    void setCountMines(uint32_t count);
    uint32_t getTileSize() const;
    void setTileSize(uint32_t size);
    void setMapSize(uint32_t width, uint32_t height);
    Point2<uint32_t> getMapSize() const;
    uint32_t getCountFlags() const;
    uint32_t getCountOpenedTiles() const;
};



class MinesweeperWidget : public SDLWidget
{
public:
    MinesweeperWidget(QWidget* parent) : SDLWidget(parent) {}
    Map _map;
private:
    int8_t _gameOverCode = 0; // 0-not over, 1-lose, 2-win

    void Init();
    void Update();
    void OnResize(int w, int h);
    void mouseReleaseEvent(QMouseEvent *event) override;
};
