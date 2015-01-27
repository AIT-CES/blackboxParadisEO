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

#ifndef _Rosenbrock_h
#define _Rosenbrock_h

#include "../eoObjFunc.h"


/**
 * \file Rosenbrock.h
 * \class Rosenbrock
 * 
 * Rosenbrock benchmark function for real valued optimization
 */ 
class Rosenbrock : public RSRRMVOF {

protected:
  virtual double eval(const std::vector<double>& _arg) {
    double sum = 0.0; 
    for(unsigned i=0;i<_arg.size()-1;i++) {
      double t1 = _arg[i+1] - _arg[i] * _arg[i]; 
      double t2 = _arg[i] - 1; 
      sum += 100 * t1*t1 + t2*t2; 
    }
    return sum;
  } 

};

#endif
