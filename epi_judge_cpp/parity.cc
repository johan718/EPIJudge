#include "test_framework/generic_test.h"
//#define LOOKUP    // lookup table variant
//#define BRUTE     // brute force solution
#define FOLD        // use the power of xor
//#define STATIC_VEC

#ifdef STATIC_VEC
// For reference, I'm keeping this code. The lambda function
// didn't work here because it kept being called. Maybe multiple instances
// of this program are being called??
static const std::vector<uint8_t> table1 = [] {
  std::vector<uint8_t> v(256);
  for (uint8_t i = 0; i < v.size(); ++i) {
    v[i] = 0;
    for (uint8_t j = 0; j < 8; ++j)
      v[i] ^= (i >> j) & 1;
    std::cout << short(i) << ' ' << short(v[i]) << std::endl;
  }
  std::cout << std::endl;
  return v;
}();

#endif // STATIC_VEC

short Parity(unsigned long long x) {
  // TODO - you fill in here.

  short result = 0;

#ifdef LOOKUP // lookup table
  static std::vector<int8_t> table(256, -1);

  uint8_t index = 0;

  // process all 8 bytes of x and build a lookup table to speed
  // up the application.
  for (uint8_t i = 0; i < 8; ++i)
  {
    index = uint8_t((x >> (i * 8)) & 0xff);
    if (table[index] == -1)
    {
      // process 8 bits of index
      table[index] = 0;
      for (uint8_t j = 0; j < 8; ++j)
        table[index] ^= (index >> j) & 0x01;
    }
    result ^= short(table[index]);
  }

#elif defined(BRUTE)  // brute force solution

  while (x)
  {
    result ^= (x & 1);
    x >>= 1;
  }

#elif defined(FOLD)   // find parity by xor'ing x onto itself

  x ^= x >> 32;
  x ^= x >> 16;
  x ^= x >> 8;
  x ^= x >> 4;
  x ^= x >> 2;
  x ^= x >> 1;
  result = x & 0x1;

#else

  while (x)
  {
    result ^= 1;
    x &= (x-1);   // drops the lowest set bit of x
  }

#endif

  return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "parity.cc", "parity.tsv", &Parity,
                         DefaultComparator{}, param_names);
}
