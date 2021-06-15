#include "exercises.h"

int maxSubsequenceDP(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
    int bestSum = INT_MIN;
    int bestI;
    int bestSize;
    int** sumValues = new int*[n];

    for(int i = 0; i < n; i++){
        sumValues[i] = new int[n];
        sumValues[i][0] = A[i];
        if(A[i] > bestSum){
            bestSum = A[i];
            bestI = i;
            bestSize = 0;   // 0 represents the index of the size(1)
        }
    }

    for(int i = 0; i < n; i++){
        for(int j = 1; j < n-i; j++){   // j < n-i because the size depends on where the currIdx is
            int currSum = sumValues[i][j-1] + A[i+j];
            sumValues[i][j] = currSum;
            if(currSum > bestSum){
                bestSum = currSum;
                bestI = i;
                bestSize = j;   // j represents the index of the size (when j=1 the size is 2)
            }
        }
    }

    i = bestI, j = bestI + bestSize;

    return bestSum;
}

int maxSubsequenceBF(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
    // TODO
	// Copy-paste your solution for ex2 of TP1 HERE
	return 0;
}

int maxSubsequenceDC(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
    // TODO
	// Copy-paste your solution for ex2 of TP3 HERE
	return 0;
}

void testPerformanceMaxSubsequence() {
	// TODO
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex6, testMaxSubsequence) {
    int A1[] = {1, 2, 3, 4};
    unsigned int n1 = 4;
    unsigned int i, j;
    EXPECT_EQ(maxSubsequenceDP(A1,n1,i,j), 10);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A2[] = {2, -1, -3, 4};
    unsigned int n2 = 4;
    EXPECT_EQ(maxSubsequenceDP(A2,n2,i,j), 4);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 3);

    int A3[] = {2, -1, 3, 4};
    unsigned int n3 = 4;
    EXPECT_EQ(maxSubsequenceDP(A3,n3,i,j), 8);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A4[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n4 = 9;
    EXPECT_EQ(maxSubsequenceDP(A4,n4,i,j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);

    testPerformanceMaxSubsequence();
}