# Multi Language Experiments
 This repo is created to experiment with different programming languages. The goal is to code up the same algorithms in as many languages as possible to compare the different approaches to the problem and the different options that they offer.

 ## The algorithms we chose to implement
We chose to compare languages in **substring searches**. The search can be done in 3 main approaches:
- The naif **running window** or the equivalent default search algorithm proposed by the language standard;
- The fast, but costly in terms of memory [**Suffix Trie**](https://en.wikipedia.org/wiki/Suffix_tree);
- The not as fast, but very memory un-expensive [**Burrows-Wheeler Transform**](https://en.wikipedia.org/wiki/Burrows–Wheeler_transform).

A specific focus will be put on this last approach. Every code snippet should be able to **compute the direct and the inverse BWT**.
For simplicity, the B-ranks will be stored explicitly, instead of using indirect approaches such as [**FM indexing**](https://en.wikipedia.org/wiki/FM-index).

# How languages are compared

## How to run tests
All programs need to perform the following tasks:
- take a file as input
- retrieve from the file:
    - ```"length"```: the string length
    - ```"str"```: a string
    - ```"sublen"```: the substring length
    - ```"sub"```: the substring to search for
    - ```"times"```: the number of times to search for the substring
- write on a different file the time it took to create each data structure and to perform all the queries

The run configuration of any program should look like this:
```
main TEST/[test_file]
```
All the test files can be found in the ```TEST``` folder, while all the test results should be put in ```TEST_RESULT``` folder.

The output file should be in JSON format and contain:
- ```"language"```: the language the code is written in
- ```"length"```: from input
- ```"sub_length"```: from input
- ```"test_times"```: from input
- ```"BWT_creation"```: time spent creating the BWT
- ```"BWT_search"```: time spent searching n times the substring in the BWT
- ```"BWT_search_parallel"```: time spent searching n times the substring in the BWT (but multi-threaded)
- ```"ST_creation"```: time spent creating the suffix trie
- ```"ST_search```": time spent searching n times the substring in the ST
- ```"NAIF_search"```: time spent searching n times the substring with the running window method.

If a search is missing, failed, or was not performed for any reason put ```Infinite```.

The main should also log the search result in std_out, but only once per search type. The print method should not influence the recorded time.

## How to generate tests
The test files can be in JSON format or in TXT format. In the Java folder, there is a **test_generator** class that can generate both. Run its main by giving it the following parameters:
```
test_generator [string length] [number of searches] [substring length] [json/txt]
```
and it will generate the test file in the correct format.

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
| Feature |     Status     |
| --- |:--------------:|
| BWT | :green_circle: |
| TWB | :green_circle: |
| Bwt search | :green_circle: |
| Bwt parallel search |  :red_circle:  |
| Suffix Trie | :green_circle: |
| Naive | :green_circle: |




# Feel free to contribute!
