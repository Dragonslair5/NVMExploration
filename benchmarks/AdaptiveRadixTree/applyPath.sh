#!/bin/bash
patch -p1 libart/src/art.c -o libart/src/art_nvmalloc.c < patch.txt
