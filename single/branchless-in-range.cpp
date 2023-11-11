#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>

int64_t clock_realtime() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

void isleep(unsigned long millisecond) {
  std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
}

#define TIMES 10000
#define N 10000

uint32_t min = 3333, max = 6666;
uint32_t pos[N];

void benchmark1() {
  isleep(1000);
  int64_t ts = clock_realtime();
  int s = 0;
  for (int k = 0; k < TIMES; k++) {
    for (int i = 0; i < N; i++) {
      uint32_t j = pos[i];
      if (j >= min && j <= max) {
        s += j;
      } else {
        s -= j;
      }
    }
  }
  ts = clock_realtime() - ts;
  printf("bench 1: time=%d s=%d\n", (int)ts, s);
}

void benchmark2() {
  isleep(1000);
  int64_t ts = clock_realtime();
  int s = 0;
  for (int k = 0; k < TIMES; k++) {
    for (int i = 0; i < N; i++) {
      uint32_t j = pos[i];
      if ((int32_t)((j - min) | (max - j)) >= 0) {
        s += j;
      } else {
        s -= j;
      }
    }
  }
  ts = clock_realtime() - ts;
  printf("bench 2: time=%d s=%d\n", (int)ts, s);
}

int main(void) {
  for (int i = 0; i < N; i++) {
    pos[i] = rand() % 10000;
  }

  benchmark1();
  benchmark2();
  return 0;
}
