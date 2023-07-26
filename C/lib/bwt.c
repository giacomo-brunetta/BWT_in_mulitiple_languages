#include "bwt.h"
#include <stdlib.h>

void changed(){return ;}

void build_suf_arr(bwt* string){

  for(int i=0;i<=string->length;i++) string->suff_arr[i]=i;

  return;
}

void build_bwt(bwt* string){

  string->bwt[string->length+1]='\0';

  for(int i=0;i<string->length+1;i++){
    if(string->suff_arr[i]) string->bwt[i]=string->string[string->suff_arr[i]-1];
    else string->bwt[i]=END;
  }

  return;
}

void build_b_rank_cum_count(bwt* string){
  int i=0;

  for(i=0;i<string->length+1;i++){
    string->b_rank[i]=0;
  }
  for(i=0;i<ALPHABET_DIMENSION;i++){
    string->cum_count[i]=0;
  }

  for(i=0;i<string->length+1;i++){
    if(string->bwt[i]!=END){
      string->b_rank[i]=string->cum_count[string->bwt[i]-'a']++;
    }
  }

  int offset = 1+string->cum_count[0];
  string->cum_count[0]=1;
  int count;

  for(char c = 'b'; c <= 'z'; c++){
    count = string->cum_count[c-'a'];
    string->cum_count[c-'a'] = offset;
    offset += count;
  }

  return;
}

void build_twb(bwt* string, char* twb){

  int i=0;
  int j=string->length-1;

  while(string->bwt[i]!=END){
    twb[j]=string->bwt[i];
    j--;
    i=string->cum_count[string->bwt[i]-'a']+string->b_rank[i];
  }

  return;
}

int leq_pairs(int a1, int a2, int b1, int b2){
  if(a1<b1 || a1==b1 && a2<=b2) return 1;
  return 0;
}

int leq_triples(int a1, int a2, int a3, int b1, int b2, int b3){
  if(a1<b1 || a1==b1 && leq_pairs(a2,a3,b2,b3)) return 1;
  return 0;
}

void radix_pass(int* a, int* b, int* r, int n, int k){
  int* c=malloc(sizeof(int)*(k+1));
  for(int i=0;i<=k;i++) c[i]=0;
  for(int i=0;i<n;i++) {c[r[a[i]]]++; }
  for(int i=0,sum=0;i<=k;i++){
    int t=c[i];
    c[i]=sum;
    sum+=t;
  }
  for(int i=0;i<n;i++) b[c[r[a[i]]]++]=a[i];
  free(c);
}

