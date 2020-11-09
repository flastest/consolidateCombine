# consolidateCombine

### Ariel Flaster and Ryan Quisenberry

"MapReduce is the worst name ever so we came up with a modern twist to the age old adage"
~ Ariel

## Map Reduce

Our mapreduce.cc defines a default hasher, an emit, a get next that gets the next thing
from a key in our `emit_map` (a map of all the things emitted) and a run that takes a map and
reduce function and an alternate hasher.


## Word Count (`wc.cc`)

Word count takes an input file and tells you how many instances there are of each word.
it takes punctuation and capitalization into account, so "Heep" is counted as a different
word than "Heep,".


#### making wordcount

To make word count, do `make wc`. To test it for correctness, Ryan made some tests which can
be run with `./test-wc.sh`.

So far, Ryan and I counted by hand. We gave it our tester file, `aaaaaaaa.in`, which has about
49k words in it. We checked by hand to make sure the results were correct AND the same across
all the tests with all the different threads.

You can go into `wc.cc` and change the numbers of mappers at the top. the tester is also super
cool if you know how to run `.sh` files. We saw that times got super a ton better as we added
more threads when running 9+ files. How neato! You can also run it on `wc-in/Copperfield.in`,
which is all of my favorite book. I haven't read love in the time of cholera, but I think David
Copperfield is a much better book.


##### tests

`./wc` can be run on anything in `wc-in/` or pretty much any text file that can be parsed.

You can use `wc.cc` freely on whatever you want. it's not what's used in our test though, it's
just for your viewing pleasure. they do the same thing barring the usage of environment
variables. You can change the numbers of threads used in `wc.cc`. There are some consts you can
change at the top that correspond to `NUM_MAPPERS` and `NUM_REDUCERS`. `wc-capture.cc` doesn't
have this. instead, it has environment variables that are set in `test_wc.sh`.


#### wc-capture and test_wc

There is also a `wc-capture.cc` that is used in the test. don't accidentally use it. You should
use the beautiful test made by ryan. just type `./test-wc.sh` and it'll automatically
compile the `wc-capture.cc` and set the needed environment variables.



## Mutual friends (`mf.cc`)

Mutual friends is a problem that we chose to solve. given lists of friends, we can find a group
and a list of friends where all members of the group are friends with the list of friends. it's
really cool. This is probably how linkedin does it (I remember learning mutual friends at WashU
and I think the prof talked about linkedin but the prof wasn't as cool as Eitan so I don't
remember the lecture and the mapping and reducing of it as much. Sorry Prof Cosgrove!).

How we do it is that our mapper takes a person and a list of their friends, and emits a pair
which is the person and one person from the list of their friends.

Then when everything is reduced, the reducer goes through every friend and finds the group of
friends that the original friend is friends with. if that group already has a mutual friend, the
original is added to that list. in the end, we're left with a group of people followed by a list
of the friends they have in common.

#### Making Friends

you make friends by doing `make mf` (read: make mofo). then you can do
```
./mf friends/[file name 1].friend friends/[file name 2].friend ...
```
There are a lot of groups of friends in that folder. Generally, you want to keep the things of
certain types (i.e. garurushotai goes with sgtfrog, and lotr goes with fellowship and the hobbit)
together through the code will work regardless you just might not see many mutual
friends if you use different groups.

#### testing

`.\mf` can be used to test things in the `friends/` directory. You can change the numbers of
mappers and reducers used in mf in mf.cc at the top.

#### mf_capture and test_mf

This does one test for correctness because mf is super hard to test for correctness in bash. Ryan
ad god attempted to write a bash that would check the contents of an output against a list of
correct  mutual friends, this proved extremely difficult likely should have written this tester in
C++. The test does record the times of running on our largest set of test files with 1 mapper/1
reducer and then again with 4 mappers/4 reducers. You can see for yourself how well it runs! we
see great speedups. One way to quickly test for a degree of correctness without knowing the
correct output more intimately is to run `./wc` on `friends/lotr.friends` and assert that all `9`
`Nazgûl` are friends with eachother and `Sauron` (each mutual friends list for a single `Nazgûl`
should be the same length but not for `Sauron`).

This works by creating a mf-capture and running it with different numbers of environment vars.


### Special thanks

Thanks to Henry!
And Google.
And Eitan.
