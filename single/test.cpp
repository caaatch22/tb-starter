// #define CATCH_CONFIG_MAIN
// #include "catch2/catch.hpp"
#include <bits/stdc++.h>
#include <date/date.h>
#include <ranges>
#include "../utils/include/random.hpp"
#include "../utils/include/utils.hpp"
#include <string_view>

using namespace std;

int main() {
  auto const dates = tbs::rng_dates(10, "20200101"sv, "20200530"sv);
  for (auto const& s : dates) {
    cout << s << '\n';
  }
}