#include <stdio.h>
#include <math.h>
#include "minunit.h"
#include "dewma.h"

void print_state(State *s) {
  printf("State(%f, %f, %lu)\n",
	 s->x[0],
	 s->x[1],
	 s->time);
}


char * test_dewma1(void)
{
  DEWMA d;
  State s1, s2;
  float z;
  int i;
  time_t t;
  float rate = 12.1f ;
  float offset = 4.5f;


  DEWMA_init(&d, 0.2f, 0.2f);

  mu_assert("init not ok", 
	    d.count == 0 &&
	    d.alpha == 0.2f &&
	    d.beta == 0.2f);

  for (i = 0; i < 100; i++) {
    z = i * rate + offset;
    t = i;
    DEWMA_estimate(&d, &s2, z, t, &s1);    
    s1 = s2;
  }
  mu_assert("estimates after 100 iterations are wrong",
	    fabs(s2.x[0] - (99*rate+offset)) < 0.001);
  mu_assert("estimates after 100 iterations are wrong",
	    fabs(s2.x[1] - rate) < 0.0001);
  return 0;
}

char * test_dewma_not_using_rate_correctly(void)
{
  DEWMA d;
  State s1, s2;
  float z;
  int i, t;
  time_t c;
  float rate = 12.1f / 1000;
  float offset = 4.5f;

  DEWMA_init(&d, 0.2f, 0.2f);

  mu_assert("init not ok", 
	    d.count == 0 &&
	    d.alpha == 0.2f &&
	    d.beta == 0.2f);

  for (i = 0; i < 100; i++) {
    t = i * 10;
    z = t * rate + offset;
    c = t;
    DEWMA_estimate(&d, &s2, z, c, &s1);    
    s1 = s2;
  }
  mu_assert("estimates after 100 iterations are wrong",
	    fabs(s2.x[0] - (rate * 990 + offset)) < 0.0001f);
  mu_assert("estimates after 100 iterations are wrong",
	    fabs(s2.x[1] - rate) < 0.0001f);
  return 0;
}


char * test_dewma2(void)
{
  DEWMA d;
  State s1, s2;
  time_t c;

  DEWMA_init(&d, 0.2f, 0.2f);

  mu_assert("init not ok", 
	    d.count == 0 &&
	    d.alpha == 0.2f &&
	    d.beta == 0.2f);

  
  c = 1000;
  DEWMA_estimate(&d, &s2, 101.f, c, &s1);
  s1 = s2;

  c = 1500;
  DEWMA_predict(&s2, &s1, c);
  
  mu_assert("should stay flat",
	    s2.x[0] == 101.f &&
	    s2.x[1] == 0.f);

  return 0;
}

char * test_dewma3(void)
{
  DEWMA d;
  State s1, s2;
  time_t c;

  DEWMA_init(&d, 0.99f, 0.99f);

  mu_assert("init not ok", 
	    d.count == 0 &&
	    d.alpha == 0.99f &&
	    d.beta == 0.99f);

  c = 1000;
  DEWMA_estimate(&d, &s2, 101.f, c, &s1);
  s1 = s2;
  
  c = 1500;

  DEWMA_estimate(&d, &s2, 103.f, c, &s1);
  s1 = s2;
  
  c = 2000;

  DEWMA_estimate(&d, &s2, 105.f, c, &s1);
  s1 = s2;

  c = 3000;

  DEWMA_predict(&s2, &s1, c);
  
  mu_assert("should roughly track trend",
	    fabs(s2.x[0] - 109.f) < 0.1 &&
	    fabs(s2.x[1] - 0.004f) < 0.1);

  return 0;
}



