#pragma once
#include <random>
#include <ctime>
#include <algorithm>
#include <vector>

template<typename T>
class RandomGenerator {
	
public:
	RandomGenerator() {
		srand((unsigned)time(0));
	}
	static int getRandomInt(int lb, int ub) {
		return lb + rand() % (ub - lb);
	}

	static void shuffleVector(std::vector<T>& v) {
		std::shuffle(std::begin(v), std::end(v), std::default_random_engine{});
	}
};
	

