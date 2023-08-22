#define CATCH_CONFIG_MAIN

#include <parallel_hashmap/phmap.h>
#include <BS_thread_pool.hpp>
#include <algorithm>
#include <catch2/catch.hpp>
#include <iostream>
#include <memory>
#include <numeric>
#include <ranges>
#include <set>
#include <string>
#include <thread>
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

template <typename K, typename V>
using pfmp = phmap::parallel_flat_hash_map<K, V>;

template <typename K, typename V>
using nodemp = phmap::node_hash_map<K, V>;

using Data = tbs::MockData;
using tbs::BigMockData;

TEST_CASE("benchmark integer", "[inserting 1000000 integers]") {
  cout << "ph-hashtable and std-hashtable benchmark" << '\n';
  cout << "inserting 1000000 integers:\n";

  Timer timer;

  std::unordered_map<int, int> stdmp;
  nodemp<int, int> nodemp;
  fmp<int, int> mp;

  constexpr int n = 1e6;
  auto const keys = tbs::distinct_vec<int, 0>(n);

  timer.reset();
  for (auto const k : keys) {
    mp[k] = rng();
  }
  cout << "flat-hashtable: " << timer << '\n';

  timer.reset();
  for (auto const k : keys) {
    stdmp[k] = rng();
  }
  cout << "std-hahtable: " << timer << '\n';

  timer.reset();
  for (auto const k : keys) {
    nodemp[k] = rng();
  }
  cout << "node-hahtable: " << timer << '\n';

  cout << "iterate over 1000000 integers:" << '\n';
  timer.reset();
  int sum = 0;
  for (auto const& [_, v] : mp) {
    sum += v;
  }
  cout << "flat-hashtable time: " << timer << ", sum = " << sum << '\n';

  timer.reset();
  for (auto const& [_, v] : stdmp) {
    sum += v;
  }
  cout << "std-hashtable time: " << timer << ", sum = " << sum << '\n';

  timer.reset();
  for (auto const& [_, v] : nodemp) {
    sum += v;
  }
  cout << "node-hashtable time: " << timer << ", sum = " << sum << '\n';
  cout << '\n';
}

TEST_CASE("flat map reserve", "[inserting 1000000 integers]") {
  cout << "inserting 1000000 integers with reserve benchmark" << '\n';

  Timer timer;

  std::unordered_map<int, int> stdmp;
  fmp<int, int> mp;
  nodemp<int, int> nodemp;

  stdmp.reserve(2e6);
  mp.reserve(2e6);
  nodemp.reserve(2e6);

  constexpr int n = 1e6;
  auto const keys = tbs::distinct_vec<int, 0>(n);

  timer.reset();
  for (auto const k : keys) {
    mp[k] = rng();
  }
  cout << "flat-hashtable with reserve: " << timer << '\n';

  timer.reset();
  for (auto const k : keys) {
    stdmp[k] = rng();
  }
  cout << "std-hahtable with reserve: " << timer << '\n';

  timer.reset();
  for (auto const k : keys) {
    nodemp[k] = rng();
  }
  cout << "node-hashtable with reserve: " << timer << '\n';

  cout << "iterate over 1000000 integers:" << '\n';
  timer.reset();
  int sum = 0;
  for (auto const& [_, v] : mp) {
    sum += v;
  }
  cout << "flat-hashtable time: " << timer << ", sum = " << sum << '\n';

  timer.reset();
  for (auto const& [_, v] : stdmp) {
    sum += v;
  }
  cout << "std-hashtable time: " << timer << ", sum = " << sum << '\n';

  timer.reset();
  for (auto const& [_, v] : nodemp) {
    sum += v;
  }
  cout << "node-hashtable time: " << timer << ", sum = " << sum << '\n';
  cout << '\n';
}

