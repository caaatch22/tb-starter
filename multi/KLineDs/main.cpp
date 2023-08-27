#include <iostream>
#include <vector>
#include <memory>
#include "Data.h"
#include "../../tbs/include/proxy.h"

struct OnTick : pro::dispatch<void(TickRO)> {
  template <class T>
  void operator()(T const& self, TickRO tick) {
    self.OnTock(tick);
  }
};


struct OnTick : pro::dispatch<void(TickRO)> {
  template <class T>
  void operator()(T const& self, TickRO tick) {
    self.OnTock(tick);
  }
};



