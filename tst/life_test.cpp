#include "life.hpp"
#include <gtest/gtest.h>

// Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
TEST(LifeTest, rule4)
{
  const int Gx = 10;
  const int Gy = 10;
  // grid with 1 bc
  Grid grid(Gx + 2, Gy + 2);

  grid.Live(3, 3);
  grid.Live(3, 2);
  grid.Live(3, 1);
  EXPECT_TRUE(grid.is_dead(4, 2));

  grid.Play();
  EXPECT_TRUE(grid.IsAlive(4, 2));
}

// Any live cell with more than three live neighbours dies, as if by overpopulation.
TEST(LifeTest, rule3)
{
  const int Gx = 10;
  const int Gy = 10;
  // grid with 1 bc
  Grid grid(Gx + 2, Gy + 2);

  grid.Live(3, 3);
  grid.Live(3, 2);
  grid.Live(3, 1);
  grid.Live(4, 3);
  grid.Live(4, 2);
  EXPECT_TRUE(grid.IsAlive(4, 2));

  grid.Play();
  EXPECT_TRUE(grid.is_dead(4, 2));
}

// Any live cell with two or three live neighbours lives on to the next generation.
TEST(LifeTest, rule2)
{
  const int Gx = 10;
  const int Gy = 10;
  // grid with 1 bc
  Grid grid(Gx + 2, Gy + 2);

  grid.Live(3, 3);
  grid.Live(3, 2);
  grid.Live(4, 3);
  EXPECT_TRUE(grid.IsAlive(3, 3));

  grid.Play();
  EXPECT_TRUE(grid.IsAlive(3, 3));
}

// Any live cell with fewer than two live neighbours dies, as if by underpopulation.
TEST(LifeTest, rule1)
{
  const int Gx = 10;
  const int Gy = 10;
  // grid with 1 bc
  Grid grid(Gx + 2, Gy + 2);

  grid.Live(3, 3);
  EXPECT_TRUE(grid.IsAlive(3, 3));

  grid.Play();
  EXPECT_TRUE(grid.is_dead(3, 3));
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}