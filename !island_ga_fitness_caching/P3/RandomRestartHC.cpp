// Brian Goldman

// Trival implementation of repeated local search

#include "RandomRestartHC.h"

// Start from a random location and keep going until a local optima
// is found
bool RandomRestartHC::iterate() {
  
  CLtgaIndividual  c_new_ind;
  c_new_ind.v_genotype = rand_vector(rand, length);
  //vector<bool> solution = rand_vector(rand, length);

  c_new_ind.fEvaluate(evaluator);
  hill_climber(rand, c_new_ind, evaluator);

  // Always possible
  return true;
}
