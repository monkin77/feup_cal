#include "exercises.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <algorithm>

using namespace std;

int editDistance(std::string pattern, std::string text) {
    int patternSize = pattern.size();
    int textSize = text.size();

    int distances[patternSize+1][textSize+1];

    for(int i = 0; i <= patternSize; i++)
        for(int j = 0; j <= textSize; j++)
            distances[i][j] = 0;

    /* Condições fronteira  */
    for(int i = 0; i <= textSize; i++)
        distances[0][i] = i;
    for(int i = 0; i <= patternSize; i++)
        distances[i][0] = i;

    for(int i = 1; i <= patternSize; i++) {  // Iterate by sizes of string
        for(int j = 1; j <= textSize; j++) {
            if( pattern[i-1] == text[j-1] )     // has to be -1 because index 0 is the empty string
                distances[i][j] = distances[i-1][j-1];  // If the last character is equal
            else {
                int leftBorder = distances[i][j-1];
                int topBorder = distances[i-1][j];
                int topLeft = distances[i-1][j-1];

                if(leftBorder < topLeft && leftBorder < topBorder)
                    distances[i][j] = leftBorder + 1;
                else if(topBorder < topLeft && topBorder < leftBorder)
                    distances[i][j] = topBorder + 1;
                else
                    distances[i][j] = topLeft + 1;
            }
        }
    }


    return distances[patternSize][textSize];
}

float numApproximateStringMatching(std::string filename, std::string toSearch) {
    fstream fin;

    fin.open(filename);

    string word;
    int numWords = 0;
    int totalDistanceSum = 0;
    while(fin >> word) {
        int currEditDistance = editDistance(toSearch, word);
        totalDistanceSum += currEditDistance;
        numWords++;
    }

    fin.close();

    float ans = (float) totalDistanceSum / numWords;
    return ans;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP10_Ex2, testEditDistance) {
    EXPECT_EQ(3, editDistance("abcdefghijkl", "bcdeffghixkl"));
    EXPECT_EQ(3, editDistance("bcdeffghixkl", "abcdefghijkl"));

    EXPECT_EQ(1, editDistance("", "b"));
    EXPECT_EQ(1, editDistance("a", ""));
    EXPECT_EQ(1, editDistance("a", "b"));
    EXPECT_EQ(0, editDistance("", ""));
    EXPECT_EQ(0, editDistance("a", "a"));
}

#define REL_PATH std::string("../TP10/") // relative path to the tests

TEST(TP10_Ex2, testNumApproximateStringMatching) {
    const float delta = 0.01;

    const float expected1 = 7.76;
    float dist1 = numApproximateStringMatching(REL_PATH + "text1.txt", "estrutur");
    ASSERT_LE(expected1-delta, dist1);
    ASSERT_GE(expected1+delta, dist1);

    const float expected2 = 7.49;
    float dist2 = numApproximateStringMatching(REL_PATH + "text2.txt", "estrutur");
    ASSERT_LE(expected2-delta, dist2);
    ASSERT_GE(expected2+delta, dist2);
}