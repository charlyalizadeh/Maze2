#include <vector>
#include <stack>
#include <cstdlib>
#include <time.h>
#include <cmath>
#include "utilities.hpp"



class Maze
{
    public:
        Maze(int width = 25,int height = 25,bool save_states = false);

        void generate(Coord const& start_node = {0,0},bool color_search = false,std::vector<Pixel_Maze> const& colors = std::vector<Pixel_Maze>());

        //Acessors | Mutators
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

    private:
        void depth_first_search(Coord const& start_node); // Regular depth-first search algorithm
        void depth_color_search(Coord const& start_node); // Depth first search which takes the colors of the nodes into account
        void delete_wall(Coord const& c1,Coord const& c2); // Delete the wall between two nodes
        std::vector<Coord> get_valid_neighbors(Coord const& current); // A valid neighbor is a node that hasn't be visited 
        inline Coord get_random_neighbor(std::vector<Coord> const& neighbors);
        Coord get_color_neighbor(std::vector<Coord> const& neighbors,Coord const& current);
        int distance_pixel(Pixel_Maze const& p1,Pixel_Maze const& p2);
        


    private:
        std::vector<bool> m_node_state; 
        std::vector<bool> m_wallH; // Horizontal walls
        std::vector<bool> m_wallV; // Vertical walls
        std::vector<State> m_states; // Contains all the states of the generation (in order to display them)
        int m_width,m_height; // Dimension of the maze
        bool m_save_states; // true -> save the states, false -> doesn't save the states
        std::vector<Pixel_Maze> m_colors; // Contains the colors of the png file we want to display. (In order to decide the next node to visit depending on the color)
        // Maybe I should have created a Node struct, but it works like this so I don't want to change it
};
