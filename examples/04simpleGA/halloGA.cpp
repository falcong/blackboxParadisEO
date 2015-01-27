//-----------------------------------------------------------------------------
// FirstRealGA.cpp
//-----------------------------------------------------------------------------
//*
// An instance of a VERY simple Real-coded Genetic Algorithm
//
//-----------------------------------------------------------------------------

#include <stdexcept>
#include <iostream>
#include <sstream>

#include <eo>
#include <es.h>

// Use functions from namespace std
using namespace std;

// REPRESENTATION
//-----------------------------------------------------------------------------
// define your individuals
/** Fitness type */ 
typedef eoMinimizingFitness Fitness; 
/** EO Real vector type */ 
typedef eoReal<Fitness> EORVT; 


// objective functions 
// ----------------------------------------------------------------------------
//#include "objfunc/simple/SimpleObj.h"
#include "objfunc/griewank/Griewank.h"
//#include "objfunc/rastrigin/Rastrigin.h"
//#include "objfunc/rosenbrock/Rosenbrock.h"

//typedef SimpleObj ObjFunc;
typedef Griewank ObjFunc;
//typedef Rastrigin ObjFunc;
//typedef Rosenbrock ObjFunc;


// GENERAL
//-----------------------------------------------------------------------------
void main_function(int argc, char **argv)
{
// PARAMETRES
  // all parameters are hard-coded!
  const unsigned int SEED = 42; // seed for random number generator
  const unsigned int VEC_SIZE = 8; // Number of object variables in genotypes
  const unsigned int POP_SIZE = 20; // Size of population
  const unsigned int T_SIZE = 3; // size for tournament selection
  const unsigned int MAX_GEN = 500; // Maximum number of generation before STOP
  const float CROSS_RATE = 0.8; // Crossover rate
  const double EPSILON = 0.01;  // range for real uniform mutation
  const float MUT_RATE = 0.5;   // mutation rate

// GENERAL
  //////////////////////////
  //  Random seed
  //////////////////////////
  //reproducible random seed: if you don't change SEED above,
  // you'll aways get the same result, NOT a random run
  rng.reseed(SEED);

// EVAL
  /////////////////////////////
  // Fitness function
  ////////////////////////////
  // Evaluation: from a plain C++ fn to an EvalFunc Object
  ObjFunc eval; 
  
// INIT
  ////////////////////////////////
  // Initilisation of population
  ////////////////////////////////

  // declare the population
  eoPop<EORVT> pop;
  // fill it!
  for (unsigned int igeno=0; igeno<POP_SIZE; igeno++)
    {
      EORVT v;          // void individual, to be filled
      for (unsigned ivar=0; ivar<VEC_SIZE; ivar++)
	{
	  double r = 2*rng.uniform() - 1; // new value, random in [-1,1)
	  v.push_back(r);       // append that random value to v
	}
      eval(v);   // evaluate it
      pop.push_back(v);         // and put it in the population
    }

// OUTPUT
  // sort pop before printing it!
  pop.sort();
  // Print (sorted) intial population (raw printout)
  cout << "Initial Population" << endl;
  cout << pop;

// ENGINE
  /////////////////////////////////////
  // selection and replacement
  ////////////////////////////////////
// SELECT
  // The robust tournament selection
  eoDetTournamentSelect<EORVT> select(T_SIZE);       // T_SIZE in [2,POP_SIZE]

// REPLACE
  // eoSGA uses generational replacement by default
  // so no replacement procedure has to be given

// OPERATORS
  //////////////////////////////////////
  // The variation operators
  //////////////////////////////////////
// CROSSOVER
  // offspring(i) is a linear combination of parent(i)
  eoSegmentCrossover<EORVT> xover;
// MUTATION
  // offspring(i) uniformly chosen in [parent(i)-epsilon, parent(i)+epsilon]
  eoUniformMutation<EORVT>  mutation(EPSILON);

// STOP
// CHECKPOINT
  //////////////////////////////////////
  // termination condition
  /////////////////////////////////////
  // stop after MAX_GEN generations
  eoGenContinue<EORVT> continuator(MAX_GEN);

// GENERATION
  /////////////////////////////////////////
  // the algorithm
  ////////////////////////////////////////
  // standard Generational GA requires
  // selection, evaluation, crossover and mutation, stopping criterion


  eoSGA<EORVT> gga(select, xover, CROSS_RATE, mutation, MUT_RATE,
		   eval, continuator);

  // Apply algo to pop - that's it!
  gga(pop);

// OUTPUT
  // Print (sorted) intial population
  pop.sort();
  cout << "FINAL Population\n" << pop << endl;
// GENERAL
}

// A main that catches the exceptions

int main(int argc, char **argv)
{
    try
    {
	main_function(argc, argv);
    }
    catch(exception& e)
    {
	cout << "Exception: " << e.what() << '\n';
    }

    return 1;
}
