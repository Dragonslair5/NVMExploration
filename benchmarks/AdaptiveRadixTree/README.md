# Adaptive Radix Tree (ART)

We utilized the source code of the ART from -> https://github.com/armon/libart

We adapted the memory allocation functions used in the ART to use the ones from the NVMalloc.

There is a Makefile that intents to build the executables with the rule __make__. For the NVMalloc version from ART, it is needed to link it with the NVMalloc library. WIthout it, it wont compile.

We provide a patch file that can be used to modify the original ART source code and produce the one adapted with the NVMalloc API. To apply this patch to the original source file, the following command can be used:

patch -p1 libart/src/art.c -o libart/src/art_nvmalloc.c < patch.txt

Or if you prefer, We also provide the already modified source code, in the __art_nvmalloc_c__ folder.

The programs are suposed to be run with at least 2 parameters, as follows:

./main.exe [file with the keys] [file with keys to be used in queries]

In the NVMalloc version, a third parameter can be used to inform the name of the dump file. If the third parameter is not sent, a standard name for the dump file is used.