//https://www.cs.helsinki.fi/u/tpkarkka/publications/jacm05-revised.pdf
void suffix_array(int* t, int* sa, int n, int k){
  int n0=(n+2)/3;
  int n1=(n+1)/3;
  int n2=n/3;
  int n02=n0+n2;
  int* r=malloc(sizeof(int)*(n02+3));
  r[n02]=0;
  r[n02+1]=0;
  r[n02+2]=0;
  int* sa12=malloc(sizeof(int)*(n02+3));
  sa12[n02]=0;
  sa12[n02+1]=0;
  sa12[n02+2]=0;
  int* r0=malloc(sizeof(int)*n0);
  int* sa0=malloc(sizeof(int)*n0);

  for(int i=0,j=0;i<n+(n0-n1);i++) if(i%3) r[j++]=i;

  radix_pass(r, sa12, t+2, n02, k);
  radix_pass(sa12, r, t+1, n02, k);
  radix_pass(r, sa12, t, n02, k);

  int name=0, c0=-1, c1=-1, c2=-1;

  for(int i=0;i<n02;i++){
    if(t[sa12[i]]!=c0 || t[sa12[i]+1]!=c1 || t[sa12[i]+2]!=c2){
      name++;
      c0=t[sa12[i]];
      c1=t[sa12[i]+1];
      c2=t[sa12[i]+2];
    }
    if(sa12[i]%3 == 1) r[sa12[i]/3] =name;
    else r[sa12[i]/3+n0]=name;
  }

  if(name<n02){
    suffix_array(r, sa12, n02, name);
    for(int i=0;i<n02;i++) r[sa12[i]]=i+1;
  }
  else for(int i=0;i<n02;i++) sa12[r[i]-1]=i;

  for(int i=0,j=0;i<n02;i++) if(sa12[i]<n0) r0[j++]=3*sa12[i];
  radix_pass(r0, sa0, t, n0, k);

  for(int p=0, u=n0-n1, k=0; k<n;k++){
    int i=sa12[u]<n0 ? sa12[u]*3+1 : (sa12[u]-n0)*3+2;
    int j=sa0[p];

    if(sa12[u]<n0 ? leq_pairs(t[i], r[sa12[u]+n0], t[j], r[j/3]) : leq_triples(t[i], t[i+1], r[sa12[u]-n0+1], t[j], t[j+1], r[j/3+n0])){
      sa[k]=i;
      u++;
      if(u==n02) for(k++;p<n0;p++,k++) sa[k]=sa0[p];
    }
    else{
      sa[k]=j;
      p++;
      if(p==n0) for(k++;u<n02;u++,k++) sa[k]=sa12[u]<n0 ? sa12[u]*3+1 : (sa12[u]-n0)*3+2;
    }

  }

  free(r);
  free(sa12);
  free(r0);
  free(sa0);
  return;
}

int bwt_search(bwt* string, char* substr, int substr_len,  int times){

  int found=0;

  if(substr[substr_len-1]-'a'==ALPHABET_DIMENSION-1 && string->cum_count[substr[substr_len-1]-'a']==string->length) return 0;

  if(string->cum_count[substr[substr_len-1]-'a']==string->cum_count[substr[substr_len-1]-'a'+1]) return 0;	  

  int iterations= substr[substr_len-1]-'a' != ALPHABET_DIMENSION-1 ? 
    string->cum_count[substr[substr_len-1]-'a'+1] - string->cum_count[substr[substr_len-1]-'a'] :
    string->length - string->cum_count[substr[substr_len-1]-'a'];

  for(int i=0;i<iterations;i++){

    int match=1;
    int index=i+string->cum_count[substr[substr_len-1]-'a'];
    int pos_substr=substr_len-1;

    while(match){

      if(--pos_substr < 0){ found++; match=0; }

      else if(string->bwt[index]!=substr[pos_substr]) match=0;

      else index=string->cum_count[string->bwt[index]-'a']+string->b_rank[index];

    }

  }

  return found>=times;

}

int bwt_search_once(bwt* string, char* substr, int substr_len){

  if(substr[substr_len-1]-'a'==ALPHABET_DIMENSION-1 && string->cum_count[substr[substr_len-1]-'a']==string->length) return 0;

  if(string->cum_count[substr[substr_len-1]-'a']==string->cum_count[substr[substr_len-1]-'a'+1]) return 0;

  int iterations= substr[substr_len-1]-'a' != ALPHABET_DIMENSION-1 ?
    string->cum_count[substr[substr_len-1]-'a'+1] - string->cum_count[substr[substr_len-1]-'a'] :
    string->length - string->cum_count[substr[substr_len-1]-'a'];

  for(int i=0;i<iterations;i++){

    int match=1;
    int index=i+string->cum_count[substr[substr_len-1]-'a'];
    int pos_substr=substr_len-1;

    while(match){

      if(--pos_substr < 0) return 1;

      else if(string->bwt[index]!=substr[pos_substr]) match=0;

      else index=string->cum_count[string->bwt[index]-'a']+string->b_rank[index];

    }

  }

  return 0;

}


void free_bwt(bwt* string){

  free(string->bwt);
  free(string->suff_arr);
  free(string->b_rank);
  free(string);

  return;
}
