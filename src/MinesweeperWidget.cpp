#include "../include/MinesweeperWidget.hpp"

void MinesweeperWidget::Init() {
    map.setCountMines(50);
    map.setTileSize(16);
    map.setMapSize(30,20);
    map.generateRandomMap();

    this->setFixedSize(map.getTileSize()*map.getMapSize().x,
                       map.getTileSize()*map.getMapSize().y);

    MapTile::initTiles("../data/tiles16.png", map.getTileSize());
}

void MinesweeperWidget::Update() {
    SDL_RenderClear(renderer);
    map.renderUserMap(renderer);
    SDL_RenderPresent(renderer);
}

void MinesweeperWidget::OnResize(int w, int h) {
    SDL_SetWindowSize(window, w, h);
}

void MinesweeperWidget::mouseReleaseEvent(QMouseEvent *event){
    if(_gameOverCode == 0){
        if(event->button() == Qt::RightButton)
            map.toggleFlag(event->x(), event->y());
        else if(event->button() == Qt::LeftButton){
            int8_t code = map.openTile(event->x(), event->y());
            if(code == 1) _gameOverCode = 1;
            else if(map.getCountOpenedTiles() >= map.getMapSize().x * map.getMapSize().y - map.getCountMines()) _gameOverCode = 2;
        }
    }
    if(_gameOverCode == 1) map.showAllMinesInUserMap();
}

auto MapTile::tileSurfaces = std::map<int32_t, std::shared_ptr<SDL_Surface>>{};

void MapTile::render(SDL_Renderer* renderer, uint32_t tileSize){
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tileSurfaces[type].get());
    SDL_Rect tilePos{pos.x, pos.y, (int)tileSize, (int)tileSize};
    SDL_RenderCopy(renderer, texture, NULL, &tilePos);
    
    SDL_DestroyTexture(texture);
}

//init MapTile::tileSurfaces. Need call once at init wiget
void MapTile::initTiles(std::string tilesPath, uint32_t tileSize){
    tileSurfaces.clear();

    char* basePath = SDL_GetBasePath();
    std::string path(basePath);
    path += tilesPath;
    free(basePath);
    
    SDL_Surface* tileset = IMG_Load(path.c_str());
    SDL_Rect cut{0,0,(int)16,(int)16}, paste{0,0,(int)tileSize,(int)tileSize};
    SDL_Surface *newTile;

    for (int i = 0; i <= 13; i++){
        newTile = SDL_CreateRGBSurface(0, tileSize, tileSize,32,0,0,0,0);
        SDL_BlitSurface(tileset, &cut, newTile, &paste);
        tileSurfaces[i] = std::shared_ptr<SDL_Surface>(newTile, SDL_FreeSurface);
        cut.x += tileSize;
        if(i == 8){
            cut.x = 0;
            cut.y += tileSize;
        }
    }

    SDL_FreeSurface(tileset);
}

int32_t Map::_getCountNearMines(uint32_t x, uint32_t y) const{
    int count = 0;
    for(int i = -1; i<=1; i++){
        for(int j = -1; j<=1; j++){
            if((i+x)<_width && (j+y)<_height){
                if(_map[i+x][j+y].type == MapTile::Type::Mine) count++;
            }
        }
    }
    return count;
}

//return 0 if success
int32_t Map::_recursiveOpenTile(uint32_t x, uint32_t y){
    std::stack<Point2<uint32_t>> st;
    st.push({x,y});
    while(!st.empty()){
        Point2<uint32_t> p = st.top();
        st.pop();

        if(p.x>=_width || p.y>=_height || 
           _map[p.x][p.y].type == MapTile::Type::Mine ||
           _userMap[p.x][p.y].type != MapTile::Type::NotVisited) continue;

        int countMines = _getCountNearMines(p.x, p.y);
        _userMap[p.x][p.y].type = countMines;
        _countOpenedTiles++;

        if(countMines==0){
            st.push({p.x - 1, p.y - 1});
            st.push({p.x - 1, p.y});
            st.push({p.x - 1, p.y + 1});
            st.push({p.x, p.y + 1});
            st.push({p.x + 1, p.y + 1});
            st.push({p.x + 1, p.y});
            st.push({p.x + 1, p.y - 1});
            st.push({p.x, p.y - 1});
        }
    }

    return 0;
}

int32_t Map::_openTile(uint32_t x, uint32_t y)
{
    if(_map[x][y].type == MapTile::Type::Mine) return MapTile::Type::VisitedMine;
    return _getCountNearMines(x, y);
}

