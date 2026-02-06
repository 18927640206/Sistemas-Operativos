principal: todo

todo: ordena_fork_mem.o ordena_hilos_mem.o todo.c
	gcc -o todo todo.c ordena_fork_mem.o ordena_hilos_mem.o -lpthread

ordena_fork_mem.o: ordena_fork_mem.c
	gcc -c ordena_fork_mem.c

ordena_hilos_mem.o: ordena_hilos_mem.c
	gcc -c ordena_hilos_mem.c
