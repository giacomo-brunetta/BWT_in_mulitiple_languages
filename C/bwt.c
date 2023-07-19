#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define END '$'
#define ALPHABET_DIMENSION 26

typedef struct {
  char* string;
  char* bwt;
  int* suff_arr;
  int* b_rank;
  int cum_count[ALPHABET_DIMENSION];
  int length;
}bwt;

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

int naif_search_optimized(char* original, int length_o, char* substr, int length_s){

  int found=0;
  int partial=0;

  for(int i=0, j=0;i<=length_o;i++){

    if(original[i]==substr[j]) { j++; partial=1;}
    else if(partial) { j=0; partial=0; }

    if(j==length_s) return 1;

  }

  return 0;
}

int naif_search(char* original, int length_o, char* substr, int length_s){

  int found=0;
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

int default_search(char* original, char* substr){

  if(strstr(original, substr)==NULL) return 0;
  return 1;

}

void free_bwt(bwt* string){

  free(string->bwt);
  free(string->suff_arr);
  free(string->b_rank);
  free(string);

  return;
}

int main(int argc, char* argv[]){

    LARGE_INTEGER frequency;
    LARGE_INTEGER t1, t2;
    double time_elapsed;

    QueryPerformanceFrequency(&frequency);

    int len_o=0;

    int len_s=0;

    int times=0;

    int len_toread=0;

    scanf("%d\n", &len_o);

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

    printf("Original String: %s\n",string->string);

    QueryPerformanceCounter(&t1);

    build_suf_arr(string);

    int numeric_string[len_o+3];

    for(i=0;i<len_o;i++) numeric_string[i]=string->string[i]-'a'+1;
    numeric_string[len_o]=0;
    numeric_string[len_o+1]=0;
    numeric_string[len_o+2]=0;

    suffix_array(numeric_string, string->suff_arr, len_o, ALPHABET_DIMENSION);

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

    QueryPerformanceCounter(&t2);

    time_elapsed=(t2.QuadPart-t1.QuadPart)*1000.0/frequency.QuadPart;

    printf("BWT Creation time: %f ms\n",time_elapsed);

    printf("BWT: %s\n",string->bwt);
    printf("B rank: ");
    for(i=0;i<len_o+1;i++) printf("%d ", string->b_rank[i]);
    puts("");
    printf("Cumulative count: ");
    for(i=0;i<ALPHABET_DIMENSION;i++) printf("%d ", string->cum_count[i]);
    puts("");

    char twb[len_o+1];
    twb[len_o]='\0';
    build_twb(string, twb);
    printf("Original stirng from bwt: %s", twb);
    puts("");

    printf("Substring length %d\n", len_s);
    printf("Substring: %s\n", substring);

    if(len_s>len_o) {printf("Substring longer than the string itself "); return 1; free_bwt(string);}

    puts("Searching with naif method...");

    int result=0;

    QueryPerformanceCounter(&t1);

    result=naif_search(string->string, len_o, substring, len_s);

    QueryPerformanceCounter(&t2);

    if(result) puts("Found");
    else puts("Not found");

    time_elapsed=(t2.QuadPart-t1.QuadPart)*1000.0/frequency.QuadPart;

    printf("Substring search naif: %f ms\n",time_elapsed);

    puts("Searching with naif method optimized...");

    QueryPerformanceCounter(&t1);

    result=naif_search_optimized(string->string, len_o, substring, len_s);

    QueryPerformanceCounter(&t2);

    if(result) puts("Found");
    else puts("Not found");

    time_elapsed=(t2.QuadPart-t1.QuadPart)*1000.0/frequency.QuadPart;

    printf("Substring search naif optimized: %f ms\n",time_elapsed);

    puts("Searching with default method...");

    QueryPerformanceCounter(&t1);

    result=default_search(string->string, substring);

    QueryPerformanceCounter(&t2);

    if(result) puts("Found");
    else puts("Not found");

    time_elapsed=(t2.QuadPart-t1.QuadPart)*1000.0/frequency.QuadPart;

    printf("Substring search default: %f ms\n",time_elapsed);

    return 1;
}
