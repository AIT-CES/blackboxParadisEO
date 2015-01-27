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
    : POP_SIZE(_POP_SIZE),MAX_GEN(_MAX_GEN),INITIALIZED(false) { 
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
   : POP_SIZE(_pop.size()),MAX_GEN(_MAX_GEN),pop(_pop),INITIALIZED(false) {  }

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
  virtual void init() {
    if(INITIALIZED) return; 
    initPS();
    INITIALIZED = true;
  }
  
  /**
   * execution
   */ 
  virtual void run()=0; 

 protected:

  virtual void initPS()=0;
  
  eoPop<EORVT> pop;         ///> population 
  eoObjFunc    eval;        ///> objective function
  unsigned int POP_SIZE;    ///> Size of population
  unsigned int MAX_GEN;     ///> Maximum number of generation before STOP
  bool         INITIALIZED; ///> whether object is initialized        

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

  // const double hypercubeRate = 0.5;     // relative weight for hypercube Xover
  // const double segmentRate = 0.5;  // relative weight for segment Xover
  // const double uniformMutRate = 0.5;  // relative weight for uniform mutation
  // const double detMutRate = 0.5;      // relative weight for det-uniform mutation
  // const double normalMutRate = 0.5;   // relative weight for normal mutation

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
   * @param _MUT_RATE   mutation rate int seed;
   */
  PopulationSearchManagerGA(const vector<double>& _lowerBound,
			    const vector<double>& _upperBound,
			    unsigned int _POP_SIZE=20,
			    unsigned int _MAX_GEN=500,
			    unsigned int _SEED=time(0),
			    float _CROSS_RATE=0.8,
			    float _MUT_RATE = 0.5) 
    : BasePopulationSearchManager<eoObjFunc>(_lowerBound,_upperBound,
					     _POP_SIZE,_MAX_GEN,_SEED),
    CROSS_RATE(_CROSS_RATE),MUT_RATE(_MUT_RATE),genCont(_MAX_GEN),continuator(genCont) {  }
  
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
			   float _MUT_RATE = 0.5) 
   : BasePopulationSearchManager<eoObjFunc>(_pop,_MAX_GEN),
    CROSS_RATE(_CROSS_RATE),MUT_RATE(_MUT_RATE),genCont(_MAX_GEN),continuator(genCont) {  }


  /// Destructor 
  virtual ~PopulationSearchManagerGA() {
    try {
    }
    catch (int e) {
      std::cerr << "exception at Destructor of BasePopulationSearchManager : Nr. " << e << std::endl;
      std::abort();
    }
  }

  /**
   * set Stopping Criteria no improvment 
   * 
   * @param MIN_GEN  Minimum number of generation before ...
   * @param STEADY_GEN stop after STEADY_GEN gen. without improvment
   */ 
  void setSteadySC(unsigned int MIN_GEN=10,unsigned int STEADY_GEN=50) {
    if(this->INITIALIZED) std::cerr << "setSteadySC::object already initialized\n";
    eoSteadyFitContinue<EORVT>  steadyCont(MIN_GEN,STEADY_GEN);
    continuator.add(steadyCont);
  }
  
 protected: 

  float CROSS_RATE;                  ///> Crossover rate
  float MUT_RATE;                    ///> mutation rate 
  eoGenContinue<EORVT> genCont;  ///> stopping criteria 
  eoCombinedContinue<EORVT> continuator;  ///> stopping criteria 
  


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
			     float _MUT_RATE = 0.5) 
    : PopulationSearchManagerGA<eoObjFunc>(_lowerBound,_upperBound,
					   _POP_SIZE,_MAX_GEN,_SEED,
					   _CROSS_RATE,_MUT_RATE),
    select(0),xover(0),mutation(0),sga(0),EPSILON(0.01),SIGMA(0.3) {  }
  
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
			   float _MUT_RATE = 0.5) 
   : PopulationSearchManagerGA<eoObjFunc>(_pop,_MAX_GEN,_CROSS_RATE,_MUT_RATE),
    select(0),xover(0),mutation(0),sga(0),EPSILON(0.01),SIGMA(0.3) {}


  /// Destructor 
  virtual ~PopulationSearchManagerSGA() {
    try {
      delete select;
      delete mutation;
      delete nmut;
      delete umut;
      delete dumut;
      delete xover; 
      delete sga;
    }
    catch (int e) {
      std::cerr << "exception at Destructor of BasePopulationSearchManager : Nr. " << e << std::endl;
      std::abort();
    }
  }


  /**
   * Initialization 
   */ 
  virtual void init() {
    init(2);
  }

  /**
   * initialization
   *
   * @param tsize  size for tournament selection [2,POP_SIZE]
   */ 
  virtual void init(unsigned int tsize) { 
    T_SIZE = tsize;
    if((T_SIZE > this->POP_SIZE) || (T_SIZE < 2))
      std::cerr << "T_SIZE in [2,POP_SIZE]\n";
    BasePopulationSearchManager<eoObjFunc>::init();
  }

  /**
   * execute 
   */ 
  virtual void run() { 
    (*sga)(this->pop);
    this->pop.sort();
    cout << "final population \n" << this->pop << endl;
  }

  /**
   * Set Tournament Selection (default)
   * 
   * @param tsize size for tournament selection [2,POP_SIZE]
   */ 
  void setTournamentSelect(unsigned int tsize)  {
    T_SIZE = tsize;
    if(this->INITIALIZED) 
      std::cerr << "Population search object already initialized \n";
    if((T_SIZE > this->POP_SIZE) || (T_SIZE < 2))
      std::cerr << "T_SIZE in [2,POP_SIZE]\n"; 
    delete select; 
    select = new eoDetTournamentSelect<EORVT>(T_SIZE);
  }

  /**
   * Set roulette wheel selection (does not work with minimizing fitness)
   */ 
  void setProportionalSelect() { 
    if(this->INITIALIZED) 
      std::cerr << "Population search object already initialized \n";
    delete select; 
    select = new eoProportionalSelect<EORVT>();
  }

  /**
   * Set stochastic binary tournament 
   *
   * @param rate 
   */
  void setStochTournamentSelect(double rate) { 
    if(this->INITIALIZED) 
      std::cerr << "Population search object already initialized \n";
    if( (rate <= 0.5) || rate > 1.0) 
      std::cerr << "rate should be ]0.5,1] \n";
    delete select; 
    select = new eoStochTournamentSelect<EORVT>(rate);
  }


  /**
   * Set random selection
   */ 
  void setRandomSelect() {
    if(this->INITIALIZED) 
      std::cerr << "Population search object already initialized \n";
    delete select; 
    select = new eoRandomSelect<EORVT>();
  }


  /**
   * set uniform mutation 
   *
   * @param _eps range for real uniform mutation  
   * @param weight 
   */ 
  void setUniformMutation(double _eps,double weight=0.5) {
    if(this->INITIALIZED)
      std::cerr << "setUniformMutation(): object already initialized\n";
    EPSILON = _eps;
    umut = new eoUniformMutation<EORVT>(EPSILON);
    if(mutation == 0) {
      mutation = new eoPropCombinedMonOp<EORVT>(*umut,weight); 
    } else {
      mutation->add(*umut,weight); 
    }
  }

  /**
   * set deterministic uniform mutation 
   *
   * @param _eps range for real uniform mutation  
   * @param weight 
   */ 
  void setDetUniformMutation(double _eps,double weight = 0.5) {
    if(this->INITIALIZED)
      std::cerr << "setDetUniformMutation(): object already initialized\n";
    EPSILON = _eps;
    dumut = new eoDetUniformMutation<EORVT>(EPSILON);
    if(mutation == 0) {
      mutation = new eoPropCombinedMonOp<EORVT>(*dumut,weight); 
    } else {
      mutation->add(*dumut,weight); 
    }
  }
  
  /**
   * set deterministic uniform mutation 
   *
   * @param _eps range for real uniform mutation  
   * @param weight 
   */ 
  void setNormalMutation(double _sigma,double weight=0.5) {
    if(this->INITIALIZED)
      std::cerr << "setNormalMutation(): object already initialized\n";
    SIGMA = _sigma;
    nmut = new eoNormalMutation<EORVT>(this->SIGMA);
    if(mutation == 0) {
      mutation = new eoPropCombinedMonOp<EORVT>(*nmut,weight); 
    } else {
      mutation->add(*nmut,weight); 
    }
  }

  
 protected:

  /**
   * Initialize population search
   */
  virtual void initPS() { 
    if(select == 0) {
      select = new eoDetTournamentSelect<EORVT>(T_SIZE);
    }
    if(xover == 0)
      xover  = new eoSegmentCrossover<EORVT>();
    if(mutation == 0) {
      eoNormalMutation<EORVT> nmut(this->SIGMA);
      mutation = new eoPropCombinedMonOp<EORVT>(nmut,0.5); 
    }
    sga = new eoSGA<EORVT>(*select,*xover,this->CROSS_RATE,
			   *mutation,this->MUT_RATE,this->eval,this->continuator);
  }


  eoSelectOne<EORVT>*   select;      ///> Selection Criteria 
  eoQuadOp<EORVT>*      xover;       ///> Crossover 
  eoPropCombinedMonOp<EORVT>*       mutation;  ///> mutation
  eoUniformMutation<EORVT>*         umut;      ///> uniform mutation
  eoDetUniformMutation<EORVT>*      dumut;     ///> deterministic uniform mutation 
  eoNormalMutation<EORVT>*          nmut;      ///> normal mutation
  eoSGA<EORVT>*         sga;         ///> GA algorithm
  unsigned int          T_SIZE;      ///>
  double                EPSILON;     ///> range for real uniform mutation  
  double                SIGMA;	     ///> std dev. for normal mutation
};

#endif
