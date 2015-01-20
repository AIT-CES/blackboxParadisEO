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
 * \file  LocalSearchManager.h
 * 
 * @author: Atiyah Elsheikh
 * @date: Oct. 2014
 * last changes : Jan. 2015
 */ 


#ifndef _localSearchManager_h
#define _localSearchManager_h


#include <typeinfo>
#include <iostream>   // cout
#include <ostream>    
#include <limits>
#include "remo/moRealTypes.h"


/**
 * max value of unsigned integer
 * 
 * @return max value 
 */
static unsigned int getMaxUnsignedInt();
static unsigned int getMaxUnsignedInt() { 
    return std::numeric_limits<unsigned>::max() - 2;  
}

/** 
 * \class BaseLocalSearchManager 
 * 
 * Local Search Manager parameterizable with arbitrary local search types and objective function
 */ 
template<class LocalSearch,class eoObjFunc>
class BaseLocalSearchManager {

public: 

  /**
   * default Constructor
   * 
   * @param _initial initial solution
   * @param _numNeighbors number of neighbors 
   * @param _boundaryRadius neighborhood boundary radius 
   * @param _maxiter maximum number of iteration
   */ 
 BaseLocalSearchManager(const EORVT& _initial,uint32_t _numNeighbors,double _boundaryRadius,unsigned int _maxiter=getMaxUnsignedInt()) 
   : ls(0),solution(_initial),mainEval(),eval(mainEval),neighborEval(eval),neighborhood(_numNeighbors,_boundaryRadius),initialized(false),maxiter(_maxiter),sc(maxiter),maxeval(0) { } 

  /// Destructor 
  virtual ~BaseLocalSearchManager() 
  { 
   try {
      delete ls;
      delete maxeval;
    }
    catch (int e) {
      std::cerr << "exception at Destructor of LocalSearchManager : Nr. " << e << std::endl;
      std::abort();
    }
  }

  /// run the algorithm 
  void run() {
     if(initialized)
       (*ls)(solution);
     else
       std::cerr << "initialization should be done by calling BaseLocalSearchManager::init()" << std::endl;
  }
  
  /// print 
  void printOn(std::ostream& _os=std::cout) const {
    _os << "\n ============================================================ \n"; 
    _os << "\n" << ls->className() << " solution : \n";
    solution.printOn(_os);
    _os << "\n obj func cntr : " << eval.value() << std::endl;
   }

  /// Initialize the object
  virtual void init() {
    initLS();
    if(!this->initialized) { 
      this->ls->setContinuator(this->sc);
    }
    this->initialized = true;
    this->solution.invalidate();
  }

  /**
   * Initialize the Local search, with a maximum allowed number of function evaluations
   * 
   * @param _maxFullEval: maximum number of function evaluations 
   */
  virtual void init(unsigned int _maxFullEval) {
    maxeval = new MaxNumEval(eval,_maxFullEval);
    if(!this->initialized) this->sc.add(*maxeval);
    init();
  }

// TODO 
// virtual void init(EORVT& _initSol);
// virtual void init(EORVT& _initSol&,GoalFunc&);
// virtual void init(EORVT& _lowBound,EORVT& _uppBound); 


  /// return the class name of the local search algorithm
  virtual std::string localSearchClassName() { 
    if(this->initialized)
      return ls->className();
    else 
      return std::string("");
  }


protected:
 
  /// allocate LocalSearch ls
  virtual void initLS()=0; 

  /// solution 
  EORVT solution;

  /// whether the manager is initialized and ready to run the local search
  bool initialized; 

  /// main goal function depending on realfunc.h 
  eoObjFunc mainEval; 

  /// neighborhood object 
  Neighborhood neighborhood;  

  /// goal function counter 
  GoalFuncCounter eval; 

  /// goal function for solution neighbors 
  NeighborEval neighborEval; 

  /// The local search 
  //LocalSearch* ls; 
  moLocalSearch<Neighbor>* ls;

  /// Maximum num. iteration 
  MaxNumIter maxiter; 

  /// Maxumum num. function evaluation
  MaxNumEval* maxeval; 

  /// Stopping Criteria
  StoppingCriteria sc; 

 private:
  
  /// prevent copy cor
  BaseLocalSearchManager(const BaseLocalSearchManager&); 

  /// prevent copy assignment
  BaseLocalSearchManager& operator=(const BaseLocalSearchManager&); 

};



/** 
 * \class BaseLocalSearchManager 
 * 
 * Standard Local search Manager 
 */ 
