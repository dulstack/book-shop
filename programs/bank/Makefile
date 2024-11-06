CC=g++
CFLAGS=-std=c++17 -I ../../include -I include
LIBS=-lpthread -lsqlite3  $(LD_LIBRARY_PATH)/libpistache.a
EXT_SRCS=../../src/auth_api.cpp ../../src/auth.cpp ../../src/db.cpp
SRCS=src/bank.cpp src/api.cpp

OUT=$(SRCS:src/%.cpp=bin/%.o)
EXT_OUT=$(EXT_SRCS:../../src/%.cpp=../../bin/%.o)
.PHONY: all
all: bin/bank
bin/bank: $(EXT_OUT) $(OUT)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
bin/%.o:src/%.cpp | bin
	$(CC) -c -o $@ $(CFLAGS) $<
../../bin/%.o:../../src/%.cpp | ../../bin
	$(CC) -c -o $@ $(CFLAGS) $<
../../bin:
	mkdir $@
bin:
	mkdir $@
