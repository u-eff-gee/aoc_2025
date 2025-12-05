#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

bool ingredient_is_fresh(
    const long long id,
    std::vector<std::pair<long long, long long>> id_ranges) {
  for (auto id_range : id_ranges) {
    if (id >= id_range.first && id <= id_range.second) {
      return true;
    }
  }
  return false;
}

int count_fresh_ingredients(
    std::vector<long long> ids,
    std::vector<std::pair<long long, long long>> id_ranges) {
  int count = 0;
  for (auto id : ids) {
    if (ingredient_is_fresh(id, id_ranges)) {
      ++count;
    }
  }
  return count;
}

std::pair<std::vector<long long>, std::vector<std::pair<long long, long long>>>
read_ids_and_id_ranges(const std::string input_file_name) {
  std::ifstream file(input_file_name);
  std::string line;
  std::vector<long long> ids;
  std::vector<std::pair<long long, long long>> id_ranges;

  size_t separator;

  while (getline(file, line)) {
    if (line == "") {
      continue;
    }
    separator = line.find("-");
    if (separator == std::string::npos) {
      ids.push_back(std::stoll(line));
    } else {
      id_ranges.push_back({std::stoll(line.substr(0, separator)),
                           std::stoll(line.substr(separator + 1))});
    }
  }

  return {ids, id_ranges};
}

std::vector<std::pair<long long, long long>>
unique_ranges(std::vector<std::pair<long long, long long>> id_ranges) {
  std::vector<std::pair<long long, long long>> unique_ranges;
  std::sort(id_ranges.begin(), id_ranges.end());
  long long current_lower_limit = id_ranges[0].first;
  long long current_upper_limit = id_ranges[0].second;
  for (size_t i = 1; i < id_ranges.size(); ++i) {
    if (id_ranges[i].first <= current_upper_limit) {
      if (id_ranges[i].second > current_upper_limit) {
        current_upper_limit = id_ranges[i].second;
      }
    } else {
      unique_ranges.push_back({current_lower_limit, current_upper_limit});
      current_lower_limit = id_ranges[i].first;
      current_upper_limit = id_ranges[i].second;
    }
  }
  if (current_lower_limit > unique_ranges.back().second) {
    unique_ranges.push_back({current_lower_limit, current_upper_limit});
  } else if (id_ranges.back().first > unique_ranges.back().second) {
    unique_ranges.push_back(id_ranges.back());
  }
  return unique_ranges;
}

long long
length_of_id_ranges(std::vector<std::pair<long long, long long>> id_ranges) {
  long long length = 0;
  for (auto id_range : id_ranges) {
    length += id_range.second - id_range.first + 1;
  }
  return length;
}

std::pair<int, long long> process_file(const std::string input_file_name) {
  std::pair<std::vector<long long>,
            std::vector<std::pair<long long, long long>>>
      ids_and_id_ranges = read_ids_and_id_ranges(input_file_name);
  return {count_fresh_ingredients(ids_and_id_ranges.first,
                                  ids_and_id_ranges.second),
          length_of_id_ranges(unique_ranges(ids_and_id_ranges.second))};
}

int main(int argc, char *argv[]) {
  std::cout << "Running unit tests ..." << std::endl;

  assert(!ingredient_is_fresh(5, {{1, 3}}));
  assert(ingredient_is_fresh(5, {{5, 5}}));
  assert(ingredient_is_fresh(5, {{1, 3}, {5, 7}}));
  assert(ingredient_is_fresh(5, {{1, 3}, {2, 5}}));

  assert(count_fresh_ingredients({1, 2, 3, 4, 5}, {{1, 3}, {2, 5}}) == 5);
  assert(count_fresh_ingredients({1, 2, 3, 4, 5}, {{1, 3}, {5, 5}}) == 4);

  std::vector<std::pair<long long, long long>> ur{{1, 5}, {15, 20}};
  assert(unique_ranges({{15, 20}, {1, 5}}) == ur);
  assert(unique_ranges({{1, 5}, {15, 20}}) == ur);
  assert(unique_ranges({{1, 5}, {1, 5}, {15, 20}}) == ur);
  assert(unique_ranges({{1, 2}, {2, 4}, {5, 5}, {15, 20}}) != ur);

  assert(length_of_id_ranges({{1, 5}, {15, 20}}) == 11);
  assert(length_of_id_ranges(unique_ranges({{1, 5}, {1, 5}, {15, 20}})) == 11);
  assert(length_of_id_ranges(
             unique_ranges({{1, 2}, {2, 4}, {5, 5}, {15, 20}})) == 11);

  std::cout << "\tUnits tests completed successfully." << std::endl;

  std::cout << "Running AOC test ..." << std::endl;

  std::vector<long long> example_ids = {1, 5, 8, 11, 17, 32};
  std::vector<std::pair<long long, long long>> example_id_ranges = {
      {3, 5}, {10, 14}, {16, 20}, {12, 18}};
  assert(count_fresh_ingredients(example_ids, example_id_ranges) == 3);
  std::vector<std::pair<long long, long long>> example_id_ranges_unique = {
      {3, 5}, {10, 20}};
  assert(unique_ranges(example_id_ranges) == example_id_ranges_unique);
  assert(length_of_id_ranges(unique_ranges(example_id_ranges)) == 14);

  std::pair<std::vector<long long>,
            std::vector<std::pair<long long, long long>>>
      ids_and_id_ranges = read_ids_and_id_ranges("input_test.txt");
  assert(ids_and_id_ranges.first == example_ids);
  assert(ids_and_id_ranges.second == example_id_ranges);

  std::pair<int, long long> example_result = process_file("input_test.txt");
  assert(example_result.first == 3);
  assert(example_result.second == 14);

  std::cout << "\tAOC test completed successfully." << std::endl;

  std::cout << "Running AOC puzzle ..." << std::endl;
  std::pair<int, long long> result = process_file("input.txt");
  std::cout << "\tResult 1/2: " << result.first << std::endl;
  std::cout << "\tResult 2/2: " << result.second << std::endl;

  return 0;
}