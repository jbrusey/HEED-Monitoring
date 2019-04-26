#     This file is part of LSip.

#     LSip is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.

#     LSip is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.

#     You should have received a copy of the GNU General Public License
#     along with LSip.  If not, see <https://www.gnu.org/licenses/>.
CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-Wall -pedantic
LDFLAGS=
LDLIBS=

SRCS=test_sip.cpp test_dewma.cpp ../sip.cpp ../dewma.cpp 
OBJS=$(subst .cpp,.o,$(SRCS))

all: test_sip
	./test_sip

test_sip: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)


depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CC) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend
