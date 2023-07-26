#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "bwt.h"
#include "suffix_trie.h"
#include "constants.h"

int naif_search(char* original, int length_o, char* substr, int length_s, int times){

  int found=0;
  int mismatch=0;

  for(int i=0;i<length_o-length_s+1;i++){

    if(original[i]==substr[0]){

      int j=1;
      while(!mismatch){
	if(original[i+j]!=substr[j]) mismatch=1;
	else j++;
	if(j==length_s) {found++;mismatch=1;}
      }
      mismatch=0;
    }

  }

  return found>=times;
}

int naif_search_once(char* original, int length_o, char* substr, int length_s){

  int mismatch=0;

  for(int i=0;i<length_o-length_s+1;i++){

    if(original[i]==substr[0]){

      int j=1;
      while(!mismatch){
        if(original[i+j]!=substr[j]) mismatch=1;
        else j++;
        if(j==length_s) return 1;
      }
      mismatch=0;
    }

  }

  return 0;
}


int default_search(char* original, char* substr, int times){

  int found=0;
  char* next = strstr(original, substr);
  while ( next != NULL ){
    next = strstr(next+1, substr);
    found++;
  }
  return found>=times;

}

int default_search_once(char* original, char* substr){

  if(strstr(original, substr)!=NULL)return 1;
  return 0;

}

void create_output_file(int len_o, int len_s, int times, double time_bwt_create, double time_bwt_search, double time_st_create, double time_st_search, double time_naif_search){

  FILE* f=fopen("RESULT/result.json","w");
  
  fprintf(f,"{\n");
  fprintf(f,"  \"language\": \"C\",\n");
  fprintf(f,"  \"length\": %d,\n",len_o);
  fprintf(f,"  \"sub_length\": %d,\n", len_s);
  fprintf(f,"  \"test_times\": %d,\n", times);
  fprintf(f,"  \"BWT_creation\": %f,\n",time_bwt_create);
  fprintf(f,"  \"BWT_search\": %f,\n",time_bwt_search);
  fprintf(f,"  \"BWT_search_parallel\": \"Infinite\",\n");
  fprintf(f,"  \"ST_creation\": %f,\n", time_st_create);
  fprintf(f,"  \"ST_search\": %f,\n", time_st_search);
  fprintf(f,"  \"NAIF_search\": %f\n", time_naif_search);
  fprintf(f,"}\n");

  fclose(f);

}


