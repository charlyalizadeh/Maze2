#include <vector>
#include <stack>
#include <cstdlib>
#include <time.h>
#include "utilities.hpp"



class Maze
{
    public:
        Maze(int width = 25,int height = 25,bool save_states = false);

        void generate(Coord start_node = {0,0});

        int get_width();
        void set_width(int new_width);

        int get_height();
        void set_height(int new_height);

        bool get_wallH(int x,int y);

        bool get_wallV(int x,int y);

        bool get_node(int x,int y);
        
        bool get_save_states();
        void set_save_states(bool new_value);

        std::vector<State> get_states();

        std::vector<Coord> get_valid_neighbors(Coord current);   
    private:
        void depth_first_search(Coord start_node);
        void delete_wall(Coord c1,Coord c2);



    private:
        std::vector<bool> m_node_state;
        std::vector<bool> m_wallH;
        std::vector<bool> m_wallV;
        std::vector<State> m_states;
        int m_width,m_height;
        bool m_save_states;
};
