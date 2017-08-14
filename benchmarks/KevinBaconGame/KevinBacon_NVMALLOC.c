#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "nvmalloc.h"

#define HASH_DOMAIN 4096 * 64 // Number of hash entries
#define EDGE_GROUP 8 // Number of edges to be puted together
#define LINE_BUFFER_LENGHT 200

const unsigned int hashMask = HASH_DOMAIN - 1; // To crop the Hash function
int colorThreshold = 1; // Minimum value must be 1 (color Scheme)
#define COLOR_STEP 1;

// dbj2 Hash function
unsigned long hash(char *str){
  unsigned long hash = 5381;
  int c;

  c = *str++;
  while (c){
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    c = *str++;
  }

  return hash;
}

// Cropping (values between 0 and hashMask)
int croppedHash(char *str){
  return (int) (hash(str) & hashMask);
}
/************/

// Nodes and Edges for the Graphs
struct _edge;
struct _node;

struct _edge{
  struct _node *target[EDGE_GROUP];
  int amount;
  struct _edge *next;
};
typedef struct _edge edge;

struct _node{
  char *name;
  int color;
  struct _edge *link;
  struct _node *prev; // For BFS (THIS IS NOT A DOUBLE-LINKED LIST!)
  struct _node *next; // Linked list for direct search
};
typedef struct _node node;

struct _pStruct{
  node **hashedActorsTable;
  node **hashedMoviesTable;
  int savedColorThreshold;
};
typedef struct _pStruct pStruct;


// Every edge should be allocated using this function
edge* newEdge(){
  edge *ep = pcalloc(1, sizeof(edge));
  return ep;
}

// Every edge should be deallocated using this function
void deleteEdge(edge *ep){
  assert(ep != NULL);
  if(ep->next != NULL){ deleteEdge(ep->next); }
  pfree(ep);
}

// Every node should be allocated using this function
node* newNode(char *name){
  assert(name != NULL);
  node *np = pcalloc(1, sizeof(node));
  np->name = pmalloc( (strlen(name) + 2) * sizeof(char)); // +2 for \0 and safeness
  strcpy(np->name, name);
  np->link = newEdge();
  return np;
}

// Every node should be deallocated using this function
void deleteNode(node *np){
  assert(np != NULL);
  pfree(np->name);
  deleteEdge(np->link);
  pfree(np);
}

// Adding edge in both directions (a->b and a<-b)
void addEdge(node *a, node *b){
  edge *ep;
  // Making A -> B
  if(a->link->amount == EDGE_GROUP){
    ep = a->link;
    while(ep->next != NULL) ep = ep->next;
    if(ep->amount == EDGE_GROUP){
      ep->next = newEdge();
      ep = ep->next;
    }
    ep->target[ep->amount] = b;
    ep->amount = ep->amount + 1;
  }else{
    a->link->target[a->link->amount] = b;
    a->link->amount = a->link->amount + 1;
  }

  // Making B -> A
  if(b->link->amount == EDGE_GROUP){
    ep = b->link;
    while(ep->next != NULL) ep = ep->next;
    if(ep->amount == EDGE_GROUP){
      ep->next = newEdge();
      ep = ep->next;
    }
    ep->target[ep->amount] = a;
    ep->amount = ep->amount + 1;
  }else{
    b->link->target[b->link->amount] = a;
    b->link->amount = b->link->amount + 1;
  }

}

/**************/

node* findNode(node *list, char *name){
  while(list != NULL){
    if(strcmp(name, list->name) == 0){
      return list; // Found the node
    }
    list = list->next;
  }

  return NULL; // Didnt find
}

void printNodeList(node *list){
  while(list != NULL){
    printf("%s\n", list->name);
    list = list->next;
  }
}

long int getLineLenght(FILE *arq){
  long int lineLenght = 0;
  long int tempLineLenght = 0;
  char c;
  c = fgetc(arq);
  while(c != EOF){
    if(c!='\n'){
      tempLineLenght++;
    }else{
      if(tempLineLenght > lineLenght){
        lineLenght = tempLineLenght;
      }
      tempLineLenght =0;
    }
    c = fgetc(arq);
  }

  if(tempLineLenght > lineLenght){
    lineLenght = tempLineLenght;
  }

  return lineLenght + 10; // +10 for \n, \0 and safeness

}

