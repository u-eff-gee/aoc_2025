#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

long long area(const std::pair<long long, long long> tile_1,
               const std::pair<long long, long long> tile_2) {
  return (llabs(tile_1.first - tile_2.first) + 1) *
         (llabs(tile_1.second - tile_2.second) + 1);
}

enum Direction { left, right, up, down };

Direction get_direction(std::pair<long long, long long> tile_1,
                        std::pair<long long, long long> tile_2) {
  if (tile_1.first == tile_2.first) {
    if (tile_1.second < tile_2.second) {
      return Direction::down;
    }
    return Direction::up;
  }
  if (tile_1.second == tile_2.second) {
    if (tile_1.first < tile_2.first) {
      return Direction::right;
    }
    return Direction::left;
  }
  return Direction::left;
}

int get_curvature(std::pair<long long, long long> tile_1,
                  std::pair<long long, long long> tile_2,
                  std::pair<long long, long long> tile_3) {
  Direction d1 = get_direction(tile_1, tile_2);
  Direction d2 = get_direction(tile_2, tile_3);
  if (d1 == d2) {
    return 0;
  }
  switch (d1) {
  case left:
    switch (d2) {
    case up:
      return 1;
    case down:
      return -1;
    }
  case right:
    switch (d2) {
    case up:
      return -1;
    case down:
      return 1;
    }
  case up:
    switch (d2) {
    case left:
      return -1;
    case right:
      return 1;
    }
  case down:
    switch (d2) {
    case left:
      return 1;
    case right:
      return -1;
    }
  }
  return 0;
}

std::vector<int>
evaluate_curvature(const std::vector<std::pair<long long, long long>> tiles) {
  std::vector<int> result(tiles.size(), 0);
  int direction;

  result[0] = get_curvature(tiles.back(), tiles[0], tiles[1]);
  for (size_t i = 1; i < tiles.size(); ++i) {
    result[i] = get_curvature(tiles[i - 1], tiles[i], tiles[i + 1]);
  }

  return result;
}

bool point_is_inside(const std::pair<long long, long long> point,
                     const std::vector<std::pair<long long, long long>> tiles,
                     const std::vector<int> curvature) {
  int n_intersections = 0;
  for (size_t i = 0; i < tiles.size() - 1; ++i) {
    if (tiles[i].second == tiles[i + 1].second) {
      if (point.second == tiles[i].second) {
        if ((point.first >= std::min(tiles[i].first, tiles[i + 1].first)) &&
            (point.first <= std::max(tiles[i].first, tiles[i + 1].first))) {
          return true;
        } else {
          if (point.first < std::min(tiles[i].first, tiles[i + 1].first) &&
              curvature[i] != curvature[i + 1]) {
            ++n_intersections;
          }
        }
      }
    } else if (point.first == tiles[i].first &&
               (point.second >=
                std::min(tiles[i].second, tiles[i + 1].second)) &&
               (point.second <=
                std::max(tiles[i].second, tiles[i + 1].second))) {
      return true;
    } else if (point.first < tiles[i].first &&
               point.second >= std::min(tiles[i].second, tiles[i + 1].second) &&
               point.second <= std::max(tiles[i].second, tiles[i + 1].second)) {
      ++n_intersections;
    }
  }
  if (tiles.back().second == tiles[0].second) {
    if (point.second == tiles.back().second) {
      if (point.first >= std::min(tiles.back().first, tiles[0].first) &&
          point.first <= std::max(tiles.back().first, tiles[0].first)) {
        return true;
      } else {
        if (point.first < std::min(tiles.back().first, tiles[0].first) &&
            curvature.back() != curvature[0]) {
          ++n_intersections;
        }
      }
    }
  } else if (point.first == tiles.back().first &&
             (point.second >= std::min(tiles.back().second, tiles[0].second)) &&
             (point.second <= std::max(tiles.back().second, tiles[0].second))) {
    return true;
  } else if (point.first < tiles[0].first &&
             point.second >= std::min(tiles.back().second, tiles[0].second) &&
             point.second <= std::max(tiles.back().second, tiles[0].second)) {
    ++n_intersections;
  }
  if (n_intersections % 2 == 0) {
    return false;
  }
  return true;
}

