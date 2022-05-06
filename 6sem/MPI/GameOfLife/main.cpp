#include <mpi.h>
#include <SFML/Graphics.hpp>

bool LIVING = false;

const int window_width = 1300;
const int window_height = 800;
const int block_size = 20;


int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0)
  {
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Conway's Game of Life");

    bool** field = new bool*[window_width / block_size];
    for (int y = 0; y < window_height / block_size; y++)
    {
      field[y] = new bool[window_width / block_size];
      for (int x = 0; x < window_width / block_size; x++)
        field[y][x] = false;
    }

    int** neigbours = new int*[window_width / block_size];
    for (int y = 0; y < window_height / block_size; y++)
    {
      neigbours[y] = new int[window_width / block_size];
      for (int x = 0; x < window_width / block_size; x++)
        neigbours[y][x] = 0;
    }

    bool mouseLock = false;
    int cursor_x = 0, cursor_y = 0;
    sf::Clock fps_clock;
    sf::Clock update_clock;

    while (true)
    {
      sf::Event event;
      window.pollEvent(event);

      if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Escape)))
        break;
      else if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::P or event.key.code == sf::Keyboard::Space))
      {
        LIVING = not LIVING;
        printf("Pause switched\n");
      }

      if (event.type == sf::Event::MouseMoved and LIVING == false)
      {
        cursor_x = event.mouseMove.x / block_size;
        cursor_y = (window_height - event.mouseMove.y) / block_size;
      }

      if (event.type == sf::Event::MouseButtonPressed and LIVING == false and mouseLock == false)
      {
        mouseLock = true;
        int x_pos = event.mouseButton.x / block_size;
        int y_pos = (window_height - event.mouseButton.y) / block_size;

        field[y_pos][x_pos] = not field[y_pos][x_pos];
      }

      if (event.type == sf::Event::MouseButtonReleased and LIVING == false and mouseLock == true)
        mouseLock = false;

      if (fps_clock.getElapsedTime().asMilliseconds() >= 15)
      {
        window.clear();

        if (LIVING == false)
        {
          sf::RectangleShape square(sf::Vector2f(19.f, 19.f));
          square.setPosition(cursor_x * block_size, window_height - (cursor_y + 1) * block_size);
          sf::Color color(80, 80, 80, 255);
          square.setFillColor(color);
          window.draw(square);
        }

        for (int y = 0; y < window_height / block_size; y++)
        {
          for (int x = 0; x < window_width / block_size; x++)
          {
            if (field[y][x])
            {
              sf::RectangleShape square(sf::Vector2f(19.f, 19.f));
        			square.setPosition(x * block_size, window_height - (y + 1) * block_size);
        			square.setFillColor(sf::Color::White);
        			window.draw(square);
            }
          }
        }

        if (LIVING and update_clock.getElapsedTime().asMilliseconds() >= 150)
        {
          update_clock.restart();
          for (int y = 0; y < window_height / block_size; y++)
          {
            for (int x = 0; x < window_width / block_size; x++)
            {
              int neighbours_count = 0;
              if (x < window_width / block_size - 1 and y < window_height / block_size - 1 and field[y+1][x+1]) neighbours_count++;
              if (y < window_height / block_size - 1 and field[y+1][x]) neighbours_count++;
              if (x < window_width / block_size - 1 and field[y][x+1]) neighbours_count++;
              if (x > 0 and y > 0 and field[y-1][x-1]) neighbours_count++;
              if (y > 0 and field[y-1][x]) neighbours_count++;
              if (x > 0 and field[y][x-1]) neighbours_count++;
              if (x > 0 and y < window_height / block_size - 1 and field[y+1][x-1]) neighbours_count++;
              if (x < window_width / block_size - 1 and y > 0 and field[y-1][x+1]) neighbours_count++;
              neigbours[y][x] = neighbours_count;
            }
          }

          for (int y = 1; y < window_height / block_size; y++)
          {
            for (int x = 1; x < window_width / block_size; x++)
            {
              int N = neigbours[y][x];
              bool alive = field[y][x];
              if (N == 3 and not alive) field[y][x] = true;
              if (not (N == 2 or N == 3) and alive) field[y][x] = false;
            }
          }
        }

        window.display();
      }
    }
  }

  MPI_Finalize();
}
