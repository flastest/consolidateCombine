### consolidateCombine

# by Ariel Flaster and Ryan Quisenberry

"MapReduce is the worst name ever so we came up with a modern twist to the age old adage" - Ariel


## Map Reduce

our mapreduce.cc defines a `default_hasher`, an `emit`, a `get_next` that gets the next thing 
from a key in our emit_map (a map of all the things emitted) and a run that takes a map and 
reduce function and an alternate hasher. 



## Word count (`wc.cc`)

`wc.cc` takes an input file and tells you how many instances there are of each word. 
it takes punctuation into account, so "Micawber" is counted as a different word than "Micawber,". 


#### making wordcount 

To make word count, do make wc. To test it for correctness, I'll add some tests (Ryan made 
some tests, they're called test-wc.sh). 

So far, Ryan and i counted by hand. We gave it our tester file, aaaaaaaa.in, which has about 49k
words in it. We checked by hand to make sure the results were correct AND the same across all 
the tests with all the different threads.

you can go int wc.cc and change the numbers of mappers at the top. the tester is also super cool if you know how to run .sh files. 

We saw that times got super a ton better as we added more threads when running 9+ files. How neato! 

you can also run it on wc-in/Copperfield.in, which is all of my favorite book. I haven't read love 
in the time of cholera, but I think David Copperfield is a much better book.


##### tests

you can test ./wc with things in the wc-in/ directory.


#### wc-capture

there's also a wc-capture that is used in the test. dont accidentally use it. 

You can use wc.cc freely on whatever you want. it's not what's used in our test though, it's just 
for your viewing pleasure. they do the same thing.


you can change the numbers of threads used in wc.cc. There are some consts you can change at the 
top that correspond to num mappers and num reducers. wc-capture.cc doesn't have this. instead, it 
has environment variables that are set in test-wc.sh. 





## Mutual friends

Mutual friends is a problem that we chose to solve. given lists of friends, we can find a group and a list of friends 
where all members of the group are friends with the list of friends. it's really cool. This is probably
how linkedin does it (I remember learning mutual friends at WashU and I think the prof talked about linkedin
but the prof wasn't as cool as eitan so I don't remember the lecture and the mapping and reducing of it
as much. Sorry Prof Cosgrove!).


How we do it is that our mapper takes a person and a list of their friends, and emits a pair which is 
the person and one person from the list of their friends. 



#### making friends

you make friends by doing `make mf` (read: make mofo). then you can do `./mf friends/[file name] <more things in the friends folder>`. there are a lot of groups of friends in that folder. generally, you want to keep the things of certain types together. so garurushotai goes with sgtfrog, and lotr goes with fellowship and the hobbit. 


