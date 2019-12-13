# BM25++
Given collection of documents and a query, rank the collection of documents according to their relevance to the query.

To compile:
`make Indexer`  
`make bm25_run`  

To create indexing:
`./Indexer --input <input file> --output <output file>`

To rank the passages:
`./bm25_run --input <input file> --topic <topic file> --output <output file>`

