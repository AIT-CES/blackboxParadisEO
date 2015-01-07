/**
    Copyright (C) Atiyah Elsheikh (Atiyah.Elsheikh@ait.ac.at,a.m.g.Elsheikh@gmail.com) 2014, AIT Austrian Institute of Technology GmbH

    This file is part of the software blackboxParadisEO

    blackboxParadisEO is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    blackboxParadisEO is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>
*/ 

/**
 * \file  SimpleRealHC.cpp
 *  
 * Complex Implementation of Hill Climbing algorithm with Paradiseo
 * 
 * @author      Atiyah Elsheikh 
 */



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

// header for functional evaluation 
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
#include "remo/src/eval/moRealBoundaryNeighbor.h"
typedef moRealBoundaryNeighbor<Fitness> Neighbor;


// header for neighborhood 
#include "remo/src/eval/moRealBoundaryNeighborhood.h"
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


void main_function(int argc, char **argv)
{
    /* =========================================================
     *
     * Parameters 
     *
     * ========================================================= */
    
    
    unsigned seed = 10;                 // seed for initial solution generation
    unsigned N = 10;                    // Goal's function dimnesion
    unsigned numNeighbors = 50;         // number of neighbors
    double   eps = 0.1;                 // boundary radius

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
 
    MaxNumIter maxiter(10000);
    MaxNumEval maxeval(eval,20000);
    StoppingCriteria sc(maxiter);
    sc.add(maxeval);

    /* =========================================================
     *
     * the local search algorithm
     *
     * ========================================================= */

    {
      moSimpleHC<Neighbor> hc(neighborhood, eval, neighborEval,sc);
      hc(solution);

      cout << "\n ============================================================ \n"; 
      cout << "\n Hill Climber optimized solution : \n"; 
      solution.printOn(cout);
      cout << "\n obj func cntr : " << eval.value();    
    }
    

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
      moFirstImprHC<Neighbor> hc2(neighborhood, eval2, neighborEval2,sc);
      hc2(solution2);

      cout << "\n ============================================================ \n"; 
      cout << "\n First Impr. HC : optimized solution : \n"; 
      solution2.printOn(cout);
      cout << "\n obj func cntr : " << eval2.value();    
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
