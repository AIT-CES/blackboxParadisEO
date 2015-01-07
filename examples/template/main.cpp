
// declaration of the namespace
using namespace std;


// the general include for eo
#include <eo>
#include "utils/eoParser.h"


/********************************************************
 *
 1.,2. representation of solutions, and neighbors
 *
 ********************************************************/ 

// header for a representation of a solution
#include "es/eoReal.h"
typedef eoMinimizingFitness Fitness; 
typedef eoReal<Fitness> EOT;


/********************************************************
 *
 3. fitness function
 *
 ********************************************************/ 

//** header for functional evaluation 
#include "realfunc.h"
typedef eoEvalFuncPtr<EOT,double,const std::vector<double>&> GoalFunc;
typedef eoEvalFuncCounter<EOT>  GoalFuncCounter;   
    
/********************************************************
 *
 4. neighbors & neighborhood 
 5. Evaluation of neighbors 
 *
 ********************************************************/ 

// header for a representation of a neighbor 
#include "mymotypes/moRealBoundaryNeighbor.h"
typedef moRealBoundaryNeighbor<Fitness> Neighbor;


// header for neighborhood 
#include "mymotypes/moRealBoundaryNeighborhood.h"
typedef moRealBoundaryNeighborhood<Fitness> Neighborhood;


// header for incremental functional evaluation 
#include "eval/moFullEvalByModif.h"
typedef moFullEvalByModif<Neighbor> NeighborEval;


/********************************************************
 *
 6. algorithms 
 *
 ********************************************************/ 

#include "algo/moSimpleHC.h"
#include "algo/moFirstImprHC.h"
#include "algo/moRandomBestHC.h"  // does not work !?
#include "algo/moNeutralHC.h"     // does not work ?!
// headers for further algorithms 
#include "algo/moSA.h"
#include "algo/moTS.h"
#include "algo/moRandomNeutralWalk.h" // does not improve fitness
#include "algo/moMetropolisHasting.h" // does not improve fitness 


/********************************************************
 *
 7. Initial solution 
 *
 ********************************************************/ 

// headers for initial solutions 
#include "es/eoEsChromInit.h"  // for initializing a solution randomly


/*******************************************************
 *
 8. stopping criteria 
 *
 *******************************************************/

// header for stopping criteria 

#include "continuator/moCombinedContinuator.h"
#include "continuator/moIterContinuator.h"
#include "continuator/moFullEvalContinuator.h"

typedef moIterContinuator<Neighbor>      MaxNumIter;
typedef moCombinedContinuator<Neighbor>  StoppingCriteria;
typedef moFullEvalContinuator<Neighbor>  MaxNumEval; 


// parser
void setParserParameters(eoParser& parser,unsigned& seed,unsigned& N, unsigned& numNeighbors, double& eps);

// running a local search 
template <class LocalSearch> 
void runLocalSearch(LocalSearch& ls,Neighborhood& neighborhood,const EOT& initial);


