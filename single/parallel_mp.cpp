#define CATCH_CONFIG_MAIN

#include <parallel_hashmap/phmap.h>
#include <algorithm>
#include <catch2/catch.hpp>
#include <iostream>
#include <memory>
#include <numeric>
#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../utils/include/random.hpp"
#include "../utils/include/timer.hpp"

using namespace std;
using bench::rng;
using bench::Timer;
using std::cout;

template <typename T>
using UP = std::unique_ptr<T>;
template <typename T>
using SP = std::shared_lock<T>;
template <typename T>
using RO = T const*;
template <typename T>
using RW = T*;

template <typename K, typename V>
using fmp = phmap::flat_hash_map<K, V>;

struct Data {
  string symbol{"000001.SZ"};
  int64_t eamcode{13000000000020020};
  Data() = default;
  Data(string smb, int64_t code) : symbol(std::move(smb)), eamcode(code) {}
};

struct NonCopyAndNonMove {
  string symbol{"000001.SZ"};
  int64_t eamcode{13000000000020020};
  NonCopyAndNonMove() = default;

  NonCopyAndNonMove(NonCopyAndNonMove const&) = delete;
  NonCopyAndNonMove(NonCopyAndNonMove&&) = delete;
  NonCopyAndNonMove& operator=(NonCopyAndNonMove const&) = delete;
  NonCopyAndNonMove& operator=(NonCopyAndNonMove&&) = delete;
};

TEST_CASE("benchmark", "[inserting 1000000 integer]") {
  cout << "ph-hashtable and std-hashtable benchmark, inserting 1000000 "
          "integer:\n";

  Timer timer;

  std::unordered_map<int, int> stdmp;
  fmp<int, int> mp;
  constexpr int n = 1e6;
  auto const keys = bench::distinct_vec<int, 0>(n);

  timer.reset();
  for (auto const k : keys) {
    mp[k] = rng();
  }
  cout << "ph-hashtable: " << timer << '\n';

  timer.reset();
  for (auto const k : keys) {
    stdmp[k] = rng();
  }
  cout << "std-hahtable: " << timer << '\n';
  cout << '\n';
}

TEST_CASE("benchmark of phmap with key type string") {
  Timer timer;

  std::unordered_map<string, UP<Data>> stdmp;
  fmp<string, UP<Data>> mp;
  constexpr int n = 1e6;
  auto const keys = bench::rng_dates(n, "20220120", "20900530");
  auto values = vector<RW<Data>>(n);
  for (int i = 0; i < n; i++) {
    values[i] = new Data;
  };

  timer.reset();
  for (int i = 0; i < n; i++) {
    mp[keys[i]] = unique_ptr<Data>{values[i]};
  }
  cout << "ph-hashtable: " << timer << '\n';

  for (int i = 0; i < n; i++) {
    REQUIRE(values[i] == mp[keys[i]].get());
  }
}

TEST_CASE("validate in rehash of phmap") {
  Timer timer;

  fmp<int, UP<Data>> mp;

  constexpr int n = 1e6;
  auto keys = bench::distinct_vec<int, 1>(n);
  vector<RW<Data>> values(n, nullptr);
  for (int i = 0; i < n; i++) {
    values[i] = new Data;
  }

  for (int i = 0; i < n; i++) {
    mp[keys[i]] = unique_ptr<Data>{values[i]};
  }

  for (int i = 0; i < n; i++) {
    REQUIRE(values[i] == mp[keys[i]].get());
  }

  timer.reset();
  int64_t dummy = 0;
  for (int i = 0; i < n; i++) {
    dummy += mp[keys[i]]->eamcode;
  }
  cout << "phmap iterate 1000000 integers: " << timer << '\n';
}

// int main() {
//   Timer timer;

//   std::unordered_map<int, UP<int>> stdmp;
//   fmp<int, UP<int>> mp;

//   constexpr int n = 1e6;
//   auto keys = bench::rng_vec<int, 1, n>(n);
//   vector<RW<int>> values(n, nullptr);

//   for (int i = 0; i < n; i++) {
//     values[i] = new int(rng());
//   }
//   timer.reset();

//   for (int i = 0; i < n; i++) {
//     auto const& k = keys[i];
//     mp[k] = unique_ptr<int>{values[i]};
//     // mp[k] = make_unique<int>(rng());
//   }
//   cout << timer << '\n';
//   timer.reset();

//   timer.reset();
//   for (auto const& k : keys) {
//     stdmp[k] = make_unique<int>(rng());
//   }
//   cout << timer << '\n';
//   timer.reset();

//   return 0;
// }