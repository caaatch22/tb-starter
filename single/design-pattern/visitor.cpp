// visitor pattern
// what ?
// 在不改变数据结构的前提下，增加新的操作方式
// why ?
// 1. 共有的一些api不可以改变
// 2. example:
// 序列化是一个常见的visitor pattern的应用
// class serialization 包括了一些序列化的操作，如，序列化到disk, socket ...
// 需要序列化的类，知道如何序列化自己的数据成员
// 需要序列化的类就可以提供一个accept方法，接受一个serialization的对象
#include <fmt/format.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

class Cat;
class Dog;

class PetVisitor {
 public:
  virtual void visit(Cat *c) = 0;
  virtual void visit(Dog *d) = 0;
};

class FeedVisitor : public PetVisitor {
 public:
  void visit(Cat *c) override { std::cout << "feed cat\n"; }
  void visit(Dog *d) override { std::cout << "feed dog\n"; }
};

class Pet {
 public:
  Pet(const std::string &color) : color_(color_) {}
  const std::string &color() const { return color_; }
  virtual void accept(PetVisitor &v) = 0;

 private:
  std::string color_;
};

class Dog : public Pet {
  void accept(PetVisitor &v) override { v.visit(this); }
};

class Cat : public Pet {
  void accept(PetVisitor &v) override { v.visit(this); }
};

/// @brief visitor with variant
template <typename... Ts>
struct Overload : Ts... {
  using Ts::operator()...;
};

template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;

auto TypeOfIntegral = Overload{
    [](char) { return "char"; },
    [](int) { return "int"; },
    [](unsigned int) { return "unsigned int"; },
    [](long int) { return "long int"; },
    [](long long int) { return "long long int"; },
    [](auto) { return "unknown type"; },
};

int main() {
  FeedVisitor fv;
  // ... other visitors
  Pet *p;
  p->accept(fv);

  std::vector<std::variant<char, long, float, int, double, long long>>
      vecVariant = {5, '2', 5.4, 100ll, 2011l, 3.5f, 2017};

  for (auto v : vecVariant) {
    fmt::println("{}", std::visit(TypeOfIntegral, v));
  }
}