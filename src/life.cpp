#include <iostream>
#include "life.hpp"
#include <vector>
#include <fstream>
#include <memory>
#include <chrono>
#include <thread>

class Grid
{
public:
    // assume 1 bc on each side
    Grid(int rows, int cols) : rows_(rows), cols_(cols)
    {
        std::cout << rows_ << std::endl;
        data_ = std::make_unique<std::unique_ptr<bool[]>[]>(rows_);
        for (int i = 0; i < rows_; ++i)
        {
            data_[i] = std::make_unique<bool[]>(cols_);
        }
    }

    bool at(int row, int col)
    {
        // if out of bound, assume dead
        if (row < 0 || row >= rows_ || col < 0 || col >= cols_)
        {
            return false;
        }
        return data_[row][col];
    }

    bool operator()(int row, int col)
    {
        return at(row, col);
    }

    bool operator=(bool live)
    {
        return live;
    }

    void live(int row, int col)
    {
        data_[row][col] = true;
    }

    void die(int row, int col)
    {
        data_[row][col] = false;
    }

    bool is_alive(int row, int col)
    {
        return data_[row][col];
    }
    bool is_dead(int row, int col)
    {
        return !data_[row][col];
    }

    int rows() const { return rows_; }
    int cols() const { return cols_; }

private:
    int rows_{}, cols_{};
    std::unique_ptr<std::unique_ptr<bool[]>[]> data_{};
};

void apply_rules(Grid &grid, int row, int col)
{
    int count = 0;
    for (int ix = -1; ix < 2; ix++)
    {
        for (int iy = -1; iy < 2; iy++)
        {
            if ((ix + iy) != 0)
            {
                count += grid(row + ix, col + iy);
            }
        }
    }
    if (grid.is_alive(row, col))
    {
        // under population
        if (count < 2)
        {
            grid.die(row, col);
        }
        // pass on to the next gen.
        else if (count == 2 || count == 3)
        {
            grid.live(row, col);
        }
        // over population
        else
        {
            grid.die(row, col);
        }
    }
    else
    {
        // reproduction
        if (count == 3)
        {
            grid.live(row, col);
        }
    }
}

void play(Grid &grid)
{
    for (int ix = 1; ix < grid.rows() - 1; ix++)
    {
        for (int iy = 1; iy < grid.cols() - 1; iy++)
        {
            std::cout << ix << " " << iy << std::endl;
            apply_rules(grid, ix, iy);
        }
    }
}

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
        play(grid);
    }

    fclose(gnuplot);

    return 0;
}