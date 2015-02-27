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
 * \file  main_basicmenu.cpp
 *  
 * Main driver for testing evolutionary optimization strategy using simple menus 
 * 
 * @author: Atiyah Elsheikh
 * @date: Jan. 2015
 * @last changes: Jan. 2015
 */ 


// declaration of the namespace
using namespace std;

// the general include for eo
#include <eo>

// include for NLP
#include "remo/moRealTypes.h"
#include "reeo/src/algo/PopulationSearchManager.h"
#include "reeo/src/algo/PopulationSearchBasicMenu.h"

// objective functions 
//#include "objfunc/simple/SimpleObj.h"
//#include "objfunc/griewank/Griewank.h"
//#include "objfunc/rastrigin/Rastrigin.h"
#include "objfunc/rosenbrock/Rosenbrock.h"

//typedef SimpleObj ObjFunc;
//typedef Griewank ObjFunc;
//typedef Rastrigin ObjFunc;
typedef Rosenbrock ObjFunc;


void main_function(int argc, char **argv)
{

  PopulationSearchBasicMenu<ObjFunc>  menu(argc,argv);

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
