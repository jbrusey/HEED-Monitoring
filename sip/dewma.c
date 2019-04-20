/* DEWMA is a filter based on
 * http://www.it.iitb.ac.in/~praj/acads/seminar/04329008_ExponentialSmoothing.pdf
 * with the b1 initial value chosen as y2 - y1
 *
 * An adjustment has also been made to store the x[1] as rate of
 * change rather than difference between samples.
 */
#include "dewma.h"
#include <assert.h>

/* DEWMA_init initialises the parameters for the filter
 * 
 * d - dewma object
 * alpha - smoothing parameter in range (0,1)
 * beta - second order smoothing parameter in range (0,1)
 */
void DEWMA_init(DEWMA *d, float alpha, float beta) { 
  d->count = 0;
  d->alpha = alpha;
  d->beta = beta;
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
void DEWMA_estimate(DEWMA * d, 
		    State * s_new, 
		    float value, 
		    time_t current,
		    State *s_old) 
{
  time_t delta_t;
  if (d->count == 0) {
    s_new->x[0] = value;
    s_new->x[1] = 0.;
    d->count++;
  }
  else {
    delta_t = subtract_time(current, s_old->time);
    s_new->x[0] = d->alpha * value + 
      (1-d->alpha) * (s_old->x[0] + s_old->x[1] * delta_t);

    if (delta_t == 0) {
      s_new->x[1] = s_old->x[1];
    }
    else {
      s_new->x[1] = d->beta * (s_new->x[0] - s_old->x[0]) / delta_t +
	(1 - d->beta) * s_old->x[1];
    }
  }
  s_new->time = current;
}

/* DEWMA_predict - predict value at time t
 * d - dewma object
 * s_predict - returned with predicted state
 * 
 */
void DEWMA_predict(State * s_predict,
		   State *s_old,
		   time_t t_predict)
{
  s_predict->x[0] = s_old->x[0] + s_old->x[1] * (subtract_time(t_predict, s_old->time));
  s_predict->x[1] = s_old->x[1];
  s_predict->time = t_predict;
}

		   
