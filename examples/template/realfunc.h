#include "mymotypes/moRealTypes.h"
#include <vector>
//-----------------------------------------------------------------------------
/** Just a simple function that takes an vector<double> and sets the fitnes
    to the sphere function. Please use doubles not float!!!
    @param _ind A floatingpoint vector
*/

double realfunc(const std::vector<double>& _ind)
{
  double sum = 0;
  for (unsigned i = 0; i < _ind.size(); i++) {
    sum += _ind[i] * _ind[i];
  }
  return sum;
}


Fitness realfunc2(const EORVT& _ind)
//FitT realfunc(const std::vector<double>& _ind)
{
  double sum = 0;
  for (unsigned i = 0; i < _ind.size(); i++) {
    sum += 2 * _ind[i] * _ind[i];
  }
  return sum;
}
