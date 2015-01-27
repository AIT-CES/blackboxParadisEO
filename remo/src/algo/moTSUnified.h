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
 * \file moTSUnified.h
 * 
 * @author : Atiyah Elsheikh
 * @date   : Jan. 2015
 * last changes : Jan. 2015 
 */ 

#include "algo/moTS.h"

/**
 * \class moTSUnified 
 * 
 * Tabu search with unified constructor for LocalSearch
 */ 

template<class Neighbor>
class moTSUnified : public moTS<Neighbor>
{
public:

  typedef typename Neighbor::EOT EOT;
  typedef moNeighborhood<Neighbor> Neighborhood ;

  /**
   * Basic constructor for a tabu search
   * @param _neighborhood the neighborhood
   * @param _fullEval the full evaluation function
   * @param _eval neighbor's evaluation function
   * @param _time the time limit for stopping criteria
   * @param _tabuListSize the size of the tabu list
   */
  moTSUnified(Neighborhood& _neighborhood,
       eoEvalFunc<EOT>& _fullEval,
       moEval<Neighbor>& _eval,
       unsigned int _time=1,
       unsigned int _tabuListSize=10
       ): moTS<Neighbor>(_neighborhood,_fullEval,_eval,_time,_tabuListSize) 
       {}

  /**
   *
   */
  virtual std::string className() const {
    return std::string("moTSUnified");
  }

};
