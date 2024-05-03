/**
 * Name:		Jiayou Qin
 * Date:		May 5, 2024
 * Subject:		CPE553
 * Filename:	getRandom.h
 * Description:	Project 2
 */


#pragma once
#ifndef GET_RANDOM_H
#define GET_RANDOM_H
#include <random>
#include <ctime>
#include <algorithm>
#include <vector>
#include <iostream>


class RandomGenerator {

public:
	RandomGenerator() {
		srand((unsigned) time(0));
	}

	//@param lb lower bound
	//@param ub upper bound
	static int getRandomInt(int lb, int ub) {
		return lb + rand() % (ub + 1 - lb);
	}

	template<typename T>
	static void shuffleVector(std::vector<T> &v) {
		std::random_shuffle(v.begin(), v.end());
	}
};
	
#endif