TEST_CASE("benchmark string", "[inserting 1000000 strings]") {
  cout << "ph-hashtable and std-hashtable benchmark" << '\n';
  cout << "inserting 1000000 string(sso):\n";

  Timer timer;

  std::unordered_map<string, UP<Data>> stdmp;
  fmp<string, UP<Data>> mp;
  nodemp<string, UP<Data>> nodemp;

  constexpr int n = 1e6;
  auto const keys = tbs::rng_dates(n, "10220120"sv, "90900530"sv);

  auto values1 = vector<RW<Data>>(n);
  for (int i = 0; i < n; i++) {
    values1[i] = new Data;
  };

  auto values2 = vector<RW<Data>>(n);
  for (int i = 0; i < n; i++) {
    values2[i] = new Data;
  };

  auto values3 = vector<RW<Data>>(n);
  for (int i = 0; i < n; i++) {
    values3[i] = new Data;
  };

  timer.reset();
  for (int i = 0; i < n; i++) {
    mp[keys[i]] = unique_ptr<Data>{values1[i]};
  }
  cout << "flat-hashtable: " << timer << '\n';

  timer.reset();
  for (int i = 0; i < n; i++) {
    stdmp[keys[i]] = unique_ptr<Data>{values2[i]};
  }
  cout << "std-hashtable: " << timer << '\n';

  timer.reset();
  for (int i = 0; i < n; i++) {
    nodemp[keys[i]] = unique_ptr<Data>{values3[i]};
  }
  cout << "node-hashtable: " << timer << '\n';

  cout << "iterate over 1000000 strings:" << '\n';
  timer.reset();
  Data sum;
  for (auto const& [_, v] : mp) {
    sum.eamcode += v->eamcode;
  }
  cout << "flat-hashtable time: " << timer << ", sum = " << sum.eamcode << '\n';

  timer.reset();
  for (auto const& [_, v] : stdmp) {
    sum.eamcode += v->eamcode;
  }
  cout << "std-hashtable time: " << timer << ", sum = " << sum.eamcode << '\n';

  timer.reset();
  for (auto const& [_, v] : nodemp) {
    sum.eamcode += v->eamcode;
  }
  cout << "node-hashtable time: " << timer << ", sum = " << sum.eamcode << '\n';
  cout << '\n';
}

TEST_CASE("benchmark with string as key and value") {
  cout << "inserting 1000000 strings as key and value:\n";
  Timer timer;

  std::unordered_map<string, string> stdmp;
  fmp<string, string> mp;
  nodemp<string, string> nodemp;

  const int n = 1e6;

  auto const keys = tbs::rng_strings(n, 128);
  // auto const keys = tbs::rng_dates(n, "10010321", "90901023");
  auto const values = tbs::rng_strings(n, 128);

  timer.reset();
  for (int i = 0; i < n; i++) {
    mp[keys[i]] = values[i];
  }
  cout << "flat-hashtable: " << timer << '\n';
  timer.reset();
  for (int i = 0; i < n; i++) {
    stdmp[keys[i]] = values[i];
  }
  cout << "std-hashtable: " << timer << '\n';

  timer.reset();
  for (int i = 0; i < n; i++) {
    nodemp[keys[i]] = values[i];
  }
  cout << "node-hashtable: " << timer << '\n';
  cout << '\n';
}

TEST_CASE("benchmark with big value") {
  cout << "inserting 1000000 integers as key and big data as value:\n";

  Timer timer;

  std::unordered_map<int64_t, BigMockData> stdmp;
  fmp<int64_t, BigMockData> mp;
  nodemp<int64_t, BigMockData> nodemp;

  const int n = 1e6;
  auto const keys = tbs::distinct_vec<int, 0>(n);

  timer.reset();
  for (auto const k : keys) {
    mp[k] = BigMockData();
  }
  cout << "flat-hashtable: " << timer << '\n';

  timer.reset();
  for (auto const k : keys) {
    stdmp[k] = BigMockData();
  }
  cout << "std-hashtable: " << timer << '\n';

  timer.reset();
  for (auto const k : keys) {
    nodemp[k] = BigMockData();
  }
  cout << "node-hashtable: " << timer << '\n';

  cout << '\n';
}

TEST_CASE("usage and benchmark for parallel version of flat_hash_map") {
  Timer timer;
  cout << "inserting 1000000 integers in one thread:\n";
  fmp<int, int> mp;
  pfmp<int, int> pmp;

  const int n = 1e6;
  auto const keys = tbs::distinct_vec<int, 0>(n);

  timer.reset();
  for (auto const k : keys) {
    mp[k] = rng();
  }
  cout << "flat-hashtable: " << timer << '\n';

  timer.reset();
  for (auto const k : keys) {
    pmp[k] = rng();
  }
  cout << "parallel-flat-hashtable: " << timer << '\n';
  cout << '\n';
}

TEST_CASE("parallel version of parallel-hash-map") {
  cout << "inserting 1000000 integers without lock in 8 threads" << '\n';
  Timer timer;
  pfmp<int, int> pmp;
  fmp<int, int> mp;

  const int n = 1e6;
  auto const keys = tbs::distinct_vec<int, 0>(n);

  constexpr int64_t num_threads = 8;
  auto thread_fn = [](pfmp<int, int>& pmp, vector<int> const& keys,
                      int thread_idx) {
    size_t modulo = pmp.subcnt() / 8;
    for (int64_t i = 0; i < keys.size(); ++i) {
      int key = keys[i];
      size_t hashval = pmp.hash(key);
      size_t idx = pmp.subidx(hashval);
      if (idx / modulo ==
          thread_idx)  // if the submap is suitable for this thread
      {
        pmp[key] = rng();  // insert the value
        // ++(num_keys[thread_idx]);  // increment count of inserted values
      }
    }
  };

  std::unique_ptr<std::thread> threads[num_threads];
  timer.reset();
  for (int i = 0; i < num_threads; i++) {
    threads[i].reset(
        new std::thread(thread_fn, std::ref(pmp), std::cref(keys), i));
  }
  timer.reset();
  for (int64_t i = 0; i < num_threads; ++i)
    threads[i]->join();
  cout << "parallel-flat-map: " << timer << '\n';

  timer.reset();
  for (auto const& key : keys) {
    mp[key] = rng();
  }
  std::cout << "inserting with std::mutex: " << timer << '\n';
  cout << '\n';
}

