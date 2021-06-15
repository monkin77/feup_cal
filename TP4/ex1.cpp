#include "exercises.h"
#include <unordered_map>

using namespace std;

unsigned long factorialRecurs(unsigned long n) {
    if(n == 0 || n == 1)
        return 1;
    int res = n*factorialRecurs(n-1);
    return res;
}

unsigned long factorialDP(unsigned long n) {
    int res = 1;
    for(int i = 1; i <= n; i++){
        res *= i;
    }
    return res;
}

/*
 * The time complexity of both methods is at first O(n), but in the case of Dynamic Linking, after the values are
 * registered, it can become better until O(1).
 * In terms of space complexity, the Dynamic linking alternative has a complexity of O(n) while the recursive
 * has O(1).
 */

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex1, FactorialTest) {
    EXPECT_EQ(1,factorialRecurs(0));
    EXPECT_EQ(1,factorialRecurs(1));
    EXPECT_EQ(2,factorialRecurs(2));
    EXPECT_EQ(6,factorialRecurs(3));
    EXPECT_EQ(24,factorialRecurs(4));
    EXPECT_EQ(120,factorialRecurs(5));
    EXPECT_EQ(3628800,factorialRecurs(10));

    EXPECT_EQ(1,factorialDP(0));
    EXPECT_EQ(1,factorialDP(1));
    EXPECT_EQ(2,factorialDP(2));
    EXPECT_EQ(6,factorialDP(3));
    EXPECT_EQ(24,factorialDP(4));
    EXPECT_EQ(120,factorialDP(5));
    EXPECT_EQ(3628800,factorialDP(10));
}