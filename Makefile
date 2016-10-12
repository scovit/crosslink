CC=gcc
CXX=g++
#
# INTERACTIONS CAN BE
# -DHARDCORE -DCONFINEMENT -DUNIFORM -DLOCALIZED -DTOPO
#
# EXTRA MEASUREMENTS INTERACTIONS CAN BE
# -DGETXYZ -DGETENERGY -DGETPERF -DGETXLINK
#
INTERACTIONS=-DXLINK
MEASUREMENTS=-DGETXLINK

#
# FOR DEBUG, CHOOSE BETWEEN
# "-Ofast" (NO DEBUGGING) AND "-g -O0" (DEBUGGING)
# "-DFASTEXP" is an experimental optimization
# -ffast-math -funsafe-math-optimizations
DEBUG=-Ofast -fno-fast-math
#DEBUG=-g -O0

CXXFLAGS=${DEBUG} -std=c++0x -mavx -Drestrict=__restrict__ -DDSFMT_MEXP=19937 -U_FORTIFY_SOURCE -fno-stack-protector ${INTERACTIONS} ${MEASUREMENTS}
CFLAGS=${DEBUG} -std=gnu99 -mavx -DDSFMT_MEXP=19937 -U_FORTIFY_SOURCE -fno-stack-protector ${INTERACTIONS} ${MEASUREMENTS}
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CFLAGS += -I/opt/X11/include -L/opt/X11/lib
	CXXFLAGS += -I/opt/X11/include -L/opt/X11/lib
endif
CPUF64=-DHAVE_POPCNT=1 -DHAVE_SSE2=1 -m64 -march=haswell -mtune=haswell -fprefetch-loop-arrays
CPUFGL=${GPUF64}

GLFILES=build/glumain.o build/gl-subs.o build/glxwindower.o build/polymer.o build/sphere.o build/inputmain.o
GLLIBS=${GLFILES} -lpthread -lGL -lXi -lreadline

all: nucleoid nucleoid.gl infofile/infotest

nucleoid: build/simulazione.o build/main.o build/dSFMT.o build/hex.o build/checkpoint.o build/infofile.o build/parse.tab.o build/lex.yy.o
	${CC} ${CFLAGS} ${CPUF64} -DNUM_THREADS=1 -o nucleoid build/simulazione.o build/main.o build/dSFMT.o build/hex.o build/checkpoint.o build/infofile.o build/parse.tab.o build/lex.yy.o -lm -lz -lcrypto

nucleoid.gl: build/simulazione.gl.o build/main.gl.o build/dSFMT.gl.o build/hex.gl.o build/checkpoint.gl.o build/infofile.gl.o build/parse.tab.gl.o build/lex.yy.gl.o ${GLFILES}
	${CXX} ${CXXFLAGS} ${CPUFGL} -DNUM_THREADS=3 -o nucleoid.gl build/simulazione.gl.o build/main.gl.o build/dSFMT.gl.o build/hex.gl.o build/checkpoint.gl.o build/infofile.gl.o build/parse.tab.gl.o build/lex.yy.gl.o ${GLLIBS} -lm -lz -lX11 -lcrypto

infofile/infotest: infofile/infotest.c build/lex.yy.o build/parse.tab.o build/infofile.o
	${CC} ${CFLAGS} ${CPUF64} -o infofile/infotest infofile/infotest.c build/lex.yy.o build/parse.tab.o build/infofile.o

build/simulazione.o: simulazione.c simulazione.h simprivate.h raytracing.h
	${CC} ${CFLAGS} ${CPUF64} -DNUM_THREADS=1 -o build/simulazione.o -c simulazione.c

build/simulazione.gl.o: simulazione.c simulazione.h simprivate.h raytracing.h
	${CC} ${CFLAGS} ${CPUFGL} -DNUM_THREADS=3 -o build/simulazione.gl.o -c simulazione.c

build/main.o: main.c
	${CC} ${CFLAGS} ${CPUF64} -DNUM_THREADS=1 -o build/main.o -c main.c

build/main.gl.o: main.c
	${CC} ${CFLAGS} ${CPUFGL} -DNUM_THREADS=3 -o build/main.gl.o -c main.c

build/dSFMT.o: dSFMT-src-2.2.1/dSFMT.c
	${CC} ${CFLAGS} ${CPUF64} -O3 -fno-strict-aliasing -DNDEBUG -o build/dSFMT.o -c dSFMT-src-2.2.1/dSFMT.c

