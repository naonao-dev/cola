﻿/**
 * @FilePath     : /cola/src/UtilsCtrl/Random/URandomV2.h
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-06-22 23:29:29
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-06-22 23:35:21
 **/
#ifndef NAO_URANDOMV2_H
#define NAO_URANDOMV2_H

#include <cstdint>

#include "../UtilsObject.h"

NAO_NAMESPACE_BEGIN

class URandomV2 : public UtilsObject
{
private:
    uint32_t seed_;

public:
    explicit URandomV2(uint32_t s)
        : seed_(s & 0x7fffffffu)
    {
        // Avoid bad seeds.
        if (seed_ == 0 || seed_ == 2147483647L) {
            seed_ = 1;
        }
    }
    uint32_t Next()
    {
        static const uint32_t M = 2147483647L;   // 2^31-1
        static const uint64_t A = 16807;         // bits 14, 8, 7, 5, 2, 1, 0
        // We are computing
        //       seed_ = (seed_ * A) % M,    where M = 2^31-1
        //
        // seed_ must not be zero or M, or else all subsequent computed values
        // will be zero or M respectively.  For all other values, seed_ will end
        // up cycling through every number in [1,M-1]
        uint64_t product = seed_ * A;

        // Compute (product % M) using the fact that ((x << 31) % M) == x.
        seed_ = static_cast<uint32_t>((product >> 31) + (product & M));
        // The first reduction may overflow by 1 bit, so we may need to
        // repeat.  mod == M is not possible; using > allows the faster
        // sign-bit-based test.
        if (seed_ > M) {
            seed_ -= M;
        }
        return seed_;
    }
    // Returns a uniformly distributed value in the range [0..n-1]
    // REQUIRES: n > 0
    uint32_t Uniform(int n) { return Next() % n; }

    // Randomly returns true ~"1/n" of the time, and false otherwise.
    // REQUIRES: n > 0
    bool OneIn(int n) { return (Next() % n) == 0; }

    // Skewed: pick "base" uniformly from range [0,max_log] and then
    // return "base" random bits.  The effect is to pick a number in the
    // range [0,2^max_log-1] with exponential bias towards smaller numbers.
    uint32_t Skewed(int max_log) { return Uniform(1 << Uniform(max_log + 1)); }
};
NAO_NAMESPACE_END
#endif
