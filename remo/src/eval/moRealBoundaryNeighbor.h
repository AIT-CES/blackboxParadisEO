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
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>
*/ 

#ifndef _moRealBoundaryNeighbor_h
#define _moRealBoundaryNeighbor_h

#include "EO.h"
#include "es/eoReal.h"
#include "neighborhood/moBackableNeighbor.h"

/**
 * \file  moRealBoundaryNeighbor.h
 * \class moRealBoundaryNeighbor 
 * 
 * This class stores the movements eps to be applied to a solution s
 * s.t. s^new_i = s_i + eps_i 
 * eps is to be computed by a neighborhood class
 * 
 * \author : Atiyah Elsheikh 
 * date: Aug. 2014
 * last changes : Jan. 2015
 */ 
template<class Fitness>
class moRealBoundaryNeighbor : public moBackableNeighbor<eoReal<Fitness>,Fitness> , public std::vector<double> {
public: 
  
        typedef eoReal<Fitness> EOT; 
	typedef std::vector<double> EOVT;
	using EOVT::operator[];
        using EOVT::size;
  	
	/**
	 * Basic constructor
	 * @param size size of the real vector
	 * @param value initial value of the real vector
	 */
        moRealBoundaryNeighbor(unsigned _size=0,double _value=0.0): 
	moBackableNeighbor<EOT,Fitness>(),EOVT(_size,_value) {
	} 

	/**
	 * Copy Constructor
	 * @param _n the neighbor to copy
	 */
        moRealBoundaryNeighbor(const moRealBoundaryNeighbor& _n) :
	        moBackableNeighbor<EOT, Fitness> (_n),EOVT(_n) {
	}

	/**
	 * Assignment operator
	 * @param _source the source neighbor
	 */
        moRealBoundaryNeighbor<Fitness> & operator=(const moRealBoundaryNeighbor<Fitness> & _source) {
	 moBackableNeighbor<EOT, Fitness>::operator=(_source);
	 EOVT::operator=(_source);
	 return *this;
       }

	/**
	 * Return the class Name
	 * @return the class name as a std::string
	 */
	virtual std::string className() const {
		return "moRealBoundaryNeighbor";
	}
	
	/**
	 * @param _neighbor a neighbor
	 * @return if _neighbor and this one are equals
	 */
	virtual bool equals(moRealBoundaryNeighbor<Fitness>& _neighbor) const {
	  if(size() == _neighbor.size()) 
	    for(int i=0;i<size();i++)
	      if ( (_neighbor[i] <= operator[](i) + 1E-10) 
		   && (_neighbor[i] >= operator[](i) - 1E-10) ) continue; 
	      else 
		return false;
	  else 
	    return false; 
	  return true; 
	}

       	/**
	 * Write object with its index
	 * @param _os A std::ostream.
	 */
	virtual void printOn(std::ostream& _os) const {
	  
	  EO<Fitness>::printOn(_os); 
	  _os << ' '; 
	  _os << size() << ' ';
	  std::copy(begin(), end(), std::ostream_iterator<double>(_os, " "));
	
	}

	/**
	 * move the solution 
	 * @param _solution a solution 
	 */ 
	virtual void move(EOT& _solution) {
	  // solution elements will move according to randomly computed moves
	  for(int i=0;i<_solution.size();i++) { 
	    _solution[i] += operator[](i);
	  } 
	  _solution.invalidate();
	}

	/**
	 *
	 */ 
	virtual void moveBack(EOT& _solution) {
	  //std::cout << " n: "  << _solution << std::endl;
	  for(int i=0;i<_solution.size();i++) { 
	    _solution[i] -= operator[](i);
	  }
	  _solution.invalidate();
	}

	
 protected:
	

};


#endif 
