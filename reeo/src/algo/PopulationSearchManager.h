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
    : POP_SIZE(_POP_SIZE),MAX_GEN(_MAX_GEN),INITIALIZED(false),eval(evalFunc) { 
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
   : POP_SIZE(_pop.size()),MAX_GEN(_MAX_GEN),pop(_pop),INITIALIZED(false),eval(evalFunc) {  }

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
  

  void setSeed(unsigned int _seed = time(0)) { 
    rng.reseed(_seed);
  }

  /**
   * execution
   */ 
  virtual void run()=0; 


   /// print 
  virtual void printOn(std::ostream& _os=std::cout) const {
    _os << "\n ============================================================ \n";
    pop.printOn(_os);
    _os << "\n obj func cntr : " << eval.value() << std::endl;
   }

 protected:

  virtual void initPS()=0;
  
  eoPop<EORVT> pop;         ///> population 
  eoObjFunc    evalFunc;        ///> objective function
  eoEvalFuncCounter<EORVT> eval;
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
    CROSS_RATE(_CROSS_RATE),MUT_RATE(_MUT_RATE),genCont(_MAX_GEN),continuator(genCont)
    // checkpoint(0),counter(0),increment(counter),monitor(false) 
    {  }
  
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
    CROSS_RATE(_CROSS_RATE),MUT_RATE(_MUT_RATE),genCont(_MAX_GEN),continuator(genCont)
    // checkpoint(0),counter(0),increment(counter),monitor(false) 
  {  }


  /// Destructor 
  virtual ~PopulationSearchManagerGA() {
    try {
      //delete checkpoint;
      delete steadyCont;
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
    steadyCont = new eoSteadyFitContinue<EORVT>(MIN_GEN,STEADY_GEN);
    continuator.add(*steadyCont);
  }

  /**
   * set stopping criteria max num. of function evaluation
   *
   * @param _maxeval max. num. of function evaluation
   */ 
  void setMaxFuncEval(unsigned int _maxeval) {
    if(this->INITIALIZED) std::cerr << "setMaxFuncEval::object already initialized\n";
    eoEvalContinue<EORVT> max(this->eval,_maxeval);
    continuator.add(max);
  }

  /**
   *
   */ 
  virtual void init() { 
    if(this->INITIALIZED) return;
    BasePopulationSearchManager<eoObjFunc>::init();
    /*checkpoint = new eoCheckPoint<EORVT>(continuator);
    checkpoint->add(increment);
    checkpoint->add(bestStat);
    checkpoint->add(secondStat);
    checkpoint->add(monitor);
    monitor.add(this->eval);
    monitor.add(bestStat);
    monitor.add(secondStat);*/
  }
  
 protected: 

  float CROSS_RATE;                       ///> Crossover rate
  float MUT_RATE;                         ///> mutation rate 
  eoGenContinue<EORVT> genCont;           ///> stopping criteria 
  eoCombinedContinue<EORVT> continuator;  ///> stopping criteria 
  eoSteadyFitContinue<EORVT>* steadyCont; ///> stopping criteria

  /*eoCheckPoint<EORVT>* checkpoint;
  unsigned int counter;
  eoIncrementor<unsigned> increment; 
  eoBestFitnessStat<EORVT> bestStat;      ///> Best fitness in population
  eoSecondMomentStats<EORVT> secondStat;  ///> Second moment stats: average and stdev
  eoStdoutMonitor monitor;  */

  /*   
   eoValueParam<unsigned> generationCounter(0,"Gen.");
   eoIncrementor<unsigned> increment(generationCounter.value()); 

              ///> The Stdout monitor
   eoFileMonitor fileMonitor("stats.xg"," "); ///> The file monitor
   checkpoint.add(fileMonitor);
   monitor.add(generaationCounter);
   eoState outState;
   outState.registerObject(parser);
   outState.registerObject(pop);
   outState.registerObject(png);
   fileMonitor.add() .... 
   eoCountedStateSaver stateSaver1(20,outState,"generation");
   eoTimedStateSaver   stateSaver2(1,outState,"time");
   checkpoint.add(stateSaver1);
   checkpoint.add(stateSaver2);
   eoEasyEA gga(checkpoint,...);
   */

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
    selectOne(0),xover(0),mutation(0),sga(0),EPSILON(0.01),SIGMA(0.3),
    sxover(0),hxover(0) {  }
  
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
    selectOne(0),xover(0),mutation(0),sga(0),EPSILON(0.01),SIGMA(0.3),
    sxover(0),hxover(0) {}


  /// Destructor 
  virtual ~PopulationSearchManagerSGA() {
    try {
      delete sga;
      delete selectOne;
      delete mutation;
      for(int i=0;i<nmut.size();i++)
	delete nmut[i];
      for(int i=0;i<umut.size();i++)
	delete umut[i];
      for(int i=0;i<dumut.size();i++)
	delete dumut[i];
      delete xover;
      delete sxover;
      delete hxover; 
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
    PopulationSearchManagerGA<eoObjFunc>::init();
  }

  /**
   * execute 
   */ 
  virtual void run() { 
    (*sga)(this->pop);
    this->pop.sort();
    //cout << "final population \n" << this->pop << endl;
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
    delete selectOne; 
    selectOne = new eoDetTournamentSelect<EORVT>(T_SIZE);
  }

  /**
   * Set roulette wheel selection (does not work with minimizing fitness)
   */ 
  void setProportionalSelect() { 
    if(this->INITIALIZED) 
      std::cerr << "Population search object already initialized \n";
    delete selectOne; 
    selectOne = new eoProportionalSelect<EORVT>();
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
    delete selectOne; 
    selectOne = new eoStochTournamentSelect<EORVT>(rate);
  }


  /**
   * Set random selection
   */ 
  void setRandomSelect() {
    if(this->INITIALIZED) 
      std::cerr << "Population search object already initialized \n";
    delete selectOne; 
    selectOne = new eoRandomSelect<EORVT>();
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
    umut.push_back(new eoUniformMutation<EORVT>(EPSILON));
    if(mutation == 0) {
      mutation = new eoPropCombinedMonOp<EORVT>(*umut[0],weight); 
    } else {
      mutation->add(*umut[umut.size()-1],weight); 
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
    dumut.push_back(new eoDetUniformMutation<EORVT>(EPSILON));
    if(mutation == 0) {
      mutation = new eoPropCombinedMonOp<EORVT>(*dumut[0],weight); 
    } else {
      mutation->add(*dumut[dumut.size()-1],weight); 
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
    nmut.push_back(new eoNormalMutation<EORVT>(this->SIGMA));
    if(mutation == 0) {
      mutation = new eoPropCombinedMonOp<EORVT>(*nmut[0],weight); 
    } else {
      mutation->add(*nmut[nmut.size()-1],weight,true); 
    }
  }

  /**
   *
   */ 
  void setSegmentCrossover(double weight) {
    if(this->INITIALIZED)
      std::cerr << "setSegmentCrossover(): object already initialized\n";
    if(sxover != 0) 
      std::cerr << "setSegmentCrossover(): has been called before\n";
    if(xover == 0) {
      sxover = new eoSegmentCrossover<EORVT>(); 
      xover= new eoPropCombinedQuadOp<EORVT>(*sxover,weight); 
    } else {
      sxover = new eoSegmentCrossover<EORVT>(); 
      xover->add(*sxover,weight); 
    }
  }

  
  /**
   *
   */ 
  void setHypercubeCrossover(double weight) {
    if(this->INITIALIZED)
      std::cerr << "setHypercubeCrossover(): object already initialized\n";
    if(hxover != 0) 
      std::cerr << "setHypercubeCrossover(): has been called before\n";
    if(xover == 0) {
      hxover = new eoHypercubeCrossover<EORVT>(); 
      xover= new eoPropCombinedQuadOp<EORVT>(*hxover,weight); 
    } else {
      hxover = new eoHypercubeCrossover<EORVT>(); 
      xover->add(*hxover,weight); 
    }
  }

  
 protected:

  /**
   * Initialize population search
   */
  virtual void initPS() { 
    if(selectOne == 0) {
      selectOne = new eoDetTournamentSelect<EORVT>(T_SIZE);
    }
    if(xover == 0) {
      sxover = new eoSegmentCrossover<EORVT>();
      xover  = new eoPropCombinedQuadOp<EORVT>(*sxover,1.0);
    }
    if(mutation == 0) {
      nmut.push_back(new eoNormalMutation<EORVT>(this->SIGMA));
      mutation = new eoPropCombinedMonOp<EORVT>(*nmut[0],1.0); 
    }
    sga = new eoSGA<EORVT>(*selectOne,*xover,this->CROSS_RATE,
			   *mutation,this->MUT_RATE,this->eval,this->continuator);
  }


  eoSelectOne<EORVT>*   selectOne;                ///> Selection Criteria 

  // eoQuadOp<EORVT>*      xover;              ///> Crossover 
  eoPropCombinedQuadOp<EORVT>*      xover;     ///> Crossover 
  eoSegmentCrossover<EORVT>*        sxover;    ///> Segment Crossover
  eoHypercubeCrossover<EORVT>*      hxover;    ///> Hybercube Crossover 

  eoPropCombinedMonOp<EORVT>*            mutation;  ///> mutation
  vector<eoUniformMutation<EORVT>*>      umut;      ///> uniform mutation
  vector<eoDetUniformMutation<EORVT>*>   dumut;     ///> deterministic uniform mutation 
  vector<eoNormalMutation<EORVT>*>       nmut;      ///> normal mutation

  unsigned int          T_SIZE;      ///>
  double                EPSILON;     ///> range for real uniform mutation  
  double                SIGMA;	     ///> std dev. for normal mutation

 private:
  eoSGA<EORVT>*         sga;         ///> GA algorithm

};



template<class eoObjFunc>
  class PopulationSearchManagerEA : public PopulationSearchManagerSGA<eoObjFunc>{

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
  PopulationSearchManagerEA(const vector<double>& _lowerBound,
			     const vector<double>& _upperBound,
			     unsigned int _POP_SIZE=20,
			     unsigned int _MAX_GEN=500,
			     unsigned int _SEED=time(0),
			     float _CROSS_RATE=0.8,
			     float _MUT_RATE = 0.5) 
    : PopulationSearchManagerSGA<eoObjFunc>(_lowerBound,_upperBound,
					   _POP_SIZE,_MAX_GEN,_SEED,
					   _CROSS_RATE,_MUT_RATE),
    transform(0),select(0),replace(0),ea(0),offspringrate(1.0)
     {  }
  
  /**
   * Basic constructor
   *
   * @param _pop        initial population 
   * @param _MAX_GEN    maximum number of generations
   * @param _CROSS_RATE Crossover rate
   * @param _EPSILON    range for real uniform mutation
   * @param _MUT_RATE   mutation rate 
   */
 PopulationSearchManagerEA(eoPop<EORVT> _pop,
			   unsigned int _MAX_GEN=500,
			   float _CROSS_RATE=0.8,
			   float _MUT_RATE = 0.5) 
   : PopulationSearchManagerSGA<eoObjFunc>(_pop,_MAX_GEN,_CROSS_RATE,_MUT_RATE),
    transform(0),select(0),replace(0),ea(0),offspringrate(1.0) {}


  /// Destructor 
  virtual ~PopulationSearchManagerEA() {
    try {
      delete select;
      delete replace;
      delete transform;
      delete ea;
    }
    catch (int e) {
      std::cerr << "exception at Destructor of BasePopulationSearchManager : Nr. " << e << std::endl;
      std::abort();
    }
  }

  /**
   *
   */
  virtual void run() {
    (*ea)(this->pop);
    this->pop.sort();
    //cout << "final population \n" << this->pop << endl;
  }


  /**
   * Initialization 
   */ 
  virtual void init() {
    init(2,offspringrate);
  }

  /**
   * initialization
   *
   * @param tsize  size for tournament selection [2,POP_SIZE]
   */ 
  virtual void init(unsigned int _tsize,double _offspringrate) { 
    offspringrate = _offspringrate; 
    PopulationSearchManagerSGA<eoObjFunc>::init(_tsize);
  }
  
  /**
   * use comma replacement strategy (select the best offspring)
   */ 
  void setCommaReplacement() {
    if(this->INITIALIZED)
      std::cerr << "setCommaReplacement() : instance already initialized\n";
    delete replace;
    replace = new eoCommaReplacement<EORVT>();
  }

  /**
   * use plus replacement strategy (select the best offspring and parants)
   */ 
  void setPlusReplacement() {
    if(this->INITIALIZED)
      std::cerr << "setPlusReplacement() : instance already initialized\n";
    delete replace;
    replace = new eoPlusReplacement<EORVT>();
  }
  
  /**
   * use EP replacement strategy (stochastic tournament among offspring 
   * and parants)
   */ 
  void setEPReplacement(unsigned int _tsize=2) {
    if(this->INITIALIZED)
      std::cerr << "setEPReplacement() : instance already initialized\n";
    delete replace;
    replace = new eoEPReplacement<EORVT>(_tsize);
  }

  /**
   * use SSGA worse replacement strategy (worse father are replaced by 
   * best offspring)
   */ 
  void setSSGAWorseReplacement() {
    if(this->INITIALIZED)
      std::cerr << "setSSGAWorseReplacement() : instance already initialized\n";
    delete replace;
    replace = new eoSSGAWorseReplacement<EORVT>();
  }

  /**
   * use SSGA Deterministic Tournament replacement (tournament among offspring 
   * and parants)
   */ 
  void setSSGADetTournamentReplacement(unsigned int _tsize=2) {
    if(this->INITIALIZED)
      std::cerr << "setSSGADetTournamentReplacement() : instance already initialized\n";
    delete replace;
    replace = new eoSSGADetTournamentReplacement<EORVT>(_tsize);
  }

  /**
   * use SSGA Stochastic Tournament replacement (tournament among offspring 
   * and parants)
   */ 
  void setSSGAStochTournamentReplacement(double _rate = 1.0) {
    if(this->INITIALIZED)
      std::cerr << "setSSGAStochTournamentReplacement() : instance already initialized\n";
    delete replace;
    replace = new eoSSGAStochTournamentReplacement<EORVT>(_rate);
  }


  /**
   * use survive and die replacement strategy
   */ 
  void setSurviveAndDieReplacement(double _surviveP=0.5,double _dieP=.5,double _surviveO=0.5,double _dieO=0.5) {
    if(this->INITIALIZED)
      std::cerr << "setSurviveAndDieReplacement() : instance already initialized\n";
    delete replace;
    replace = new eoDeterministicSaDReplacement<EORVT>(_surviveP,_dieP,_surviveP,_dieP,true);
  }


  

 protected:
  
  /**
   *
   */ 
  virtual void initPS() { 
    PopulationSearchManagerSGA<eoObjFunc>::initPS();
    select = new eoSelectPerc<EORVT>(*(this->selectOne),offspringrate);
    if(replace == 0)
      replace = new eoGenerationalReplacement<EORVT>();
    transform = new eoSGATransform<EORVT>(*(this->xover),this->CROSS_RATE,*(this->mutation),this->MUT_RATE);
    ea = new eoEasyEA<EORVT>(this->continuator,this->eval,*select,*transform,*replace);
  }

  eoSGATransform<EORVT>*              transform; ///>
  eoSelectPerc<EORVT>*                select;    ///> Selection Criteria
  eoReplacement<EORVT>*   replace;   ///> Generational replacement

  double offspringrate;  ///> growing rate of offsprings w.r.t. population
 private: 
  
  eoEasyEA<EORVT>*                    ea;        ///> Evolutionary algorithm
  
};

#endif
