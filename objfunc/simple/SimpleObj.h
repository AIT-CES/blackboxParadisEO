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

#ifndef _SimpleObj_h
#define _SimpleObj_h

#include "../eoObjFunc.h"

/**
 * \file SimpleObj.h
 * \class SimpleObj
 * A simple objective function as a benchmark function for real valued optimization
 */ 
class SimpleObj: public RSRRMVOF {

protected:

  virtual double eval(const std::vector<double>& _arg) {
    double sum = 0; 
    for (unsigned i = 0; i < _arg.size(); i++) {
      sum += 2 * _arg[i] * _arg[i];
    }
    return sum;
  } 

};

#endif
