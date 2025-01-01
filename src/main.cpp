#include <fstream>
#include <chrono>
#include <thread>
#include "life.hpp"
#include <string>

void init(FILE *gnuplot, int rows, int cols)
{
  if (!gnuplot)
  {
    std::cerr << "Error opening Gnuplot!" << std::endl;
    exit(EXIT_FAILURE);
  }

  // fprintf(gnuplot, "set style line 1 pt 5 ps 1.6 lc rgb 'blue'\n");

  // fprintf(gnuplot, "set grid xtics\n");
  // fprintf(gnuplot, "set grid ytics\n");
  // fprintf(gnuplot, "set autoscale xfix\n");
  // fprintf(gnuplot, "set autoscale yfix\n");
  // fprintf(gnuplot, "set autoscale cbfix\n");

  fprintf(gnuplot, "set palette defined (0 'white', 1 'black')\n");
  fprintf(gnuplot, "set xrange [%d:%d]\n", 0, rows);
  fprintf(gnuplot, "set yrange [%d:%d]\n", 0, cols);
  // fprintf(gnuplot, "set size ratio -1\n");
  // fprintf(gnuplot, "set xlabel 'X-axis'\n");
  // fprintf(gnuplot, "set ylabel 'Y-axis'\n");
  fprintf(gnuplot, "set cbrange [0:1]\n");
  fprintf(gnuplot, "unset colorbox\n");
  fprintf(gnuplot, "unset key\n");

  // fprintf(gnuplot, "unset xtics\n");
  // fprintf(gnuplot, "unset ytics\n");
  // fprintf(gnuplot, "unset xlabel\n");
  // fprintf(gnuplot, "unset ylabel\n");
}
void Plot(FILE *gnuplot, Grid &grid, int rep)
{
  fprintf(gnuplot, "set title \"rep=%s\"\n", std::to_string(rep).c_str());

  // fprintf(gnuplot, "plot '-' with points ls 1\n");
  fprintf(gnuplot, "set xtics 1\n");
  fprintf(gnuplot, "set ytics 1\n");
  fprintf(gnuplot, "set grid\n");
  fprintf(gnuplot, "plot '-' matrix with image\n");
  for (int ix = 1; ix < grid.rows() - 1; ix++)
  {
    for (int iy = 1; iy < grid.cols() - 1; iy++)
    {
      fprintf(gnuplot, "%d ", grid.IsAlive(ix, iy));
      // if (grid(ix, iy))
      // {
      //   // fprintf(gnuplot, "%f %f\n", ix + 0.5, iy + 0.5);
      // }
    }
    fprintf(gnuplot, "\n");
  }
  fprintf(gnuplot, "e\n");
  fprintf(gnuplot, "refresh\n");
  fflush(gnuplot);
}

int main(int argc, char *argv[])
{
  int reps = 10;
  if (argc > 1)
  {
    reps = atoi(argv[1]);
    std::cout << reps << std::endl;
  }
  const int Gx = 20;
  const int Gy = 20;

  // grid with 1 bc
  Grid grid(Gx + 2, Gy + 2);

  // init
  grid.Live(3, 3);
  grid.Live(3, 4);
  grid.Live(2, 4);
  grid.Live(4, 4);
  grid.Live(4, 5);
  grid.Dump();
  // Open a pipe to Gnuplot
  FILE *gnuplot = popen("gnuplot -persistent", "w");
  init(gnuplot, Gx, Gy);

  for (int rep = 0; rep < reps; rep++)
  {
    Plot(gnuplot, grid, rep);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    grid.Play();
  }

  fclose(gnuplot);

  return 0;
}