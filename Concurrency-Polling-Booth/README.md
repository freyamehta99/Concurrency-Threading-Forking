PROBLEM STATEMENT:
==================

+ People are fed up with waiting at polling booths on the day of the election. So the government has decided to improve its efficiency by automating the waiting process.
+ From now on, voters will be robots and there will be more than one EVM at each booth. Each of these EVMs can now accept vote from more than one person at a time by having different number of slots for voting. However one person can only vote once.
+ Each robot and each EVM is controlled by a thread. You have been hired to write synchronization functions that will guarantee orderly use of EVMs.
+ You must define a structure struct booth, plus several functions described below:
  1. When an EVM is free and is ready to be used, it invokes the function polling_ready_evm(struct booth *booth, int count) where count indicates how many slots are available to vote at this instant (by this we mean that the count is not fixed every time we call a specific EVM. There is a possibility that a particular EVM invokes this once with count = 5 and later with count = 7. Use a random function to determine the count value from a fixed range (1<=count<=10)) The function must not return until the EVM is satisfactorily allocated (all voters are in their slot, and either the EVM is full or all waiting voters have been assigned – note that a voter doesn’t wait for a particular EVM but he only waits to vote. So it will return if there is no voter that is waiting at the booth to vote).
  2. When a voter robot arrives at the booth, it first invokes the function voter_wait_for_evm(struct booth *booth) This function must not return until an EVM is available in the booth (i.e. a call to the polling_ready_evm is in progress) and there are enough free slots on the EVM for this voter to vote (one slot for one voter) Once this function returns, the voter robot will move the voter to the assigned
slot (do not worry about how this works!).
  3. Once the voter enters the slot, he/she will call the function voter_in_slot(struct booth *booth) to let
the EVM know that they reached the slot.
  4. Assume that there are a fixed number of voters per booth (no voters will come after this process starts), a fixed number of EVMs per booth and a fixed number of booths (will be given as inputs to the program)
  5. Stop this simulation when all the voters are done with voting. Note that an EVM can actually be used more than once for voting So basically when there are still voters left all EVM's should invoke polling_ready_evm().
  6. You may assume that there is never more than one EVM in the booth available free at once (have a look at the sample output) and that any voter can vote in any EVM. Any number of EVM's can call polling_ready_evm(). But only one will be ready to vote at a time.
  7. Create a file that contains a declaration for struct booth and defines the three functions described above and the function booth_init which will be invoked to initialize the booth object when the system boots. Use appropriate small delays (sleep()) for the simulation.
  8. No semaphores are to be used. You should not use more than one lock in each struct booth. Use mutexes and conditional variables to do this question.
  9. Your code must allow multiple voters to board simultaneously (it must be possible for several voters to have called voter_wait_for_evm, and for that function to have returned for each of the voters, before any of the voters calls voter_in_slot)
  10. Your code must not result in busy-waiting (deadlocks)
  11. You can define more functions if needed. For the functions mentioned in the problem statement, you may extend them by adding new arguments for them but don't decrease them.

+ **Kindly note that this problem's goal is make you realize the use of threads and synchronization techniques.
Output doesn't matter. It only depends how you simulate. Make reasonable assumptions so that your solution
doesn't deviate a lot from the problem.**


SOLUTION EXPLAINED:
===================

+ So here ther are three structs defined.
1. EVM:Has idx of EVM,thread id of thread corresponding to that evm,corresponding booth,corresponding no of slots and a boolean flag for its working(1 -> working).
2. VOTER: Has idx of Voter,thread id of thread corresponding to that voter,corresponding booth,corresponding evm ans status(new,waiting,assigned or completed).
3. BOOTH: Has idx of Booth,thread id of thread corresponding to that booth,no of voters, no of slots, no voters who are done, all the voters, mutex to assign voter and corresponding conditions.

INITIALIZING FUNCTIONS FOR VOTERS,EVM,BOOTH:
--------------------------------------------

+ It creates corresponding threads and structs for each of them and joins them accordingly.

MAIN FLOW:
----------

+ It initializes,creates thread and joins Booth threads.

BOOTH THREAD:
-------------

+ It initializes,creates thread and joins EVM threads.
+ It initializes,creates thread and joins Voter threads.

VOTER THREAD:
-------------

+ Voter waits for a EVM slot to be free for it.
+ On gaining it it votes.
+ The it signals that is is free so someone else can occupy it.

EVM THREAD:
-----------

+ EVMs waits for booth to start.
+ Then slots are free and waits for voter to take it.
+ If voter is done it declares itself to be  free.
+ If all the voters are done then its shuts down.

RUNNING IT:
-----------
1. gcc -pthread EVM.c
2. ./a.out
3. Input number of booths
4. For each booth input number of EVMs, max number of slots of EVM(less than or equal to 10),number of Voters.

