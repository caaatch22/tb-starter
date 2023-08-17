#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

TEST_CASE("INFO output test") {
  INFO("This is an info message.");
  REQUIRE(true);
}
