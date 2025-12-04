#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int sum_within_radius(const std::vector<std::vector<int>> &grid, const int n_x,
                      const int n_y, const int radius,
                      bool count_self = false) {
  int sum = 0;
  for (int x_pos = std::max(n_x - radius, 0);
       x_pos <= std::min(n_x + radius, (int)grid[n_y].size() - 1); ++x_pos) {
    for (int y_pos = std::max(n_y - radius, 0);
         y_pos <= std::min(n_y + radius, (int)grid.size() - 1); ++y_pos) {
      sum += grid[y_pos][x_pos];
    }
  }
  if (!count_self) {
    return sum - grid[n_y][n_x];
  }
  return sum;
}

std::vector<std::vector<int>> analyze_grid(std::vector<std::vector<int>> grid,
                                           const int radius) {
  std::vector<std::vector<int>> result;
  for (size_t i = 0; i < grid.size(); ++i) {
    result.push_back(std::vector<int>(grid[i].size(), 0));
  }
  for (int n_y = 0; n_y < grid.size(); ++n_y) {
    for (int n_x = 0; n_x < grid[n_y].size(); ++n_x) {
      result[n_y][n_x] = sum_within_radius(grid, n_x, n_y, radius);
    }
  }
  return result;
}

int count_grid(std::vector<std::vector<int>> grid,
               std::vector<std::vector<int>> analyzed_grid,
               const int max_score) {
  int count = 0;
  for (int n_y = 0; n_y < grid.size(); ++n_y) {
    for (int n_x = 0; n_x < grid[n_y].size(); ++n_x) {
      if (grid[n_y][n_x] == 1 && analyzed_grid[n_y][n_x] < max_score) {
        ++count;
      }
    }
  }
  return count;
}

std::vector<int> parse_line(std::string line) {
  std::vector<int> result(line.size(), 0);
  for (int n_c = 0; n_c < line.size(); ++n_c) {
    if (line[n_c] == '@') {
      result[n_c] = 1;
    }
  }
  return result;
}

std::vector<std::vector<int>> read_grid(const std::string input_file_name) {
  std::vector<std::vector<int>> result;
  std::ifstream file(input_file_name);
  std::string line;
  while (std::getline(file, line)) {
    result.push_back(parse_line(line));
  }
  return result;
}

bool update_grid(std::vector<std::vector<int>> &grid,
                 std::vector<std::vector<int>> analyzed_grid,
                 const int max_score) {
  bool grid_updated = false;
  for (size_t n_line = 0; n_line < grid.size(); ++n_line) {
    for (size_t n_column = 0; n_column < grid[n_line].size(); ++n_column) {
      if (grid[n_line][n_column] == 1 &&
          analyzed_grid[n_line][n_column] < max_score) {
        grid[n_line][n_column] = 0;
        grid_updated = true;
      }
    }
  }
  return grid_updated;
}

std::vector<int> process_file(const std::string input_file_name) {
  std::vector<std::vector<int>> grid = read_grid(input_file_name);
  int score = 0;
  std::vector<int> result;
  std::vector<std::vector<int>> analyzed_grid;
  bool grid_updated = true;
  while (grid_updated) {
    analyzed_grid = analyze_grid(grid, 1);
    score += count_grid(grid, analyzed_grid, 4);
    result.push_back(score);
    grid_updated = update_grid(grid, analyzed_grid, 4);
  }
  return result;
}

