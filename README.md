# consolidateCombine

#### Ariel Flaster and Ryan Quisenberry

MapReduce is the worst name ever so we came up with a modern twist to the age old adage



## Word count

To make word count, do make wc. to test it for correctness, I'll add some tests. So far, Ryan and i counted by hand. We gave it our tester file, aaaaaaaa.in, which has about 49k
words in it. We checked by hand to make sure the results were correct AND the same across all the tests with all the different threads.

We saw that times got super a ton better as we added more threads when running `time ./wc aaaaaaaaa.in aaaaaaaaa.in aaaaaaaaa.in aaaaaaaaa.in aaaaaaaaa.in aaaaaaaaa.in `

#### wc-capture

there's also a wc-capture that is used in the test. we hid it away so you dont accidentally use it. 
You can use wc.cc freely on whatever you want. it's not what's used in our test though, it's just for your viewing pleasure.


you can change the numbers of threads used in wc.cc. There are some consts you can change.


