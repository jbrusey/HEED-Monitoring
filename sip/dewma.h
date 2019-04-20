#ifndef _DEWMA_H
#define _DEWMA_H

#include "subtracttime.h"

typedef struct State {
  time_t time;
  float x[2];
} State;

typedef struct DEWMA {
  time_t count;
  float alpha, beta;
} DEWMA;

void DEWMA_init(DEWMA *d, float alpha, float beta);
void DEWMA_estimate(DEWMA *d, 
		    State *s_new, 
		    float value, 
		    time_t current, 
		    State *s_old);
void DEWMA_predict(State *s_predict,
		   State *s_old,
		   time_t t_predict);



#endif
