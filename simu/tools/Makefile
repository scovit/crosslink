
all: ac diag distancematrix inertia merr pc puntadiacency rho ss

ac:
	gcc -O2 -std=gnu99 -o ac ac.c

diag:
	gcc -O2 -std=gnu99 -o diag diag.c -lm

distancematrix:
	gcc -O2 -std=gnu99 -o distancematrix distancematrix.c 

inertia:
	gcc -O2 -std=gnu99 -o inertia inertia.c 

merr:
	gcc -O2 -std=gnu99 -o merr merr.c 

pc:
	gcc -O2 -std=gnu99 -o pc pc.c -lm

puntadiacency:
	gcc -O2 -std=gnu99 -o puntadiacency puntadiacency.c -lm

rho:
	gcc -O2 -std=gnu99 -o rho rho.c -lm

ss:
	gcc -O2 -std=gnu99 -o ss ss.c 

clean:
	rm ac diag distancematrix inertia merr pc puntadiacency rho ss

