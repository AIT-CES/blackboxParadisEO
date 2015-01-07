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

#ifndef _Utilities_h
#define _Utilities_h

#include "remo/moRealTypes.h"
#include "es/eoEsChromInit.h"


/**
 * \file Utilities.h
 * \class Utilities
 * 
 * A collection of utility routines 
 */ 
class Utilities {


public:
  /**
   *
   */ 
  static void getRandomSolution(EORVT& solution,const std::vector<double>& lowBounds,const std::vector<double>& uppBounds) {
    eoRealVectorBounds bounds(lowBounds,uppBounds); 
    eoEsChromInit<EORVT> init(bounds);
    init(solution);
    solution.invalidate();
  }

  /**
   *
   */ 
  static void getBounds(const eoRealVectorBounds& bounds, std::vector<double>& lowBounds,std::vector<double>& uppBounds) {
    lowBounds.resize(bounds.size());
    uppBounds.resize(bounds.size()); 
    for(int i=0;i<bounds.size();i++) {
      lowBounds[i] = bounds[i]->minimum();
      uppBounds[i] = bounds[i]->maximum();
    }
  }

}; 


#endif 
