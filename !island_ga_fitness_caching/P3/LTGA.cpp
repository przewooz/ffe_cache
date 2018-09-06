// Brian Goldman

// Implements the Linkage Tree Genetic Algorithm
// Allows for features not used in the paper, like
// removing the binary tournament, performing hill climbing, etc

#include "LTGA.h"

// Constructs and evaluates the initial population of solutions
LTGA::LTGA(Random& _rand, Evaluator& _evaluator, Configuration& _config)
    : Optimizer(_rand, _evaluator, _config),
      pop(_config, _evaluator) {
  pop_size = config.get<int>("pop_size");
  disable_binary_insert = config.get<int>("binary_insert") != 1;

  // optionally applies a hill climber to the initial solutions
  hc = config.get<hill_climb::pointer>("hill_climber");

  
  float fitness;
  vector<CLtgaIndividual> solutions;
  for (size_t i = 0; i < pop_size; i++) {
    // create and evaluate solutions
	  CLtgaIndividual  c_new_ind;
	  c_new_ind.v_genotype = rand_vector(rand, length);
    //auto solution = rand_vector(rand, length);
    //fitness = evaluator.evaluate(solution);
	  c_new_ind.fEvaluate(evaluator);
    // Apply hill climber if configured to do so
    hc(rand, c_new_ind, evaluator);
    solutions.push_back(c_new_ind);
    fitnesses[c_new_ind.v_genotype] = fitness;
  }
  // Inserts the solutions into the population using a binary tournament
  binary_insert(rand, solutions, pop, _evaluator);
}

// Determines which solutions in the population should be used when
// calculating the pairwise entropy.
void LTGA::binary_insert(Random& rand, vector<CLtgaIndividual> & solutions,
                         Population& next_pop, Evaluator& _evaluator)
{
  std::shuffle(solutions.begin(), solutions.end(), rand);
  for (size_t i = 0; i + 1 < solutions.size(); i += 2) {
    /*// compete adjacent solutions in the solutions vector
    if (fitnesses[solutions[i]] < fitnesses[solutions[i + 1]]) {
      next_pop.add(solutions[i + 1]);
      // if disable_binary_insert is true, the worse solution
      // is still used when calculating entropy.
      next_pop.add(solutions[i], disable_binary_insert);
    } else {
      next_pop.add(solutions[i]);
      next_pop.add(solutions[i + 1], disable_binary_insert);
    }*/
	// compete adjacent solutions in the solutions vector
	if (solutions[i].fEvaluate(_evaluator) < solutions[i + 1].fEvaluate(_evaluator)) 
	{
		next_pop.add(solutions[i + 1].v_genotype);
		// if disable_binary_insert is true, the worse solution
		// is still used when calculating entropy.
		next_pop.add(solutions[i].v_genotype, disable_binary_insert);
	}
	else {
		next_pop.add(solutions[i].v_genotype);
		next_pop.add(solutions[i + 1].v_genotype, disable_binary_insert);
	}
  }
}

// Performs a single generation of evolution
void LTGA::generation() {
  // constructs the required crossovers
  pop.rebuild_tree(rand);
  float fitness;
  Population next_generation(config, evaluator);

  fitnesses.clear();
  for (int ii = 0; ii < pop.solutions.size(); ii++)
  {
	  fitnesses[pop.solutions.at(ii).v_genotype] = pop.solutions.at(ii).fEvaluate(evaluator);
  }//for (int ii = 0; ii < pop.solutions.size(); ii++)
  pop.pv_fitnesses = &fitnesses;

  // temporary storage of the generated solutions
  vector<CLtgaIndividual> solutions;
  for (auto solution : pop.solutions) {
    //fitness = fitnesses[solution.v_genotype];
    // uses crossover to improve a copy of the solution
    pop.improve(rand, solution, evaluator);
    //fitnesses[solution.v_genotype] = solution.fEvaluate(evaluator);
    solutions.push_back(solution);
  }
  // creates the next_generation from the offspring solutions
  binary_insert(rand, solutions, next_generation, evaluator);
  pop = next_generation;
}

// Implements the Optimization interface, returns true as long as
// LTGA thinks it can still improve
bool LTGA::iterate() {
  generation();
  // keeps track of if the new population actually has different solutions
  // than the previous generation

  //decltype(pop_set) new_set(pop.solutions.begin(), pop.solutions.end());
  decltype(pop_set) new_set;

  for (int ii = 0; ii < pop.solutions.size(); ii++)
	  new_set.insert(pop.solutions.at(ii).v_genotype);


  if (new_set == pop_set) {
    // all organisms identical after two generation, so stop
    return false;
  }
  pop_set = new_set;
  return true;
}
