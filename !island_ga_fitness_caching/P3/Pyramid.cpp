/*
 * Pyramid.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: goldman
 */

#include "Pyramid.h"

// Applies crossover between the passed in solution as each level
// of the pyramid
bool Pyramid::climb(CLtgaIndividual & solution) {
	return climb(solution, 0, false, false);
}

bool Pyramid::climb(CLtgaIndividual & solution, vector<size_t> & addedToLevels) {
	bool b_added = false;

	if (add_unique(solution, 0)) {
		addedToLevels.push_back(0);
		b_added = true;
	}

	for (size_t lev = 0; lev < pops.size(); lev++) {
		float prev = solution.fEvaluate(evaluator);
		// Use population clusters and population solutions to make new solution
		pops[lev].improve(rand, solution, evaluator);
		// add it to the next level if its a strict fitness improvement,
		// or configured to always add solutions
		if (!only_add_improvements || prev < solution.fEvaluate(evaluator)) {
			if (add_unique(solution, lev + 1)) {
				addedToLevels.push_back(lev + 1);
				b_added = true;
			}
		}
	}

	return b_added;
}

bool Pyramid::climb(CLtgaIndividual & solution, size_t level, bool hillClimber, bool climbOnlyIfUnique) {
	if (hillClimber) {
		hill_climber(rand, solution, evaluator);
	}

	// attempts to add this solution to the base level
	bool b_added = add_unique(solution, level);

	if (!climbOnlyIfUnique || b_added) {
		for (size_t lev = level; lev < pops.size(); lev++) {
			float prev = solution.fEvaluate(evaluator);
			// Use population clusters and population solutions to make new solution
			pops[lev].improve(rand, solution, evaluator);
			// add it to the next level if its a strict fitness improvement,
			// or configured to always add solutions
			if (!only_add_improvements || prev < solution.fEvaluate(evaluator)) {
				b_added = add_unique(solution, lev + 1) || b_added;
			}
		}
	}
	else
	{
		if (CLtgaIndividual::piCachingPopulation != nullptr)  (*CLtgaIndividual::piCachingPopulation)++;
	}

	return b_added;
}

bool Pyramid::feedback(CLtgaIndividual & solution, float & fitness) {
	bool b_added = false;
	
	for (size_t lev = 0; lev < pops.size(); lev++) {
		float prev = fitness;
		// Use population clusters and population solutions to make new solution
		pops[lev].improve(rand, solution, evaluator);
		// add it to the next level if its a strict fitness improvement,
		// or configured to always add solutions
		if (prev < fitness) {
			b_added = add_unique(solution, pops.size()) || b_added;
		}
	}

	return b_added;
}

// Add the solution to the specified level of the population, only if it is
// unique.  Returns true if added.
bool Pyramid::add_unique(CLtgaIndividual & solution, size_t level) {
  if (seen.find(solution.v_genotype) == seen.end()) {
    if (pops.size() == level) {
      // Create new levels as necessary
      pops.push_back(Population(config, evaluator));
    }
    // Add the solution and rebuild the tree
    pops[level].add(solution.v_genotype);
    pops[level].rebuild_tree(rand);
    // track that this solution is now in the pyramid
    seen.insert(solution.v_genotype);
    return true;
  }
  return false;
}

// Performs a full iteration of P3
bool Pyramid::iterate() {
  // generate a random solution
	CLtgaIndividual  c_new_ind;
  //vector<bool> solution = rand_vector(rand, length);
  c_new_ind.v_genotype = rand_vector(rand, length);

  //float fitness = evaluator.evaluate(solution);
  c_new_ind.fEvaluate(evaluator);
  // perform a local search hill climber
  hill_climber(rand, c_new_ind, evaluator);
  // perform crossover with each level of the pyramid
  climb(c_new_ind);
  // P3 never "converges"

  return true;
}
