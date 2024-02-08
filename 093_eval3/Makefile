CC=g++

CFLAGS=-std=c++11 -Wall

TARGETS=cyoa-step1 cyoa-step2 cyoa-step3 cyoa-step4

all: $(TARGETS)

cyoa-step1: cyoa-step1.o
	$(CC) $(CFLAGS) -o cyoa-step1 cyoa-step1.o

cyoa-step2: cyoa-step2.o
	$(CC) $(CFLAGS) -o cyoa-step2 cyoa-step2.o

cyoa-step3: cyoa-step3.o
	$(CC) $(CFLAGS) -o cyoa-step3 cyoa-step3.o
 
cyoa-step4: cyoa-step4.o
	$(CC) $(CFLAGS) -o cyoa-step4 cyoa-step4.o

cyoa-step1.o: cyoa-step1.cpp page.hpp story.hpp
	$(CC) $(CFLAGS) -c cyoa-step1.cpp

cyoa-step2.o: cyoa-step2.cpp page.hpp story.hpp
	$(CC) $(CFLAGS) -c cyoa-step2.cpp

cyoa-step3.o: cyoa-step3.cpp page.hpp story.hpp
	$(CC) $(CFLAGS) -c cyoa-step3.cpp
 
cyoa-step4.o: cyoa-step4.cpp page.hpp story.hpp
	$(CC) $(CFLAGS) -c cyoa-step4.cpp
 
clean:
	rm -f *.o $(TARGETS)
