/**
    Copyright (C) Atiyah Elsheikh (Atiyah.Elsheikh@ait.ac.at,a.m.g.Elsheikh@gmail.com) 2014, 
    AIT Austrian Institute of Technology GmbH

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
    along with blackboxParadisEO.  If not, see <http://www.gnu.org/licenses/>
*/ 

/**
 * \file  main_localsearch.cpp
 *  
 * Main driver for testing local search heuristics 
 * 
 * @author: Atiyah Elsheikh
 * @date: August 2014
 * @last changes: Oct. 2014
 */ 

// declaration of the namespace
using namespace std;

// the general include for eo
#include <eo>

// include for NLP
#include "remo/moRealTypes.h"
#include "remo/src/algo/localSearchManager.h"

// objective functions 
//#include "objfunc/simple/SimpleObj.h"
//#include "objfunc/griewank/Griewank.h"
//#include "objfunc/rastrigin/Rastrigin.h"
#include "objfunc/rosenbrock/Rosenbrock.h"

//typedef SimpleObj ObjFunc;
//typedef Griewank ObjFunc;
//typedef Rastrigin ObjFunc;
typedef Rosenbrock ObjFunc;

// include for solution initialization
#include "util/Utilities.h"

void main_function(int argc, char **argv)
{
   
  /* =========================================================
   *
   * Solution declaration 
   * Initialization of the solution
   *
   * ========================================================= */

  unsigned N = 10;  // Goal's function dimnesion
  EORVT initialSolution(N);
  for(int i=0;i<initialSolution.size();i++) initialSolution[i] = 10*i; 
  cout << "Initial solution : \n"; 
  initialSolution.printOn(cout);

  /// An alternative 
  /// vector<double> lowBounds(N,-100),uppBounds(N,100); 
  /// Utilities::getRandomSolution(initialSolution,lowBounds,uppBounds);    
    
  /* =========================================================
   *
   * the local search algorithm
   *
   * ========================================================= */
 
    LocalSearchManager<SimpleHillClimbing,ObjFunc> simpleHC(initialSolution); 
    simpleHC.init(200000);
    simpleHC.run();
    simpleHC.printOn();

    /* =========================================================
     *
     * First Impr. HC
     *
     * ========================================================= */
   
    LocalSearchManager<FirstImprHillClimbing,ObjFunc> FIHC(initialSolution); 
    FIHC.init();
    FIHC.run();
    FIHC.printOn();

    /* =========================================================
     *
     * SA
     *
     * ========================================================= */

    LocalSearchManager<SimulatedAnnealing,ObjFunc> SA(initialSolution); 
    SA.init();
    SA.run();
    SA.printOn();
   
    /* =========================================================
     *
     * TS
     *
     * ========================================================= */
   

    LocalSearchManagerTS<ObjFunc> TS(initialSolution,50,0.1,10000);
    TS.setTimeLimit(1);
    TS.setTabuListSize(100);
    TS.init();
    TS.run();
    TS.printOn();
   
    /// OR 

    LocalSearchManager<TabuSearch,ObjFunc> TS2(initialSolution); 
    TS2.init(2000000);
    TS2.run();
    TS2.printOn();

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
