all: escalona.c 
	gcc -g -o escalona escalona.c grafo.c linkedlist.c

clean: 
	$(RM) escalona