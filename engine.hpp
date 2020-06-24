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
    Maze_Generator(std::string filename,bool save_states = false,int node_width = 1,int node_height = 1,int wall_width = 1,int border = 1,int pixel_width = 1,int pixel_height = 1) :m_save_states(save_states), m_node_width(node_width), m_node_height(node_height), m_wall_width(wall_width),m_border(border),m_pixel_width(pixel_width),m_pixel_height(pixel_height)
    {

		sAppName = "Maze generator";
        m_maze = new Maze(1,1,save_states);
        m_colors = new olc::Sprite(1,1);
        m_png_loaded = false;
        load_png(filename);
    }


public:
	bool OnUserCreate() override
	{
        m_index_state = 0;
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

    void generate(bool color_search = false)
    {
        int width = m_maze->get_width();
        int height = m_maze->get_height();
        m_true_width = 2 * m_border + width * (m_node_width + m_wall_width) -  m_wall_width;
        m_true_height = 2 * m_border + height * (m_node_height + m_wall_width) - m_wall_width;
        Construct(m_true_width,m_true_height, m_pixel_width, m_pixel_height);
        m_maze->generate(Coord(0,0),color_search,convert_sprite_pixels(*m_colors));
        if(m_save_states)
            m_states = m_maze->get_states();
    }

public:
    void load_png(std::string filename,bool set_dimension = true)
    {
        m_colors = new olc::Sprite(filename);
        m_png_loaded = true;
        if(m_colors->width!=0 && m_colors->height!=0 && set_dimension)
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
    int m_index_state;



    void display_maze()
    {
        Clear(olc::BLACK);
        display_walls();
        display_nodes();
    }

    void display_states()
    {
        display_state(m_states[m_index_state]);
        if(m_index_state<m_states.size()-1)
            m_index_state++;
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

    //Convert a Sprite into a vector of Pixel_Maze, it's used in order to generate the maze depending on the colors of the sprite
    std::vector<Pixel_Maze> convert_sprite_pixels(olc::Sprite const& sprite)
    {
        std::vector<Pixel_Maze> colors = std::vector<Pixel_Maze>();
        for(int i = 0;i<sprite.height;i++)
        {
            for(int j = 0;j<sprite.width;j++)
            {
                olc::Pixel p = sprite.GetPixel(j,i);
                colors.push_back(Pixel_Maze(p.r,p.g,p.b));
            }
        }

        return colors;
    }
};



