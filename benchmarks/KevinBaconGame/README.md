# Kevin Bacon Game

We adapted the allocation functions used to build the graph of the Kevin Bacon Game with the ones from the NVMalloc API.

A version with the NVMalloc API (__KevinBacon_NVMALLOC.c__) and without it (__KevinBacon.c__) are presented here.

A Makefile is provided intending to build both versions with the __make__ rule. For the NVMalloc version to be build, it is needed to link it with the NVMalloc API library.

Both versions expect at least 2 parameters as follow:

./KevinBacon.exe [Database file] [List of actors for queries (in pairs)]

A third parameter can be provided in then NVMalloc API version to inform the name of the dump file. If the third parameter is not provided, it will use a default name for the dump file.

The script that we used to download the database from IMDB is also provided: __baixa_e_processa_arquivos.sh__
It produces the file __atores.completos__, that is the database that we used.