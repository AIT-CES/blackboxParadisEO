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
 * \file  PopulationSearchManager.h
 * 
 * @author: Atiyah Elsheikh
 * @date: Jan. 2015
 * last changes : Jan. 2015
 */ 


#ifndef _PopulationSearchManager_h
#define _PopulationSearchManager_h

#include <typeinfo>
#include <iostream>   // cout
#include <ostream>    
#include <sstream>
#include <limits>

#include <stdexcept>
#include <eo>
#include <es.h>

#include "remo/moRealTypes.h"
#include "util/Utilities.h"

using namespace std;


/** 
 * \class BasePopulationSearchManager 
 * 
 * Population Search Manager parameterizable with arbitrary population search types and objective function
 */ 
template<class eoObjFunc>
class BasePopulationSearchManager {

 public:
  
  /**
   * Basic constructor
   *
   * @param _lowerBound lower bound of initial population
   * @param _upperBound upper bound of initial population 
   * @param _POP_SIZE   population size 
   * @param _MAX_GEN    maximum number of generations
   * @param _SEED random seed for population initialization
   */
  BasePopulationSearchManager(const vector<double>& _lowerBound,
			      const vector<double>& _upperBound,
			      unsigned int _POP_SIZE=20,
			      unsigned int _MAX_GEN=500,
			      unsigned int _SEED=time(0)) 
    : POP_SIZE(_POP_SIZE),MAX_GEN(_MAX_GEN) { 
    rng.reseed(_SEED);
    initPopulation(_lowerBound,_upperBound);
  }


  /**
   * Basic constructor
   *
   * @param _pop        initial population 
   * @param _MAX_GEN    maximum number of generations
   */
 BasePopulationSearchManager(eoPop<EORVT> _pop,
			     unsigned int _MAX_GEN=500) 
   : POP_SIZE(_pop.size()),MAX_GEN(_MAX_GEN),pop(_pop) {  }

  /// Destructor 
  virtual ~BasePopulationSearchManager() {
    try {
    }
    catch (int e) {
      std::cerr << "exception at Destructor of BasePopulationSearchManager : Nr. " << e << std::endl;
      std::abort();
    }
  }

  /**
   * get current population 
   */ 
  eoPop<EORVT> getPopulation() {
    return pop; 
  }

  /**
   * initialization 
   */ 
  virtual void init()=0;
  
  /**
   * execution
   */ 
  virtual void run()=0; 

 protected:
  
  eoPop<EORVT> pop;       ///> population 
  eoObjFunc    eval;      ///> objective function
  unsigned int POP_SIZE;  ///> Size of population
  unsigned int MAX_GEN;   ///> Maximum number of generation before STOP

 private:
  /**
   * initialize population 
   */ 
  void initPopulation(const vector<double>& _lowerBound,const vector<double>& _upperBound) {
    for (unsigned int igeno=0; igeno<POP_SIZE; igeno++)
    {
      EORVT v;                  // void individual, to be filled
      Utilities::getRandomSolution(v,_lowerBound,_upperBound);
      eval(v);                  // evaluate it
      pop.push_back(v);         // and put it in the population
    }
  }

  /// prevent copy cor
  BasePopulationSearchManager(const BasePopulationSearchManager&); 

  /// prevent copy assignment
  BasePopulationSearchManager& operator=(const BasePopulationSearchManager&); 
  
};


/** 
 * \class PopulationSearchManagerGA 
 * 
 * Population Search Manager parameterizable with arbitrary population search types and objective function
 */ 
template<class eoObjFunc>
  class PopulationSearchManagerGA : public BasePopulationSearchManager<eoObjFunc>{

 public:
  /**
   * Basic constructor
   *
   * @param _lowerBound lower bound of initial population
   * @param _upperBound upper bound of initial population 
   * @param _POP_SIZE   population size 
   * @param _MAX_GEN    maximum number of generations
   * @param _SEED random seed for population initialization
   * @param _CROSS_RATE Crossover rate
   * @param _EPSILON    range for real uniform mutation
   * @param _MUT_RATE   mutation rate int seed;
   */
  PopulationSearchManagerGA(const vector<double>& _lowerBound,
			    const vector<double>& _upperBound,
			    unsigned int _POP_SIZE=20,
			    unsigned int _MAX_GEN=500,
			    unsigned int _SEED=time(0),
			    float _CROSS_RATE=0.8,
			    double _EPSILON = 0.01,
			    float _MUT_RATE = 0.5) 
    : BasePopulationSearchManager<eoObjFunc>(_lowerBound,_upperBound,
					     _POP_SIZE,_MAX_GEN,_SEED),
    CROSS_RATE(_CROSS_RATE),EPSILON(_EPSILON),MUT_RATE(_MUT_RATE),continuator(_MAX_GEN)
  {}
  
  /**
   * Basic constructor
   *
   * @param _pop        initial population 
   * @param _MAX_GEN    maximum number of generations
   * @param _CROSS_RATE Crossover rate
   * @param _EPSILON    range for real uniform mutation
   * @param _MUT_RATE   mutation rate 
   */
 PopulationSearchManagerGA(eoPop<EORVT> _pop,
			   unsigned int _MAX_GEN=500,
			   float _CROSS_RATE=0.8,
			   double _EPSILON = 0.01,
			   float _MUT_RATE = 0.5) 
   : BasePopulationSearchManager<eoObjFunc>(_pop,_MAX_GEN),
    CROSS_RATE(_CROSS_RATE),EPSILON(_EPSILON),MUT_RATE(_MUT_RATE),continuator(_MAX_GEN) {  }


  /// Destructor 
  virtual ~PopulationSearchManagerGA() {
    try {
    }
    catch (int e) {
      std::cerr << "exception at Destructor of BasePopulationSearchManager : Nr. " << e << std::endl;
      std::abort();
    }
  }

  
 protected: 

  float CROSS_RATE;                  ///> Crossover rate
  double EPSILON;                    ///> range for real uniform mutation  
  float MUT_RATE;                    ///> mutation rate 
  eoGenContinue<EORVT> continuator;  ///> stopping criteria 
  


};


