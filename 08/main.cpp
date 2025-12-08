#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

struct Distance {
  double value;
  int jb1;
  int jb2;

  Distance(const double value, const int jb1, const int jb2)
      : value(value), jb1(jb1), jb2(jb2){};

  bool operator<(Distance d) { return value < d.value; }
};

struct JunctionBox {
  std::array<int, 3> position;
  int circuit;

  JunctionBox(const std::array<int, 3> position, int circuit = -1)
      : position(position), circuit(circuit) {}
};

void reset_circuits(std::vector<JunctionBox> &jbs) {
  for (size_t i = 0; i < jbs.size(); ++i) {
    jbs[i].circuit = -(i + 1);
  }
}

double distance(const JunctionBox jb1, const JunctionBox jb2) {
  return (pow(jb1.position[0] - jb2.position[0], 2) +
          pow(jb1.position[1] - jb2.position[1], 2) +
          pow(jb1.position[2] - jb2.position[2], 2));
}

std::vector<Distance>
evaluate_distances(std::vector<JunctionBox> junction_boxes) {
  std::vector<Distance> result;
  for (size_t i = 0; i < junction_boxes.size(); ++i) {
    for (size_t j = i + 1; j < junction_boxes.size(); ++j) {
      result.push_back(
          Distance(distance(junction_boxes[i], junction_boxes[j]), i, j));
    }
  }
  return result;
}

void evaluate_circuits(std::vector<Distance> dis, std::vector<JunctionBox> &jbs,
                       size_t n_max, bool sort_distances = true) {
  if (sort_distances) {
    std::sort(dis.begin(), dis.end());
  }
  int current_circuit = 0;
  int united_circuit, old_circuit;
  for (size_t n = 0; n < n_max; ++n) {
    if (jbs[dis[n].jb1].circuit < 0 && jbs[dis[n].jb2].circuit < 0) {
      jbs[dis[n].jb1].circuit = current_circuit;
      jbs[dis[n].jb2].circuit = current_circuit;
      ++current_circuit;
    } else if (jbs[dis[n].jb1].circuit != jbs[dis[n].jb2].circuit) {
      if (jbs[dis[n].jb1].circuit < 0 && jbs[dis[n].jb2].circuit >= 0) {
        jbs[dis[n].jb1].circuit = jbs[dis[n].jb2].circuit;
      } else if (jbs[dis[n].jb2].circuit < 0 && jbs[dis[n].jb1].circuit >= 0) {
        jbs[dis[n].jb2].circuit = jbs[dis[n].jb1].circuit;
      } else {
        united_circuit =
            std::min({jbs[dis[n].jb1].circuit, jbs[dis[n].jb2].circuit});
        old_circuit =
            std::max({jbs[dis[n].jb1].circuit, jbs[dis[n].jb2].circuit});
        for (size_t m = 0; m < jbs.size(); ++m) {
          if (jbs[m].circuit == old_circuit) {
            jbs[m].circuit = united_circuit;
          }
        }
      }
    }
  }
}

std::vector<std::pair<int, int>> count_circuits(std::vector<JunctionBox> jbs) {
  std::vector<std::pair<int, int>> result;
  bool circuit_found = false;
  for (auto jb : jbs) {
    circuit_found = false;
    for (size_t i = 0; i < result.size(); ++i) {
      if (result[i].second == jb.circuit) {
        result[i].first += 1;
        circuit_found = true;
        break;
      }
    }
    if (!circuit_found) {
      result.push_back(std::pair<int, int>{1, jb.circuit});
    }
  }
  return result;
}

int score(std::vector<std::pair<int, int>> circuits, int n_max) {
  int result = 1;
  int n = 0;
  std::sort(circuits.begin(), circuits.end());
  for (int i = circuits.size() - 1; i >= 0; --i) {
    if (circuits[i].second >= 0) {
      result *= circuits[i].first;
      ++n;
    }
    if (n == n_max) {
      break;
    }
  }
  return result;
}

bool contains_unconnected(std::vector<std::pair<int, int>> circuit_stats) {
  for (auto c : circuit_stats) {
    if (c.second == -1) {
      return true;
    }
  }
  return false;
}