/********** BFS *************/

// Every node should have color 0 and origin nullified (->prev = NULL)
/*void resetNodesForBFS(node *list){
  while(list != NULL){
    //printf("%s\n", list->name);
    list->color = 0;
    list->prev = NULL;
    list = list->next;
  }
}*/

// Struct just to point to a node
struct _nodeSearch{
  node *tar;
  struct _nodeSearch *prev;
  struct _nodeSearch *next;
};
typedef struct _nodeSearch nodeSearch;

// Queue to be used by the BFS algorithm
struct _searchQueue{
    nodeSearch head;
    nodeSearch tail;
};
typedef struct _searchQueue searchQueue;

void enqueueSearch(searchQueue *seQ, nodeSearch *nS){
  nS->prev = seQ->tail.prev;
  nS->next = &seQ->tail;
  seQ->tail.prev = nS;
  nS->prev->next = nS;
}

void dequeueSearch(searchQueue *seQ){
  if(seQ->head.next == &seQ->tail){
    return;
  }

  nodeSearch *noS;

  seQ->head.next->next->prev = &seQ->head;
  noS = seQ->head.next->next;
  free(seQ->head.next);
  seQ->head.next = noS;
}

int isSearchQueueEmpty(searchQueue *seQ){
  if(seQ->head.next == &seQ->tail){
    return 1; // Vazio
  }else{
    return 0;
  }
}

node* searchQueueFront(searchQueue *seQ){
  return seQ->head.next->tar;
}

// Breadth-First Search algorithm
// Returns the found node (if found) with all the track (->prevs) to the source node
// Return NULL if couldnt find the target node
node* BFS(node *np, char *target){

  int i;

  searchQueue seQ;
  seQ.head.next = &seQ.tail;
  seQ.tail.prev = &seQ.head;
  np->color = colorThreshold;
  np->prev = NULL;
  nodeSearch *noS;

  noS = malloc(sizeof(nodeSearch));
  noS->tar = np;
  enqueueSearch(&seQ, noS);

  node *curNp;

  while(isSearchQueueEmpty(&seQ) == 0){
    curNp = searchQueueFront(&seQ);
    if(strcmp(curNp->name, target) == 0){
      while(!isSearchQueueEmpty(&seQ)){
        dequeueSearch(&seQ);
      }
      return curNp; // Found :)
    }

    edge *ep; //edge pointer
    ep = curNp->link;
    while(ep != NULL){
      for(i = 0; i < ep->amount; i++){
        if(ep->target[i]->color < colorThreshold){
          noS = malloc(sizeof(nodeSearch));
          noS->tar = ep->target[i];
          noS->tar->color = colorThreshold;
          noS->tar->prev = curNp;
          enqueueSearch(&seQ, noS);
        }
      }
      ep = ep->next;
    }
    dequeueSearch(&seQ);
  }

  return NULL; // Could not find :/

}

void printAntecessorFirst(node *np, int i){
  assert(np!=NULL);
  if(np->prev!=NULL){
    printAntecessorFirst(np->prev, !i);
  }
  if(i == 0){
    printf("%s\n", np->name);
  }else{
    printf("trabalhou em \"%s\" com\n", np->name);
  }
}
/**********/


