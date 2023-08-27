#include <iostream>
#include <ranges>
#include <vector>
#include <memory>
#include <string>
#include "Data.h"
#include "../../tbs/include/proxy.h"

struct BarT {
  double high;
  double low;
  double open;
  double close;
  int64_t amount;
};

using BarTPtr = std::unique_ptr<BarT>;

struct BarSeries {
  std::vector<BarTPtr> bars;
  uint16_t offset;
};

struct KLineDs {
  void OnInit();
  void OnBeforeTrade();
  void OnTick(TickRO tick);
  void OnAfterTrading();
  void OnMarketClose();
};
