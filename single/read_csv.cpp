#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <range/v3/all.hpp>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "timer.hpp"

using dataset_sequence_type = std::vector<std::vector<std::string>>;
auto read_csv(std::filesystem::path file)
    -> std::optional<dataset_sequence_type> {
  std::ifstream data_file(file);
  if (!std::filesystem::exists(file) || !data_file.is_open()) {
    return {};
  }

  dataset_sequence_type result;
  for (std::string line; std::getline(data_file, line);) {
    auto tokens = line                                        //
                  | std::views::split(',')                    //
                  | std::views::transform([](auto&& token) {  //
                      return std::string_view(&*token.begin(),
                                              std::ranges::distance(token));  //
                    });
    result.push_back(
        dataset_sequence_type::value_type(tokens.begin(), tokens.end()));
  }
  return result;
}

auto read_csv_v3(std::filesystem::path file)
    -> std::optional<dataset_sequence_type> {
  std::ifstream data_file(file);
  if (!data_file.is_open()) {
    return {};
  }
  using namespace ranges;
  auto const result =
      getlines(data_file)  // ["aa,a" "b,bb" "cc,c"]
      | views::transform([](auto&& s) { return s | views::split(','); })  //
      | to<dataset_sequence_type>;
  return result;
}

int main() {
  std::filesystem::path chip("../test/test_data/chip_dataset.csv");
  tbs::Timer timer;
  auto result = read_csv_v3(chip);
  fmt::print("{} s\n", timer);
  std::cout << result.value().size() << '\n';
  timer.reset();

  result = read_csv(chip);
  fmt::print("{} s\n", timer);
  std::cout << result.value().size() << '\n';
  return 0;
}