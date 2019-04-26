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

#ifndef _DEWMA_H
#define _DEWMA_H


typedef unsigned int siptime_t;
#define TIME_MAX UINT_MAX

struct State {
  siptime_t time;
  float x[2];
};

	
class DEWMA {
public:
  DEWMA(float alpha, float beta);

  void estimate(State *s_new,
		float value,
		siptime_t current,
		State *s_old);
  
  void predict(State *s_predict,
	       State *s_old,
	       siptime_t t_predict);

  siptime_t subtract_time(siptime_t current_time,
		       siptime_t prev_time);

private:
  unsigned int _count;
  float _alpha, _beta;

};




#endif
