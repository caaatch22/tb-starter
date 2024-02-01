#ifndef TBS_OMEAG_EXCEPTION_HPP
#define TBS_OMEAG_EXCEPTION_HPP

#include <boost/stacktrace.hpp>
#include <iostream>
#include <source_location>
#include <string>

namespace tbs {

namespace st = boost::stacktrace;

template <typename T>
class OmegaException {
 public:
  OmegaException(
      std::string str, T data,
      const std::source_location &loc = std::source_location::current(),
      st::stacktrace trace = st::stacktrace())
      : err_str_{std::move(str)},
        user_data_{std::move(data)},
        location_{loc},
        backtrace_{trace} {}

  std::string &what() { return err_str_; }
  const std::string &what() const noexcept { return err_str_; }
  const std::source_location &where() const noexcept { return location_; }
  const st::stacktrace &stack() const noexcept { return backtrace_; }
  T &data() { return user_data_; }
  const T &data() const noexcept { return user_data_; }

 private:
  std::string err_str_;
  T user_data_;
  const std::source_location location_;
  const st::stacktrace backtrace_;
};

}  // namespace tbs

namespace st = boost::stacktrace;
std::ostream &operator<<(std::ostream &os,
                         const std::source_location &location) {
  os << location.file_name() << "(" << location.line() << ":"
     << location.column() << "), function `" << location.function_name() << "`";
  return os;
}

std::ostream &operator<<(std::ostream &os, const st::stacktrace &backtrace) {
  for (auto iter = backtrace.begin(); iter != (backtrace.end() - 1); ++iter) {
    os << iter->source_file() << "(" << iter->source_line()
       << ") : " << iter->name() << "\n";
  }
  return os;
}

#endif  // TBS_OMEAG_EXCEPTION_HPP