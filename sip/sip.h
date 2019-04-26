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

#ifndef _SIP_H
#define _SIP_H

#include "dewma.h"
#include <stdbool.h>

typedef unsigned int seq_t;

class SIP {
public: 
  SIP(float alpha, 
      float beta, 
      float threshold,
      siptime_t heartbeat_millis);

  bool update(State *state_to_transmit,
	      seq_t *seq,
	      float value,
	      siptime_t current);

  void transmitted_ok(seq_t seq);

  
private:
  bool heartbeat(siptime_t current);

  bool eventful(State xs,
		State x);

  DEWMA _dewma;
  float _threshold;
  State _pending;
  State _sink;
  State _state;
  bool _sink_ok;
  siptime_t _heartbeat_millis;
  seq_t _pending_seq;
  seq_t _seq;
};

	     
#endif
