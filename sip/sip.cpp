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
#include "sip.h"
#include <math.h>
#include <assert.h>
#include <limits.h>

/* SIP::init - initialise SIP object
 */
SIP::SIP(float alpha,
	 float beta,
	 float threshold,
	 siptime_t heartbeat_millis):
  _dewma(alpha, beta),
  _threshold(threshold),
  _heartbeat_millis(heartbeat_millis)
{
  _pending.time = 
    _state.time = 
    _sink.time = TIME_MAX;
  _sink_ok = false;
  _pending_seq =
    _seq = 0;
}

/* SIP::heartbeat
 *
 * don't send a heartbeat if
 * - the sink has received at least one packet ok AND
 * - the time difference between the last sink packet and the current
 *   time is less than some limit.
 */
bool SIP::heartbeat(siptime_t current) 
{ 
  return ! (_sink_ok &&
	    _dewma.subtract_time(current, _sink.time) < _heartbeat_millis);
}

bool SIP::eventful(State xs,
		   State x) {
  return fabs(xs.x[0] - x.x[0]) > _threshold;
}

/* SIP::update - receive a new sensor reading and determine whether or
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
bool SIP::update(State *state_to_transmit,
		 unsigned int *seq,
		 float value,
		 siptime_t current)
{
  State new_state, xs;
  _dewma.estimate(&new_state, 
		 value,
		 current,
		 &_state);

  _state = new_state;

  if (_sink_ok) {
    _dewma.predict(&xs,
		   &_sink,
		   current);
  }
  if (!_sink_ok ||
      eventful(xs, new_state) ||
      heartbeat(current)) {
    *state_to_transmit = 
      _pending = _state;
    _pending_seq = 
      *seq = _seq++;
    return true;
  }
  else
    return false;
}

/* SIP::transmitted_ok - should be called after SIP::update provided a
 * state to transmit and that state was successfully transmitted.
 * Note: if the sequence number does not match, the pending packet
 * will be considered still pending until the next transmission.
 *
 * seq - sequence number of successfully transmitted state
 */
void SIP::transmitted_ok(seq_t seq) 
{
  if (_pending_seq == seq) {
    _sink = _pending;
    _sink_ok = true;
  }
}

  
