all: escalona.c 
	gcc -o escalona escalona.c grafo.c

clean: 
	$(RM) escalona