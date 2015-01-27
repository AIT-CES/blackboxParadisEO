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
 * \file moRealTypes.h
 * 
 * @author : Atiyah Elsheikh
 * @date   : Oct. 2014
 * last changes : Jan. 2015 
 */ 

#ifndef _moRealTypes_h
#define _moRealTypes_h


//
// see t-eoReal.cpp
//
/********************************************************
 *
 1.,2. representation of solutions, and neighbors
 *
 ********************************************************/ 

// header for a representation of a solution
#include "es/eoReal.h"
/** Fitness type */ 
typedef eoMinimizingFitness Fitness; 
/** EO Real vector type */ 
typedef eoReal<Fitness> EORVT; 


/********************************************************
 *
 3. fitness function
 *
 ********************************************************/ 

//** header for functional evaluation 

//typedef eoEvalFuncPtr<EORVT,double,const std::vector<double>&> GoalFunc;
/** counter of objective function evaluation */ 
typedef eoEvalFuncCounter<EORVT>  GoalFuncCounter;   
    
/********************************************************
 *
 4. neighbors & neighborhood 
 5. Evaluation of neighbors 
 *
 ********************************************************/ 

// header for a representation of a neighbor 
#include "src/eval/moRealBoundaryNeighbor.h"
/** Neighbor */ 
typedef moRealBoundaryNeighbor<Fitness> Neighbor;

// header for neighborhood 
#include "src/eval/moRealBoundaryNeighborhood.h"
/** Neighborhood */ 
typedef moRealBoundaryNeighborhood<Fitness> Neighborhood;

// header for incremental functional evaluation 
#include "eval/moFullEvalByModif.h"
/** */ 
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
//#include "algo/moTS.h"
#include "src/algo/moTSUnified.h" 
#include "algo/moRandomNeutralWalk.h" // does not improve fitness
#include "algo/moMetropolisHasting.h" // does not improve fitness 

/** SimpleHillClimbing */ 
typedef moSimpleHC<Neighbor>    SimpleHillClimbing;
/** First improvment of HC */ 
typedef moFirstImprHC<Neighbor> FirstImprHillClimbing;
/** Simulated Annealing*/
typedef moSA<Neighbor>          SimulatedAnnealing;
/** Tabu Search*/ 
//typedef moTS<Neighbor>          TabuSearch;
typedef moTSUnified<Neighbor>   TabuSearch;


/********************************************************
 *
 7. Initial solution 
 *
 ********************************************************/ 

// headers for initial solutions 
// #include "es/make_real.h" // make_genotype(parser,state,EORVT())


/*******************************************************
 *
 8. stopping criteria 
 *
 *******************************************************/

// header for stopping criteria 
#include "continuator/moCombinedContinuator.h"
#include "continuator/moIterContinuator.h"
#include "continuator/moFullEvalContinuator.h"
/** maximum number of iteration */ 
typedef moIterContinuator<Neighbor>      MaxNumIter;
/** stopping criteria */ 
typedef moCombinedContinuator<Neighbor>  StoppingCriteria;
/** maximum number of functions evaluation */ 
typedef moFullEvalContinuator<Neighbor>  MaxNumEval; 

// see also
// moBestNoImproveContinuator
// moTimeContinuator

#endif
