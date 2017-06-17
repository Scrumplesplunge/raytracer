#include "random.h"

#include <mutex>

static uint32_t GenerateSeed() {
  static random_generator generator;
  static std::mutex mutex;
  std::unique_lock<std::mutex> lock(mutex);
  std::uniform_int_distribution<uint32_t> random_seed;
  return random_seed(generator);
}

random_generator& RandomGenerator() {
  thread_local random_generator generator{GenerateSeed()};
  return generator;
}
