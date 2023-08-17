#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include "../utils/include/random.hpp"
#include "../utils/include/timer.hpp"

using namespace std;
using bench::Timer;

struct Data {
  string symbol{"000001.SZ"};
  double asp{14.32};
  double bsp{16.12};
};

enum class DataType : int {
  UnKnown = -1,
  TickT = 0,
  ZbOrder = 1,
  ZbTrade = 2,
};

struct Test {
  DataType type;
  Data data;
  Test() = default;
  Test(DataType type_, const Data& data_) : type(type_), data(data_) {}
};

pair<double, vector<Test*>> up2ptr(const vector<unique_ptr<Test>>& src, int n) {
  vector<Test*> res(n);
  double time = 0;
  Timer timer;
  for (int i = 0; i < n; i++) {
    timer.reset();
    res[i] = src[i].get();
    time += timer.elapsed();
  }

  return {time, res};
}

pair<double, vector<Test*>> sp2ptr(const vector<shared_ptr<Test>>& src, int n) {
  vector<Test*> res(n);
  Timer timer;
  double time = 0;
  for (int i = 0; i < n; i++) {
    timer.reset();
    res[i] = src[i].get();
    time += timer.elapsed();
  };
  return {time, res};
}

pair<double, vector<unique_ptr<Test>>> ptr2up(const vector<Test*>& src, int n) {
  vector<unique_ptr<Test>> res;
  res.reserve(n);
  double time = 0;
  for (int i = 0; i < n; i++) {
    Timer timer;
    unique_ptr<Test> p(src[i]);
    time += timer.elapsed();
    res.emplace_back(std::move(p));
  }
  return {time, std::move(res)};
}

pair<double, vector<shared_ptr<Test>>> ptr2sp(const vector<Test*>& src, int n) {
  vector<shared_ptr<Test>> res;
  res.reserve(n);
  double time = 0;
  for (int i = 0; i < n; i++) {
    Timer timer;
    shared_ptr<Test> p(src[i]);
    time += timer.elapsed();
    res.emplace_back(std::move(p));
  }
  return {time, res};
}

pair<double, vector<shared_ptr<Test>>> ptr2sp2(const vector<Test*>& src,
                                               int n) {
  vector<shared_ptr<Test>> res(n);
  // res.reserve(n);
  double time = 0;
  for (int i = 0; i < n; i++) {
    Timer timer;
    res[i] = shared_ptr<Test>(src[i]);
    time += timer.elapsed();
    // res.emplace_back(std::move(p));
  }
  return {time, res};
}

pair<double, vector<Test const*>> sp2ptrconst(
    const vector<shared_ptr<Test>>& src,
    int n) {
  vector<Test const*> res(n);
  // res.reserve(n);
  double time = 0;
  for (int i = 0; i < n; i++) {
    Timer timer;
    res[i] = src[i].get();
    time += timer.elapsed();
    // res.emplace_back(std::move(p));
  }
  return {time, res};
}

pair<double, vector<Test const*>> sp2ptrconst2(
    const vector<shared_ptr<Test>>& src,
    int n) {
  vector<Test const*> res(n);
  // res.reserve(n);
  double time = 0;
  for (int i = 0; i < n; i++) {
    Timer timer;
    res[i] = const_cast<Test const*>(src[i].get());
    time += timer.elapsed();
    // res.emplace_back(std::move(p));
  }
  return {time, res};
}

pair<double, vector<unique_ptr<Test>>> ptrconst2up(
    const vector<Test const*>& src,
    int n) {
  vector<unique_ptr<Test>> res;
  res.reserve(n);
  double time = 0;
  for (int i = 0; i < n; i++) {
    Timer timer;
    unique_ptr<Test> p(const_cast<Test*>(src[i]));
    time += timer.elapsed();
    res.emplace_back(std::move(p));
  }
  return {time, std::move(res)};
}

pair<double, vector<shared_ptr<Test>>> ptrconst2sp(
    const vector<Test const*>& src,
    int n) {
  vector<shared_ptr<Test>> res;
  res.reserve(n);
  double time = 0;
  for (int i = 0; i < n; i++) {
    Timer timer;
    shared_ptr<Test> p(const_cast<Test*>(src[i]));
    time += timer.elapsed();
    res.emplace_back(std::move(p));
  }
  return {time, res};
}

void baseline(const vector<int64_t>& src, int n) {
  vector<int64_t> res(n);
  double time = 0;
  for (int i = 0; i < n; i++) {
    Timer timer;
    res[i] = src[i];
    time += timer.elapsed();
  }
  std::cout << "baseline time: " << time << " us" << '\n';
  // avoid optimization
  for (int i = 0; i < 10; i++) {
    cout << res[i];
  }
  cout << '\n';
}

int main() {
  const int n = 1e6;
  vector<Test*> test1(n);
  for (auto& p : test1) {
    p = new Test(DataType::TickT, Data());
  }

  vector<Test const*> test2(n);
  for (auto& p : test2) {
    p = new Test(DataType::ZbOrder, Data());
  }

  vector<unique_ptr<Test>> test3(n);
  for (auto& p : test3) {
    p = make_unique<Test>(DataType::ZbTrade, Data());
  }
  vector<shared_ptr<Test>> test4(n);
  for (auto& p : test4) {
    p = make_shared<Test>(DataType::TickT, Data());
  }

  auto [time1, res1] = up2ptr(test3, 1000000);
  std::cout << "up2ptr time = " << time1 << " us" << '\n';

  auto [time2, res2] = sp2ptr(test4, 1000000);
  std::cout << "sp2ptr time = " << time2 << " us" << '\n';

  auto [time3, res3] = ptr2up(test1, 1000000);
  std::cout << "ptr2up time = " << time3 << " us" << '\n';

  auto [time4, res4] = ptr2sp(test1, 1000000);
  std::cout << "ptr2sp time = " << time4 << " us" << '\n';

  auto [time5, res5] = ptr2sp2(test1, 1000000);
  std::cout << "ptr2sp2 time = " << time5 << " us" << '\n';

  auto [time6, res6] = ptrconst2up(test2, 1000000);
  std::cout << "ptrconst2up time = " << time6 << " us" << '\n';

  auto [time7, res7] = ptrconst2sp(test2, 1000000);
  std::cout << "ptrconst2ptr time = " << time7 << " us" << '\n';

  auto [time8, res8] = sp2ptrconst(test4, 1000000);
  std::cout << "sp2ptrconst time = " << time8 << " us" << '\n';

  auto [time9, res9] = sp2ptrconst2(test4, 1000000);
  std::cout << "sp2ptrconst2 time = " << time9 << " us" << '\n';

  vector<int64_t> a(n);
  iota(a.begin(), a.end(), 1);
  random_shuffle(a.begin(), a.end());
  baseline(a, 100000);

  cout << res4[0].get() << '\n';

  // for (int i = 0; i < 10; i ++) {
  //   cout << res1[i] << '\n';
  // }

  // Test* p = new Test();
  // cout << p << '\n';
  // unique_ptr<Test> up(p);

  // cout << up.get() << '\n';
  return 0;
}