# Multi-threading

To speed up the scan, we can implement multi-threading. I used `pthread` to implement threading since it's POSIX.

The port range is divided evenly between each thread. The port range is stored in an argument struct so that it can be passed into the thread function. Each thread performs a vanilla TCP scan on the specified port range.

To append a valid ports to a linked list in multi-threading, we can use mutex to lock the resource in, append to it, then release it later on for other threads. This is to prevent race conditions.
