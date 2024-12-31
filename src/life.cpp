#include <iostream>
#include "life.hpp"

Grid::Grid(int rows, int cols) : rows_(rows), cols_(cols)
{
    // data_ = std::make_unique<std::unique_ptr<bool[]>[]>(rows);
    // count_ = std::make_unique<std::unique_ptr<uint8_t[]>[]>(rows);
    data_ = new bool *[rows];
    count_ = new int *[rows];
    for (size_t i = 0; i < rows; ++i)
    {
        // data_[i] = std::make_unique<bool[]>(cols);
        // count_[i] = std::make_unique<uint8_t[]>(cols);
        data_[i] = new bool[cols]{};
        count_[i] = new int[cols]{};
    }
}

bool Grid::at(int row, int col)
{
    // if out of bound, assume dead
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_)
    {
        return false;
    }
    return data_[row][col];
}

void Grid::set_count(int row, int col, int count)
{
    count_[row][col] = count;
}

uint8_t Grid::get_count(int row, int col)
{
    // if out of bound, assume dead so count = 0
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_)
    {
        return 0;
    }
    return count_[row][col];
}

void Grid::Dump(bool dump_count)
{
    std::cout << "  ";
    for (int ix = 0; ix < rows_; ix++)
    {
        std::cout << ix << " ";
    }
    std::cout << std::endl;
    for (int ix = 0; ix < rows_; ix++)
    {
        std::cout << ix << " ";
        for (int iy = 0; iy < cols_; iy++)
        {
            if (!dump_count)
            {
                if (is_alive(ix, iy))
                {
                    std::cout << "x ";
                }
                else
                {
                    std::cout << "- ";
                }
            }
            else
            {
                std::cout << (int)get_count(ix, iy) << " ";
            }
        }
        std::cout << std::endl;
    }
}

void Grid::Count(int row, int col)
{
    int count{};
    for (const auto &ix : {-1, 0, 1})
    {
        for (const auto &iy : {-1, 0, 1})
        {
            if (ix == 0 && iy == 0)
            {
                continue;
            }
            if (at(row + ix, col + iy))
            {
                count++;
            }
        }
    }
    if (count > 0)
    {
        set_count(row, col, count);
    }
}

void Grid::Apply_rules(int row, int col)
{
    auto count = get_count(row, col);
    if (is_alive(row, col))
    {
        // under population
        if (count < 2)
        {
            die(row, col);
        }
        // pass on to the next gen.
        else if (count == 2 || count == 3)
        {
            live(row, col);
        }
        // over population
        else
        {
            die(row, col);
        }
    }
    else
    {
        // reproduction
        if (count == 3)
        {
            live(row, col);
        }
    }
}

void Grid::play()
{
    for (int ix = 1; ix < rows() - 1; ix++)
    {
        for (int iy = 1; iy < cols() - 1; iy++)
        {
            // std::cout << ix << " " << iy << std::endl;
            Count(ix, iy);
        }
    }
    for (int ix = 1; ix < rows() - 1; ix++)
    {
        for (int iy = 1; iy < cols() - 1; iy++)
        {
            // std::cout << ix << " " << iy << std::endl;
            Apply_rules(ix, iy);
        }
    }
}
