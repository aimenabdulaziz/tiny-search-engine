# CS50 Lab 4
## Aimen Abdulaziz, Spring 2022

### Crawler

The `crawler` crawls a website and retrieves webpages starting with a specified URL. It parses the initial webpage, extracts any embedded `href` URLs and retrieves those pages, and crawls the pages found at those URLs, but limiting itself to `maxDepth` hops from the seed URL. When the crawler process is complete, the indexing of the collected documents can begin.

### Usage

You should provide arguments for `crawler` in the following format: 
```console 
./crawler seedURL pageDirectory maxDepth
```

### Exit Status

- 0: successful run
- 1: incorrect number of arguments
- 2: arg3 is not a valid integer
- 3: non-internal url
- 4: invalid pathname
- 5: arg3 is a valid integer but it is not within the permitted range of [0,10]
- 99: null pointer while allocating memory or opening a file

### Assumptions

The `pageDirectory` provided by the user does not contain any files whose name is an integer (i.e., 1, 2, ...)

- `seedURL` should be a valid internal URL
- `pageDirectory` should be an existing path directory with writing permission 
- `maxDepth` should be an integer between 0-10

### Compilation

To compile crawler, simply run `make`. 

To run automated test cases, simply execute `make test`. Make sure you have compiled `crawler` before running test cases.

To test with valgrind, run `make valgrind`. I have added a test case in the `Makefile`, but feel free to modify that and test with your preferred test case.

### Limitation
Only works in the CS50 playground: `http://cs50tse.cs.dartmouth.edu/`