void Map::_setRandomMine(){
    Point2 p{rand()%_width, rand()%_height};
    while(p.y < _height){
        if(_map[p.x][p.y].type != MapTile::Type::Mine){
            _map[p.x][p.y].type = MapTile::Type::Mine;
            break;
        }
        p.x++;
        if(p.x >= _width){
            p.y++;
            p.x = 0;
        }
    }
}

//call clear for map and usermap and set count flags and opened tiles = 0
void Map::_clearMaps(){
    _countFlags = 0;
    _countOpenedTiles = 0;
    _map.clear();
    _userMap.clear();
}

void Map::showAllMinesInUserMap(){
    for(int i = 0; i<_width; i++){
        for(int j = 0; j<_height; j++){
            if(_map[i][j].type == MapTile::Type::Mine && _userMap[i][j].type != MapTile::Type::VisitedMine){
                _userMap[i][j].type = MapTile::Type::Mine;
            }
        }
    }
}

//return 0 if success
//return 1 if clicked to mine
//return 2 if wrong x or y coordinates
//return 3 if clicked to flag
int32_t Map::openTile(uint32_t x, uint32_t y){

    Point2<uint32_t> p(x, y);
    p = p / _tileSize;

    if(p.x >= _width || p.y >= _height) return 2;
    if(_userMap[p.x][p.y].type == MapTile::Type::Flag) return 3;

    int32_t openedTile = _openTile(p.x, p.y);

    if(openedTile == 0) _recursiveOpenTile(p.x, p.y);
    _userMap[p.x][p.y].type = openedTile;
    
    if(openedTile == MapTile::Type::VisitedMine){
        if(_countOpenedTiles == 0){
            while(openedTile == MapTile::Type::VisitedMine){
                _map[p.x][p.y].type = 0; //delete mine
                _userMap[p.x][p.y].type = MapTile::Type::NotVisited;
                _setRandomMine(); //generate mine
                openedTile = _openTile(p.x, p.y);
            }
            if(openedTile == 0) _recursiveOpenTile(p.x, p.y);
            _userMap[p.x][p.y].type = openedTile;
        }
        else return 1;
    }
    if(openedTile != 0) _countOpenedTiles++;

    return 0;
}

//return 0 on success
//return 2 if wrong x or y coordinated
int32_t Map::toggleFlag(uint32_t x, uint32_t y){
    Point2<uint32_t> p(x, y);
    p = p / _tileSize;

    if(p.x >= _width || p.y >= _height) return 2;

    if(_userMap[p.x][p.y].type == MapTile::Type::Flag){
        _userMap[p.x][p.y].type = MapTile::Type::NotVisited;
        _countFlags--;
    }
    else if (_userMap[p.x][p.y].type == MapTile::Type::NotVisited){
        _userMap[p.x][p.y].type = MapTile::Type::Flag;
        _countFlags++;
    }
    
    return 0;
}

void Map::renderUserMap(SDL_Renderer* renderer)
{
    for(uint32_t x = 0; x<_width; x++){
        for(uint32_t y = 0; y<_height; y++){
            _userMap[x][y].render(renderer, _tileSize);
        }
    }
}

//clear maps and generate new
void Map::generateRandomMap()
{
    _clearMaps();
    for(int32_t x = 0; x<_width; x++){
        _map.push_back(std::vector<MapTile>{});
        for(int32_t y = 0; y<_height; y++){
            _map[x].push_back(MapTile({static_cast<int32_t>(_tileSize * x),
                                       static_cast<int32_t>(_tileSize * y)}));
        }
    }
    _userMap = _map;

    for(int i = 0; i<_countMines; i++){
        _setRandomMine();
    }
}

inline uint32_t Map::getCountMines() const{
    return _countMines;
}

void Map::setCountMines(uint32_t count){
    _countMines = count;
}

inline uint32_t Map::getTileSize() const{
    return _tileSize;
}

void Map::setTileSize(uint32_t size)
{
    _tileSize = size;
}

void Map::setMapSize(uint32_t width, uint32_t height)
{
    _width = width;
    _height = height;
}

inline Point2<uint32_t> Map::getMapSize() const{
    return {_width, _height};
}

inline uint32_t Map::getCountOpenedTiles() const{
    return _countOpenedTiles;
}

MinesweeperWidget::MinesweeperWidget(QWidget* parent): SDLWidget(parent) {}

void MinesweeperWidget::resetGame(){
    map.generateRandomMap();
    _gameOverCode = 0;
}