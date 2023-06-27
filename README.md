# ProdTimer
**Assumptions:**
1.  Default timer granularity 50 ms.
2.  Default max delay 24 hours.

**Design Alternatives:**
1) Single Hashing Wheel with Unordered Timer Lists:-
   Create a circular buffer with a fixed number of slots. We store a count of how many times around the ring each timer is in the future.
   To insert a timer that expires j time units in the future, compute a counter value _ c = j / num-buckets_ and a slot delta s = j % num-buckets.
   Insert the timer slots around the ring with its counter value c. Keep the timers in an unordered list in each slot.

2) Hierarchical Timing Wheels implementation.
   This approach uses multiple timing wheels in a hierarchy.
   We want to store timers with 5 granularity, that can be set for up to 1 days in the future.
   We construct four wheels:
   - An hours wheel with 24 slots
   - A minutes wheel with 60 slots
   - A seconds wheel with 60 slots
   - A millisecond wheel with 1000/50 = 20 slots

**Comparison:**

- N = Timers count
- M = Total number of slots available
- L = Number of levels in Hierarchical timer wheel
- T = Average timer interval

- c_i the cost of hashing and indexing one entry in Approach 1
- c_m the cost of moving timer entries to the next wheel in Approach 2

- Cost of Approach 1 = (N/M)*c_i
- Cost of Approach 2 = N*(L/T)*c_m

Assuming c_i =~ c_m for small values of T and large values of M, Approach 1 can be better than Approach 2 for both START-TIMER and PER-TICK-BOOKKEEPING.

However, for large values of T and small values of M, Approach 2  will have a better average cost (latency) for PER-TICK-BOOKKEEPING but a greater cost for START-TIMER

This design chooses Hierarchical Timing Wheels implementation.

**Further improvement:**
1. TimerID: Time is 32 bit, monotonically increasing. For long running timers, Timer ID may collide. A separate TimerID generator to generate unique IDs should be used
2. Timer slots are allocated and deallocated on heap. As an improvement Timer slots should be acquired from a pool of timer slots. A precreted pool will also solve the uniqure TimerID,
3. This design chooses Hierarchical Timing Wheel, Time.cpp should choose using policy class to choose appropriate implementation
4. Worker thread is fixed. It should use a worker pool instead for improved concurrency.
5. Sleep as Tick or Select() as Tick. Select is stable and used since many year. Sleep precision depends on platform. 