bool rectangle_is_inside(
    const std::pair<long long, long long> tile_1,
    const std::pair<long long, long long> tile_2,
    const std::vector<std::pair<long long, long long>> tiles,
    std::vector<int> curvature) {
  std::pair<long long, long long> x = std::minmax(tile_1.first, tile_2.first);
  std::pair<long long, long long> y = std::minmax(tile_1.second, tile_2.second);
  std::vector<std::pair<long long, long long>> rectangle = {
      {x.first, y.first},
      {x.first, y.second},
      {x.second, y.second},
      {x.second, y.first}};

  for (auto tile : tiles) {
    if (tile == tile_1 || tile == tile_2) {
      continue;
    }
    if (tile.first > x.first && tile.first < x.second &&
        tile.second > y.first && tile.second < y.second) {
      return false;
    }
  }
  if (!(point_is_inside({x.second, y.first}, tiles, curvature) &&
        point_is_inside({x.second, y.second}, tiles, curvature) &&
        point_is_inside({x.first + 1, y.second - 1}, tiles, curvature) &&
        point_is_inside({x.second - 1, y.second - 1}, tiles, curvature))) {
    return false;
  }
  for (long long xx = x.first; xx <= x.second; ++xx) {
    if (!point_is_inside({xx, y.first}, tiles, curvature) ||
        !point_is_inside({xx, y.second}, tiles, curvature)) {
      return false;
    }
  }
  for (long long yy = y.first; yy <= y.second; ++yy) {
    if (!point_is_inside({x.first, yy}, tiles, curvature) ||
        !point_is_inside({x.second, yy}, tiles, curvature)) {
      return false;
    }
  }
  return true;
}

long long largest_area(const std::vector<std::pair<long long, long long>> tiles,
                       const bool inside = false) {
  long long result = 0ll;
  long long current_area;
  bool calculate = false;
  std::vector<int> curvature = evaluate_curvature(tiles);
  for (size_t i = 0; i < tiles.size(); ++i) {
    for (size_t j = i + 1; j < tiles.size(); ++j) {
      std::cout << i * tiles.size() + j << " / "
                << tiles.size() * (tiles.size() - 1) << std::endl;
      calculate =
          inside ? rectangle_is_inside(tiles[i], tiles[j], tiles, curvature)
                 : true;
      if (calculate) {
        current_area = area(tiles[i], tiles[j]);
        if (current_area > result) {
          result = current_area;
        }
      }
    }
  }
  return result;
}

std::vector<std::pair<long long, long long>>
read_file(const std::string input_file_name) {
  std::vector<std::pair<long long, long long>> result;
  std::ifstream file(input_file_name);
  std::string line;
  size_t comma;
  long long x, y;
  while (getline(file, line)) {
    comma = line.find(",");
    result.push_back(
        {stoll(line.substr(0, comma)), stoll(line.substr(comma + 1))});
  }
  return result;
}

int main(int argc, char *argv[]) {
  std::cout << "Running unit tests ..." << std::endl;

  assert(area({2, 5}, {11, 1}) == 50);

  std::cout << "\tUnits tests completed successfully." << std::endl;

  std::cout << "Running AOC test ..." << std::endl;

  std::vector<std::pair<long long, long long>> example_input = {
      {7, 1}, {11, 1}, {11, 7}, {9, 7}, {9, 5}, {2, 5}, {2, 3}, {7, 3},
  };

  assert(largest_area(example_input) == 50);
  assert(largest_area(read_file("input_test.txt")) == 50);
  std::vector<int> example_curvature = {1, 1, 1, 1, -1, 1, 1, -1};
  assert(evaluate_curvature(example_input) == example_curvature);
  example_curvature = evaluate_curvature(example_input);

  std::vector<std::pair<long long, long long>> example_ranges{
      {-2, -1}, {7, 11}, {7, 11}, {2, 11},  {2, 11},
      {2, 11},  {9, 11}, {9, 11}, {-2, -1},
  };
  for (long long i = 0; i < 14; ++i) {
    for (long long j = 0; j < 9; ++j) {
      if (example_ranges[j].first <= i && i <= example_ranges[j].second) {
        assert(point_is_inside({i, j}, example_input, example_curvature));
      } else {
        assert(!point_is_inside({i, j}, example_input, example_curvature));
      }
    }
  }
  assert(largest_area(example_input, true) == 24);

  std::cout << "\tAOC test completed successfully." << std::endl;

  std::cout << "Running AOC puzzle ..." << std::endl;
  std::cout << "\tResult 1/2: " << largest_area(read_file("input.txt"))
            << std::endl;
  std::cout << "\tResult 2/2: " << largest_area(read_file("input.txt"), true)
            << std::endl;

  return 0;
}