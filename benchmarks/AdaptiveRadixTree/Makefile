LIBARTPATH=libart/src

all: main.exe main_nvmalloc.exe 

clean:
	$(RM) *.o *.exe

main.exe: $(LIBARTPATH)/art.c main.c
	gcc $(LIBARTPATH)/art.c main.c -I$(LIBARTPATH) -o main.exe -std=gnu99 -Wall

main_nvmalloc.exe: $(LIBARTPATH)/art_nvmalloc.c main_nvmalloc.c
	gcc -L$(NVMALLOCPATH)/lib -I$(LIBARTPATH) $(LIBARTPATH)/art_nvmalloc.c main_nvmalloc.c -o main_nvmalloc.exe -Wall -O3 -std=gnu99 -DPAGE_SIZE=4096 -I$(NVMALLOCPATH)/src -lrt -lnvmalloc

