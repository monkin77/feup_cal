// By: Gonçalo Leão

#include "exercises.h"

int maxSubsequence(int A[], unsigned int n, unsigned int &i, unsigned int &j) {
    int maxSum = 0;
    int currSum = 0;
    i = 0;
    int newI = i;
    for(int a = 0; a < n; a++){
        currSum = currSum + A[a];
        if(currSum > maxSum) {
            maxSum = currSum;
            j = a;
            i = newI;
        }
        else if(currSum > 0){
            continue;
        }
        else{
            currSum = 0;
            newI = a+1;
        }
    }

    return maxSum;
}


/// TESTS ///
#include <gtest/gtest.h>

TEST(TP1_Ex2, maxSubsequence) {
    int A[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n = 9;
    unsigned int i, j;
    EXPECT_EQ(maxSubsequence(A,n,i,j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);
}