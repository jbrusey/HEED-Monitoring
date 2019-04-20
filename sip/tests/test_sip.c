#include <math.h>
#include "minunit.h"
#include "sip.h"
#include "test_dewma.h"


char * test_sip1(void)
{
  SIP sip;
  int i;
  int c = 0;
  time_t time = 0;
  State xmit;
  seq_t seq;

  SIP_init(&sip, 0.2f, 0.2f, 0.5f, 1000*60*60*24);

  mu_assert("init not ok", 
	    sip.dewma.count == 0 &&
	    sip.dewma.alpha == 0.2f &&
	    sip.dewma.beta == 0.2f &&
	    sip.threshold == 0.5f &&
	    sip.pending.time == TIME_MAX &&
	    sip.sink.time == TIME_MAX &&
	    sip.state.time == TIME_MAX &&
	    !sip.sink_ok &&
	    sip.heartbeat_millis == 1000*60*60*24 &&
	    sip.pending_seq == 0 &&
	    sip.seq == 0
);

  

  for (i = 0; i < 100; i++) { 
    if (SIP_update(&sip, &xmit, &seq, 101.0, time)) {
      c++;
      SIP_transmitted_ok(&sip, seq);
    }
    time += 1000;
  }
  mu_assert("wrong number of messages generated", c == 1);
  
  for (i = 0; i < 10; i++) { 
    if (SIP_update(&sip, &xmit, &seq, 0.0, time)) {
      c++;
      SIP_transmitted_ok(&sip, seq);
    }
    time += 1000;
  }
  mu_assert("wrong number of messages generated", c == 10);

  return 0;
}




int tests_run = 0;
 
char * all_tests(void)
{
  mu_run_test(test_dewma1); 
  mu_run_test(test_dewma_not_using_rate_correctly); 
  mu_run_test(test_dewma2); 
  mu_run_test(test_dewma3);
  mu_run_test(test_sip1);
  
  return 0;
}
 
int main(void) {
    char *result = all_tests();
    
    if (result != 0)
        printf("%s\n", result);

    else
        printf("ALL TESTS PASSED\n");
    
    printf("Tests run: %d\n", tests_run);
 
    return result != 0;
}