pStruct* buildTheGraph(int argc, char **argv){

  pStruct *pRoot;
  node **listOfActors; // Hash table for actors
  node **listOfMovies; // Hash table for movies

  pRoot = pmalloc(sizeof(pStruct));
  pRoot->savedColorThreshold = colorThreshold;
  listOfActors = pcalloc(1, HASH_DOMAIN * sizeof(node*));
  listOfMovies = pcalloc(1, HASH_DOMAIN * sizeof(node*));
  pRoot->hashedActorsTable = listOfActors;
  pRoot->hashedMoviesTable = listOfMovies;
  pset_root(pRoot);

  FILE *fp_database;

  fp_database = fopen(argv[1], "rt");
  assert(fp_database != NULL);

  //******************* BUILDING THE GRAPH *********************************//
  unsigned long int lineBufferLenght = getLineLenght(fp_database);
  rewind(fp_database);

  int aux;
  char *lineBuffer = malloc(lineBufferLenght * sizeof(char));
  char *curActor, *curMovie;
  long int actorHashValue, movieHashValue;
  node *actorNode;
  node *movieNode;

  aux = getline(&lineBuffer, &lineBufferLenght, fp_database);
  while(aux > 0){
    curActor = strtok(lineBuffer, "\t");
    curMovie = strtok(NULL, "\n");
    actorHashValue = croppedHash(curActor);
    movieHashValue = croppedHash(curMovie);

    actorNode = findNode(listOfActors[actorHashValue], curActor);
    if(actorNode == NULL){
      actorNode = newNode(curActor);
      actorNode->next = listOfActors[actorHashValue];
      listOfActors[actorHashValue] = actorNode;
    }


    movieNode = findNode(listOfMovies[movieHashValue], curMovie);
    if(movieNode == NULL){
      movieNode = newNode(curMovie);
      movieNode->next = listOfMovies[movieHashValue];
      listOfMovies[movieHashValue] = movieNode;
    }


    addEdge(actorNode, movieNode);

    aux = getline(&lineBuffer, &lineBufferLenght, fp_database);
  }

  free(lineBuffer);
  fclose(fp_database);
  return pRoot;
  //************************************************************************//
}


void makeSearches(pStruct *pRoot, int argc, char **argv){

  assert(pRoot != NULL);

  FILE *fp_requests;
  int aux;
  unsigned long int lineBufferLenght = LINE_BUFFER_LENGHT;
  node *actorNode;
  long int actorHashValue;

  node **listOfActors; // Hash table for actors
  //node **listOfMovies; // Hash table for movies

  listOfActors = pRoot->hashedActorsTable;
  //listOfMovies = pRoot->hashedMoviesTable;


  fp_requests = fopen(argv[2], "rt");
  assert(fp_requests != NULL);

  //***************** DOING THE SEARCHES *************************************//
  node *curNp;

  char *actorOrigin = malloc(lineBufferLenght * sizeof(char));
  char *actorDestiny = malloc(lineBufferLenght * sizeof(char));


  aux = getline(&actorOrigin, &lineBufferLenght, fp_requests);
  actorOrigin[strlen(actorOrigin) -1 ] = '\0';
  aux = getline(&actorDestiny, &lineBufferLenght, fp_requests);
  if(actorDestiny[strlen(actorDestiny) -1 ] == '\n'){
    actorDestiny[strlen(actorDestiny) -1 ] = '\0';
  }

  while(aux > 0){

    actorHashValue = croppedHash(actorOrigin);
    actorNode = findNode(listOfActors[actorHashValue], actorOrigin);
    assert(actorNode != NULL);

    /*for( i = 0; i < HASH_DOMAIN; i++){
      resetNodesForBFS(listOfActors[i]);
    }
    for( i = 0; i < HASH_DOMAIN; i++){
      resetNodesForBFS(listOfMovies[i]);
    }*/

    curNp = BFS(actorNode, actorDestiny);

    if(curNp == NULL){
      printf("CAMINHO NAO ENCONTRADO\n");
    }else{
      printAntecessorFirst(curNp, 0U);
    }

    aux = getline(&actorOrigin, &lineBufferLenght, fp_requests);
    actorOrigin[strlen(actorOrigin) -1 ] = '\0';
    aux = getline(&actorDestiny, &lineBufferLenght, fp_requests);
    if(actorDestiny[strlen(actorDestiny) -1 ] == '\n'){
      actorDestiny[strlen(actorDestiny) -1 ] = '\0';
    }

    colorThreshold = colorThreshold + COLOR_STEP;

  }

  fclose(fp_requests);

}



int main(int argc, char **argv){

  if(argc < 3 || argc > 4){
    printf("[ERROR] Expected exactly 2 or 3 parameters\n");
    return -1;
  }

  if(argc == 4){
    pinit(argv[3]);
  }else{
    pinit("kb.dump");
  }

  pStruct *pRoot;

  pRoot = pget_root();

  if( pRoot == NULL ){
    pRoot = buildTheGraph(argc, argv);
  }

  colorThreshold = pRoot->savedColorThreshold;

  makeSearches(pRoot, argc, argv);

  pRoot->savedColorThreshold = colorThreshold;
  pdump();

  return 0;
}
