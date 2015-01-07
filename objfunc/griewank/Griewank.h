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

#ifndef _Griewank_h
#define _Griewank_h

#include "../eoObjFunc.h"


/**
 * \file Griewank.h
 * \class Griewank
 * Griewank benchmark function for real valued optimization
 */ 
class Griewank: public RSRRMVOF {

  virtual double eval(const std::vector<double>& _arg) {
    double sum = 0; 
    for (unsigned i = 0; i < _arg.size(); i++) {
      sum += _arg[i] * _arg[i];
    }
    sum /= 4000.0; 
    double prod;
    for (unsigned i = 0; i < _arg.size(); i++) {
      prod *= cos(_arg[i]/sqrt(i+1.0)); 
    }
    sum = sum - prod + 1;
    return sum;
  } 

};

#endif
