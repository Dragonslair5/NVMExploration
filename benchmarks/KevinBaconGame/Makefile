
all: KevinBacon_NVMALLOC.o KevinBacon_NVMALLOC.exe KevinBacon.exe

clean:
	$(RM) *.o *.exe

KevinBacon.exe: KevinBacon.c
	gcc -O3 KevinBacon.c -o KevinBacon.exe

KevinBacon_NVMALLOC.o: KevinBacon_NVMALLOC.c
	gcc -c KevinBacon_NVMALLOC.c -o KevinBacon_NVMALLOC.o -Wall -O3 -std=gnu99 -DPAGE_SIZE=4096 -I$(NVMALLOCPATH)/src -lrt -lnvmalloc

KevinBacon_NVMALLOC.exe: KevinBacon_NVMALLOC.o
	gcc -L$(NVMALLOCPATH)/lib KevinBacon_NVMALLOC.o -o KevinBacon_NVMALLOC.exe -Wall -O3 -std=gnu99 -DPAGE_SIZE=4096 -I$(NVMALLOCPATH)/src -lrt -lnvmalloc
