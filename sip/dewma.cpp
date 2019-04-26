/*
    This file is part of LSip.

    LSip is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LSip is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LSip.  If not, see <https://www.gnu.org/licenses/>.
*/


/* DEWMA is a filter based on
 * http://www.it.iitb.ac.in/~praj/acads/seminar/04329008_ExponentialSmoothing.pdf
 * with the b1 initial value chosen as y2 - y1
 *
 * An adjustment has also been made to store the x[1] as rate of
 * change rather than difference between samples.
 */
#include "dewma.h"
#include <assert.h>
#include <limits.h>

/* DEWMA_init initialises the parameters for the filter
 * 
 * d - dewma object
 * alpha - smoothing parameter in range (0,1)
 * beta - second order smoothing parameter in range (0,1)
 */
DEWMA::DEWMA(float alpha, float beta):
  _first(true),
  _alpha(alpha),
  _beta(beta)
{ 
  assert(alpha > 0 && alpha < 1);
  assert(beta > 0 && beta < 1);
}

/* DEWMA_estimate produces a new state estimate based on
 * the old state, new value and time. 
 * d - dewma object
 * s_new - returned as new state
 * value - sensor value
 * s_old - previous state
 */
void DEWMA::estimate(State * s_new, 
		     float value, 
		     siptime_t current,
		     State *s_old) 
{
  siptime_t delta_t;
  if (_first) {
    s_new->x[0] = value;
    s_new->x[1] = 0.f;
    _first = false;
  }
  else {
    delta_t = subtract_time(current, s_old->time);
    s_new->x[0] = _alpha * value + 
      (1-_alpha) * (s_old->x[0] + s_old->x[1] * delta_t);

    if (delta_t == 0) {
      s_new->x[1] = s_old->x[1];
    }
    else {
      s_new->x[1] = _beta * (s_new->x[0] - s_old->x[0]) / delta_t +
	(1 - _beta) * s_old->x[1];
    }
  }
  s_new->time = current;
}

/* DEWMA_predict - predict value at time t
 * d - dewma object
 * s_predict - returned with predicted state
 * 
 */
void DEWMA::predict(State * s_predict,
		    State *s_old,
		    siptime_t t_predict)
{
  s_predict->x[0] = s_old->x[0] + s_old->x[1] * (subtract_time(t_predict, s_old->time));
  s_predict->x[1] = s_old->x[1];
  s_predict->time = t_predict;
}

		   
/**
 * subtract_time
 * find time difference between two times
 * assumes siptime_t wraps after reaching TIME_MAX 
 */
siptime_t DEWMA::subtract_time(siptime_t current_time, siptime_t prev_time)
{
  if (current_time < prev_time) // deal with overflow
    return ((TIME_MAX - prev_time) + current_time + 1);
  else
    return (current_time - prev_time);
}

