#include "exercises.h"

unsigned long s_recursive(unsigned int n, unsigned int k) {
    if(k == 1 || k==n)
        return 1;

    return s_recursive(n-1, k-1) + k*s_recursive(n-1, k);
}

unsigned long b_recursive(unsigned int n) {
    unsigned long res = 0;
    for(int k = 1; k <= n; k++)
        res += s_recursive(n, k);
    return res;
}

unsigned long s_dynamic(unsigned int n, unsigned int k) {
    unsigned long** solutions = new unsigned long*[n+1];
    for(int i = 0; i <= n; ++i)
        solutions[i] = new unsigned long[k+1];
    for (int i = 1; i <= n; ++i) {
        solutions[i][1] = 1;
        solutions[i][i] = 1;
    }

    for (int i = 3; i <= n; ++i) {
        for (int j = 2; j <= k; ++j) {
            if (i == j) continue;
            solutions[i][j] = solutions[i-1][j-1] + j*solutions[i-1][j];
        }
    }
    return solutions[n][k];
}

unsigned long b_dynamic(unsigned int n) {
    unsigned long sum = 0;
    for(int i = 1; i <=n; i++)
        sum += s_dynamic(n, i);
    return sum;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex5, testPartitioningRecursive) {
    EXPECT_EQ(1,s_recursive(3,3));
    EXPECT_EQ(3025,s_recursive(9,3));
    EXPECT_EQ(22827,s_recursive(10,6));

    EXPECT_EQ(5,b_recursive(3));
    EXPECT_EQ(203,b_recursive(6));
    EXPECT_EQ(1382958545,b_recursive(15));
}

TEST(TP4_Ex5, testPartitioningDynamic) {
    EXPECT_EQ(1,s_dynamic(3,3));
    EXPECT_EQ(3025,s_dynamic(9,3));
    EXPECT_EQ(22827,s_dynamic(10,6));

    EXPECT_EQ(5,b_dynamic(3));
    EXPECT_EQ(203,b_dynamic(6));
    EXPECT_EQ(1382958545,b_dynamic(15));
}