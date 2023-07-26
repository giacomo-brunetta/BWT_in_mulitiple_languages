#include "constants.h"

struct suffix_trie{
  struct suffix_trie* children[ALPHABET_DIMENSION+1];
};

void initialize_suffix_trie_node(struct suffix_trie** node);
void create_suffix_trie(struct suffix_trie** node, char* original, int size);
int suffix_trie_search(struct suffix_trie* root, char* substr, int size_s, int times);
int suffix_trie_search_once(struct suffix_trie* root, char* substr, int size_s);
