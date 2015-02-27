/**
    Copyright (C) Atiyah Elsheikh (Atiyah.Elsheikh@ait.ac.at,a.m.g.Elsheikh@gmail.com) 2014,2015 
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
 * \file  main_population.cpp
 *  
 * Main driver for testing population-based search 
 * 
 * @author: Atiyah Elsheikh
 * @date: Jan. 2015
 * @last changes: Jan. 2015
 */ 

// declaration of the namespace
using namespace std;

// the general include for eo
#include <eo>

// include for NLP
#include "remo/moRealTypes.h"
#include "reeo/src/algo/PopulationSearchManager.h"

// objective functions 
#include "objfunc/simple/SimpleObj.h"
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
  vector<double> lowBound(10,-100);
  vector<double> uppBound(10,100);
  unsigned int seed = time(0); 
  PopulationSearchManagerSGA<ObjFunc> gamanager(lowBound,uppBound,50,100000,seed,0.8,0.5);
  
  gamanager.setSteadySC(10,50);
  gamanager.setSegmentCrossover(1.0);
  gamanager.setHypercubeCrossover(1.0);

  gamanager.setUniformMutation(0.01,0.5);
  gamanager.setDetUniformMutation(0.1,0.5);
  gamanager.setNormalMutation(1,0.5);
  gamanager.setNormalMutation(0.1,0.5);
  gamanager.setNormalMutation(0.01,0.5);
  
  //gamanager.setTournamentSelect(); // default 
  //gamanager.setProportionalSelect(); 
  //gamanager.setStochTournamentSelect(0.99);
  //gamanager.setRandomSelect();
  gamanager.init(3);
  gamanager.run();


  PopulationSearchManagerEA<ObjFunc> eamanager(lowBound,uppBound,50,100000,seed,0.8,0.5);
  
  eamanager.setSteadySC(10,50);
  eamanager.setSegmentCrossover(1.0);
  eamanager.setHypercubeCrossover(1.0);

  eamanager.setUniformMutation(0.01,0.5);
  eamanager.setDetUniformMutation(0.1,0.5);
  eamanager.setNormalMutation(1,0.5);
  eamanager.setNormalMutation(0.1,0.5);
  eamanager.setNormalMutation(0.01,0.5);
  
  //eamanager.setTournamentSelect(); // default 
  //eamanager.setProportionalSelect(); 
  //eamanager.setStochTournamentSelect(0.99);
  //eamanager.setRandomSelect();
  eamanager.setCommaReplacement();
  //eamanager.setPlusReplacement();
  //eamanager.setEPReplacement(5);
  // eamanager.setSSGAWorseReplacement();
  //eamanager.setSSGADetTournamentReplacement(5);
  //eamanager.setSSGAStochTournamentReplacement(0.5);
  // eamanager.setSurviveAndDieReplacement(0.5,0.5);
   
  eamanager.init(3,1.0);
  eamanager.run();

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

