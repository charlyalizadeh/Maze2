#include "engine.hpp"

int main() 
{
    Maze_Generator* test = new Maze_Generator(200,200,false,1,1,1,1,1,1);
    test->load_png("joconde.png");
    test->generate();
    test->Start();
    return 0;
}
