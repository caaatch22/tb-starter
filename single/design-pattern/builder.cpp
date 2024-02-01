#include <fmt/format.h>

#include <algorithm>
#include <string>
#include <vector>

class HTMLElement {
 public:
  HTMLElement(const std::string& name, const std::string& text)
      : name_(name), text_(text) {}

  std::string str() const {
    return fmt::format("<{}>{}</{}>", name_, text_, name_);
  }

 private:
  friend class HTMLBuilder;
  std::string name_;
  std::string text_;
  std::vector<HTMLElement> children_;
};

class HTMLBuilder {
 public:
  explicit HTMLBuilder(const std::string& name, const std::string& text = "")
      : root_(name, text) {}

  HTMLBuilder& add_child(const std::string& name, const std::string& text) {
    root_.children_.emplace_back(name, text);
    return *this;
  }

  std::string str() const { return root_.str(); }
  operator HTMLElement() const { return root_; }

 private:
  HTMLElement root_;
};

int main() {
  HTMLElement el =
      HTMLBuilder("ul").add_child("li", "hello").add_child("li", "world");
  fmt::print("{}\n", el.str());
  return 0;
}
