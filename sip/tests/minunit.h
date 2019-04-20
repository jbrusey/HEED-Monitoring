/*************************************************************
 Modification history:

 1. 26/4/2013 J. Brusey add __FILE__ and __LINE__ so that it easier to
 see where an assert is coming from. 


 *************************************************************/
#ifndef MINUNIT_H
#define MINUNIT_H

#include <stdio.h>
#define mu_strify1(s) #s
#define mu_strify(s) mu_strify1(s)
#define mu_assert(message, test) do { if (!(test)) return __FILE__ ":" mu_strify(__LINE__) ": assertion failed: " message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; if (message) return message; } while (0)
extern int tests_run;

#endif