int find_last_pairs_product_of_x_coordinates(std::vector<Distance> dis,
                                             std::vector<JunctionBox> &jbs) {
  std::sort(dis.begin(), dis.end());
  evaluate_circuits(dis, jbs, 1, false);
  std::vector<std::pair<int, int>> circuit_stats = count_circuits(jbs);
  int n = 2;
  while (contains_unconnected(circuit_stats) || circuit_stats.size() > 2) {
    evaluate_circuits(dis, jbs, n, false);
    circuit_stats = count_circuits(jbs);
    ++n;
  }
  for (auto d : dis) {
    if (jbs[d.jb1].circuit != jbs[d.jb2].circuit) {
      return jbs[d.jb1].position[0] * jbs[d.jb2].position[0];
    }
  }
  return 0;
}

std::pair<int, int> evaluate(std::vector<JunctionBox> jbs,
                             const int n_max_pairs, int n_max) {
  std::vector<Distance> dis = evaluate_distances(jbs);
  evaluate_circuits(dis, jbs, n_max_pairs);
  std::vector<std::pair<int, int>> circuit_stats = count_circuits(jbs);
  reset_circuits(jbs);
  return {score(circuit_stats, n_max),
          find_last_pairs_product_of_x_coordinates(dis, jbs)};
}

std::vector<JunctionBox> read_file(const std::string input_file_name) {
  std::vector<JunctionBox> result;
  int x, y, z;
  std::ifstream file(input_file_name);
  std::string line;
  size_t first_comma, second_comma;
  int n_jb = 0;
  while (getline(file, line)) {
    first_comma = line.find(",");
    x = stoi(line.substr(0, first_comma));
    second_comma = line.substr(first_comma + 1).find(",");
    y = stoi(line.substr(first_comma + 1, second_comma));
    z = stoi(line.substr(first_comma + second_comma + 2));
    result.push_back(JunctionBox({x, y, z}, -(n_jb + 1)));
  }
  return result;
}

std::pair<int, int> process_file(const std::string input_file_name,
                                 const int n_max_pairs, int n_max) {
  std::vector<JunctionBox> jbs = read_file(input_file_name);
  return evaluate(jbs, n_max_pairs, n_max);
}

