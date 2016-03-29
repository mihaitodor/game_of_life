#include "model.h"

#include <random>
#include <algorithm>
#include <functional>

void Model::Randomize() {
	std::random_device randDevice;
	std::default_random_engine engine(randDevice());
	std::bernoulli_distribution distribution;

	for (auto &row : state) {
		for (auto &cell : row) {
			cell = distribution(engine);
		}
	}
}