#include "exercises.h"
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
#include <time.h>

using namespace std;

std::string calcSum(int sequence[], unsigned long n) {
    vector<unsigned int> bestIndex(n+1);    // saves the best index for each m
    vector<unsigned int> bestSum(n+1);      // saves the best sum for each m
    vector<vector<int> > sums(n, vector<int>(n+1));    // 2D array, such that it saves the sum for a starting index i and the next j elements

    bestSum[0] = 0;
    for(int i = 0; i <= n; i++) bestSum[i] = INT_MAX;
    for(int i = 0; i < n; i++){
        sums[i][0] = 0;
        sums[i][1] = sequence[i];
    }

    for(int m = 1; m <= n; m++){
        for(int i = 0; i <= n-m; i++){
            int newSum = sums[i][m-1] + sums[i+m-1][1];
            sums[i][m] = newSum;
            if(newSum < bestSum[m]){
                bestSum[m] = newSum;
                bestIndex[m] = i;
            }
        }
    }
    string ans = "";
    for(int i = 1; i <= n; i++){
        ans += to_string(bestSum[i]) + "," + to_string(bestIndex[i])  + ";";
    }
    return ans;
}

void testPerformanceCalcSum() {
    ofstream fout;
    fout.open("ex4Res.txt");
    srand(time(NULL));

    for(int n = 500; n <= 50000; n += 500){
        int sequence[n];
        for(int i = 0; i < n; i++)
            sequence[i] = rand() % 10 + 1;

        auto start = chrono::high_resolution_clock::now();
        calcSum(sequence, n);
        auto finish = chrono::high_resolution_clock::now();
        auto mili = chrono::duration_cast<chrono::milliseconds>(finish - start).count();
        fout << n << ";" << mili << endl;
    }

}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP4_Ex4, CalcSumArrayTest) {
    int sequence[5] = {4,7,2,8,1};
    int sequence2[9] = {6,1,10,3,2,6,7,2,4};

    EXPECT_EQ("1,4;9,1;11,2;18,1;22,0;",calcSum(sequence, 5));
    EXPECT_EQ("1,1;5,3;11,3;16,1;20,3;24,3;31,1;35,1;41,0;",calcSum(sequence2, 9));
	
	testPerformanceCalcSum();
}