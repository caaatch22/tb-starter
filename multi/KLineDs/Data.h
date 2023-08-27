#include <string>
#include <vector>
#include <memory>

struct TickT {
  int64_t epoch_exchg_time;
  std::vector<double> ask_px;
  std::vector<double> bid_px;
  std::string symbol;
  int64_t eamcode;
};

using TickTPtr = std::unique_ptr<TickT>;
using TickRO = TickT const*;
using TickRW = TickT*;