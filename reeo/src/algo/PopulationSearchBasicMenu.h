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
 * \file  PopulationSearchBasicMenu.h
 * \class PopulationSearchBasicMenu 
 * 
 * A class for evolutionary algorithm associated with a basic menu
 * 
 * @author: Atiyah Elsheikh
 * @date: Jan. 2015
 * last changes: Jan. 2015
 */ 


#ifndef _PopulationSearchBasicMenu_h
#define _PopulationSearchBasicMenu_h

// declaration of the namespace
using namespace std;

// standard includes
#include <typeinfo>
#include <string.h>

// the general include for eo
#include <eo>
#include "utils/eoParser.h"

// include for NLP
#include "remo/moRealTypes.h"

// include for solution initialization
#include "util/Utilities.h"

#include "PopulationSearchManager.h"

template<class eoObjFunc> 
  class PopulationSearchBasicMenu {

 public:
  
  /**
   * basic Cor. basic menu default value using the program name 
   * 
   * @param _argc number of command line arguments 
   * @param _argv command line arguments 
   */
  PopulationSearchBasicMenu(int _argc,char** _argv)
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
  PopulationSearchBasicMenu(int _argc,char** _argv,const string& _filename) 
    : parser(_argc,_argv),manager(0) { 
    init(_filename);
  } 

  /**
   * Dcor.
   */
  ~PopulationSearchBasicMenu() {
    delete manager; 
    for(int i=0;i<parameters.size();i++)
      delete parameters[i];
  }

 protected:

  eoParser parser;                                ///> Menu parser 

  PopulationSearchManagerEA<eoObjFunc>* manager;  ///> Population search; 

  vector<eoParam*> parameters;                    ///> parameters  of the menu

 private: 
  /// prevent copy cor
  PopulationSearchBasicMenu(const PopulationSearchBasicMenu&); 
  
  /// prevent = operator 
  PopulationSearchBasicMenu& operator=(const PopulationSearchBasicMenu&);


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
    T processFlag(const T& def,const char* cmd,const char* comment,
		  bool req=false,const char* menu="") 
    {
      eoValueParam<T>* param = new eoValueParam<T>(def,cmd,comment,'\0',req);
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
    const char* SCMENU   = "Stopping Criteria";
    const char* EAMENU   = "Evolutionary Algorithm";

     // execute the local search
    bool execute  = processFlag<bool>(false,"execute","Execute the local search");

    // random seed parameter
    uint32_t seed = processFlag<uint32_t>(time(0), "seed", "Random number seed");
   
    // problem size
    unsigned N = processFlag<unsigned>(unsigned(10),"vecSize","The number of variables ",false,FUNCMENU);

    unsigned popSize = processFlag<unsigned>(20,"popSize","Population size",false,EAMENU);

     // boundaries box 
    eoValueParam<eoRealVectorBounds> boundsParam(
			 eoRealVectorBounds(N, -100, 100),
			 "initBounds",
			 "Bounds for initialization (MUST be bounded)");    
    parser.processParam(boundsParam, FUNCMENU);

    unsigned maxGen = processFlag<unsigned>(500,"maxGen", "Maximum number of generations",false,SCMENU);

   // operators probabilities at the algorithm level
    double pCross = processFlag<double>(double(0.8), "pCross", "Probability of Crossover",false,EAMENU);

    double pMut = processFlag<double>(double(0.5), "pMut", "Probability of Mutation",false,EAMENU);

    string str_status = _filename + ".status"; 
    string statusParam = processFlag<string>(str_status,"status","Status file",false,"Persistence");

    eoRealVectorBounds& temp = boundsParam.value();
    eoRealVectorBounds bounds;
    bounds.resize(temp.size());
    for(int i=0;i<temp.size();i++) 
      bounds[i] = temp[i];
    
    vector<double> lowBounds,uppBounds; 
    Utilities::getBounds(bounds,lowBounds,uppBounds);

    manager = new PopulationSearchManagerEA<eoObjFunc>(lowBounds,uppBounds,
						       popSize,maxGen,seed,pCross,pMut);
    
    uint32_t maxeval = processFlag<uint32_t>(10000,"maxeval","max. number of function's evaluations",false,SCMENU);  
    manager->setMaxFuncEval(maxeval);

     unsigned minGen = processFlag<unsigned>(unsigned(100), "minGen", "Minimum number of generations",false,SCMENU);
    unsigned steadyGen = processFlag<unsigned>(unsigned(50), "steadyGen", "Number of generations with no improvement",false,SCMENU);
    manager->setSteadySC(minGen,steadyGen);

    unsigned tSize = processFlag<unsigned>(2, "tSize", "Tournament size in [2,popSize[",false,EAMENU); 
    manager->setTournamentSelect(tSize);

    // relative rates for crossovers
    double hypercubeRate = processFlag<double>(double(1), "hypercubeRate", "Relative rate for hypercube crossover", false, EAMENU );
    double segmentRate = processFlag<double>(double(1), "segmentRate", "Relative rate for segment crossover",false, EAMENU );

    manager->setSegmentCrossover(segmentRate);
    manager->setHypercubeCrossover(hypercubeRate);

    unsigned nUniformMutations = processFlag<unsigned>(1,"nUniformMutation","Num. of uniform mutations",false,EAMENU);
    for(int i=1; i<=nUniformMutations;i++) {
      std::ostringstream s1,s2;
      s1 << "uniformMutWeight" << i;
      s2 << "epsilon" << i;
      string str1 = s1.str();
      string str2 = s2.str();
      double weight = processFlag<double>(1.0,str1.c_str(),"uniform mutation weight",false,EAMENU);
      double epsilon = processFlag<double>(i==1?1.0:1.0/((i-1)*10.0),str2.c_str(),"range for uniform mutation",false,EAMENU);
      manager->setUniformMutation(epsilon,weight);
    }

    unsigned nDetUniformMutations = processFlag<unsigned>(1,"nDetUniformMutation","Num. of deterministic uniform mutations",false,EAMENU);
    for(int i=1; i<=nDetUniformMutations;i++) {
      std::ostringstream s1,s2;
      s1 << "detUniformMutWeight" << i;
      s2 << "detEpsilon" << i;
      string str1 = s1.str();
      string str2 = s2.str();
      double weight = processFlag<double>(1.0,str1.c_str(),"deterministic uniform mutation weight",false,EAMENU);
      double epsilon = processFlag<double>(i==1?1.0:1.0/((i-1)*10.0),str2.c_str(),"range for uniform mutation",false,EAMENU);
      manager->setDetUniformMutation(epsilon,weight);
    }

    unsigned nNormalMutations = processFlag<unsigned>(1,"nNormalMutation","Num. of normal mutations",false,EAMENU);
    for(int i=1; i<=nNormalMutations;i++) {
      std::ostringstream s1,s2;
      s1 << "normalMutWeight" << i;
      s2 << "sigma" << i;
      string str1 = s1.str();
      string str2 = s2.str();
      double weight = processFlag<double>(1.0,str1.c_str(),"normal mutation weight",false,EAMENU);
      double sigma = processFlag<double>(i==1?1.0:1.0/((i-1)*10.0),str2.c_str(),"range for uniform mutation",false,EAMENU);
      manager->setNormalMutation(sigma,weight);
    }

    {
      string selection = processFlag<string>(string("DetTour"),"selection","Selection: DetTour, StochTour or Roulette",false,EAMENU);
      
      if(!selection.compare("DetTour")) {
	unsigned tsize = processFlag<unsigned>(2,"tsize","tournament size [2,PopSize]",false,EAMENU);
	manager->setTournamentSelect(tsize);
      } else if(!selection.compare("StochTour")) {
	unsigned tsize = processFlag<unsigned>(2,"tsize","tournament size [2,PopSize]",false,EAMENU);
	manager->setStochTournamentSelect(tsize);
      } else if(!selection.compare("Roulette")) {
	  manager->setProportionalSelect();
      } else 
	  std::cerr << "selection: DetTour, StochTour or Roulette\n";
    }

    {
      string replacement = processFlag<string>(string("Comma"),"replacement","Replacement: Comma, Plus, EPTour, SSGAWorst, SSGADet",false,EAMENU);
      if(!replacement.compare("Comma")) {
	manager->setCommaReplacement();
      } else if(!replacement.compare("Plus")) {
	manager->setPlusReplacement();
      } else if(!replacement.compare("EPTour")) {
		unsigned ttsize = processFlag<unsigned>(2,"ttsize","tournament size [2,PopSize]",false,EAMENU);
	manager->setEPReplacement(ttsize);
      } else if(!replacement.compare("SSGAWorst")) {
	manager->setSSGAWorseReplacement(); 
      } else if(!replacement.compare("SSGADet")) { 
	unsigned ttsize = processFlag<unsigned>(2,"ttsize","tournament size [2,PopSize]",false,EAMENU);
	manager->setSSGADetTournamentReplacement(ttsize);
      } else 
	std::cerr << "Replacement : Comma, Plus, EPTour, SSGAWorst, SSGADet\n";
    }
     
    
    if (parser.userNeedsHelp()) {
      parser.printHelp(cout);
      return; 
    }
    
    if (!statusParam.empty()) {
      ofstream os(statusParam.c_str());
      os << parser;// and you can use that file as parameter file
    }
    
    cout << "Initial population : \n"; 
    eoPop<EORVT> population = manager->getPopulation(); 
    population.printOn(cout);

    if(execute) {
      manager->init();
      manager->run();
      manager->printOn();
    } 

  }
};

#endif
