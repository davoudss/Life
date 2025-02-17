#ifndef LIFE_HPP_
#define LIFE_HPP_
#include <iostream>
#include <memory>

class Grid
{
public:
  // assume 1 bc on each side
  Grid(int rows, int cols);
  bool At(int row, int col);
  bool operator()(int row, int col)
  {
    return At(row, col);
  }

  bool operator=(bool live)
  {
    return live;
  }

  void Live(int row, int col)
  {
    data_[row][col] = true;
  }

  void Die(int row, int col)
  {
    data_[row][col] = false;
  }

  bool IsAlive(int row, int col)
  {
    return data_[row][col];
  }
  bool is_dead(int row, int col)
  {
    return !IsAlive(row, col);
  }

  void Count(int row, int col);
  void ApplyRules(int row, int col);
  void Play();

  void set_count(int row, int col, uint8_t count);
  uint8_t get_count(int row, int col);

  void Dump(bool dump_count = false);
  int rows() const { return rows_; }
  int cols() const { return cols_; }

private:
  int rows_{}, cols_{};
  std::unique_ptr<std::unique_ptr<bool[]>[]> data_{};
  std::unique_ptr<std::unique_ptr<uint8_t[]>[]> count_{};
};

#endif