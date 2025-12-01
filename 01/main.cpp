#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

class Dial {
public:
  int value;
  const int start_value;
  const int period;
  int zero_counter;
  int zero_counter_all;

  Dial(const int start_value, const int period)
      : start_value(start_value), value(start_value), period(period),
        zero_counter(0), zero_counter_all(0){};

  void operator+(int n) {
    value = value + n;
    zero_counter_all += value / period;
    if (value == 0) {
      --zero_counter_all;
    }
    value = value % period;
    if (value == 0) {
      zero_counter++;
    };
  }

  void operator-(int n) {
    zero_counter_all += abs((period - value + n) / period);
    if (value == 0) {
      --zero_counter_all;
    }
    value = (value - n) % period;
    if (value < 0) {
      value = period + value;
    }
    if (value == 0) {
      zero_counter++;
    };
  }

  void reset() {
    value = start_value;
    zero_counter = 0;
    zero_counter_all = 0;
  }
};

void process_file(const std::string file_name, Dial &dial) {
  std::ifstream ifs(file_name);
  std::string line;
  int n;
  while (std::getline(ifs, line)) {
    n = std::stoi(line.substr(1));
    if (line[0] == 'R') {
      dial + n;
    } else {
      dial - n;
    }
  }
}

int main(int argc, char *argv[]) {
  std::cout << "Running unit tests ..." << std::endl;
  Dial dial(50, 100);
  assert(dial.value == 50);
  assert(dial.zero_counter == 0);
  assert(dial.zero_counter_all == 0);
  dial + 49;
  assert(dial.value == 99);
  assert(dial.zero_counter == 0);
  assert(dial.zero_counter_all == 0);

  dial + 1;
  assert(dial.value == 0);
  assert(dial.zero_counter == 1);
  assert(dial.zero_counter_all == 1);

  dial + 10;
  dial - 40;
  assert(dial.value == 70);
  assert(dial.zero_counter == 1);
  assert(dial.zero_counter_all == 2);

  dial - 20;
  assert(dial.value == 50);
  assert(dial.zero_counter == 1);
  assert(dial.zero_counter_all == 2);

  dial + 333;
  assert(dial.value == 83);
  assert(dial.zero_counter == 1);
  assert(dial.zero_counter_all == 5);

  dial - 93;
  assert(dial.value == 90);
  assert(dial.zero_counter == 1);
  assert(dial.zero_counter_all == 6);

  dial - 444;
  assert(dial.value == 46);
  assert(dial.zero_counter == 1);
  assert(dial.start_value == 50);
  assert(dial.zero_counter_all == 10);

  dial.reset();
  assert(dial.value == 50);
  assert(dial.zero_counter == 0);
  assert(dial.zero_counter_all == 0);

  dial - 50;
  dial - 2;
  assert(dial.value == 98);
  assert(dial.zero_counter == 1);
  assert(dial.zero_counter_all == 1);

  dial + 2;
  dial + 111;
  assert(dial.value == 11);
  assert(dial.zero_counter == 2);
  assert(dial.zero_counter_all == 3);

  std::cout << "\tUnits tests completed successfully." << std::endl;

  std::cout << "Running AOC test ..." << std::endl;
  dial.reset();
  process_file("input_test.txt", dial);

  assert(dial.zero_counter == 3);
  assert(dial.value == 32);
  assert(dial.zero_counter_all == 6);

  std::cout << "\tAOC test completed successfully." << std::endl;

  std::cout << "Running AOC puzzle ..." << std::endl;
  dial.reset();
  process_file("input.txt", dial);
  std::cout << "\tResult 1/2: " << dial.zero_counter << std::endl;
  std::cout << "\tResult 2/2: " << dial.zero_counter_all << std::endl;
}