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


#ifndef _moRealBoundaryNeighborhood_h
#define _moRealBoundaryNeighborhood_h

#include <vector>
#include "neighborhood/moNeighborhood.h"
#include "utils/eoRndGenerators.h"

/**
 * \file moRealBoundaryNeighborhood.h
 * \class moRealBoundaryNeighborhood.h
 * 
 * A class for a neighborhood that generates neighbors g of a solution s
 * where g \in N_eps(s), N_eps(s) is specified via a random generator   
 * It works with classes of types moBackableNeighbor, e.g. moRealNeighbor
 * 
 * \todo a template for a generator 
 * 
 * @author: Atiyah Elsheikh
 * @date: Sep. 2014
 * last changes: Oct. 2014
 */
template <class Fitness>
class moRealBoundaryNeighborhood : virtual public moNeighborhood<moRealBoundaryNeighbor<Fitness> >
{
public:

  typedef moRealBoundaryNeighbor<Fitness> Neighbor; 
  
  /**
   * Define type of a solution corresponding to Neighbor
   */
  typedef typename Neighbor::EOT EOT;

  /**
   * Constructor
   * @param _maxNumNeighbors
   */
 moRealBoundaryNeighborhood(unsigned _maxNumNeighbors,double _eps)
   : moNeighborhood<Neighbor>(),maxNumNeighbors(_maxNumNeighbors),eps(_eps),numGenNeighbors(0) {
  }

  /**
   * Test if it exist a neighbor
   * @param _solution the solution to explore
   * @return true if the neighborhood was not empty (bit string larger than 0)
   */
  virtual bool hasNeighbor(EOT& _solution) {
    if(numGenNeighbors > maxNumNeighbors) {
      numGenNeighbors = 0; 
      return false;
    }
    return true;
    //return numGenNeighbors < maxNumNeighbors; 
  }

  /**
   * Initialization of the neighborhood
   * @param _solution the solution to explore
   * @param _current the first neighbor
   */
  virtual void init(EOT& _solution, Neighbor& _current) {
    numGenNeighbors = 0; 
    next(_solution,_current);
  }

  /**
   * Give the next neighbor
   * @param _solution the solution to explore
   * @param _current the next neighbor
   */
  virtual void next(EOT& _solution, Neighbor& _current) {
    if(_current.size() != _solution.size()) {
      _current.resize(_solution.size()); 
    }
    for(int i=0;i<_solution.size();i++) {
      double v = (generator()- 0.5) * eps * 2;
      _current[i] = v; //_solution[i] + v;
    }
    numGenNeighbors++;  
  }

  /**
   * Test if there is again a neighbor
   * @param _solution the solution to explore
   * @return true if there is again a neighbor not explored
   */
  virtual bool cont(EOT& _solution) {
    return hasNeighbor(_solution);
  }
  
  /**
   * Return the class Name
   * @return the class name as a std::string
   */
  virtual std::string className() const {
    return "moRealBoundaryNeighborhood";
  }

protected:
    /// the radius of the boundary around a solution to be explored
    double eps; 

    /// maximum number of neighbors 
    int   maxNumNeighbors; 

    /// number of generated neighbors 
    int   numGenNeighbors; 

    /// uniform generator
    eoUniformGenerator<double> generator;
};

#endif
