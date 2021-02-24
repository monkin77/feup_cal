#include "exercises.h"
#include <iostream>
#include <stdio.h>

using namespace std;

void copyArray(unsigned int* v1, unsigned int* v2, int n){
    for(int i = 0; i < n; i++){
        v2[i] = v1[i];
    }
}

unsigned int calculateCoinsUsed(unsigned int used[], int n){
    unsigned int ans = 0;
    for(int i = 0; i < n; i++)
        ans += used[i];
    return ans;
}

void printUsedCoins(unsigned int usedCoins[], int n){
    for(int i = 0; i < n; i++){
        cout << "Coins of " << i << " used: " << usedCoins[i] << endl;
    }
    cout << "-------------" << endl;
}

void fillArrayWithZeros(unsigned int arr[], int n){
    for(int i = 0; i < n; i++)
        arr[i] = 0;
}

bool realChangeMakingBacktracking(unsigned int C[], unsigned int Stock[], unsigned int n, unsigned int T, unsigned int usedCoins[]){
    if(T == 0){
        return true;
    }

    if(T == INT16_MAX){     // code to tell that Target is less than 0 since is unsigned
        return false;
    }

    int minUsedCoins = INT_MAX;
    unsigned int bestUsedCoins[n];
    unsigned int bestStock[n];

    for(int i = 0; i < n; i++){
        if(Stock[i] > 0){
            unsigned int* auxStock = new unsigned int[n];
            unsigned int* auxUsedCoins = new unsigned int[n];
            copyArray(Stock, auxStock, n);
            copyArray(usedCoins, auxUsedCoins, n);
            (auxStock[i])--;
            (auxUsedCoins[i])++;
            unsigned int newTarget = T-C[i] >= 0 ? T-C[i] : INT16_MAX;
            bool specificSol = realChangeMakingBacktracking(C, auxStock, n, newTarget, auxUsedCoins);

            if(specificSol){
                unsigned int usedCoins = calculateCoinsUsed(auxUsedCoins, n);
                if(usedCoins < minUsedCoins){
                    minUsedCoins = usedCoins;
                    copyArray(auxUsedCoins, bestUsedCoins, n);
                    copyArray(auxStock, bestStock, n);
                }
            }

            delete auxStock;
            delete auxUsedCoins;
        }
    }

    if(minUsedCoins == INT_MAX)
        return false;

    copyArray(bestUsedCoins, usedCoins, n);
    copyArray(bestStock, Stock, n);
    return true;
}

bool changeMakingBacktracking(unsigned int C[], unsigned int Stock[], unsigned int n, unsigned int T, unsigned int usedCoins[]) {
    fillArrayWithZeros(usedCoins, n);
    bool res = realChangeMakingBacktracking(C, Stock, n, T, usedCoins);     // array is only 0's
    return res;
}



/// TESTS ///
#include <gtest/gtest.h>

TEST(TP2_Ex3, hasBFChangeCanonical) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {1,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingBacktracking(C,Stock,n,13,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);
    printUsedCoins(usedCoins, n);

    unsigned int Stock2[] = {1,2,4,2};
    EXPECT_EQ(changeMakingBacktracking(C,Stock2,n,38,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 1);
    EXPECT_EQ(usedCoins[2], 3);
    EXPECT_EQ(usedCoins[3], 2);
    printUsedCoins(usedCoins, n);

    unsigned int Stock3[] = {10,2,4,2};
    EXPECT_EQ(changeMakingBacktracking(C,Stock3,n,10,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 0);
    EXPECT_EQ(usedCoins[3], 1);
    printUsedCoins(usedCoins, n);
}

TEST(TP2_Ex3, hasBFChangeNonCanonical) {
    unsigned int C[] = {1,4,5};
    unsigned int Stock[] = {2,2,1};
    unsigned int n = 3;
    unsigned int usedCoins[3];

    EXPECT_EQ(changeMakingBacktracking(C,Stock,n,6,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 1);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingBacktracking(C,Stock,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);

    unsigned int Stock2[] = {8,2,1};
    EXPECT_EQ(changeMakingBacktracking(C,Stock2,n,7,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 2);
    EXPECT_EQ(usedCoins[1], 0);
    EXPECT_EQ(usedCoins[2], 1);

    EXPECT_EQ(changeMakingBacktracking(C,Stock2,n,8,usedCoins), true);
    EXPECT_EQ(usedCoins[0], 0);
    EXPECT_EQ(usedCoins[1], 2);
    EXPECT_EQ(usedCoins[2], 0);
}

TEST(TP2_Ex3, hasNoBFChange) {
    unsigned int C[] = {1,2,5,10};
    unsigned int Stock[] = {0,1,1,1};
    unsigned int n = 4;
    unsigned int usedCoins[4];

    EXPECT_EQ(changeMakingBacktracking(C,Stock,n,18,usedCoins), false);
    EXPECT_EQ(changeMakingBacktracking(C,Stock,n,1,usedCoins), false);
}