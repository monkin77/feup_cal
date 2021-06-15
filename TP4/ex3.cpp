#include "exercises.h"
#include <vector>

using namespace std;

/*
 * !!! Tentar iterar a partir do target + alto. Como está agora, ao ver a coin de valor 10, depois de tirar 10 2x
como já nao há stock dessa coin para de ser possível.
 * */

bool changeMakingDP(unsigned int C[], unsigned int Stock[], unsigned int n, unsigned int T, unsigned int usedCoins[]) {
    for(int i = 0; i < n; i++) usedCoins[i] = 0;

    if(T == 0)
        return true;

    unsigned int minCoins[T+1];   // array with minCoins for each value of k
    unsigned int lastCoin[T+1];   // array with lastCoin idx for each value of k
    vector<vector<unsigned int>> stockForEachTarget(T+1, vector<unsigned int>(n));

    for(int i = 0; i <= T; i++){    // set all stock values to equal initial
        for(int j = 0; j < n; j++)
            stockForEachTarget[i][j] = Stock[j];
    }

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
                vector<unsigned int> specStock(n);
                specStock = stockForEachTarget[k-C[i-1]];   // gets the stock of target - coinValue

                if(specStock[i-1] > 0){
                    //specStock = stockForEachTarget[k - C[i-1]];
                    specStock[i-1]--;
                    stockForEachTarget[k] = specStock;  // updates with new stock for k
                    minCoins[k] = 1 + minCoins[k - C[i-1]];
                    lastCoin[k] = i-1;
                }
                else{
                    for(int a = i-2; a >= 1; a--){
                        if( ( minCoins[k-C[a]] != -1 ) && ( (minCoins[k] == -1) || (1+minCoins[k-C[a]] <  minCoins[k]))){
                            specStock = stockForEachTarget[k - C[a]];
                            if(specStock[a] > 0){
                                specStock[a]--;
                                stockForEachTarget[k] = specStock;
                                minCoins[k] = 1 + minCoins[k-C[a]];
                                lastCoin[k] = a;
                                break;
                            }
                        }
                    }
                }
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

TEST(TP4_Ex3, hasChangeCanonical) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {1,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingDP(C,Stock,n,13,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);

    unsigned int Stock2[] = {1,2,4,2};
    EXPECT_EQ(changeMakingDP(C,Stock2,n,38,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 3);
    EXPECT_EQ(usedCoins[3], 2);

    unsigned int Stock3[] = {10,2,4,2};
    EXPECT_EQ(changeMakingDP(C,Stock3,n,10,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);
}

TEST(TP4_Ex3, hasChangeNonCanonical) {
    unsigned int C[] = {1,4,5};
    unsigned int Stock[] = {2,2,1};
    unsigned int n = 3;
    unsigned int usedCoins[3];

    EXPECT_EQ(changeMakingDP(C,Stock,n,6,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingDP(C,Stock,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);

    unsigned int Stock2[] = {8,2,1};
    EXPECT_EQ(changeMakingDP(C,Stock2,n,7,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 2);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingDP(C,Stock2,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);
}

TEST(TP4_Ex3, hasNoChange) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {0,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingDP(C,Stock,n,18,usedCoins), false);
    EXPECT_EQ(changeMakingDP(C,Stock,n,1,usedCoins), false);
}