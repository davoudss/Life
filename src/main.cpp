#include <fstream>
#include <chrono>
#include <thread>
#include "life.hpp"

int main()
{
  const int Gx = 10;
  const int Gy = 10;

  // grid with 1 bc
  Grid grid(Gx + 2, Gy + 2);

  // init
  grid.live(3, 3);
  grid.live(3, 4);
  grid.live(4, 3);

  Play play(grid);

  // Open a pipe to Gnuplot
  FILE *gnuplot = popen("gnuplot -persistent", "w");
  if (!gnuplot)
  {
    std::cerr << "Error opening Gnuplot!" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Set up some initial Gnuplot commands
  // fprintf(gnuplot, "unset key");
  fprintf(gnuplot, "set style line 1 pt 5 ps 4.67 lc rgb 'blue'\n");

  // Set the plot range
  fprintf(gnuplot, "set xrange [-10:20]\n");
  fprintf(gnuplot, "set yrange [-10:20]\n");

  for (int rep = 0; rep < 1; rep++)
  {
    fprintf(gnuplot, "plot '-' with points ls 1\n");

    for (int ix = 0; ix < Gx; ix++)
    {
      for (int iy = 0; iy < Gy; iy++)
      {
        if (grid(ix + 1, iy + 1))
        {
          fprintf(gnuplot, "%d %d\n", ix, iy);
        }
      }
    }
    fprintf(gnuplot, "e\n");
    fflush(gnuplot);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    play.do_play();
  }

  fclose(gnuplot);

  return 0;
}