# Multi Language Experiments
 This repo is created to experiment with different programming languages. The goal is to code up the same algorithms in as many languages as possible to compare the different approaches to the problem and the different options that they offer.

 ## The algorithms we chose to implement
We chose to compare languages in the problem of **substring search**. The search can be done in 3 main approaches:
- The naif **running window** or the equivalent default search algorithm proposed by the language standard;
- The fast, but costly in terms of memory [**Suffix Trie**](https://en.wikipedia.org/wiki/Suffix_tree);
- The not as fast, but very memory un-expensive [**Burrows-Wheeler Transform**](en.wikipedia.org/wiki/Burrows–Wheeler_transform).

A specific focus will be put on this last approach. Every code snippet should be able to **compute the direct and the inverse BWT**.
For simplicity, the B-ranks will be stored explicitly, instead of using indirect approaches such as [**FM indexing**](https://en.wikipedia.org/wiki/FM-index).

# What we have done so far

### Java
| Feature | Status |
| --- | :---: |
| BWT | :green_circle: |
| TWB | :green_circle: |
| Bwt search | :green_circle: |
| Bwt parallel search | :green_circle: |
| Suffix Trie | :green_circle: |
| Naive | :green_circle: |


### C
| Feature | Status |
| --- | :---: |
| BWT | :green_circle: |
| TWB | :green_circle: |
| Bwt search | :red_circle: |
| Bwt parallel search | :red_circle: |
| Suffix Trie | :red_circle: |
| Naive | :red_circle: |

### Feel free to contribute!
