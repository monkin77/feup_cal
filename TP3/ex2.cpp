#include "exercises.h"


int maxSubsequenceDC(int A[], unsigned int n, int &i, int &j) {
    if(n == 1){
        i = 0, j = 0;
        return A[0];
    }

    int arrSizeIncrementer = 0;
    if(n%2 != 0){
        arrSizeIncrementer++;
    }

    int leftArr[n/2 + arrSizeIncrementer], rightArr[n/2];   // Divide into left and right array
    int rightArrIdx = 0;
    for(int idx = 0; idx < n; idx++){
        if(idx < n/2 + arrSizeIncrementer){
            leftArr[idx] = A[idx];
        }
        else{
            rightArr[rightArrIdx] = A[idx];
            rightArrIdx++;
        }
    }

    int leftI, leftJ, rightI, rightJ;
    int maxSubArrLeft = maxSubsequenceDC(leftArr, n/2+arrSizeIncrementer, leftI, leftJ);
    int maxSubArrRight = maxSubsequenceDC(rightArr, n/2, rightI, rightJ);

    int leftMiddleIdx = n/2-1+arrSizeIncrementer, rightMiddleIdx = n/2 + arrSizeIncrementer;
    int leftMiddleBestSum = -1e-5, rightMiddleBestSum = -1e-5;    // best recorded sums
    int sumLeft = 0, sumRight = 0;  // current sums
    int leftCrossBestIdx = leftMiddleIdx, rightCrossBestIdx = rightMiddleIdx;   // left and right idx of the cross subarray
    bool leftAddedToCross = false, rightAddedToCross = false;   // Need to guarantee that the cross subarray has elements from both sides
    while(leftMiddleIdx >= 0){
        sumLeft += A[leftMiddleIdx];
        if(sumLeft >= leftMiddleBestSum){
            leftAddedToCross = true;
            leftMiddleBestSum = sumLeft;
            leftCrossBestIdx = leftMiddleIdx;
        }
        leftMiddleIdx--;
    }
    while(rightMiddleIdx < n){
        sumRight += A[rightMiddleIdx];
        if(sumRight >= rightMiddleBestSum){
            rightAddedToCross = true;
            rightMiddleBestSum = sumRight;
            rightCrossBestIdx = rightMiddleIdx;
        }
        rightMiddleIdx++;
    }

    int leftAndRightBestSum = leftMiddleBestSum + rightMiddleBestSum;   // this value is always >= 0 and
                                                                        // represents the best cross array sum
    if(maxSubArrLeft > maxSubArrRight){
        if((maxSubArrLeft > leftAndRightBestSum) || (!(leftAddedToCross & rightAddedToCross))){
            i = leftI, j = leftJ;
            return maxSubArrLeft;
        }
    }
    else{
        if((maxSubArrRight > leftAndRightBestSum) || (!(leftAddedToCross & rightAddedToCross))){
            i = rightI+n/2;
            j = rightJ+n/2;
            return maxSubArrRight;
        }
    }
    // The max subarray has values from the left and right
    i = leftCrossBestIdx, j = rightCrossBestIdx;
    return leftAndRightBestSum;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP3_Ex2, testMaxSubsequence) {
    int A1[] = {1, 2, 3, 4};
    unsigned int n1 = 4;
    int i, j;
    EXPECT_EQ(maxSubsequenceDC(A1,n1,i,j), 10);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A2[] = {2, -1, -3, 4};
    unsigned int n2 = 4;
    EXPECT_EQ(maxSubsequenceDC(A2,n2,i,j), 4);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 3);

    int A3[] = {2, -1, 3, 4};
    unsigned int n3 = 4;
    EXPECT_EQ(maxSubsequenceDC(A3,n3,i,j), 8);
    EXPECT_EQ(i, 0);
    EXPECT_EQ(j, 3);

    int A4[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    unsigned int n4 = 9;
    EXPECT_EQ(maxSubsequenceDC(A4,n4,i,j), 6);
    EXPECT_EQ(i, 3);
    EXPECT_EQ(j, 6);
}