int main(int argc, char *argv[]) {
  std::cout << "Running unit tests ..." << std::endl;

  assert(sum_within_radius({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 0, 0, 0) == 0);
  assert(sum_within_radius({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 0, 0, 0, true) ==
         1);
  assert(sum_within_radius({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 0, 0, 1) == 11);
  assert(sum_within_radius({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 1, 0, 1) == 19);
  assert(sum_within_radius({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 0, 1, 1) == 23);
  assert(sum_within_radius({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 0, 0, 2) == 44);
  assert(sum_within_radius({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 1, 1, 2) == 40);

  assert(sum_within_radius({{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}}, 0, 0,
                           0) == 0);
  assert(sum_within_radius({{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}}, 0, 0,
                           1) == 9);
  assert(sum_within_radius({{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}}, 0, 0,
                           2) == 20);
  assert(sum_within_radius({{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}}, 0, 2,
                           1) == 28);

  std::vector<std::vector<int>> result =
      analyze_grid({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 1);
  assert(result[0][0] == 11);
  assert(result[1][0] == 23);
  assert(result[0][1] == 19);

  std::vector<int> parsed_line = parse_line("..@@.@@@@.");
  assert(parsed_line.size() == 10);
  assert(parsed_line[0] == 0);
  assert(parsed_line[1] == 0);
  assert(parsed_line[2] == 1);
  assert(parsed_line[3] == 1);
  assert(parsed_line[4] == 0);
  assert(parsed_line[5] == 1);
  assert(parsed_line[6] == 1);
  assert(parsed_line[7] == 1);
  assert(parsed_line[8] == 1);
  assert(parsed_line[9] == 0);

  std::cout << "\tUnits tests completed successfully." << std::endl;

  std::cout << "Running AOC test ..." << std::endl;

  std::vector<std::vector<int>> example_grid = {
      {0, 0, 1, 1, 0, 1, 1, 1, 1, 0}, {1, 1, 1, 0, 1, 0, 1, 0, 1, 1},
      {1, 1, 1, 1, 1, 0, 1, 0, 1, 1}, {1, 0, 1, 1, 1, 1, 0, 0, 1, 0},
      {1, 1, 0, 1, 1, 1, 1, 0, 1, 1}, {0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
      {0, 1, 0, 1, 0, 1, 0, 1, 1, 1}, {1, 0, 1, 1, 1, 0, 1, 1, 1, 1},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 0}, {1, 0, 1, 0, 1, 1, 1, 0, 1, 0}};

  std::vector<std::vector<int>> example_grid_analysis =
      analyze_grid(example_grid, 1);
  assert(example_grid_analysis[0][0] == 2);
  assert(example_grid_analysis[0][1] == 4);
  assert(example_grid_analysis[0][2] == 3);
  assert(count_grid(example_grid, analyze_grid(example_grid, 1), 4) == 13);

  std::vector<std::vector<int>> example_grid_read_from_file =
      read_grid("input_test.txt");
  for (size_t i = 0; i < example_grid.size(); ++i) {
    for (size_t j = 0; j < example_grid.size(); ++j) {
      assert(example_grid[i][j] == example_grid_read_from_file[i][j]);
    }
  }
  std::vector<int> example_result = process_file("input_test.txt");
  assert(example_result[0] == 13);
  assert(example_result[example_result.size() - 1] == 43);

  std::vector<std::vector<int>> updated_example_grid_example = {
      {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, {0, 1, 1, 0, 1, 0, 1, 0, 1, 1},
      {1, 1, 1, 1, 1, 0, 0, 0, 1, 1}, {1, 0, 1, 1, 1, 1, 0, 0, 1, 0},
      {0, 1, 0, 1, 1, 1, 1, 0, 1, 0}, {0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
      {0, 1, 0, 1, 0, 1, 0, 1, 1, 1}, {0, 0, 1, 1, 1, 0, 1, 1, 1, 1},
      {0, 1, 1, 1, 1, 1, 1, 1, 1, 0}, {0, 0, 0, 0, 1, 1, 1, 0, 0, 0}};
  assert(update_grid(example_grid, example_grid_analysis, 4));
  for (size_t i = 0; i < updated_example_grid_example.size(); ++i) {
    for (size_t j = 0; j < updated_example_grid_example.size(); ++j) {
      assert(example_grid[i][j] == updated_example_grid_example[i][j]);
    }
  }
  std::cout << "\tAOC test completed successfully." << std::endl;

  std::cout << "Running AOC puzzle ..." << std::endl;
  std::vector<int> puzzle_result = process_file("input.txt");
  std::cout << "\tResult 1/2: " << puzzle_result[0] << std::endl;
  std::cout << "\tResult 2/2: " << puzzle_result[puzzle_result.size() - 1]
            << std::endl;

  return 0;
}