template<class eoObjFunc>
  class PopulationSearchManagerSGA : public PopulationSearchManagerGA<eoObjFunc>{

 public:
  /**
   * Basic constructor
   *
   * @param _lowerBound lower bound of initial population
   * @param _upperBound upper bound of initial population 
   * @param _POP_SIZE   population size 
   * @param _MAX_GEN    maximum number of generations
   * @param _SEED random seed for population initialization
   * @param _CROSS_RATE Crossover rate
   * @param _EPSILON    range for real uniform mutation
   * @param _MUT_RATE   mutation rate int seed;
   */
  PopulationSearchManagerSGA(const vector<double>& _lowerBound,
			     const vector<double>& _upperBound,
			     unsigned int _POP_SIZE=20,
			     unsigned int _MAX_GEN=500,
			     unsigned int _SEED=time(0),
			     float _CROSS_RATE=0.8,
			     double _EPSILON = 0.01,
			     float _MUT_RATE = 0.5) 
    : PopulationSearchManagerGA<eoObjFunc>(_lowerBound,_upperBound,
					   _POP_SIZE,_MAX_GEN,_SEED,
					   _CROSS_RATE,_EPSILON,_MUT_RATE),
    select(0),xover(0),mutation(0),INITIALIZED(false),sga(0){  }
  
  /**
   * Basic constructor
   *
   * @param _pop        initial population 
   * @param _MAX_GEN    maximum number of generations
   * @param _CROSS_RATE Crossover rate
   * @param _EPSILON    range for real uniform mutation
   * @param _MUT_RATE   mutation rate 
   */
 PopulationSearchManagerSGA(eoPop<EORVT> _pop,
			   unsigned int _MAX_GEN=500,
			   float _CROSS_RATE=0.8,
			   double _EPSILON = 0.01,
			   float _MUT_RATE = 0.5) 
   : PopulationSearchManagerGA<eoObjFunc>(_pop,_MAX_GEN,_CROSS_RATE,_EPSILON,_MUT_RATE),
    select(0),xover(0),mutation(0),INITIALIZED(false),sga(0) {}


  /// Destructor 
  virtual ~PopulationSearchManagerSGA() {
    try {
      delete select;
      delete mutation;
      delete xover; 
      delete sga;
    }
    catch (int e) {
      std::cerr << "exception at Destructor of BasePopulationSearchManager : Nr. " << e << std::endl;
      std::abort();
    }
  }



  virtual void init() {
    init(2);
  }

  /**
   * initialization
   *
   * @param T_SIZE 
   */ 
  virtual void init(unsigned int T_SIZE) { 
    if(INITIALIZED) return;
    if(select == 0) {
      if((T_SIZE > this->POP_SIZE) || (T_SIZE < 2))
	std::cerr << "T_SIZE in [2,POP_SIZE]\n"; 
      select = new eoDetTournamentSelect<EORVT>(T_SIZE);
    }
    if(xover == 0)
      xover  = new eoSegmentCrossover<EORVT>();
    if(mutation == 0)
      mutation = new eoUniformMutation<EORVT>(this->EPSILON); 
    sga = new eoSGA<EORVT>(*select,*xover,this->CROSS_RATE,
			   *mutation,this->MUT_RATE,this->eval,this->continuator);
    INITIALIZED=true;
  }

  /**
   * execute 
   */ 
  virtual void run() { 
    (*sga)(this->pop);
    this->pop.sort();
    cout << "final population \n" << this->pop << endl;
  }

 protected:

  eoSelectOne<EORVT>*   select;      ///> Selection Criteria 
  eoQuadOp<EORVT>*      xover;       ///> Crossover 
  eoMonOp<EORVT>*       mutation;    ///> mutation
  eoSGA<EORVT>*         sga;        ///> GA algorithm
  bool                  INITIALIZED; ///> whether object is initialized        

};

#endif
