CC=gcc
CXX=g++
#
# INTERACTIONS CAN BE
# -DHARDCORE -DCONFINEMENT -DUNIFORM -DLOCALIZED -DTOPO
#
# EXTRA MEASUREMENTS INTERACTIONS CAN BE
# -DGETXYZ -DGETENERGY -DGETPERF
#
INTERACTIONS=-DHARDCORE -DUNIFORM -DLOCALIZED
MEASUREMENTS=-DGETXYZ -DGETENERGY

#
# FOR DEBUG, CHOOSE BETWEEN
# "-Ofast" (NO DEBUGGING) AND "-g -O0" (DEBUGGING)
# "-DFASTEXP" is an experimental optimization
# -ffast-math -funsafe-math-optimizations
DEBUG=-Ofast -fno-fast-math

CXXFLAGS=${DEBUG} -std=c++0x -Drestrict=__restrict__ -DDSFMT_MEXP=19937 -U_FORTIFY_SOURCE -fno-stack-protector ${INTERACTIONS} ${MEASUREMENTS}
CFLAGS=${DEBUG} -std=gnu99 -DDSFMT_MEXP=19937 -U_FORTIFY_SOURCE -fno-stack-protector ${INTERACTIONS} ${MEASUREMENTS}
CPUF32=-DHAVE_SSE2=1 -m32 -march=core2 -mtune=core2 -mfpmath=sse
CPUF64NPC=-DHAVE_POPCNT=0 -DHAVE_SSE2=1 -m64 -march=opteron -mtune=opteron -fprefetch-loop-arrays
CPUF64=-DHAVE_POPCNT=1 -DHAVE_SSE2=1 -m64 -march=corei7 -mtune=corei7 -fprefetch-loop-arrays

GLFILES=build/glumain.o build/gl-subs.o build/glxwindower.o build/polymer.o build/sphere.o build/inputmain.o
GLLIBS=${GLFILES} -lpthread -lGL -lXi -lreadline

all: nucleoid nucleoid.32 nucleoid.npc nucleoid.gl

nucleoid: build/simulazione.o build/main.o build/dSFMT.o build/hex.o
	${CC} ${CFLAGS} ${CPUF64} -DNUM_THREADS=1 -o nucleoid build/simulazione.o build/main.o build/dSFMT.o build/hex.o -lm -lz

nucleoid.32: build/simulazione.32.o build/main.32.o build/dSFMT.32.o build/hex.32.o
	${CC} ${CFLAGS} ${CPUF32} -DNUM_THREADS=1 -o nucleoid.32 build/simulazione.32.o build/main.32.o build/dSFMT.32.o build/hex.32.o -lm -lz

nucleoid.npc: build/simulazione.npc.o build/main.o build/dSFMT.o build/hex.o
	${CC} ${CFLAGS} ${CPUF64NPC} -DNUM_THREADS=1 -o nucleoid.npc build/simulazione.npc.o build/main.o build/dSFMT.o build/hex.o -lm -lz

nucleoid.gl: build/simulazione.gl.o build/main.gl.o build/dSFMT.32.o build/hex.32.o ${GLFILES}
	${CXX} ${CXXFLAGS} ${CPUF32} -DNUM_THREADS=3 -o nucleoid.gl build/simulazione.gl.o build/main.gl.o build/dSFMT.32.o build/hex.32.o ${GLLIBS} -lm -lz

build/simulazione.o: simulazione.c simulazione.h simprivate.h raytracing.h
	${CC} ${CFLAGS} ${CPUF64} -DNUM_THREADS=1 -o build/simulazione.o -c simulazione.c

build/simulazione.32.o: simulazione.c simulazione.h simprivate.h raytracing.h
	${CC} ${CFLAGS} ${CPUF32} -DNUM_THREADS=1 -o build/simulazione.32.o -c simulazione.c

build/simulazione.npc.o: simulazione.c simulazione.h simprivate.h raytracing.h
	${CC} ${CFLAGS} ${CPUF64NPC} -DNUM_THREADS=1 -o build/simulazione.npc.o -c simulazione.c

build/simulazione.gl.o: simulazione.c simulazione.h simprivate.h raytracing.h
	${CC} ${CFLAGS} ${CPUF32} -DNUM_THREADS=3 -o build/simulazione.gl.o -c simulazione.c

build/main.o: main.c
	${CC} ${CFLAGS} ${CPUF64} -DNUM_THREADS=1 -o build/main.o -c main.c

build/main.32.o: main.c
	${CC} ${CFLAGS} ${CPUF32} -DNUM_THREADS=1 -o build/main.32.o -c main.c

build/main.gl.o: main.c
	${CC} ${CFLAGS} ${CPUF32} -DNUM_THREADS=3 -o build/main.gl.o -c main.c

build/dSFMT.o: dSFMT-src-2.2.1/dSFMT.c
	${CC} ${CFLAGS} ${CPUF64} -O3 -fno-strict-aliasing -DNDEBUG -o build/dSFMT.o -c dSFMT-src-2.2.1/dSFMT.c

build/dSFMT.32.o: dSFMT-src-2.2.1/dSFMT.c
	${CC} ${CFLAGS} ${CPUF32} -O3 -fno-strict-aliasing -DNDEBUG -o build/dSFMT.32.o -c dSFMT-src-2.2.1/dSFMT.c

build/hex.o: hex.c
	${CC} ${CFLAGS} ${CPUF64} -o build/hex.o -c hex.c

build/hex.32.o: hex.c
	${CC} ${CFLAGS} ${CPUF32} -o build/hex.32.o -c hex.c

build/glumain.o: 3d/glumain.cxx
	${CXX} ${CXXFLAGS} ${CPUF32} -DNUM_THREADS=3 -o build/glumain.o -c 3d/glumain.cxx

build/gl-subs.o: 3d/gl-subs.cxx 3d/gl-subs.hxx
	${CXX} ${CXXFLAGS} ${CPUF32} -DNUM_THREADS=3 -o build/gl-subs.o -c 3d/gl-subs.cxx

build/glxwindower.o: 3d/glxwindower.cxx 3d/glxwindower.hxx 3d/windower.hxx
	${CXX} ${CXXFLAGS} ${CPUF32} -DNUM_THREADS=3 -o build/glxwindower.o -c 3d/glxwindower.cxx

build/polymer.o: 3d/polymer.cxx 3d/polymer.hxx 3d/buffered_geom.hxx
	${CXX} ${CXXFLAGS} ${CPUF32} -DNUM_THREADS=3 -o build/polymer.o -c 3d/polymer.cxx

build/sphere.o: 3d/sphere.cxx 3d/sphere.hxx 3d/buffered_geom.hxx
	${CXX} ${CXXFLAGS} ${CPUF32} -DNUM_THREADS=3 -o build/sphere.o -c 3d/sphere.cxx

build/inputmain.o: 3d/inputmain.cxx
	${CXX} ${CXXFLAGS} ${CPUF32} -DNUM_THREADS=3 -o build/inputmain.o -c 3d/inputmain.cxx

clean:
	rm nucleoid nucleoid.32 nucleoid.npc nucleoid.gl build/*.o
