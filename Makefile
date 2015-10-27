FLAGS=-std=gnu99 -Wall

GCC_FLAGS="gcc $(FLAGS)"
MPICC_FLAGS="mpicc $(FLAGS)"

clean:
	rm -rf obj
	rm -rf target

obj:
	mkdir obj

target:
	mkdir target

pdijkstra: target drivers/pdijkstra.c obj/pdijkstra.o obj/prnggraph.o
	"$(GCC_FLAGS)" -fopenmp -o target/pdijkstra drivers/pdijkstra.c src/pdijkstra.h src/prnggraph.h obj/pdijkstra.o obj/prnggraph.o

obj/pdijkstra.o: obj src/pdijkstra.h src/pdijkstra.c
	"$(GCC_FLAGS)" -fopenmp -c -o obj/pdijkstra.o src/pdijkstra.c

obj/prnggraph.o: obj src/prnggraph.h src/prnggraph.c
	"$(GCC_FLAGS)" -fopenmp -c -o obj/prnggraph.o src/prnggraph.c

obj/rnggraph.o: obj src/rnggraph.h src/rnggraph.c
	"$(GCC_FLAGS)" -c -o obj/rnggraph.o src/rnggraph.c

debug-pdijkstra: target drivers/pdijkstra.c obj/pdijkstra-debug.o obj/prnggraph-debug.o
	"$(GCC_FLAGS)" -fopenmp -g -o target/pdijkstra-debug drivers/pdijkstra.c src/pdijkstra.h src/prnggraph.h obj/pdijkstra-debug.o obj/prnggraph-debug.o
	cgdb --args target/pdijkstra-debug 8 0 0 0 4

obj/pdijkstra-debug.o: obj src/pdijkstra.h src/pdijkstra.c
	"$(GCC_FLAGS)" -fopenmp -c -g -o obj/pdijkstra-debug.o src/pdijkstra.c

obj/prnggraph-debug.o: obj src/prnggraph.h src/prnggraph.c
	"$(GCC_FLAGS)" -fopenmp -c -g -o obj/prnggraph-debug.o src/prnggraph.c
