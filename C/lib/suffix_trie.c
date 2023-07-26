#include "suffix_trie.h"
#include <stdio.h>
#include <stdlib.h>


void initialize_suffix_trie_node(struct suffix_trie** node){
  for(int i=0;i<ALPHABET_DIMENSION+1;i++){
    (*node)->children[i]=NULL;
  }
  return;
}

void create_suffix_trie(struct suffix_trie** root, char* original, int size){
  if(original[size]!=END){
    printf("Insert end character %c then redo creation...\n",END);
    return;
  }

  for(int i=0;i<size;i++){
    struct suffix_trie* temp=*root;
    for(int j=i;j<size;j++){
      if(temp->children[original[j]-'a']==NULL){
	temp->children[original[j]-'a']=malloc(sizeof(struct suffix_trie));
	initialize_suffix_trie_node(&(temp->children[original[j]-'a']));
      }
      temp=temp->children[original[j]-'a'];
    }
    temp->children[ALPHABET_DIMENSION]=malloc(sizeof(struct suffix_trie));
  }



}

int walk_and_count_branches_suffix_trie(struct suffix_trie* node, int found){
  int iteration=0;
  for(int i=0;i<ALPHABET_DIMENSION+1;i++){
    if(iteration){
      if(node->children[i]!=NULL){
      	found++;
    	found=walk_and_count_branches_suffix_trie(node->children[i],found);
      }
    }
    else if(node->children[i]!=NULL){
      iteration++;
      found=walk_and_count_branches_suffix_trie(node->children[i],found);
    }
  }
  return found;
}

int suffix_trie_search(struct suffix_trie* root, char* substr, int size_s, int times){
  int found=0;

  struct suffix_trie* temp=root;

  for(int i=0;i<size_s;i++){
    if(temp->children[substr[i]-'a']==NULL)return 0;

    temp=temp->children[substr[i]-'a'];
  }

  found=walk_and_count_branches_suffix_trie(temp, 1);

  return found>=times;
}

int suffix_trie_search_once(struct suffix_trie* root, char* substr, int size_s){

  struct suffix_trie* temp=root;

  for(int i=0;i<size_s;i++){
    if(temp->children[substr[i]-'a']==NULL)return 0;

    temp=temp->children[substr[i]-'a'];
  }

  return 1;
}

