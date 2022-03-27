CC=g++
CFLAGS=-I.
DEPS = process.hpp queue.hpp
OBJ = main.o process.o queue.o

%.o: %.cpp $(DEPS)
	$(CC) -std=c++11 -c -o $@ $< $(CFLAGS)

CPUSchedulerMLFQ-runner: $(OBJ)
	$(CC) -std=c++11 -o $@ $^ $(CFLAGS)

clean:
	$(RM) $(OBJ) CPUSchedulerMLFQ-runner
