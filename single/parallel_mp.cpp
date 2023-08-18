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
#include "../tbs/include/random.hpp"
#include "../tbs/include/timer.hpp"
#include "../tbs/include/utils.hpp"

using namespace std;
using std::cout;
using tbs::rng;
using tbs::Timer;

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

using Data = tbs::MockData;

TEST_CASE("benchmark", "[inserting 1000000 integer]") {
  cout << "ph-hashtable and std-hashtable benchmark, inserting 1000000 "
          "integer:\n";

  Timer timer;

  std::unordered_map<int, int> stdmp;
  fmp<int, int> mp;
  constexpr int n = 1e6;
  auto const keys = tbs::distinct_vec<int, 0>(n);

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
  cout << "ph-hashtable and std-hashtable benchmark, inserting 1000000 "
          "string(sso):\n";
  Timer timer;

  std::unordered_map<string, UP<Data>> stdmp;
  fmp<string, UP<Data>> mp;
  constexpr int n = 1e6;
  auto const keys = tbs::rng_dates(n, "20220120"sv, "90900530"sv);

  auto values1 = vector<RW<Data>>(n);
  for (int i = 0; i < n; i++) {
    values1[i] = new Data;
  };

  auto values2 = vector<RW<Data>>(n);
  for (int i = 0; i < n; i++) {
    values2[i] = new Data;
  };

  timer.reset();
  for (int i = 0; i < n; i++) {
    mp[keys[i]] = unique_ptr<Data>{values1[i]};
  }
  cout << "ph-hashtable: " << timer << '\n';

  timer.reset();
  for (int i = 0; i < n; i++) {
    stdmp[keys[i]] = unique_ptr<Data>{values2[i]};
  }
  cout << "std-hashtable: " << timer << '\n';
}

TEST_CASE("validate in rehash of phmap") {
  Timer timer;

  fmp<int, UP<Data>> mp;

  constexpr int n = 1e6;
  auto keys = tbs::distinct_vec<int, 1>(n);
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
