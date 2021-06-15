#include "exercises.h"

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

/*
 * Utilização de Prefixo que também é sufixo
*/
int kmpMatcher(std::string text, std::string pattern) {
    int textLength = text.length();
    int patternLength = pattern.length();
    vector<int> computePrefix = computePrefixFunction(pattern);
    int numCharMatched = 0;

    int numMatches = 0;

    for(int i = 0; i < textLength; i++) {
        while(numCharMatched > 0 && pattern[numCharMatched] != text[i])
            numCharMatched = computePrefix[numCharMatched - 1];     // Next character does not match
        if(pattern[numCharMatched] == text[i])
            numCharMatched++;   // next character matches

        if(numCharMatched == patternLength){
            cout << "Pattern occurs with shift " << i - (patternLength - 1)<< endl;
            numCharMatched = computePrefix[numCharMatched - 1]; // Look for the next match
            numMatches++;
        }
    }

    return numMatches;
}

vector<int> computePrefixFunction(string pattern) {
    int m = pattern.length();
    vector<int> ans(m);
    ans[0] = 0;
    int k = 0;
    for(int i = 1; i < m; i++) {
        while(k > 0 && pattern[k] != pattern[i])
           k = ans[k-1];
        if(pattern[k] == pattern[i])
            k++;
        ans[i] = k;
    }
    return ans;
}

int numStringMatching(std::string filename, std::string toSearch) {
    fstream fin;
    fin.open(filename);

    string line;
    int totalOccur = 0;
    while(getline(fin, line)) {
        totalOccur += kmpMatcher(line, toSearch);
    }

    fin.close();

    return totalOccur;
}

/// TESTS ///
#include <gtest/gtest.h>

TEST(TP10_Ex1, testKmpMatcher) {
    EXPECT_EQ(3, kmpMatcher("aaabaabaacaabaa", "aabaa"));

    EXPECT_EQ(0, kmpMatcher("", "a"));
    EXPECT_EQ(1, kmpMatcher("a", "a"));
}

#define REL_PATH std::string("../TP10/") // relative path to the tests

TEST(TP10_Ex1, testNumStringMatching) {
    int num1 = numStringMatching(REL_PATH + "text1.txt", "estrutura de dados");
    EXPECT_EQ(3, num1);

    int num2=numStringMatching(REL_PATH +"text2.txt", "estrutura de dados");
    EXPECT_EQ(2,num2);
}