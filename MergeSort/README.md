+ Unlike conventional mergesort, mergesort can also be implemented using threads and fork.


USING FORK:
===========

+ Forking means creating two processes. One is parent other is child.
+ Thus unlike threads hiearchy exsists.
+ As they are diffrent processes chance of deadlock is reduced.
+ Code available [here](./MergeSort_FORK.c)
+ Here shared memory is used, which is a necessity to share data between processes.
+ Here recurssive mergeSort are considered as diffrent processes.


USING THREADS:
==============

+ Here threads are used instead of processes.
+ Thus there is no hiearchy here.
+ Here locks need to be used wisely. As there is only one lock, deadlock not possiblr.
+ Code available [here](./MergeSort_THREAD.c)
+ Here recurssive mergeSort are considered diffrent threads.
+ These threads are joined at last.
+ We can say that threads are faster than processes as they are LWPs

* **NOTE:THESE BOTH ARE SLOWER THAN NORMAL MERGESORT AS IT IS OPTIMISED BY OS ITSELF,WHICH HAS A BETTER ALGORITHM THAN THESE NAIVE ALGORITHMS**
