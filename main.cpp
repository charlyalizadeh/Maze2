#include "engine.hpp"

int main() 
{
    Maze_Generator* test = new Maze_Generator("./image/maze.png",true);
    test->generate(false);
    test->Start();
    return 0;
}
