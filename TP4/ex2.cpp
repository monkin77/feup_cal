#include "exercises.h"
#include <vector>

using namespace std;

bool changeMakingUnlimitedDP(unsigned int C[], unsigned int n, unsigned int T, unsigned int usedCoins[]) {
    for(int i = 0; i < n; i++) usedCoins[i] = 0;

    if(T == 0)
        return true;

    unsigned int minCoins[T+1];   // array with minCoins for each value of k
    unsigned int lastCoin[T+1];   // array with lastCoin idx for each value of k

    for(int i = 0; i <= n; i++){     // iterating number of coins used
        for(int k = 0; k <= T; k++){    // iterating target value
            if(k == 0){
                minCoins[k] = 0;
                lastCoin[k] = -1;
            }
            else if(i == 0 && k > 0){
                minCoins[k] = -1;
                lastCoin[k] = -1;
            }
            else if( (C[i-1] <= k) && ( minCoins[k-C[i-1]] != -1 ) && ( (minCoins[k] == -1) || (1+minCoins[k-C[i-1]] <  minCoins[k]) ) ){     // i-1 is the current coin index in the array since for i=0 represents 0 coin
                minCoins[k] = 1 + minCoins[k - C[i-1]];
                lastCoin[k] = i-1;
            }
            else{
                // minCoins and lastCoin remains the same
            }
        }
    }

    if(lastCoin[T] == -1)
        return false;

	int auxT = T;
    while(auxT > 0){
        int coinUsedIdx = lastCoin[auxT];
        usedCoins[coinUsedIdx]++;
        auxT -= C[coinUsedIdx];
    }

    return true;
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex2, hasChangeCanonical) {
    unsigned int C[] = {1,2,5,10};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,13,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,38,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 1);
    EXPECT_EQ(usedCoins[3], 3);

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,10,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);
}

TEST(TP4_Ex2, hasChangeNonCanonical) {
    unsigned int C[] = {1,4,5};
    unsigned int n = 3;
    unsigned int usedCoins[3];

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,6,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,7,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 2);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);
}

TEST(TP4_Ex2, hasNoChange) {
    unsigned int C[] = {5,10};
    unsigned int n = 2;
    unsigned int usedCoins[2];

    EXPECT_EQ(changeMakingUnlimitedDP(C,n,1,usedCoins), false);
}