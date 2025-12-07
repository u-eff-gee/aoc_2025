#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

struct InputFile {
  int n_columns;
  int n_rows;
  int n_terms;
  std::string input_file_name;
  std::vector<std::vector<char>> raw_input;
  std::vector<int> operators;
  std::vector<std::pair<int, int>> columns;

  InputFile(const std::string input_file_name)
      : input_file_name(input_file_name) {}

  void analyze() {
    n_rows = 1;
    std::ifstream file(input_file_name);
    std::string line;
    getline(file, line);
    n_columns = line.size();
    while (getline(file, line)) {
      n_rows++;
    }
  }

  void initialize() {
    for (int i = 0; i < n_rows; ++i) {
      raw_input.push_back(std::vector<char>(n_columns, ' '));
    }
  }

  void read() {
    initialize();
    std::ifstream file(input_file_name);
    std::string line;
    for (int n_row = 0; n_row < n_rows; ++n_row) {
      getline(file, line);
      for (int n_col = 0; n_col < n_columns; ++n_col) {
        raw_input[n_row][n_col] = line[n_col];
      }
    }
  }

  void parse_operators() {
    for (int n_col = 0; n_col < n_columns; ++n_col) {
      if (raw_input.back()[n_col] == '+') {
        operators.push_back(0);
      }
      if (raw_input.back()[n_col] == '*') {
        operators.push_back(1);
      }
    }
    n_terms = operators.size();
  };

  void find_table_columns() {
    bool is_column;
    columns.push_back(std::pair<int, int>{0, 0});
    for (int n_col = 0; n_col < n_columns - 1; ++n_col) {
      is_column = true;
      for (int n_row = 0; n_row < n_rows - 1; ++n_row) {
        if (raw_input[n_row][n_col] != ' ') {
          is_column = false;
          break;
        }
      }
      if (is_column) {
        columns.back().second = n_col - 1;
        columns.push_back(std::pair<int, int>{n_col + 1, n_columns - 1});
      }
    }
  }

  std::vector<long long> parse_column_lr(int n_column) {
    std::vector<long long> result(n_rows - 1, 0ll);
    std::stringstream number;
    for (int n_row = 0; n_row < n_rows - 1; ++n_row) {
      number.str("");
      for (int n_col = columns[n_column].first;
           n_col <= columns[n_column].second; ++n_col) {
        if (raw_input[n_row][n_col] != ' ') {
          number << raw_input[n_row][n_col];
        }
      }
      result[n_row] = std::stoll(number.str());
    }
    return result;
  }

  std::vector<long long> parse_column_tb(int n_column) {
    std::vector<long long> result(
        columns[n_column].second - columns[n_column].first + 1, 0ll);
    std::stringstream number;
    for (int n_col = columns[n_column].first; n_col <= columns[n_column].second;
         ++n_col) {
      number.str("");
      for (int n_row = 0; n_row < n_rows - 1; ++n_row) {
        if (raw_input[n_row][n_col] != ' ') {
          number << raw_input[n_row][n_col];
        }
      }
      result[n_col - columns[n_column].first] = std::stoll(number.str());
    }
    return result;
  }

  long long calculate_result(bool lr) {
    long long result = 0ll;
    std::vector<long long> numbers;
    for (int n_term = 0; n_term < n_terms; ++n_term) {
      if (lr) {
        numbers = parse_column_lr(n_term);
      } else {
        numbers = parse_column_tb(n_term);
      }
      if (operators[n_term] == 0) {
        result += std::accumulate(numbers.begin(), numbers.end(), 0ll);
      } else {
        result += std::accumulate(numbers.begin(), numbers.end(), 1ll,
                                  std::multiplies<long long>());
      }
    }
    return result;
  }
};

std::pair<long long, long long>
process_file(const std::string input_file_name) {
  InputFile input_file(input_file_name);
  input_file.analyze();
  input_file.read();
  input_file.parse_operators();
  input_file.find_table_columns();
  return {input_file.calculate_result(true),
          input_file.calculate_result(false)};
}

int main(int argc, char *argv[]) {
  std::cout << "Running unit tests ..." << std::endl;

  InputFile input_file("input_test.txt");
  input_file.analyze();
  assert(input_file.n_columns == 15);
  assert(input_file.n_rows == 4);

  input_file.read();
  std::vector<char> input_file_first_line{'1', '2', '3', ' ', '3',
                                          '2', '8', ' ', ' ', '5',
                                          '1', ' ', '6', '4', ' '};
  std::vector<char> input_file_operator_line{'*', ' ', ' ', ' ', '+',
                                             ' ', ' ', ' ', '*', ' ',
                                             ' ', ' ', '+', ' ', ' '};
  assert(input_file.raw_input[0] == input_file_first_line);
  assert(input_file.raw_input.back() == input_file_operator_line);

  std::vector<int> example_operators = {1, 0, 1, 0};
  input_file.parse_operators();
  assert(input_file.n_terms == 4);
  assert(input_file.operators == example_operators);

  std::vector<std::pair<int, int>> example_columns{
      {0, 2}, {4, 6}, {8, 10}, {12, 14}};
  input_file.find_table_columns();
  assert(input_file.columns == example_columns);

  std::vector<long long> first_problem_numbers = {123, 45, 6};
  std::vector<long long> first_problem_numbers_2 = {1, 24, 356};

  assert(input_file.parse_column_lr(0) == first_problem_numbers);
  assert(input_file.parse_column_tb(0) == first_problem_numbers_2);

  std::cout << "\tUnits tests completed successfully." << std::endl;

  std::cout << "Running AOC test ..." << std::endl;

  assert(input_file.calculate_result(true) == 4277556);
  assert(input_file.calculate_result(false) == 3263827);

  std::pair<long long, long long> example_result =
      process_file("input_test.txt");
  assert(example_result.first == 4277556);
  assert(example_result.second == 3263827);

  std::cout << "\tAOC test completed successfully." << std::endl;

  std::cout << "Running AOC puzzle ..." << std::endl;
  std::pair<long long, long long> puzzle_result = process_file("input.txt");
  std::cout << "\tResult 1/2: " << puzzle_result.first << std::endl;
  std::cout << "\tResult 1/2: " << puzzle_result.second << std::endl;

  return 0;
}