// By: Gonçalo Leão

#include "exercises.h"
#include <vector>
#include <climits>

int calculate_no_coins(unsigned int usedCoins[], int n){
    int ans = 0;
    for(int i = 0; i < n; i++){
        ans += usedCoins[i];
    }
    return ans;
}

bool arrIsEmpty(unsigned int Stock[], int n){
    for(int i = 0; i < n; i++){
        if(Stock[i] > 0)
            return true;
    }
    return false;
}

void copyArr(unsigned int arr1[], unsigned int arr2[], int n){
    for(int i = 0; i < n; i++){
        arr2[i] = arr1[i];
    }
}

bool changeMakingBF(unsigned int C[], unsigned int Stock[], unsigned int n, unsigned int T, unsigned int usedCoins[]) {
/*
    if(T < 0){
        return -1;
    }

    if(T == 0){
        return 0;
    }

    int minimum = INT_MAX;
    for(int i = 0; i < n; i++){
        unsigned int* minUsedCoins = new unsigned int[n];
        unsigned int* minStock = new unsigned int[n];
        copyArr(usedCoins, minUsedCoins, n);
        copyArr(Stock, minStock, n);
        if(Stock[i] > 0){
            unsigned int coin = C[i];
            minStock[i] -= 1;
            minUsedCoins[i] += 1;
            int changeResult = changeMakingBF(C, minStock, n, T-coin, minUsedCoins);

            if(changeResult >= 0 && changeResult < minimum){
                minimum =  1 + changeResult;
                copyArr(minUsedCoins, usedCoins, n);
                copyArr(minStock, Stock, n);
            }
        }
    }

    if(minimum == INT_MAX)
        return -1;
    else
        return minimum;
*/
    for (int i = 0; i < n; ++i) usedCoins[i] = 0;
    if (n == 1) {
        int sum = 0;
        while (sum < T && Stock[0] > 0){
            sum += C[0];
            usedCoins[0]++;
            Stock[0]--;
        }
        return sum == T;
    }
    int bestNum = -1;
    unsigned int* bestCoins = new unsigned int[n];
    for (int i = 0; i <= Stock[n-1]; ++i) {
        if (i*C[n-1] > T) break;
        usedCoins[n-1] = i;
        unsigned int* tempStock = new unsigned int[n];
        unsigned int* tempUsed = new unsigned int[n];
        for (int l = 0; l < n; ++l) tempStock[l] = Stock[l];
        for (int l = 0; l < n; ++l) tempUsed[l] = usedCoins[l];
        if (changeMakingBF(C, Stock, n - 1, T - i*C[n-1], usedCoins)) {
            int sum = 0;
            for (int j = 0; j < n; ++j) sum += usedCoins[j];
            if (sum < bestNum || bestNum == -1) {
                bestNum = sum;
                for (int l = 0; l < n; ++l) bestCoins[l] = usedCoins[l];
            }
        }
        for (int l = 0; l < n; ++l) Stock[l] = tempStock[l];
        for (int l = 0; l < n; ++l) usedCoins[l] = tempUsed[l];
        delete tempUsed;
        delete tempStock;
    }
    if (bestNum != -1) {
        for (int l = 0; l < n; ++l) usedCoins[l] = bestCoins[l];
        delete bestCoins;
        return true;
    }
    return false;


}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex3, hasBFChangeCanonical) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {1,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingBF(C,Stock,n,13,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);

    unsigned int Stock2[] = {1,2,4,2};
    EXPECT_EQ(changeMakingBF(C,Stock2,n,38,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 3);
    EXPECT_EQ(usedCoins[3], 2);
}

TEST(TP1_Ex3, hasBFChangeNonCanonical) {
    unsigned int C[] = {1,4,5};
    unsigned int Stock[] = {2,2,1};
    unsigned int n = 3;
    unsigned int usedCoins[3];

    EXPECT_EQ(changeMakingBF(C,Stock,n,6,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingBF(C,Stock,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);
}

TEST(TP1_Ex3, hasNoBFChange) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {0,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingBF(C,Stock,n,18,usedCoins), false);
    EXPECT_EQ(changeMakingBF(C,Stock,n,1,usedCoins), false);
}