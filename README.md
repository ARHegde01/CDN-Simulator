## CDN-Simulator 

### The CDN Simulator is a simple project that aims to simulate the behavior of a Content Delivery Network (CDN) by caching files and serving them efficiently based on user requests. It provides a basic implementation of caching algorithms and allows you to explore caching strategies and evaluate their performance.
--- 

### Features

The CDN Simulator project includes the following features:

- Caching of files: The simulator can cache both text and binary files locally to improve response times for subsequent requests.
- File freshness management: Each cached file has a freshness count associated with it, indicating the number of requests before it becomes stale.
- File retrieval: The simulator provides methods to retrieve files from the cache. It supports retrieving files in both text and binary formats.
- Cache eviction: When the freshness count of a file reaches zero, it is considered stale and can be evicted from the cache to make room for new files.
- Cache statistics: The simulator tracks various statistics, including the total number of requests, the total number of files requested, the average requests per file, the most requested file, and the total times files were read from disk.
- Cache purging: The simulator allows you to purge the entire cache, removing all cached files and starting fresh.

---

### Usage: 

To use the CDN Simulator, follow these steps:

- Include the cacheCDN.h header file in your project.
- Create an instance of the ICS45C::CDN::Cache class, specifying the freshness count for the cache.
- Use the provided methods to interact with the cache, such as retrieving files, checking cache status, marking files as fresh, purging the cache, and obtaining cache statistics.
- Refer to the code documentation and comments within the cacheCDN.h file for detailed information on each method and its parameters.

---

### Practical Uses
The CDN Simulator project can be useful for the following purposes:

- Learning and experimentation: The project provides a simplified implementation of a CDN, allowing you to understand and experiment with caching concepts, caching algorithms, and eviction policies.
- Performance evaluation: You can use the simulator to evaluate the performance of different caching strategies by measuring cache hit rates, average response times, and other relevant metrics.
- Algorithm development: The project serves as a foundation for developing and testing new caching algorithms or modifying existing ones to suit specific requirements.
- Education and research: The simulator can be used as a teaching tool or for academic research to study the behavior of CDNs and explore caching optimization techniques.
