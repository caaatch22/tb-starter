#include <iostream>
#include <chrono>
#include "../tbs/include/timer.hpp"

using tbs::Timer;

class BaseVirtual {
public:
 virtual int getValue() = 0;
};

template <typename Derived>
class CRTPBase {
public:
    int getValue() {
        return static_cast<Derived*>(this)->getValue();
    }
};

class DerivedVirtual : public BaseVirtual {
public:
    int getValue() override {
        return 42;
    }
};

class DerivedCRTP : public CRTPBase<DerivedCRTP> {
public:
    int getValue() {
        return 42;
    }
};

int main() {
    const int iterations = 1e8;
    Timer<tbs::nano> timer;
    // Benchmark using virtual function
    BaseVirtual* virtualObj = new DerivedVirtual();
    timer.reset();
    for (int i = 0; i < iterations; ++i) {
      int value = virtualObj->getValue();
    }
    std::cout << "Virtual function time: " << timer << '\n';
    delete virtualObj;

    // Benchmark using CRTP
    DerivedCRTP* crtpObj = new DerivedCRTP();
    auto crtpStart = std::chrono::high_resolution_clock::now();
    timer.reset();
    for (int i = 0; i < iterations; ++i) {
      int value = crtpObj->getValue();
    }
    std::cout << "CRTP time: " << timer << "\n";
    auto crtpEnd = std::chrono::high_resolution_clock::now();
    delete crtpObj;

    return 0;
}
