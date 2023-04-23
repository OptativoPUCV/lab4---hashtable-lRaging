#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) 
{
  size_t indice = hash(key, map->capacity);
  while (map->buckets[indice] && map->buckets[indice]->key != NULL) 
  {
    if (strcmp(map->buckets[indice]->key, key) == 0) return;
    indice = (indice + 1) % map->capacity; 
  }
  
  Pair *newPair = createPair(key, value);
  map->buckets[indice] = newPair;
  map->size++;
  map->current = indice;
}

void enlarge(HashMap * map) 
{
  enlarge_called = 1; //no borrar (testing purposes)
  Pair **old_buckets = map->buckets;
  map->capacity *= 2;
  map->buckets = (Pair**) calloc(map->capacity, sizeof(Pair*))
  map->size = 0;
  for (int indice = 0; indice < map->capacity; indice++) 
  {
    if (old_buckets[indice] != NULL && old_buckets[indice]->key != NULL) 
    {
      insertMap(map, old_buckets[indice]->key, old_buckets[indice]->value);
    }
  }
  free(old_buckets);
}


HashMap * createMap(long capacity)
{
  HashMap *mapa = (HashMap *)malloc(sizeof(HashMap));
  mapa->buckets = (Pair **)calloc(capacity, sizeof(Pair *));

  mapa->capacity = capacity;
  mapa->current = -1;
  return mapa;
}

void eraseMap(HashMap * map,  char * key) 
{    
  if(map==NULL || key == NULL) return;
  size_t indice = hash(key, map->capacity);
  while (map->buckets[indice] && map->buckets[indice]->key != NULL) 
  {
    if (map->buckets[indice]->key != NULL && strcmp(map->buckets[indice]->key, key) == 0) 
    {
      map->buckets[indice]->key = NULL;
      map->current = indice;
      map->size--;
      break;
    }
    indice = (indice + 1) % map->capacity; 
  }
  
}

Pair * searchMap(HashMap * map,  char * key) 
{   
  if(map==NULL || key == NULL) return NULL;
  size_t indice = hash(key, map->capacity);

  while (map->buckets[indice] && map->buckets[indice]->key != NULL) 
  {
    if (strcmp(map->buckets[indice]->key, key) == 0) 
    {
      map->current = indice;
      return map->buckets[indice];
    }
    indice = (indice + 1) % map->capacity; 
  }
  
  return NULL;
}

Pair * firstMap(HashMap * map) 
{
  if(map==NULL) return NULL;
  for(size_t indice = 0; indice<=map->capacity ; indice++)
  {
    if(map->buckets[indice]!=NULL && map->buckets[indice]->key !=NULL)
    {
      map->current = indice;
      return map->buckets[indice];
    }
  }
  return NULL;
}

Pair * nextMap(HashMap * map) 
{
  for (int indice = map->current+1; indice < map->capacity; indice++) 
  {
    if (map->buckets[indice] != NULL && map->buckets[indice]->key != NULL) 
    {
      map->current = indice;
      return map->buckets[indice];
    }
  }
  return NULL;
}