template<class LocalSearch,class eoObjFunc>
  class LocalSearchManager : public BaseLocalSearchManager<LocalSearch,eoObjFunc>{

 public: 

 /**
   * default Constructor
   * 
   * @param _initial initial solution
   * @param _numNeighbors number of neighbors 
   * @param _boundaryRadius neighborhood boundary radius 
   * @param _maxiter maximum number of iteration
   */ 
 LocalSearchManager(const EORVT& _initial,
		    uint32_t _numNeighbors=50,
		    double _boundaryRadius=0.1,
		    unsigned int _maxiter = getMaxUnsignedInt()
		    ) : BaseLocalSearchManager<LocalSearch,eoObjFunc>(_initial,_numNeighbors,_boundaryRadius,_maxiter) {}

  /// Initialize the object
  virtual void initLS() {
    if(!this->initialized) {
      if(typeid(LocalSearch) == typeid(TabuSearch)) { // does not compile for tabu search, because it does not have adequate constructor with 3 arguments ... 
	this->ls = new TabuSearch (this->neighborhood,this->eval,this->neighborEval,3,10);
      }
      else { 
	this->ls = new LocalSearch(this->neighborhood,this->eval,this->neighborEval);
      }
    }
  }

  protected:
};


/** 
 * \class LocalSearchManagerTS 
 * 
 * Local search Manager for TabuSearch 
 */ 
template<class eoObjFunc>
class LocalSearchManagerTS: public LocalSearchManager<TabuSearch,eoObjFunc> {

public:

  /**
   * default Constructor
   * 
   * @param _initial initial solution
   * @param _numNeighbors number of neighbors 
   * @param _boundaryRadius neighborhood boundary radius 
   * @param _maxiter maximum number of iteration
   */  
  LocalSearchManagerTS(const EORVT& _initial,
		      uint32_t _numNeighbors=50,double _boundaryRadius=0.1,
		       unsigned int _maxiter = getMaxUnsignedInt()) : LocalSearchManager<TabuSearch,eoObjFunc>(_initial,_numNeighbors,_boundaryRadius,_maxiter),time(3),tabuListSize(1000) { }

  /// Initialize the object
  virtual void initLS() {
    if(!this->initialized) {
      this->ls = new TabuSearch (this->neighborhood,this->eval,this->neighborEval,time,tabuListSize);
    }
  }

  /**
   * set Time limit 
   */ 
  void setTimeLimit(unsigned int _time) {
    time = _time;
  }

  /**
   * set Time limit 
   */ 
  void setTabuListSize(unsigned int _tabuListSize) {
    tabuListSize = _tabuListSize;
  }

 protected:
  
  /** time limit of for stopping criteria */ 
  unsigned int time; 

  /** tabu list size of the tabu list*/ 
  unsigned int tabuListSize;
 
};


/** 
 * \class LocalSearchManagerTS 
 * 
 * Local search Manager for TabuSearch 
 */ 
template<class eoObjFunc>
class LocalSearchManagerSA: public LocalSearchManager<SimulatedAnnealing,eoObjFunc> {

public:

  /**
   * default Constructor
   * 
   * @param _initial initial solution
   * @param _numNeighbors number of neighbors 
   * @param _boundaryRadius neighborhood boundary radius 
   * @param _maxiter maximum number of iteration
   */  
  LocalSearchManagerSA(const EORVT& _initial,
		       uint32_t _numNeighbors=50,double _boundaryRadius=0.1,
		       unsigned int _maxiter = getMaxUnsignedInt()) : LocalSearchManager<SimulatedAnnealing,eoObjFunc>(_initial,_numNeighbors,_boundaryRadius,_maxiter),initT(10),alpha(0.9),span(100),finalT(0.01) { }

  /// Initialize the object
  virtual void initLS() {
    if(!this->initialized) {
      this->ls = new SimulatedAnnealing(this->neighborhood,this->eval,this->neighborEval,initT,alpha,span,finalT);
    }
  }

  /**
   * set initial temperature of cooling schedule 
   */ 
  void setInitTemp(double _initT) {
    initT = _initT;
  }

  /**
   * set factor of decreasing for cooling schedule
   */ 
  void setAlpha(double _alpha) {
    alpha = _alpha;
  }

  /**
   * set number of iteration with equal temperature for cooling schedule 
   */
  void setSpan(unsigned _span) {
    span = _span; 
  }

  /**
   * set final temperature 
   */
  void setFinalTemp(double _finalT) {
    finalT = _finalT;
  }


 protected:

  /** initial temperature for cooling schedule */
  double initT;
  
  /** factor of decreasing for cooling schedule*/
  double alpha;
  
  /** number of iteration with equal temperature for cooling schedule */
  unsigned span;
  
  /** final temperature*/ 
  double finalT;
  
};



#endif
