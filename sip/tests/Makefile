CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-Wall -pedantic
LDFLAGS=
LDLIBS=

SRCS=test_sip.c test_dewma.c ../sip.c ../subtracttime.c ../dewma.c 
OBJS=$(subst .c,.o,$(SRCS))

all: test_sip
	./test_sip

test_sip: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)


depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CC) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend
