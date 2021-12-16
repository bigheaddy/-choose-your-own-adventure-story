CPPFLAGS=-ggdb3 -Wall -Werror -pedantic -std=gnu++11
PROGS=cyoa-step1 cyoa-step2 cyoa-step3 cyoa-step4
OBJS=$(patsubst %,%.o,$(PROGS)) Page.o CYOA.o
all: $(PROGS)
cyoa-step%: cyoa-step%.o Page.o CYOA.o
	g++ $(CPPFLAGS) -o $@ $^
%.o: %.cpp
	g++ $(CPPFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *~ $(PROGS) $(OBJS)

Page.o: Page.hpp
CYOA.o: CYOA.hpp
