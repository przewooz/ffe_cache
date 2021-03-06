// Brian Goldman

// Implementation of the various local search algorithms used in
// informal testing.

#include "HillClimb.h"
#include <algorithm>

using namespace std;

// Template specialization to allow Configuration to return
// a hill climber function pointer given a string key
template<>
hill_climb::pointer Configuration::get(const string key) {
  return hill_climb::lookup[get<string>(key)];
}

// Iteratively tests bits in a random order, accepting improvements as they
// are found.  Tracks bits that have been tested since the last modification
// to prevent waste.
void hill_climb::first_improvement(Random & rand, CLtgaIndividual & solution, Evaluator& evaluator) {
	float  fitness;

	unsigned long ul_key_old;
	bool  b_rated_old;
	float f_fitness_old;

	fitness = solution.fEvaluate(evaluator);

  // Set up data structure for random bit selection
  vector<int> options(solution.v_genotype.size());
  iota(options.begin(), options.end(), 0);
  float new_fitness;
  bool improvement;
  // keep track of locations already tried since last improvement
  std::unordered_set<int> tried;


  // Keep looping until there is no single bit flip improvement
  do {
    improvement = false;
    // Test the bits in a random order
    std::shuffle(options.begin(), options.end(), rand);
    for (const auto& index : options) {
      // If this location has already been tried, skip to the next one
      if (tried.count(index) != 0) {
        continue;
      }

      // flip and evaluate the modification
	  ul_key_old = solution.ul_key;
	  b_rated_old = solution.b_rated;
	  f_fitness_old = solution.f_fitness;

      solution.v_genotype[index] = !solution.v_genotype[index];
	  solution.b_rated = false;
	  new_fitness = solution.fEvaluate(evaluator);
      if (fitness < new_fitness) {
        // Keep change, update variables
        fitness = new_fitness;
        improvement = true;
        tried.clear();
      } else {
        // Revert the change
        solution.v_genotype[index] = !solution.v_genotype[index];
		solution.ul_key = ul_key_old;
		solution.b_rated = b_rated_old;
		solution.f_fitness = f_fitness_old;
      }
      tried.insert(index);
    }
  } while (improvement);
}

// Only modify the solution once you know which single bit change will
// result in the best fitness improvement
void hill_climb::steepest_ascent(Random & rand, CLtgaIndividual & solution, Evaluator& evaluator) {
  float new_fitness;
  bool improved;

  float  fitness;

  unsigned long ul_key_old;
  bool  b_rated_old;
  float f_fitness_old;

  fitness = solution.fEvaluate(evaluator);


  // keep a list of which locations are tied for the most improvement
  vector<size_t> bests;
  // Stores which bit was flipped to create the current solution
  size_t previous = -1;
  // Keep looping until there is no single bit flip improvement
  do {
    improved = false;
    for (size_t working = 0; working < solution.v_genotype.size(); working++) {
      if (working != previous) {
        // flip the bit and determine the new fitness
		  ul_key_old = solution.ul_key;
		  b_rated_old = solution.b_rated;
		  f_fitness_old = solution.f_fitness;

        solution.v_genotype[working] = !solution.v_genotype[working];
		solution.b_rated = false;
		new_fitness = solution.fEvaluate(evaluator);

        if (fitness <= new_fitness) {
          // strict improvements clear out the old list
          if (fitness < new_fitness) {
            fitness = new_fitness;
            improved = true;
            bests.clear();
          }
          // Record the index of each solution that obtained the best fitness
          bests.push_back(working);
        }
        // revert the change
        solution.v_genotype[working] = !solution.v_genotype[working];

		solution.ul_key = ul_key_old;
		solution.b_rated = b_rated_old;
		solution.f_fitness = f_fitness_old;
      }
    }
    if (improved) {
      // choose a random gene from those resulting in an equally large improvement
      int index = std::uniform_int_distribution<int>(0, bests.size() - 1)(rand);
      previous = bests[index];
      
	  solution.v_genotype[previous] = !solution.v_genotype[previous];
	  solution.b_rated = false;
	  solution.fEvaluate(evaluator);

      bests.clear();
    }
  } while (improved);
}

// Do nothing, just return the solution as is
void hill_climb::no_action(Random & rand, CLtgaIndividual & solution,
                           Evaluator& evaluator) {
}

// Compare solution with a randomly generated opponent, return whichever
// has the higher fitness
void hill_climb::binary_tournament(Random & rand, CLtgaIndividual & solution,  Evaluator& evaluator) {
  auto guess = rand_vector(rand, solution.v_genotype.size());

  CLtgaIndividual  c_random;
  c_random.v_genotype = guess;

  //float guess_fitness = evaluator.evaluate(guess);
  if (solution.fEvaluate(evaluator) < c_random.fEvaluate(evaluator)) {
    solution = c_random;
    //fitness = guess_fitness;
  }
}

// Attempt to flip each bit in a random order, accepting improvements
// as they are found.  Test each bit exactly once for improvement
void hill_climb::once_each(Random & rand, CLtgaIndividual & solution,
                           Evaluator& evaluator) {
  vector<int> options(solution.v_genotype.size());
  iota(options.begin(), options.end(), 0);
  float new_fitness;
  std::shuffle(options.begin(), options.end(), rand);


  bool improved;

  float  fitness;

  unsigned long ul_key_old;
  bool  b_rated_old;
  float f_fitness_old;

  fitness = solution.fEvaluate(evaluator);


  for (const auto& index : options) {
    // flip the bit and evaluate new fitness
	  ul_key_old = solution.ul_key;
	  b_rated_old = solution.b_rated;
	  f_fitness_old = solution.f_fitness;

    solution.v_genotype[index] = !solution.v_genotype[index];

	solution.b_rated = false;
	new_fitness = solution.fEvaluate(evaluator);

    if (fitness < new_fitness) {
      fitness = new_fitness;
    } else {
      solution.v_genotype[index] = !solution.v_genotype[index];

	  solution.ul_key = ul_key_old;
	  solution.b_rated = b_rated_old;
	  solution.f_fitness = f_fitness_old;
    }
  }
}