void main_function(int argc, char **argv)
{
    /* =========================================================
     *
     * Parameters from parser
     *
     * ========================================================= */
    
    eoParser parser(argc, argv);
    unsigned seed;                 // seed for initial solution generation
    unsigned N;                    // Goal's function dimnesion
    unsigned numNeighbors;         // number of neighbors
    double   eps;                  // boundary radius

    setParserParameters(parser,seed,N,numNeighbors,eps);

    /* =========================================================
     *
     * Random seed
     *
     * ========================================================= */

    rng.reseed(seed);

    /* =========================================================
     *
     * Solution declaration 
     * Initialization of the solution
     *
     * ========================================================= */

    vector<double> lowBounds(N,-100),uppBounds(N,100); 
    eoRealVectorBounds bounds(lowBounds,uppBounds); 
    eoEsChromInit<EOT> init(bounds);
    
    EOT initialSolution; 
    init(initialSolution);
    
    initialSolution.invalidate();
    cout << "Initial solution : \n"; 
    initialSolution.printOn(cout);

    /* =========================================================
     *
     * Eval fitness function (full evaluation)
     *
     * ========================================================= */
    
    GoalFunc mainEval(realfunc);
    GoalFuncCounter eval(mainEval);  
    
    EOT solution(initialSolution);

    eval(solution);
   
    cout << "\n obj func val  : " << solution.fitness();
    cout << "\n obj func cntr : " << eval.value() << std::endl;    

    /* =========================================================
     *
     * Nieghbor and neighborhood explorer 
     *
     * ========================================================= */

    // neighbor declaration 
    Neighbor neighbor; 
    
    // neighborhood explorer 
    Neighborhood neighborhood(50,0.1); // number of neighbors , size of possible boundary 

    neighborhood.init(solution,neighbor);
    cout << "\n solution : \n"; 
    solution.printOn(cout);
    cout << "\n neighbor : \n"; 
    neighbor.printOn(cout);

  
    /* =========================================================
     *
     * evaluation of a neighbor solution
     *
     * ========================================================= */

    NeighborEval neighborEval(eval);
    neighborEval(solution,neighbor);
    
    cout << "\n solution : \n"; 
    solution.printOn(cout);
    cout << "\n neighbor : \n"; 
    neighbor.printOn(cout);

    cout << "\n obj func cntr : " << eval.value();    
       
    /* =========================================================
     *
     * Stopping conditions 
     *
     * ========================================================= */
 
    // see t-eoReal.cpp, make_continue()

    /* =========================================================
     *
     * the local search algorithm
     *
     * ========================================================= */

    moSimpleHC<Neighbor> hc(neighborhood, eval, neighborEval);
    hc(solution);

    cout << "\n ============================================================ \n"; 
    cout << "\n Hill Climber optimized solution : \n"; 
    solution.printOn(cout);
    cout << "\n obj func cntr : " << eval.value();    

    

    /* =========================================================
     *
     * First Impr. HC
     *
     * ========================================================= */
   
    {
      initialSolution.invalidate();
      EOT solution2(initialSolution); 

      GoalFunc mainEval2(realfunc);
      GoalFuncCounter eval2(mainEval2);
      NeighborEval neighborEval2(eval2);
      moFirstImprHC<Neighbor> hc2(neighborhood, eval2, neighborEval2);
      hc2(solution2);

      cout << "\n ============================================================ \n"; 
      cout << "\n First Impr. HC : optimized solution : \n"; 
      solution2.printOn(cout);
      cout << "\n obj func cntr : " << eval2.value();    

      runLocalSearch(hc2,neighborhood,initialSolution);
     }

     /* =========================================================
     *
     * SA
     *
     * ========================================================= */
   
    {
      initialSolution.invalidate();
      EOT solution_SA(initialSolution); 

      GoalFunc mainEval_SA(realfunc);
      GoalFuncCounter eval_SA(mainEval_SA);
      NeighborEval neighborEval_SA(eval_SA);
      moSA<Neighbor> SA(neighborhood, eval_SA, neighborEval_SA);
      SA(solution_SA);

      cout << "\n ============================================================ \n"; 
      cout << "\n SA : optimized solution : \n"; 
      solution_SA.printOn(cout);
      cout << "\n obj func cntr : " << eval_SA.value();    

    }

    /* =========================================================
     *
     * TS
     *
     * ========================================================= */
   
    {
      initialSolution.invalidate();
      EOT solution_TS(initialSolution); 

      GoalFunc mainEval_TS(realfunc);
      GoalFuncCounter eval_TS(mainEval_TS);
      NeighborEval neighborEval_TS(eval_TS);
      moTS<Neighbor> TS(neighborhood, eval_TS, neighborEval_TS,1,1000);
      TS(solution_TS);

      cout << "\n ============================================================ \n"; 
      cout << "\n TS : optimized solution : \n"; 
      solution_TS.printOn(cout);
      cout << "\n obj func cntr : " << eval_TS.value();    
    
      
    }  

    

}


template <class LocalSearch> 
void runLocalSearch(LocalSearch& dummy,Neighborhood& neighborhood,const EOT& initial) {
 
  EOT solution(initial);
  solution.invalidate();
  
  GoalFunc mainEval(realfunc);
  GoalFuncCounter eval(mainEval);
  NeighborEval neighborEval(eval);
  
  LocalSearch ls(neighborhood,eval,neighborEval);
  ls(solution);
  
  cout << "\n ============================================================ \n"; 
  cout << "\n" << ls.className() << " optimized solution : \n";
  solution.printOn(cout);
  cout << "\n obj func cntr : " << eval.value() << std::endl;     
}


void setParserParameters(eoParser& parser,unsigned& seed,unsigned& N,unsigned& numNeighbors,double& eps) {

  eoState  state; 

  // random seed parameter
  eoValueParam<uint32_t> seedParam(time(0), "seed", "Random number seed", 'S');
  parser.processParam( seedParam );
  seed = seedParam.value();

  // dimension of goal function
  eoValueParam<uint32_t> dimParam(10, "dimension", "Dimension of goal function", 'D');
  parser.processParam( dimParam );
  N = dimParam.value();

  // dimension of goal function
  eoValueParam<uint32_t> numNeigParam(10, "numneighbors", "Num. of neighbors", 'G');
  parser.processParam( numNeigParam );
  N = numNeigParam.value();

  // the name of the "status" file where all actual parameter values will be saved
    
  string str_status = parser.ProgramName() + ".status"; // default value
  eoValueParam<string> statusParam(str_status.c_str(), "status", "Status file");
  parser.processParam( statusParam, "Persistence" );
  
  if (parser.userNeedsHelp()) {
    parser.printHelp(cout);
    exit(1);
  }
  if (statusParam.value() != "") {
    ofstream os(statusParam.value().c_str());
    os << parser;// and you can use that file as parameter file
  }

}

// A main that catches the exceptions

int main(int argc, char **argv)
{
    try {
        main_function(argc, argv);
    }
    catch (exception& e) {
        cout << "Exception: " << e.what() << '\n';
    }
    return 1;
}
