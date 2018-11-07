randlibsw.so: randlibsw.c
	$(CC) -shared -fPIC $(CFLAGS) randlibsw.c -o randlibsw.so

randlibhw.so: randlibhw.c 
	$(CC) -shared -fPIC $(CFLAGS) randlibhw.c -o randlibhw.so

randmain: randmain.o randcpuid.o
	$(CC) $(CFLAGS) randmain.o randcpuid.o -ldl -Wl,-rpath=$PWD -o randmain
