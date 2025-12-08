#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

int perform_step(std::vector<int> &state, const std::vector<int> transform) {
  int n_splits = 0;
  for (size_t i = 0; i < transform.size(); ++i) {
    if (transform[i] == 1) {
      if (state[i] == 1) {
        if (i > 0) {
          state[i - 1] = 1;
        }
        state[i] = 0;
        if (i < state.size() - 1) {
          state[i + 1] = 1;
        }
        ++n_splits;
      }
    }
  }
  return n_splits;
}

std::vector<std::vector<int>> read_file(const std::string input_file_name) {
  std::ifstream file(input_file_name);
  std::string line;
  std::vector<std::vector<int>> result;
  getline(file, line);
  result.push_back(std::vector<int>(line.size(), 0));
  for (size_t i = 0; i < line.size(); ++i) {
    if (line[i] == 'S') {
      result[0][i] = 1;
    }
  }
  while (getline(file, line)) {
    result.push_back(std::vector<int>(line.size(), 0));
    for (size_t i = 0; i < line.size(); ++i) {
      if (line[i] == '^') {
        result.back()[i] = 1;
      }
    }
  }

  return result;
}

int propagate(
  std::vector<std::vector<int>> input
){
  int n_splits = 0;
  std::vector<int> state = input[0];
  for(size_t i = 1; i < input.size(); ++i){
    n_splits += perform_step(state, input[i]);
  }
  return n_splits;
}

int process_file(const std::string input_file_name){
  std::vector<std::vector<int>> input = read_file(input_file_name);
  return propagate(input);
}

int main(int argc, char *argv[]) {
  std::cout << "Running unit tests ..." << std::endl;

  std::vector<int> state{0, 0, 1, 0, 0};
  perform_step(state, {0, 0, 0, 0, 0});
  std::vector<int> state_after_step_1{0, 0, 1, 0, 0};
  assert(state == state_after_step_1);
  std::vector<int> state_after_step_2{0, 1, 0, 1, 0};
  perform_step(state, {0, 0, 1, 0, 0});
  assert(state == state_after_step_2);
  perform_step(state, {1, 0, 0, 0, 0});
  assert(state == state_after_step_2);
  perform_step(state, {1, 0, 1, 0, 0});
  assert(state == state_after_step_2);
  std::vector<int> state_after_step_3{1, 0, 1, 1, 0};
  perform_step(state, {0, 1, 0, 0, 0});
  assert(state == state_after_step_3);

  std::cout << "\tUnits tests completed successfully." << std::endl;

  std::cout << "Running AOC test ..." << std::endl;

  std::vector<std::vector<int>> input = read_file("input_test.txt");
  std::vector<int> input_first_line{0, 0, 0, 0, 0, 0, 0, 1,
                                    0, 0, 0, 0, 0, 0, 0};
    std::vector<int> input_seventh_line{0, 0, 0, 0, 0, 1, 0, 1,
                                    0, 1, 0, 0, 0, 0, 0};
  assert(input[0] == input_first_line);
  assert(input[6] == input_seventh_line);

  assert(process_file("input_test.txt") == 21);

  std::cout << "\tAOC test completed successfully." << std::endl;

  std::cout << "Running AOC puzzle ..." << std::endl;
  std::cout << "\tResult 1/2: " << process_file("input.txt") << std::endl;

  return 0;
}