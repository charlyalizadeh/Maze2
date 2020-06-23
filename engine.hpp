#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.hpp"
#include "maze.hpp"
#include <string>


class Maze_Generator : public olc::PixelGameEngine
{
public:
	Maze_Generator(int width = 20,int height = 20,bool save_states = false,int node_width = 5,int node_height = 5,int wall_width = 2,int border = 2,int pixel_width = 5,int pixel_height = 5) :m_save_states(save_states), m_node_width(node_width), m_node_height(node_height), m_wall_width(wall_width),m_border(border),m_pixel_width(pixel_width),m_pixel_height(pixel_height)
	{
		// Name you application
		sAppName = "Maze generator";
        m_maze = new Maze(width,height,m_save_states);
        
        m_colors = new olc::Sprite(width,height);
        m_png_loaded = false;
	}

public:
	bool OnUserCreate() override
	{
        index_state = 0;
        c_x=0;
        c_y=0;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        if(m_save_states)
            display_states();
        else
            display_maze();
        return true;
	}

    void generate()
    {
        int width = m_maze->get_width();
        int height = m_maze->get_height();
        m_true_width = 2 * m_border + width * (m_node_width + m_wall_width) -  m_wall_width;
        m_true_height = 2 * m_border + height * (m_node_height + m_wall_width) - m_wall_width;
        Construct(m_true_width,m_true_height, m_pixel_width, m_pixel_height);
        m_maze->generate();
        if(m_save_states)
            m_states = m_maze->get_states();
    }

public:
    void load_png(std::string filename,bool set_dimension = true)
    {
        m_colors = new olc::Sprite(filename);
        m_png_loaded = true;
        if(set_dimension)
        {
            int width = m_colors->width;
            int height = m_colors->height;
            set_width(width);
            set_height(height);
        }
    }
    void set_width(int new_width)
    {
        m_maze->set_width(new_width);
    }
    void set_height(int new_height)
    {
        m_maze->set_height(new_height);
    }

private:
    Maze* m_maze;
    std::vector<State> m_states;
    int m_node_width,m_node_height;
    int m_wall_width;
    int m_border; 
    int m_pixel_width,m_pixel_height;
    bool m_save_states;
    olc::Sprite* m_colors;
    bool m_png_loaded;
    int m_true_width,m_true_height;


    //Debug variables
    int c_x,c_y;
    int index_state;



    void display_maze()
    {
        Clear(olc::BLACK);
        display_walls();
        display_nodes();
    }

    void display_state(State state)
    {
        for(int i = 0;i<m_maze->get_height();i++)
        {
            for(int j = 0;j<m_maze->get_width();j++)
            {
                if(state.node_state[i*m_maze->get_width()+j])
                    draw_node(j,i);
                else
                    draw_node(j,i,olc::BLACK);
            }
        }

        for(int i = 0;i<m_maze->get_height();i++)
        {
            for(int j = 0;j<m_maze->get_width()-1;j++)
            {
                if(state.wallV[i*(m_maze->get_width()-1)+j])
                    draw_wallV(j,i);
            }
        }

        for(int i = 0;i<m_maze->get_height()-1;i++)
        {
            for(int j = 0;j<m_maze->get_width();j++)
            {
                if(state.wallH[i*m_maze->get_width()+j])
                    draw_wallH(j,i);
            }
        }
    }

    void display_nodes()
    {
        for(int i = 0;i<m_maze->get_height();i++)
        {
            for(int j = 0;j<m_maze->get_width();j++)
            {
                if(m_maze->get_node(j,i))
                    draw_node(j,i);
                else
                    draw_node(j,i,olc::BLACK);
            }
        }
       
    }

    void display_walls()
    {
       for(int i = 0;i<m_maze->get_height()-1;i++)
        {
            for(int j = 0;j<m_maze->get_width();j++)
            {
                if(m_maze->get_wallH(j,i))
                    draw_wallH(j,i);
                else
                    draw_wallH(j,i,olc::BLACK);
            }
        }
       for(int i = 0;i<m_maze->get_height();i++)
        {
            for(int j = 0;j<m_maze->get_width()-1;j++)
            {
                if(m_maze->get_wallV(j,i))
                    draw_wallV(j,i);
                else
                    draw_wallV(j,i,olc::BLACK);
            }
        }

 

    }


    void draw_node(int x,int y,olc::Pixel p = olc::WHITE)
    {
        int true_x = m_border+x*(m_node_width+m_wall_width);
        int true_y = m_border+y*(m_node_height+m_wall_width);
        if(m_png_loaded && p == olc::WHITE)
        {
            p = m_colors->GetPixel(x,y);
        }
        FillRect(true_x,true_y,m_node_width,m_node_height,p);
    }

    void draw_wallH(int x,int y,olc::Pixel p = olc::WHITE)
    {
        int true_x = m_border + x * (m_node_width+m_wall_width);
        int true_y = m_border + m_node_height + y * (m_node_height+m_wall_width);
        if(m_png_loaded)
        {
            if(p==olc::WHITE)
                p = m_colors->GetPixel(x,y);
/*            if(p==olc::BLACK)
                p = olc::WHITE;*/
        }

        FillRect(true_x,true_y,m_node_width,m_wall_width,p);
    }

    void draw_wallV(int x,int y,olc::Pixel p = olc::WHITE)
    {
        int true_x = m_border + m_node_width +x*(m_node_width+m_wall_width);
        int true_y = m_border + y*(m_node_height+m_wall_width);

        if(m_png_loaded)
        {
            if(p==olc::WHITE)
                p = m_colors->GetPixel(x,y);
/*            if(p==olc::BLACK)
                p = olc::WHITE;*/
        }
        FillRect(true_x,true_y,m_wall_width,m_node_height,p);
    }

    void draw_node_neighbors(int x, int y,olc::Pixel p = olc::GREEN)
    {
        std::vector<Coord> neighbors = m_maze->get_valid_neighbors(Coord(x,y));
        for(const auto& c : neighbors)
            draw_node(c.x,c.y,p);
    }

    //Debug methods
    void neighbors_test()
    {
        Clear(olc::BLACK);
        draw_node(c_x,c_y);
        draw_node_neighbors(c_x,c_y);
        if(GetKey(olc::UP).bPressed)
        {
            c_y--;
            if(c_y<0)
                c_y = m_maze->get_height() - 1;
        }

        if(GetKey(olc::DOWN).bPressed)
        {
            c_y++;
            if(c_y>m_maze->get_height()-1)
                c_y = 0;
        }

        if(GetKey(olc::LEFT).bPressed)
        {
            c_x--;
            if(c_x<0)
                c_x = m_maze->get_width() - 1;
        }

        if(GetKey(olc::RIGHT).bPressed)
        {
            c_x++;
            if(c_x>m_maze->get_width() - 1)
                c_x = 0;
        }
    }
    void display_states()
    {
        display_state(m_states[index_state]);
        if(index_state<m_states.size()-1 /*&& (GetKey(olc::SPACE).bPressed || GetKey(olc::SPACE).bHeld)*/)
            index_state++;
    }
    void draw_all_walls()
    {
        for(int i = 0;i<m_maze->get_height();i++)
            for(int j = 0;j<m_maze->get_width()-1;j++)
                draw_wallV(j,i);

        for(int i = 0;i<m_maze->get_height()-1;i++)
            for(int j = 0;j<m_maze->get_width();j++)
                draw_wallH(j,i);

    }

};



