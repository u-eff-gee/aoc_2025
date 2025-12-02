#include <cassert>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

std::vector<std::pair<long long, long long>>
read_ranges(const std::string input_file_name) {
  std::string ranges_str;
  std::ifstream file(input_file_name);
  getline(file, ranges_str);

  size_t current_position = 0;
  size_t dist_to_next_range_delimiter = 0;
  size_t dist_to_next_start_stop_delimiter = 0;
  std::string range;
  std::vector<std::pair<long long, long long>> ranges;
  bool end_of_line_reached = false;
  do {
    dist_to_next_range_delimiter =
        ranges_str.substr(current_position).find(',');
    if (dist_to_next_range_delimiter == ranges_str.npos) {
      dist_to_next_range_delimiter = ranges_str.size() - current_position;
      end_of_line_reached = true;
    }
    dist_to_next_start_stop_delimiter =
        ranges_str.substr(current_position, dist_to_next_range_delimiter)
            .find("-");
    ranges.push_back(std::pair<long long, long long>(
        std::stol(ranges_str.substr(current_position,
                                    current_position +
                                        dist_to_next_start_stop_delimiter)),
        std::stol(ranges_str.substr(current_position +
                                        dist_to_next_start_stop_delimiter + 1,
                                    dist_to_next_range_delimiter))));
    current_position += dist_to_next_range_delimiter + 1;
  } while (!end_of_line_reached);
  return ranges;
}

bool contains_twice_repeated_sequence(const long long n) {
  std::string n_str = std::to_string(n);
  size_t n_digits = n_str.size();
  if (n_digits % 2) {
    return false;
  }
  for (size_t i = 0; i < n_digits / 2; ++i) {
    if (n_str[i] != n_str[n_digits / 2 + i]) {
      return false;
    }
  }
  return true;
}

bool contains_repeated_sequence_with_given_length(const std::string n_str,
                                                  const size_t n_digits,
                                                  const size_t period_length) {
  if(n_digits % period_length != 0){
    return false;
  }
  for (size_t n_period = 1; n_period < n_digits / period_length; ++n_period) {
    for (size_t i = 0; i < period_length; ++i) {
      if (n_str[i] != n_str[n_period * period_length + i]) {
        return false;
      }
    }
  }
  return true;
}

bool contains_repeated_sequence(const long long n) {
  std::string n_str = std::to_string(n);
  size_t n_digits = n_str.size();

  for (size_t period_length = 1; period_length <= n_digits / 2;
       ++period_length) {
        if(
          contains_repeated_sequence_with_given_length(n_str, n_digits, period_length)
        ){
          return true;
        }
  }
  return false;
}

std::pair<long long, long long> process_file(const std::string file_name) {
  std::vector<std::pair<long long, long long>> ranges = read_ranges(file_name);
  std::pair<long long, long long> result = {0, 0};
  for (size_t n_range = 0; n_range < ranges.size(); ++n_range) {
    for (long long n = ranges[n_range].first; n <= ranges[n_range].second;
         ++n) {
      if (contains_repeated_sequence(n)){
        if (contains_twice_repeated_sequence(n)) {
          result.first += n;
        }
        result.second += n;
      }
    }
  }
  return result;
};

int main(int argc, char *argv[]) {
  std::cout << "Running unit tests ..." << std::endl;
  std::vector<std::pair<long long, long long>> test_ranges = {
      {11, 22},
      {95, 115},
      {998, 1012},
      {1188511880, 1188511890},
      {222220, 222224},
      {1698522, 1698528},
      {446443, 446449},
      {38593856, 38593862},
      {565653, 565659},
      {824824821, 824824827},
      {2121212118, 2121212124}};
  std::vector<std::pair<long long, long long>> ranges =
      read_ranges("input_test.txt");
  for (size_t i = 0; i < test_ranges.size(); ++i) {
    assert(test_ranges[i].first == ranges[i].first);
    assert(test_ranges[i].second == ranges[i].second);
  }

  assert(contains_twice_repeated_sequence(11));
  assert(contains_twice_repeated_sequence(1010));
  assert(contains_twice_repeated_sequence(38593859));
  assert(!contains_twice_repeated_sequence(111));

  assert(contains_repeated_sequence(111));
  assert(!contains_repeated_sequence(121));
  assert(contains_repeated_sequence(121212));
  assert(!contains_repeated_sequence_with_given_length("123412341234", 12, 1));
  assert(!contains_repeated_sequence_with_given_length("123412341234", 12, 2));
  assert(!contains_repeated_sequence_with_given_length("123412341234", 12, 3));
  assert(contains_repeated_sequence_with_given_length("123412341234", 12, 4));
  assert(!contains_repeated_sequence_with_given_length("123412341234", 12, 5));
  assert(!contains_repeated_sequence_with_given_length("1231231", 7, 3));
  assert(!contains_repeated_sequence(1231231));

  std::cout << "\tUnits tests completed successfully." << std::endl;

  std::pair<long long, long long> result;
  std::cout << "Running AOC test ..." << std::endl;
  result = process_file("input_test.txt");
  assert(result.first == 1227775554);
  assert(result.second == 4174379265);
  std::cout << "\tAOC test completed successfully." << std::endl;

  std::cout << "Running AOC puzzle ..." << std::endl;
  result = process_file("input.txt");
  std::cout << "\tResult 1/2: " << result.first << std::endl;
  std::cout << "\tResult 2/2: " << result.second << std::endl;

  return 0;
}