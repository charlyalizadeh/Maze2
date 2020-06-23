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
