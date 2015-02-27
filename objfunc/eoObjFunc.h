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
 * \file eoObjFunc.h
 * 
 * \author : Atiyah Elsheikh
 * \date : Oct. 2014
 * last changes : Jan. 2015
 */ 

#ifndef _eoObjFunc_h
#define _eoObjFunc_h

/// general outline 
/// check 

#include <eo>
#include "es/eoReal.h"
#include <vector>

/*
Reminder: 
class eoEvalFunc<EOT> {
   virtual void operator()(EOT& _eo);
};

class eoEvalFuncPtr<EOT,Fit,FArg> {
   eoEvalFuncPtr(Fit(*_eval)(FArg)); 
   virtual void operator()(EOT& _eo); // _eo.fitness( _eval((FArg)(_eo)))
};
*/ 
/**
 * \class eoObjFunc a generic class for prototyping objective functions 
 */
template<class EOT,class Fitness = typename EOT::Fitness, class FuncArg = const EOT&> 
class eoObjFunc : public eoEvalFunc<EOT> {

public:

  /**
   * Cor
   */ 
  eoObjFunc():INITIALIZED(false) {}

  /** 
   * evaluation
   * @param _eo argument
   */ 
  virtual void operator()(EOT& _eo) = 0;

  /** 
   * initialize 
   */ 
  virtual void init()=0;

  /**
   * whether obj function is initialized  
   */ 
  virtual bool isInitialized() {
    return INITIALIZED;
  }

 protected:
  virtual Fitness eval(FuncArg) = 0;

  bool INITIALIZED;

};  


/**
 * \class RSRRMVOF
 * real single return real multiple-valued objective function
 */ 
class RSRRMVOF : public eoObjFunc<EORVT,double,const std::vector<double>&> {
  
public:

  virtual void operator()(EORVT& _eo) {
    const std::vector<double>& v = (std::vector<double>)( _eo);
    _eo.fitness(this->eval(v));
  };

  virtual void init() {
    INITIALIZED = true;
  }

protected:
  //virtual double eval(const std::vector<double>& arg) = 0; 

};

#endif
