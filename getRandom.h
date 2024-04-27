#pragma once
#ifndef GET_RANDOM_H
#define GET_RANDOM_H
#include <random>
#include <ctime>
#include <algorithm>
#include <vector>


class RandomGenerator {
	
public:
	RandomGenerator() {
		srand((unsigned)time(0));
	}
	//@param lb lower bound
	//@param ub upper bound
	static int getRandomInt(int lb, int ub) {
		return lb + rand() % (ub - lb);
	}
	template<typename T>
	static void shuffleVector(std::vector<T>& v) {
		std::shuffle(std::begin(v), std::end(v), std::default_random_engine{});
	}
};
	
#endif
