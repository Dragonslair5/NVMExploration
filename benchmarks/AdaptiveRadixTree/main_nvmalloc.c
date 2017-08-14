#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "art.h"
#include "nvmalloc.h"

#define RECEIVER_LENGTH 60

int visitLeafs(void *data, const unsigned char *key, uint32_t key_len, void *value){
  printf("data -> %s\n", (char*) data);
  printf("value -> %s\n", (char*) value);
  return 0;
}

art_tree* buildTheArtTree(int argc, char **argv){
  printf("Building up the Adaptive Radix Tree...\n");
  art_tree *artTree;
  unsigned long int receiverLenght = RECEIVER_LENGTH;
  char *stringReceiver = malloc(receiverLenght * sizeof(char));
  int aux;
  char *nodeCreater;


  assert(argc >= 3);
  FILE *arqIn = fopen(argv[1], "rt");
  assert(arqIn != NULL);



  artTree = pmalloc(sizeof(art_tree));
  aux = art_tree_init(artTree);
  pset_root(artTree);

  aux = getline(&stringReceiver, &receiverLenght, arqIn);
  while(aux > 0){
    if(stringReceiver[strlen(stringReceiver)-1] == '\n'){
      stringReceiver[strlen(stringReceiver)-1] = '\0';
    }
    nodeCreater = pmalloc( (strlen(stringReceiver) + 1) * sizeof(char));
    strcpy(nodeCreater, stringReceiver);

    art_insert(artTree, (unsigned char*) nodeCreater, strlen(nodeCreater), nodeCreater);

    aux = getline(&stringReceiver, &receiverLenght, arqIn);
  }

  free(stringReceiver);
  fclose(arqIn);

  printf("Adaptive Radix Tree built\n");
  return artTree;
}


void makeSearches(art_tree *artTree, int argc, char **argv){
  printf("Processing queries...\n");
  unsigned long int receiverLenght = RECEIVER_LENGTH;
  char *queryReceiver = malloc(receiverLenght * sizeof(char));
  int aux;
  char *queryResult;

  assert(argc >= 3);
  FILE *arqIn = fopen(argv[2], "rt");
  assert(arqIn != NULL);

  aux = getline(&queryReceiver, &receiverLenght, arqIn);

  while(aux > 0){
    if(queryReceiver[strlen(queryReceiver)-1] == '\n'){
      queryReceiver[strlen(queryReceiver)-1] = '\0';
    }
    printf("Searching %s...\n", queryReceiver);
    queryResult = art_search(artTree, (unsigned char*) queryReceiver, strlen(queryReceiver));

    if(queryResult){
      printf("Address = %p Found -> %s\n", queryResult, queryResult);
    }else{
      printf("Key %s not found :(\n", queryReceiver);
    }

    aux = getline(&queryReceiver, &receiverLenght, arqIn);
  }


  free(queryReceiver);
  fclose(arqIn);
  printf("Queries processed\n");
}



int main(int argc, char **argv){

  if(argc < 3 || argc > 4){
    printf("[ERROR] Expected exactly 2 or 3 parameters\n");
    return -1;
  }


  art_tree *artTree;

  if(argc == 4){
    pinit(argv[3]);
  }else{
    pinit("art.dump");
  }

  artTree = pget_root();

  if(artTree == NULL){
    artTree = buildTheArtTree(argc, argv);
  }

  makeSearches(artTree, argc, argv);

  //pdump();

  return 0;
}
