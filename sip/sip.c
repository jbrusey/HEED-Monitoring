#include "sip.h"
#include <math.h>
#include <assert.h>
#include <limits.h>

/* prototypes */
bool SIP_heartbeat(SIP * s, time_t current);
bool SIP_eventful(SIP * s, State xs, State x);

/* SIP_init - initialise SIP object
 */
void SIP_init(SIP * s,
	      float alpha,
	      float beta,
	      float threshold,
	      time_t heartbeat_millis)
{
  DEWMA_init(&s->dewma, alpha, beta);
  s->threshold = threshold;
  s->pending.time = 
    s->state.time = 
    s->sink.time = TIME_MAX;
  s->sink_ok = false;
  s->heartbeat_millis = heartbeat_millis;
  s->pending_seq =
    s->seq = 0;
}

/* SIP_heartbeat
 *
 * don't send a heartbeat if
 * - the sink has received at least one packet ok AND
 * - the time difference between the last sink packet and the current
 *   time is less than some limit.
 */
bool SIP_heartbeat(SIP * s,
		   time_t current) 
{ 
  return ! (s->sink_ok &&
	    subtract_time(current, s->sink.time) < s->heartbeat_millis);
}

bool SIP_eventful(SIP * s, 
		  State xs,
		  State x) {
  return fabs(xs.x[0] - x.x[0]) > s->threshold;
}

/* SIP_update - receive a new sensor reading and determine whether or
 * not it is eventful.
 * 
 * Returns true if eventful.
 * s - SIP object
 * state_to_transmit - State object to transmit (only valid if true
 * returned).
 * seq - sequence number to transmit
 * value - new sensor reading
 * current - time when sensor reading taken
 * 
 */
bool SIP_update(SIP * s,
		State * state_to_transmit,
		unsigned int * seq,
		float value,
		time_t current)
{
  State new_state, xs;
  DEWMA_estimate(&s->dewma,
		 &new_state, 
		 value,
		 current,
		 &s->state);

  s->state = new_state;

  if (s->sink_ok) {
    DEWMA_predict(&xs,
		  &s->sink,
		  current);
  }
  if (!s->sink_ok ||
      SIP_eventful(s, xs, new_state) ||
      SIP_heartbeat(s, current)) {
    *state_to_transmit = 
      s->pending = s->state;
    s->pending_seq = 
      *seq = s->seq++;
    return true;
  }
  else
    return false;
}

/* SIP_transmitted_ok - should be called after SIP_update provided a
 * state to transmit and that state was successfully transmitted.
 * Note: if the sequence number does not match, the pending packet
 * will be considered still pending until the next transmission.
 *
 * seq - sequence number of successfully transmitted state
 */
void SIP_transmitted_ok(SIP * s, 
			seq_t seq) 
{
  if (s->pending_seq == seq) {
    s->sink = s->pending;
    s->sink_ok = true;
  }
}

  
