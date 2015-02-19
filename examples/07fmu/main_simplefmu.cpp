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
 * \file  main_simplefmu.cpp
 *  
 * Main driver for testing optimization strategy using simple menus 
 * 
 * @author: Atiyah Elsheikh
 * @date: Feb. 2015
 * @last changes: Feb. 2015
 */ 

// declaration of the namespace
using namespace std;

// the general include for eo
#include <eo>

// include for NLP
#include "remo/moRealTypes.h"
#include "objfunc/simplefmu/SimpleFMU.h"
#include "remo/src/algo/LocalSearchManager.h"
#include "remo/src/algo/localSearchBasicMenu.h"
typedef SimpleFMU  ObjFunc;


void main_function(int argc, char **argv)
{

  
  // LocalSearchBasicMenu<SimpleHillClimbing,ObjFunc> menu(argc,argv);
  // LocalSearchBasicMenu<FirstImprHillClimbing,ObjFunc> menu2(argc,argv);
  LocalSearchBasicMenu<SimulatedAnnealing,ObjFunc> menu3(argc,argv);
  // LocalSearchBasicMenu<TabuSearch,ObjFunc> menu4(argc,argv); //does not work at the moment

}


// A main that catches the exceptions
int main(int argc, char **argv)
{
    try {
        main_function(argc, argv);
    }
    catch (exception& e) {
        cout << "Exception: " << e.what() << '\n';
    }
    return 1;
}
