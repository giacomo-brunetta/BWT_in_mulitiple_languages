#ifndef BWT_H_INCLUDED
#define BWT_H_INCLUDED

#include "constants.h"

typedef struct {
  char* string;
  char* bwt;
  int* suff_arr;
  int* b_rank;
  int cum_count[ALPHABET_DIMENSION];
  int length;
}bwt;

void build_suf_arr(bwt* string);

void build_bwt(bwt* string);

void build_b_rank_cum_count(bwt* string);

void build_twb(bwt* string, char* twb);

int leq_pairs(int a1, int a2, int b1, int b2);

int leq_triples(int a1, int a2, int a3, int b1, int b2, int b3);

void radix_pass(int* a, int* b, int* r, int n, int k);

void suffix_array(int* t, int* sa, int n, int k);

int bwt_search(bwt* string, char* substr, int substr_len, int times);

int bwt_search_once(bwt* string, char* substr, int substr_len);

void free_bwt(bwt* string);

#endif
