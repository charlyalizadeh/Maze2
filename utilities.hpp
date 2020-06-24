#include <vector>

struct Coord
{
    int x,y;
    Coord(int _x=0,int _y=0) : x(_x),y(_y){}
};

struct State
{
    std::vector<bool> node_state,wallH,wallV;
    State(std::vector<bool> _node_state,std::vector<bool> _wallH,std::vector<bool>_wallV) : node_state(_node_state),wallH(_wallH),wallV(_wallV){}
};

struct Pixel_Maze
{
    int red, green, blue;
    Pixel_Maze(int _red, int _green, int _blue) : red(_red),green(_green),blue(_blue){}
};
