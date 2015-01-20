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
 * \file  LocalSearchBasicMenu.h
 * \class LocalSearchBasicMenu 
 * 
 * A class for arbitrary searches associated with a basic menu
 * 
 * @author: Atiyah Elsheikh
 * @date: Sep. 2014
 * last changes: Oct. 2014
 */ 

#ifndef _localSearchBasicMenu_h
#define _localSearchBasicMenu_h

// declaration of the namespace
using namespace std;

// standard includes
#include <typeinfo>

// the general include for eo
#include <eo>
#include "utils/eoParser.h"

// include for NLP
#include "remo/moRealTypes.h"

// include for solution initialization
#include "util/Utilities.h"

template<class LocalSearch, class eoObjFunc> 
  class LocalSearchBasicMenu {


 public: 
  /**
   * basic Cor. basic menu default value using the program name 
   * 
   * @param _argc number of command line arguments 
   * @param _argv command line arguments 
   */
  LocalSearchBasicMenu(int _argc,char** _argv)
    : parser(_argc,_argv),manager(0) { 
    init(parser.ProgramName());
  } 

  /**
   * basic Cor. basic menu using a user input file
   * 
   * @param _argc number of command line arguments 
   * @param _argv command line arguments
   + @param _filename: name of an input file for a user menu
   */
  LocalSearchBasicMenu(int _argc,char** _argv,const string& _filename) 
    : parser(_argc,_argv),manager(0) { 
    init(_filename);
  } 

  /**
   * Dcor.
   */
  ~LocalSearchBasicMenu() {
    delete manager; 
    for(int i=0;i<parameters.size();i++)
      delete parameters[i];
  }

 protected: 
  /// prevent empty cor
  LocalSearchBasicMenu() { } 
  
  /// Menu parser
  eoParser parser; 

  /// Local search; 
  BaseLocalSearchManager<LocalSearch,eoObjFunc>* manager;

  /// parameters  of the menu
  vector<eoParam*> parameters; 

 private:
  
  /// prevent copy cor
  LocalSearchBasicMenu(const LocalSearchBasicMenu&); 
  
  /// prevent = operator 
  LocalSearchBasicMenu& operator=(const LocalSearchBasicMenu&);

  /**
   * create and process a flag within a parser 
   * 
   * @param def      default value 
   * @param cmd      long format of command e.g. --cmd 
   * @param comment  comment to the command 
   * @param s        short format of comand e.g. -s
   * @param req      whether command is required 
   * @param menu     name of the menu of the command 
   */ 
  template<class T> 
    T processFlag(const T& def,const char* cmd,const char* comment,char s,
		  bool req=false,const char* menu="") 
    {
      eoValueParam<T>* param = new eoValueParam<T>(def,cmd,comment,s,req);
      parameters.push_back(param); 
      parser.processParam(*param,menu);
      return param->value();
    }

  /**
   * initialize the basic menu
   * 
   * @param: _filename name of the basic menu file name 
   */ 
  void init(const string& _filename) {
    
    const char* FUNCMENU = "Goal Function";
    const char* LSMENU   = "Local Search General";
    const char* TSMENU   = "Tabu Search";
    const char* SCMENU   = "Stopping Criteria";
    const char* SAMENU   = "Simulated Annealing";

    // execute the local search
    bool execute  = processFlag<bool>(false,"execute","Execute the local search",'E');

    // random seed parameter
    uint32_t seed = processFlag<uint32_t>(time(0), "seed", "Random number seed", 'S');
   
    // problem size
    unsigned N = processFlag<unsigned>(unsigned(10),"vecSize","The number of variables ",'n',false,FUNCMENU);
    cout << "N : " << N << "\n"; 
    
    // boundaries box 
    eoValueParam<eoRealVectorBounds> boundsParam(
			 eoRealVectorBounds(N, -100, 100),
			 "initBounds",
			 "Bounds for initialization (MUST be bounded)",
			 'B');
    parser.processParam(boundsParam, FUNCMENU);
    
    // number of neighbors in a neighborhood
    uint32_t numNeighbors = processFlag<uint32_t>(50, "numneighbors", "Num. of neighbors", 'G',false,LSMENU);

    double boundaryRadius = processFlag<double>(0.1,"boundaryradius", "The radious of the neighborhood",'R',false,LSMENU);

    uint32_t maxiter = processFlag<uint32_t>(1000,"maxiter","max. number of iterations",'i',false,SCMENU);

    uint32_t maxeval = processFlag<uint32_t>(10000,"maxeval","max. number of function's evaluations",'e',false,SCMENU);
    
    string str_status = _filename + ".status"; 
    string statusParam = processFlag<string>(str_status,"status","Status file",'F',false,"Persistence");

    /*eoValueParam<string> statusParam(str_status.c_str(), "status", "Status file");
      parser.processParam( statusParam, "Persistence" );*/
    
    // @todo boundaryRadius
    /*  
     stopping conditions:
     stop on the number of iterations
     stop on the number of full evaluation
     stop on the number of neighbor evaluation
     stop on the fitness value reached
     stop on a time limit 
    */
    // see make_continue.h
    // the name of the "status" file where all actual parameter values will be saved
       
    uint32_t tabuListSize = 1000;
    uint32_t timeLimit = 3;

    double initT = 10.0; 
    double alpha = 0.9;
    unsigned span = 100; 
    double finalT = 0.01;

    eoRealVectorBounds& temp = boundsParam.value();
    eoRealVectorBounds bounds;
    bounds.resize(temp.size());
    for(int i=0;i<temp.size();i++) 
      bounds[i] = temp[i];

    // random seed 
    rng.reseed(seed);

    // initalSolution
    EORVT initialSolution; 

    vector<double> lowBounds,uppBounds; 
    Utilities::getBounds(bounds,lowBounds,uppBounds);
    Utilities::getRandomSolution(initialSolution,lowBounds,uppBounds);    
    cout << "Initial solution : \n"; 
    initialSolution.printOn(cout);
   
    // @todo check if type is TabuSearch, does not work for TS 
    if(typeid(LocalSearch) == typeid(TabuSearch)) {
      manager = (BaseLocalSearchManager<LocalSearch,eoObjFunc> *) new LocalSearchManagerTS<eoObjFunc>(initialSolution,numNeighbors,boundaryRadius,maxiter);
      timeLimit = processFlag<uint32_t>(timeLimit, "timeLimit", "time limit", 'T',false,TSMENU);
      tabuListSize = processFlag<uint32_t>(tabuListSize, "tabuListSize", "tabu list size", 'L',false,TSMENU);
      ((LocalSearchManagerTS<eoObjFunc>*) manager)->setTimeLimit(timeLimit);
      ((LocalSearchManagerTS<eoObjFunc>*) manager)->setTabuListSize(tabuListSize);
    } 
    else if(typeid(LocalSearch) == typeid(SimulatedAnnealing)) {
      manager = (BaseLocalSearchManager<LocalSearch,eoObjFunc> *) new LocalSearchManagerSA<eoObjFunc>(initialSolution,numNeighbors,boundaryRadius,maxiter);
      initT = processFlag<double>(initT, "initTemp", "initial temperature", 'I',false,SAMENU);
      alpha = processFlag<double>(alpha, "alpha", "factor of decreasing for cooling schedule", 'A',false,SAMENU);
      span = processFlag<unsigned>(span, "span", "number of iteration with equal temperature", 'S',false,SAMENU);
      finalT = processFlag<double>(finalT, "finalTemp", "final temperature", 'F',false,SAMENU);
      ((LocalSearchManagerSA<eoObjFunc>*) manager)->setInitTemp(initT);
      ((LocalSearchManagerSA<eoObjFunc>*) manager)->setAlpha(alpha);
      ((LocalSearchManagerSA<eoObjFunc>*) manager)->setSpan(span);
      ((LocalSearchManagerSA<eoObjFunc>*) manager)->setFinalTemp(finalT);
    }
    else {
      manager = new LocalSearchManager<LocalSearch,eoObjFunc>(initialSolution,numNeighbors,boundaryRadius);
    }
    
    if (parser.userNeedsHelp()) {
      parser.printHelp(cout);
      return; 
    }
    
    if (!statusParam.empty()) {
      ofstream os(statusParam.c_str());
      os << parser;// and you can use that file as parameter file
    }
    
    if(execute) {
      manager->init(maxeval);
      manager->run();
      manager->printOn();
    } 
  }
  
};

#endif
