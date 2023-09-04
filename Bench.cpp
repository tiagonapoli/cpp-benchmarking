// Bench.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma comment(lib, "shlwapi.lib")
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <string_view>
#include "benchmark/benchmark.h"

using namespace std;

struct CompositeKey {
    string a;
    string b;

    bool operator<(const CompositeKey& other) const {
        int res = a.compare(other.a);
        return res != 0 ? res < 0 : b < other.b;
    }
};

struct CompositeKey2 {
    string a;
    string b;

    bool operator<(const CompositeKey2& other) const {
        int res = a.compare(other.a);
        if (res == 0) return b < other.b;
        return res < 0;
    }
};

vector<pair<string, string>> rawInput;
vector<CompositeKey> inputCompositeKey;
vector<CompositeKey2> inputCompositeKey2;
vector<string> inputKey;

map<CompositeKey, int> dictCompositeKey;
map<CompositeKey2, int> dictCompositeKey2;
map<pair<string, string>, int> dictPair;
map<string, int> dictSingleKey;

void readInput()
{
    if(rawInput.size() != 0) return;

    std::string filename = "C:/personal/Benchmark/Bench/input.csv";

    // Create an input file stream object
    std::ifstream inputFile;

    // Open the file for reading
    inputFile.open(filename);

    // Check if the file is opened successfully
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        exit(1); // Exit with an error code
    }

    int k = 0;
    string a;
    string b;
    while(inputFile >> a) {
        inputFile >> b;
        rawInput.push_back({a, b});
    }

    random_shuffle(rawInput.begin(), rawInput.end());
    for(auto& el : rawInput) {
        dictCompositeKey[CompositeKey{ a, b }] = k;
        dictCompositeKey2[CompositeKey2{ a, b }] = k;
        dictPair[el] = k;
        dictSingleKey[a + ',' + b] = k++;
        inputCompositeKey.push_back(CompositeKey{ el.first, el.second });
        inputCompositeKey2.push_back(CompositeKey2{el.first, el.second});
        inputKey.push_back(el.first + ',' + el.second);
    }

    // Close the file
    inputFile.close();
}

static void BM_CompositeKeyMap(benchmark::State& state) {
    readInput();
    for (auto _ : state) {
        int res = 0;
        for (auto& el : inputCompositeKey) {
            int k = dictCompositeKey[el];
            benchmark::DoNotOptimize(res += k);
        }
    }
}

static void BM_CompositeKey2Map(benchmark::State& state) {
    readInput();
    for (auto _ : state) {
        int res = 0;
        for (auto& el : inputCompositeKey2) {
            int k = dictCompositeKey2[el];
            benchmark::DoNotOptimize(res += k);
        }
    }
}

static void BM_PairKeyMap(benchmark::State& state) {
    readInput();
    for (auto _ : state) {
        int res = 0;
        for (auto& el : rawInput) {
            int k = dictPair[el];
            benchmark::DoNotOptimize(res += k);
        }
    }
}

static void BM_SingleKeyMap(benchmark::State& state) {
    readInput();
    for (auto _ : state) {
        int res = 0;
        for(auto& el : inputKey) {
            int k = dictSingleKey[el];
            benchmark::DoNotOptimize(res += k);
        }
    }
}

BENCHMARK(BM_SingleKeyMap);
BENCHMARK(BM_CompositeKeyMap);
BENCHMARK(BM_CompositeKey2Map);
BENCHMARK_MAIN();
