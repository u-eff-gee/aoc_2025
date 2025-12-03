#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

long long maximum_joltage_single_battery_n_digits(std::vector<int> battery, const int n){
  std::vector<int> digits(n);
  std::vector<int>::iterator current_digit = std::max_element(battery.begin(), battery.end()-n+1);
  digits[0] = *current_digit;
  for(int n_digit = 1; n_digit < n; ++n_digit){
    current_digit = std::max_element(current_digit+1, battery.end()-(n-n_digit-1));
    digits[n_digit] = *current_digit;
  }
  long long result = 0;
  for(int n_digit = 0; n_digit < n; ++n_digit){
    result += digits[n_digit]*std::pow(10, n-n_digit-1);
  }
  return result;
}

long long maximum_joltage_multiple_batteries(std::vector<std::vector<int>> batteries, const int n){
  long long result = 0;
  for(size_t n_battery = 0; n_battery < batteries.size(); ++n_battery){
    result += maximum_joltage_single_battery_n_digits(batteries[n_battery], n);
  }
  return result;
}

std::vector<int> parse_battery(const std::string battery_string){
  std::vector<int> battery(battery_string.size());
  for(size_t i = 0; i < battery_string.size(); ++i){
    battery[i] = std::stoi(battery_string.substr(i,1));
  }
  return battery;
}

std::vector<std::vector<int>> read_batteries(const std::string input_file_name){
  std::vector<std::vector<int>> batteries;
  std::ifstream file(input_file_name);
  std::string battery_string;
  std::vector<int> battery;
  while(getline(file, battery_string)){
    batteries.push_back(parse_battery(battery_string));
  };
  return batteries;
}

long long process_file(const std::string input_file_name, int n){
  return maximum_joltage_multiple_batteries(read_batteries(input_file_name), n);
}

int main(int argc, char *argv[]) {
  std::cout << "Running unit tests ..." << std::endl;

  assert(maximum_joltage_single_battery_n_digits({9,8,7,6,5,4,3,2,1,1,1,1,1,1,1}, 1) == 9);
  assert(maximum_joltage_single_battery_n_digits({9,8,7,6,5,4,3,2,1,1,1,1,1,1,1}, 2) == 98);
  assert(maximum_joltage_single_battery_n_digits({1,8,7,6,5,4,3,2,1,1,1,1,1,1,9}, 1) == 9);
  assert(maximum_joltage_single_battery_n_digits({9,8,7,6,5,4,3,2,1,1,1,1,1,1,1}, 3) == 987);
  assert(maximum_joltage_single_battery_n_digits({8,1,1,1,1,1,1,1,1,1,1,1,1,1,9}, 2) == 89);
  assert(maximum_joltage_single_battery_n_digits({2,3,4,2,3,4,2,3,4,2,3,4,2,7,8}, 2) == 78);
  assert(maximum_joltage_single_battery_n_digits({8,1,8,1,8,1,9,1,1,1,1,2,1,1,1}, 2) == 92);

  assert(maximum_joltage_multiple_batteries({
    {9,8,7,6,5,4,3,2,1,1,1,1,1,1,1},
    {8,1,1,1,1,1,1,1,1,1,1,1,1,1,9},
    {2,3,4,2,3,4,2,3,4,2,3,4,2,7,8},
    {8,1,8,1,8,1,9,1,1,1,1,2,1,1,1}
  }, 2) == 357);

  std::vector<int> battery = parse_battery("0123");
  for(int i = 0; i < 4; ++i){
    assert(battery[i] == i);
  }

  std::cout << "\tUnits tests completed successfully." << std::endl;

  std::cout << "Running AOC test ..." << std::endl;

  std::vector<std::vector<int>> batteries_ref = {
    {9,8,7,6,5,4,3,2,1,1,1,1,1,1,1},
    {8,1,1,1,1,1,1,1,1,1,1,1,1,1,9},
    {2,3,4,2,3,4,2,3,4,2,3,4,2,7,8},
    {8,1,8,1,8,1,9,1,1,1,1,2,1,1,1}
  };
  std::vector<std::vector<int>> batteries = read_batteries("input_test.txt");
  assert(batteries.size() == 4);
  assert(batteries.size() == 4);
  for(size_t n_battery = 0; n_battery < batteries_ref.size(); ++n_battery){
    assert(batteries[n_battery].size() == 15);
    for(size_t n_digit = 0; n_digit < batteries_ref[n_battery].size(); ++n_digit){
      assert(batteries[n_battery][n_digit] == batteries_ref[n_battery][n_digit]);
    }
  }
  assert(process_file("input_test.txt", 2) == 357);
  assert(process_file("input_test.txt", 12) == 3121910778619);

  std::cout << "\tAOC test completed successfully." << std::endl;

  std::cout << "Running AOC puzzle ..." << std::endl;
  std::cout << "\tResult 1/2: " << process_file("input.txt", 2) << std::endl;
  std::cout << "\tResult 2/2: " << process_file("input.txt", 12)  << std::endl;

  return 0;
}