build/dSFMT.gl.o: dSFMT-src-2.2.1/dSFMT.c
	${CC} ${CFLAGS} ${CPUFGL} -O3 -fno-strict-aliasing -DNDEBUG -o build/dSFMT.gl.o -c dSFMT-src-2.2.1/dSFMT.c

build/hex.o: hex.c hex.h
	${CC} ${CFLAGS} ${CPUF64} -o build/hex.o -c hex.c

build/hex.gl.o: hex.c hex.h
	${CC} ${CFLAGS} ${CPUFGL} -o build/hex.gl.o -c hex.c

build/checkpoint.o: checkpoint.c checkpoint.h
	${CC} ${CFLAGS} ${CPUF64} -o build/checkpoint.o -c checkpoint.c

build/checkpoint.gl.o: checkpoint.c checkpoint.h
	${CC} ${CFLAGS} ${CPUFGL} -o build/checkpoint.gl.o -c checkpoint.c

build/infofile.o: infofile/infofile.c infofile/infofile.h infofile/parse.h
	${CC} ${CFLAGS} ${CPUF64} -o build/infofile.o -c infofile/infofile.c

build/infofile.gl.o: infofile/infofile.c infofile/infofile.h infofile/parse.h
	${CC} ${CFLAGS} ${CPUFGL} -o build/infofile.gl.o -c infofile/infofile.c

build/parse.tab.gl.o: build/parse.tab.c build/parse.tab.h
	${CC} ${CFLAGS} ${CPUFGL} -o build/parse.tab.gl.o -c build/parse.tab.c

build/lex.yy.gl.o: build/lex.yy.c
	${CC} ${CFLAGS} ${CPUFGL} -o build/lex.yy.gl.o -c build/lex.yy.c

build/parse.tab.o: build/parse.tab.c build/parse.tab.h
	${CC} ${CFLAGS} ${CPUF64} -o build/parse.tab.o -c build/parse.tab.c

build/lex.yy.o: build/lex.yy.c build/parse.tab.h
	${CC} ${CFLAGS} ${CPUF64} -o build/lex.yy.o -c build/lex.yy.c

build/lex.yy.c: infofile/lex.l
	flex -obuild/lex.yy.c infofile/lex.l

build/%.tab.c build/%.tab.h: infofile/%.y
	bison -d -b build/parse infofile/parse.y;

build/glumain.o: 3d/glumain.cxx
	${CXX} ${CXXFLAGS} ${CPUFGL} -DNUM_THREADS=3 -o build/glumain.o -c 3d/glumain.cxx

build/gl-subs.o: 3d/gl-subs.cxx 3d/gl-subs.hxx
	${CXX} ${CXXFLAGS} ${CPUFGL} -DNUM_THREADS=3 -o build/gl-subs.o -c 3d/gl-subs.cxx

build/glxwindower.o: 3d/glxwindower.cxx 3d/glxwindower.hxx 3d/windower.hxx
	${CXX} ${CXXFLAGS} ${CPUFGL} -DNUM_THREADS=3 -o build/glxwindower.o -c 3d/glxwindower.cxx

build/polymer.o: 3d/polymer.cxx 3d/polymer.hxx 3d/buffered_geom.hxx
	${CXX} ${CXXFLAGS} ${CPUFGL} -DNUM_THREADS=3 -o build/polymer.o -c 3d/polymer.cxx

build/xlinker.o: 3d/xlinker.cxx 3d/xlinker.hxx 3d/buffered_geom.hxx
	${CXX} ${CXXFLAGS} ${CPUFGL} -DNUM_THREADS=3 -o build/xlinker.o -c 3d/xlinker.cxx

build/sphere.o: 3d/sphere.cxx 3d/sphere.hxx 3d/buffered_geom.hxx
	${CXX} ${CXXFLAGS} ${CPUFGL} -DNUM_THREADS=3 -o build/sphere.o -c 3d/sphere.cxx

build/inputmain.o: 3d/inputmain.cxx
	${CXX} ${CXXFLAGS} ${CPUFGL} -DNUM_THREADS=3 -o build/inputmain.o -c 3d/inputmain.cxx

clean:
	rm nucleoid nucleoid.gl infofile/infotest build/*.o build/*.c build/*.h