int main(int argc, char *argv[]) {
  std::cout << "Running unit tests ..." << std::endl;

  std::vector<JunctionBox> jbs{
      JunctionBox({1, 2, 3}, -1),
      JunctionBox({4, 5, 6}, -2),
      JunctionBox({7, 8, 10}, -3),
  };
  std::vector<Distance> dis = evaluate_distances(jbs);
  assert(dis[0].value == 27.0);
  assert(dis[0].jb1 == 0);
  assert(dis[0].jb2 == 1);
  assert(dis[1].value == 121.0);
  assert(dis[1].jb1 == 0);
  assert(dis[1].jb2 == 2);
  assert(dis[2].value == 34.0);
  assert(dis[2].jb1 == 1);
  assert(dis[2].jb2 == 2);

  std::sort(dis.begin(), dis.end());
  assert(dis[0].value == 27.0);
  assert(dis[1].value == 34.0);
  assert(dis[2].value == 121.0);

  jbs = {
      JunctionBox({100, 0, 0}, -1), JunctionBox({6, 0, 0}, -2),
      JunctionBox({120, 0, 0}, -3), JunctionBox({4, 0, 0}, -4),
      JunctionBox({150, 0, 0}, -5), JunctionBox({0, 0, 0}, -6),
      JunctionBox({1, 0, 0}, -7),
  };
  dis = evaluate_distances(jbs);
  evaluate_circuits(dis, jbs, 2);
  assert(jbs[0].circuit == -1);
  assert(jbs[1].circuit == 1);
  assert(jbs[2].circuit == -3);
  assert(jbs[3].circuit == 1);
  assert(jbs[4].circuit == -5);
  assert(jbs[5].circuit == 0);
  assert(jbs[6].circuit == 0);
  reset_circuits(jbs);
  evaluate_circuits(dis, jbs, 3);
  assert(jbs[0].circuit == -1);
  assert(jbs[1].circuit == 0);
  assert(jbs[2].circuit == -3);
  assert(jbs[3].circuit == 0);
  assert(jbs[4].circuit == -5);
  assert(jbs[5].circuit == 0);
  assert(jbs[6].circuit == 0);
  reset_circuits(jbs);
  evaluate_circuits(dis, jbs, 4);
  assert(jbs[0].circuit == -1);
  assert(jbs[1].circuit == 0);
  assert(jbs[2].circuit == -3);
  assert(jbs[3].circuit == 0);
  assert(jbs[4].circuit == -5);
  assert(jbs[5].circuit == 0);
  assert(jbs[6].circuit == 0);
  reset_circuits(jbs);
  evaluate_circuits(dis, jbs, 7);
  assert(jbs[0].circuit == 2);
  assert(jbs[1].circuit == 0);
  assert(jbs[2].circuit == 2);
  assert(jbs[3].circuit == 0);
  assert(jbs[4].circuit == -5);
  assert(jbs[5].circuit == 0);
  assert(jbs[6].circuit == 0);

  std::vector<std::pair<int, int>> circuit_stats =
      count_circuits(std::vector<JunctionBox>{
          JunctionBox({0, 0, 0}, -1), JunctionBox({0, 0, 0}, -1),
          JunctionBox({0, 0, 0}, -1), JunctionBox({0, 0, 0}, -1),
          JunctionBox({0, 0, 0}, -1), JunctionBox({0, 0, 0}, 1),
          JunctionBox({0, 0, 0}, 1), JunctionBox({0, 0, 0}, 1),
          JunctionBox({0, 0, 0}, 100), JunctionBox({0, 0, 0}, 100)});
  assert(circuit_stats.size() == 3);
  assert(circuit_stats[0].second == -1);
  assert(circuit_stats[0].first == 5);
  assert(circuit_stats[1].second == 1);
  assert(circuit_stats[1].first == 3);
  assert(circuit_stats[2].second == 100);
  assert(circuit_stats[2].first == 2);

  assert(score(circuit_stats, 1) == 3);
  assert(score(circuit_stats, 2) == 6);
  assert(score(circuit_stats, 3) == 6);

  std::cout << "\tUnits tests completed successfully." << std::endl;

  std::cout << "Running AOC test ..." << std::endl;
  std::vector<JunctionBox> example_jbs = {
      JunctionBox({162, 817, 812}), JunctionBox({57, 618, 57}),
      JunctionBox({906, 360, 560}), JunctionBox({592, 479, 940}),
      JunctionBox({352, 342, 300}), JunctionBox({466, 668, 158}),
      JunctionBox({542, 29, 236}),  JunctionBox({431, 825, 988}),
      JunctionBox({739, 650, 466}), JunctionBox({52, 470, 668}),
      JunctionBox({216, 146, 977}), JunctionBox({819, 987, 18}),
      JunctionBox({117, 168, 530}), JunctionBox({805, 96, 715}),
      JunctionBox({346, 949, 466}), JunctionBox({970, 615, 88}),
      JunctionBox({941, 993, 340}), JunctionBox({862, 61, 35}),
      JunctionBox({984, 92, 344}),  JunctionBox({425, 690, 689})};
  std::vector<Distance> example_dis = evaluate_distances(example_jbs);
  std::sort(example_dis.begin(), example_dis.end());

  std::pair<int, int> example_result = evaluate(example_jbs, 10, 3);
  assert(example_result.first == 40);
  assert(example_result.second == 25272);

  example_jbs = read_file("input_test.txt");
  assert(example_jbs[0].position[0] == 162);
  assert(example_jbs[0].position[1] == 817);
  assert(example_jbs[0].position[2] == 812);
  assert(example_jbs.back().position[0] == 425);
  assert(example_jbs.back().position[1] == 690);
  assert(example_jbs.back().position[2] == 689);

  example_result = process_file("input_test.txt", 10, 3);
  assert(example_result.first == 40);
  assert(example_result.second == 25272);

  std::cout << "\tAOC test completed successfully." << std::endl;

  std::cout << "Running AOC puzzle ..." << std::endl;
  std::pair<int, int> puzzle_result = process_file("input.txt", 1000, 3);
  std::cout << "\tResult 1/2: " << puzzle_result.first << std::endl;
  std::cout << "\tResult 2/2: " << puzzle_result.second << std::endl;

  return 0;
}