int main(int argc, char* argv[]){

  set_frequency();

  int len_o=0;

  int len_s=0;

  int times=0;

  double time_bwt_create=0;
  double time_bwt_search=0;
  double time_st_create=0;
  double time_st_search=0;
  double time_naif_search=0;

  size_t len_toread=0;

  scanf("%d\n", &len_o);

  printf("String length %d\n", len_o);

  bwt* string=malloc(sizeof(bwt));

  string->string=malloc(sizeof(char)*(len_o+2));
  string->bwt=malloc(sizeof(char)*(len_o+1));
  string->suff_arr=malloc(sizeof(int)*(len_o+1));
  string->b_rank=malloc(sizeof(int)*(len_o+1));
  string->length=len_o;

  char* temp_string=NULL;
  getline(&temp_string, &len_toread, stdin);

  strncpy(string->string,temp_string,len_o);
  string->string[len_o]=END;
  string->string[len_o+1]='\0';

  scanf("%d\n", &len_s);

  char* substring=NULL;

  getline(&substring, &len_toread, stdin);

  substring[len_s]='\0';

  scanf("%d\n", &times);

  int i=0;

  //printf("Original String: %s\n",string->string);

  start_timer();

  build_suf_arr(string);

  int* numeric_string=malloc(sizeof(int)*(len_o+3));

  for(i=0;i<len_o;i++) numeric_string[i]=string->string[i]-'a'+1;
  numeric_string[len_o]=0;
  numeric_string[len_o+1]=0;
  numeric_string[len_o+2]=0;

  suffix_array(numeric_string, string->suff_arr, len_o, ALPHABET_DIMENSION);

  free(numeric_string);

  //shift last character, should not be done in original algorithm
  for(int i=len_o;i>0;i--){
    string->suff_arr[i]=string->suff_arr[i-1];
  }
  string->suff_arr[0]=len_o;

  //printf("Suffix Array: ");
  //for(i=0;i<len_o+1;i++) printf("%d ", string->suff_arr[i]);
  //puts("");

  build_bwt(string);
  build_b_rank_cum_count(string);

  end_timer();

  time_bwt_create=get_time_elapsed();

  printf("BWT Creation time: %f s\n",time_bwt_create);

/*
  printf("BWT: %s\n",string->bwt);

  printf("B rank: ");
  for(i=0;i<len_o+1;i++) printf("%d ", string->b_rank[i]);
  puts("");

  printf("Cumulative count: ");
  for(i=0;i<ALPHABET_DIMENSION;i++) printf("%d ", string->cum_count[i]);
  puts("");
*/

  start_timer();

  char* twb=malloc(sizeof(char)*(len_o+1));
  twb[len_o]='\0';
  build_twb(string, twb);

  end_timer();

  printf("TWB Creation time: %f s\n",get_time_elapsed());

  free(twb);

  //printf("Original stirng from bwt: %s", twb);
  //puts("");

  printf("Substring length %d\n", len_s);
  //printf("Substring: %s\n", substring);

  if(len_s>len_o) {printf("Substring longer than the string itself "); return 1; free_bwt(string);}

  puts("Searching with naif method...");

  int result=0;

  start_timer();

  result=naif_search(string->string, len_o, substring, len_s, times);

  end_timer();

  if(result) puts("Found");
  else puts("Not found");

  time_naif_search=get_time_elapsed();

  printf("Substring search naif: %f s\n",time_naif_search);

  puts("Searching with naif method once...");

  start_timer();

  result=naif_search_once(string->string, len_o, substring, len_s);

  end_timer();

  if(result) puts("Found");
  else puts("Not found");

  printf("Substring search naif once: %f s\n",get_time_elapsed());

  puts("Searching with default method...");

  start_timer();

  result=default_search(string->string, substring, times);

  end_timer();

  if(result) puts("Found");
  else puts("Not found");

  printf("Substring search default: %f s\n",get_time_elapsed());

  puts("Searching with default method once...");

  start_timer();

  result=default_search_once(string->string, substring);

  end_timer();

  if(result) puts("Found");
  else puts("Not found");

  printf("Substring search default once: %f s\n",get_time_elapsed());

  puts("Searching using bwt...");

  start_timer();

  result=bwt_search(string, substring, len_s, times);

  end_timer();

  if(result) puts("Found");
  else puts("Not found");

  time_bwt_search=get_time_elapsed();

  printf("Substring search bwt: %f s\n",time_bwt_search);

  puts("Searching using bwt once...");

  start_timer();

  result=bwt_search_once(string, substring, len_s);

  end_timer();

  if(result) puts("Found");
  else puts("Not found");

  printf("Substring search bwt once: %f s\n",get_time_elapsed());

  puts("Creating suffix trie...");

  start_timer();

  struct suffix_trie* root=malloc(sizeof(struct suffix_trie));
  initialize_suffix_trie_node(&root);

  create_suffix_trie(&root, string->string, len_o);

  end_timer();

  time_st_create=get_time_elapsed();

  printf("Suffix trie created: %f s\n", time_st_create);

  puts("Searching using suffix trie...");
  
  start_timer();

  result=suffix_trie_search(root, substring, len_s, times);

  end_timer();

  if(result) puts("Found");
  else puts("Not found");

  time_st_search=get_time_elapsed();

  printf("Substring search suffix trie: %f s\n",time_st_search);

  puts("Searching using suffix trie but just once...");

  start_timer();

  result=suffix_trie_search_once(root, substring, len_s);

  end_timer();

  if(result) puts("Found");
  else puts("Not found");

  printf("Substring search suffix trie once: %f s\n",get_time_elapsed());

  create_output_file(len_o, len_s, times, time_bwt_create, time_bwt_search, time_st_create, time_st_search, time_naif_search);

  return 1;
}
