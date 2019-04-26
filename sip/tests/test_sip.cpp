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
#include "../sip.h"
#include "test_dewma.h"
  const char* test_sip1(void)
  {
    SIP sip(0.2f, 0.2f, 0.5f, 1000*60*60*24);
    int i;
    int c = 0;
    siptime_t time = 0;
    State xmit;
    seq_t seq;
  
    for (i = 0; i < 100; i++) { 
      if (sip.update(&xmit, &seq, 101.0, time)) {
	c++;
	sip.transmitted_ok(seq);
      }
      time += 1000;
    }
    mu_assert("wrong number of messages generated", c == 1);
  
    for (i = 0; i < 10; i++) { 
      if (sip.update(&xmit, &seq, 0.0, time)) {
	c++;
	sip.transmitted_ok(seq);
      }
      time += 1000;
    }
    mu_assert("wrong number of messages generated", c == 10);

    return 0;
  }




  int tests_run = 0;
 
  const char* all_tests(void)
  {
    mu_run_test(test_dewma1); 
    mu_run_test(test_dewma_not_using_rate_correctly); 
    mu_run_test(test_dewma2); 
    mu_run_test(test_dewma3);
    mu_run_test(test_sip1);
  
    return 0;
  }
 
  int main(void) {
    const char *result = all_tests();
    
    if (result != 0)
      std::cout << result << std::endl;

    else
      std::cout << "All tests passed" << std::endl;

    std::cout << "Tests run: " << tests_run << std::endl;
 
    return result != 0;
  }
