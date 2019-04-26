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
#include <iostream>
#include <math.h>
#include "minunit.h"
#include "../dewma.h"


const char * test_dewma1(void)
{
  DEWMA d(0.2f, 0.2f);
  State s1, s2;
  float z;
  siptime_t t;
  float rate = 12.1f ;
  float offset = 4.5f;

  for (int i = 0; i < 100; i++) {
    z = i * rate + offset;
    t = i;
    d.estimate(&s2, z, t, &s1);    
    s1 = s2;
  }
  mu_assert("estimates after 100 iterations are wrong",
	    fabs(s2.x[0] - (99*rate+offset)) < 0.001);
  mu_assert("estimates after 100 iterations are wrong",
	    fabs(s2.x[1] - rate) < 0.0001);
  return 0;
}

const char* test_dewma_not_using_rate_correctly(void)
{
  DEWMA d(0.2f, 0.2f);
  State s1, s2;
  float z;
  int i, t;
  siptime_t c;
  float rate = 12.1f / 1000;
  float offset = 4.5f;

  for (i = 0; i < 100; i++) {
    t = i * 10;
    z = t * rate + offset;
    c = t;
    d.estimate(&s2, z, c, &s1);    
    s1 = s2;
  }
  mu_assert("estimates after 100 iterations are wrong",
	    fabs(s2.x[0] - (rate * 990 + offset)) < 0.0001f);
  mu_assert("estimates after 100 iterations are wrong",
	    fabs(s2.x[1] - rate) < 0.0001f);
  return 0;
}


const char* test_dewma2(void)
{
  DEWMA d(0.2f, 0.2f);
  State s1, s2;
  siptime_t c;

  c = 1000;
  d.estimate(&s2, 101.f, c, &s1);
  s1 = s2;

  c = 1500;
  d.predict(&s2, &s1, c);
  
  mu_assert("should stay flat",
	    s2.x[0] == 101.f &&
	    s2.x[1] == 0.f);

  return 0;
}

const char* test_dewma3(void)
{
  DEWMA d(0.99f, 0.99f);
  State s1, s2;
  siptime_t c;

  c = 1000;
  d.estimate(&s2, 101.f, c, &s1);
  s1 = s2;
  
  c = 1500;

  d.estimate(&s2, 103.f, c, &s1);
  s1 = s2;
  
  c = 2000;

  d.estimate(&s2, 105.f, c, &s1);
  s1 = s2;

  c = 3000;

  d.predict(&s2, &s1, c);
  
  mu_assert("should roughly track trend",
	    fabs(s2.x[0] - 109.f) < 0.1 &&
	    fabs(s2.x[1] - 0.004f) < 0.1);

  return 0;
}



