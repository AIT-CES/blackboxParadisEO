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
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>
*/ 

/**
 * \file  realfunc.h
 *  
 * An implementation of a simple objective function
 *
 */


#include "remo/moRealTypes.h"
#include <vector>
//-----------------------------------------------------------------------------

/** 
 * Just a simple function that takes an vector<double> and sets the fitnes
 * to the sphere function. Please use doubles not float!!!
 * @param _ind A floatingpoint vector
 * @return fitness value
*/
double realfunc(const std::vector<double>& _ind)
{
  double sum = 0;
  for (unsigned i = 0; i < _ind.size(); i++) {
    sum += _ind[i] * _ind[i];
  }
  return sum;
}

/**
 * for testing purposes
 * 
 * @param _ind A floatingpoint vector
 * @return fitness value
 */ 
Fitness realfunc2(const EORVT& _ind)
//FitT realfunc(const std::vector<double>& _ind)
{
  double sum = 0;
  for (unsigned i = 0; i < _ind.size(); i++) {
    sum += 2 * _ind[i] * _ind[i];
  }
  return sum;
}
