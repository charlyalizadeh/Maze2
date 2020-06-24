#include "maze.hpp"

// Contructor
Maze::Maze(int width,int height,bool save_states) : m_width(width), m_height(height), m_save_states(save_states)
{
    // true -> wall, false -> void
    m_node_state = std::vector<bool>(width*height,false); 
    m_wallH = std::vector<bool>(width*(height-1),false); 
    m_wallV = std::vector<bool>((width-1)*height,false);
    m_states = std::vector<State>(); 
}

void Maze::generate(Coord const& start_node,bool color_search,std::vector<Pixel_Maze> const& colors)
{
    if(color_search)
    {
        m_colors = colors;
        depth_color_search(start_node);
    }
    else
    {
        depth_first_search(start_node);
    }
}


void Maze::depth_first_search(Coord const& start_node)
{
    srand(time(NULL));
    std::stack<Coord> current_stack = std::stack<Coord>();
    current_stack.push(start_node);
    m_node_state[start_node.y*m_width + start_node.x] = true;
    Coord current_node = Coord();
    while(!current_stack.empty())
    {
        current_node = current_stack.top();
        current_stack.pop();
        std::vector<Coord> neighbors = get_valid_neighbors(current_node);
        if(!neighbors.empty())
        {
            current_stack.push(current_node);
            Coord new_current_node = get_random_neighbor(neighbors);
            delete_wall(current_node,new_current_node);
            m_node_state[new_current_node.y*m_width+new_current_node.x] = true;
            current_stack.push(new_current_node);
        }
        if(m_save_states)
            m_states.push_back(State(m_node_state,m_wallH,m_wallV));
    }
}
void Maze::depth_color_search(Coord const& start_node)
{
    std::stack<Coord> current_stack = std::stack<Coord>();
    current_stack.push(start_node);
    m_node_state[start_node.y*m_width + start_node.x] = true;
    Coord current_node = Coord();
    while(!current_stack.empty())
    {
        current_node = current_stack.top();
        current_stack.pop();
        std::vector<Coord> neighbors = get_valid_neighbors(current_node);
        if(!neighbors.empty())
        {
            current_stack.push(current_node);
            Coord new_current_node = get_color_neighbor(neighbors,current_node);
            delete_wall(current_node,new_current_node);
            m_node_state[new_current_node.y*m_width+new_current_node.x] = true;
            current_stack.push(new_current_node);
        }
        if(m_save_states)
            m_states.push_back(State(m_node_state,m_wallH,m_wallV));
    }
}

Coord Maze::get_random_neighbor(std::vector<Coord> const& neighbors)
{
    int index = rand() % neighbors.size();
    return neighbors[index];
}
Coord Maze::get_color_neighbor(std::vector<Coord> const& neighbors,Coord const& current)
{
    Coord neighbor = Coord();
    int distance = 500; // The maximum difference betwen two pixels is d({0,0,0},{255,255,255}) = 441.672956
    for(auto const& n : neighbors)
    {
        int temp_distance = distance_pixel(m_colors[n.y*m_width+n.x],m_colors[current.y*m_width+current.x]);
        if(temp_distance<distance)
        {
            neighbor = n;
            distance = temp_distance;
        }
    }
    return neighbor;
}

int Maze::distance_pixel(Pixel_Maze const& p1,Pixel_Maze const& p2)
{
    // Euclidian distance
    return sqrt((p1.red-p2.red)*(p1.red-p2.red)+(p1.green-p2.green)*(p1.green-p2.green)+(p1.blue-p2.blue)*(p1.blue-p2.blue));
}
std::vector<Coord> Maze::get_valid_neighbors(Coord const& current)
{
    std::vector<Coord> neighbors = std::vector<Coord>();
    for(int i = -1;i<2;i+=2)            
    {

        Coord neighbor_h = Coord(current.x+i,current.y);
        Coord neighbor_v = Coord(current.x,current.y+i);
        if(neighbor_h.x>=0 && neighbor_h.x<m_width)
            if(!m_node_state[neighbor_h.y*m_width+neighbor_h.x])
                neighbors.push_back(neighbor_h);
        if(neighbor_v.y>=0 && neighbor_v.y<m_height)
            if(!m_node_state[neighbor_v.y*m_width+neighbor_v.x])
                neighbors.push_back(neighbor_v);

    }
    return neighbors;
}
void Maze::delete_wall(Coord const& c1,Coord const& c2)
{
    if(c1.y==c2.y) // If c1 and c2 are on the same line, we want to delete an vertical wall.
    {                                                                          
        int x = std::min(c1.x,c2.x); // We take the min index between c1 and c2.
        m_wallV[c1.y*(m_width-1)+x] = true;                                   
    }                                                                          
    else // Else we want to delete a horizontal wall
    {
        int y = std::min(c1.y,c2.y);
        m_wallH[y*(m_width)+c1.x] = true;
    }
}



//Accessors | Mutators
int Maze::get_height(){return m_height;}
void Maze::set_height(int new_height)
{
    m_height = new_height;
    m_node_state = std::vector<bool>(m_width*m_height,false);
    m_wallH = std::vector<bool>(m_width*(m_height-1),false);
    m_wallV = std::vector<bool>((m_width-1)*m_height,false);

}

int Maze::get_width(){return m_width;}
void Maze::set_width(int new_width)
{
    m_width = new_width;
    m_node_state = std::vector<bool>(m_width*m_height,false);
    m_wallH = std::vector<bool>(m_width*(m_height-1),false);
    m_wallV = std::vector<bool>((m_width-1)*m_height,false);
}

bool Maze::get_wallH(int x,int y){return m_wallH[y*(m_width)+x];}
bool Maze::get_wallV(int x,int y){return m_wallV[y*(m_width-1)+x];}
bool Maze::get_node(int x,int y){return m_node_state[y*m_width+x];}

bool Maze::get_save_states(){return m_save_states;}
void Maze::set_save_states(bool new_value){m_save_states=new_value;}
std::vector<State> Maze::get_states()
{
    return m_states;
}