// TEST_CASE("parallel-hash-map") {
//   cout << "inserting 1000000 integers with locks in 8 threads" << '\n';
//   Timer timer;

//   fmp<int, int> fmp;
//   std::mutex mut;

//   phmap::parallel_flat_hash_map<
//       int, int, phmap::priv::hash_default_hash<int>,
//       phmap::priv::hash_default_eq<int>,
//       phmap::priv::Allocator<std::pair<const int, int>>, 4, std::mutex>
//       mp;

//   phmap::parallel_flat_hash_map<
//       int, int, phmap::priv::hash_default_hash<int>,
//       phmap::priv::hash_default_eq<int>,
//       phmap::priv::Allocator<std::pair<int const, int>>, 4,
//       std::shared_mutex> smp;

//   constexpr int n = 1e6;
//   auto const keys = tbs::distinct_vec<int, 0>(n);

//   BS::thread_pool pool(8);

//   timer.reset();
//   pool.push_loop(keys.size(), [&](size_t l, size_t r) {
//     for (size_t i = l; i < r; i++) {
//       mp[keys[i]] = rng();
//     }
//   });
//   pool.wait_for_tasks();
//   cout << "parallel_flat_map with std::mutex: " << timer << '\n';

//   timer.reset();
//   pool.push_loop(keys.size(), [&](size_t l, size_t r) {
//     for (size_t i = l; i < r; i++) {
//       smp[keys[i]] = rng();
//     }
//   });
//   pool.wait_for_tasks();
//   cout << "parallel_flat_map with std::shared_mutex: " << timer << '\n';

//   timer.reset();
//   pool.push_loop(keys.size(), [&](size_t l, size_t r) {
//     for (size_t i = l; i < r; i++) {
//       std::lock_guard lock(mut);
//       fmp[keys[i]] = rng();
//     }
//   });
//   pool.wait_for_tasks();
//   cout << "flat_map with std::mutex by hand: " << timer << '\n';

//   fmp.clear();
//   mp.clear();
//   smp.clear();

//   auto const rkeys = tbs::rng_vec<int, 0, n>(n);
//   auto s = std::set<int>(rkeys.begin(), rkeys.end());
//   cout << "all size = " << rkeys.size() << " distinct key size = " <<
//   s.size()
//        << '\n';
//   timer.reset();
//   pool.push_loop(keys.size(), [&](size_t l, size_t r) {
//     for (size_t i = l; i < r; i++) {
//       mp[keys[i]] = rng();
//     }
//   });
//   pool.wait_for_tasks();
//   cout << "parallel_flat_map with std::mutex: " << timer << '\n';

//   timer.reset();
//   pool.push_loop(keys.size(), [&](size_t l, size_t r) {
//     for (size_t i = l; i < r; i++) {
//       smp[keys[i]] = rng();
//     }
//   });
//   pool.wait_for_tasks();
//   cout << "parallel_flat_map with std::shared_mutex: " << timer << '\n';

//   timer.reset();
//   pool.push_loop(keys.size(), [&](size_t l, size_t r) {
//     for (size_t i = l; i < r; i++) {
//       std::lock_guard lock(mut);
//       fmp[keys[i]] = rng();
//     }
//   });
//   pool.wait_for_tasks();
//   cout << "flat_map with std::mutex by hand: " << timer << '\n';
// }

// TEST_CASE("validate in rehash of phmap") {
//   Timer timer;

//   fmp<int, UP<Data>> mp;

//   constexpr int n = 1e6;
//   auto keys = tbs::distinct_vec<int, 1>(n);
//   vector<RW<Data>> values(n, nullptr);
//   for (int i = 0; i < n; i++) {
//     values[i] = new Data;
//   }

//   for (int i = 0; i < n; i++) {
//     mp[keys[i]] = unique_ptr<Data>{values[i]};
//   }

//   for (int i = 0; i < n; i++) {
//     REQUIRE(values[i] == mp[keys[i]].get());
//   }

//   timer.reset();
//   int64_t dummy = 0;
//   for (int i = 0; i < n; i++) {
//     dummy += mp[keys[i]]->eamcode;
//   }
//   // cout << "phmap iterate 1000000 integers: " << timer << '\n';
// }
