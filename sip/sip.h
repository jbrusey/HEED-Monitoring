#ifndef _SIP_H
#define _SIP_H

#include "dewma.h"
#include <stdbool.h>

typedef unsigned int seq_t;

typedef struct SIP {
  DEWMA dewma;
  float threshold;
  State pending;
  State sink;
  State state;
  bool sink_ok;
  time_t heartbeat_millis;
  seq_t pending_seq;
  seq_t seq;
} SIP;

void SIP_init(SIP * s, 
	      float alpha, 
	      float beta, 
	      float threshold,
	      time_t heartbeat_millis);
bool SIP_update(SIP * s,
		State * state_to_transmit,
		seq_t * seq,
		float value,
		time_t current);
void SIP_transmitted_ok(SIP * s,
			seq_t seq);
	     
#endif
