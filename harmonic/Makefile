LDLIBS          = -lm

# Optimize, add debugging symbols
CFLAGS = -g3 -Wall -O6 -DNDEBUG -fpic

all: CleanOptPRVote.o CleanOptPRVote psipr.cpython-38-x86_64-linux-gnu.so

psipr.cpython-38-x86_64-linux-gnu.so: CleanOptPRVote.o
	gcc -shared -o libCleanOptPRVote.so CleanOptPRVote.o
	python ./build_cffi.py
