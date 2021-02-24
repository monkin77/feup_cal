// By: Gonçalo Leão

#include "exercises.h"

unsigned int sumArray(unsigned int a[], unsigned int n) {
    unsigned int sum = 0;
    for(unsigned int i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum;
}

#include <iostream>

bool isCanonical(unsigned int C[], unsigned int n) {
    for(int x = C[2] + 2; x < C[n-2]+C[n-1]; x++){
        unsigned int Stock[n];
        for(int i = 0; i < n; i++){
            Stock[i] = INT_MAX;
        }
        unsigned int usedCoinsByGreedy[n], usedCoinsByBrute[n];
        if(changeMakingGreedy(C, Stock, n, x, usedCoinsByGreedy) && changeMakingBF(C, Stock, n, x, usedCoinsByBrute)){
            for(int i = 0; i < n; i++){
                if(usedCoinsByBrute[i] != usedCoinsByGreedy[i]){
                    return false;
                }
            }
        }
    }
    return true;
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex5, canonicalTrue) {
    unsigned int C[] = {1,2,5};
    EXPECT_EQ(isCanonical(C,3), true);

    unsigned int C2[] = {1,2,5,10};
    EXPECT_EQ(isCanonical(C2,4), true);
}

TEST(TP1_Ex5, canonicalFalse) {
    unsigned int C[] = {1,4,5};
    EXPECT_EQ(isCanonical(C,3), false);
}