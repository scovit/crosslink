all: ac diag distancematrix inertia merr pc puntadiacency rho ss adiacency

ac: ac.c
	gcc -O2 -std=gnu99 -o ac ac.c

diag: diag.c
	gcc -O2 -std=gnu99 -o diag diag.c -lm

distancematrix: distancematrix.c
	gcc -O2 -std=gnu99 -o distancematrix distancematrix.c 

inertia: inertia.c
	gcc -O2 -std=gnu99 -o inertia inertia.c 

merr: merr.c
	gcc -O2 -std=gnu99 -o merr merr.c 

pc: pc.c
	gcc -O2 -std=gnu99 -o pc pc.c -lm

puntadiacency: puntadiacency.c
	gcc -O2 -std=gnu99 -o puntadiacency puntadiacency.c -lm

adiacency: adiacency.c
	gcc -O2 -std=gnu99 -o adiacency adiacency.c -lm

rho: rho.c
	gcc -O2 -std=gnu99 -o rho rho.c -lm

ss: ss.c
	gcc -O2 -std=gnu99 -o ss ss.c 

clean:
	rm ac diag distancematrix inertia merr pc puntadiacency rho ss adiacency

