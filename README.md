About
=====

A C++ optimization software based on paradiseo 2.0.1 (http://paradiseo.gforge.inria.fr/).
The high-level concepts of single-based metaheuristics are simplified for non-linear real valued objective functions.
It provides unified interfaces of arbitrary s-metaheuristics for end users requiring only start solution and objective functions.  


Version & release notes
=======================
r1.0 : first release 

Author 
======

Atiyah Elsheikh, Energy Department, Austrian Institute of Technology GmbH (AIT), Vienna, Austria
Email: Atiyah.Elsheikh@ait.ac.at 
		
License
=======

This software is distributed under the terms and conditions of LGPL


Reference
=========
	
If this software is useful for you, please reference the following paper: 	
	
Elsheikh, A. Derivative-based hybrid heuristics for continuous-time 
simulation-optimization Simulation Modelling Practice and Theory, vol. 46, pp. 164 - 175, 2014.

@ARTICLE{Elsheikh2013a,
  author = {Atiyah Elsheikh},
  title = {Derivative-based hybrid heuristics for continuous-time simulation-optimization},
  journal = {Simulation Modelling Practice and Theory},
  year = {2014},
  volume = {46},
  pages = {pp. 164 -- 175},
  month = {Aug.},
  part = {SI: Simulation-Optimization of Complex Systems: Methods and Applications},
  doi = {10.1016/j.simpat.2013.11.011}
}


Tested platforms
================

  * Tested with Cygwin gcc version 4.8.3
  * paradisEO 2.0.1 (http://paradiseo.gforge.inria.fr/)


Structure
=========

Directories are

examples/ 		: examples of software suage
	01completeHC/	: ParadisEO way for solving nlp optimization problem (the difficult way)
	02localsearch/	: unified interface for employing s-metaheuristics (the easier way)
	03basicmenu/	: menu files for executing optimization , @see ReadMe.txt 
objfunc/		: Interfaces and examples of objective functions
	griewank/	
	rastrigin/
	rosenbrock/
	simple/
remo/			: real-valued moving objects (corresponds to mo/ in paradisEO) 
	src/
		algo/	: unified interfaces and flag menus for s-metaheuristics
		eval/	: basic classes for evaluating solutions of real-valued nlp

	
Installation
=============

The sources are provided as header files. They just need to be included when using them. @see examples.


Getting started
===============

@see examples/02* , examples/03* 


Generating APIs with Doxygen
============================
 
$ mkdir -p api
$ mkdir -p api/all 
$ doxygen Doxyfile 

Important TAGS in Doxyfile
--------------------------
INPUT, PROJECT_NAME, OUTPUT_DIR
