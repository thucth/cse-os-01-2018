## Note

+ BYTE = char

+ addr_t = uint32_t

+ ins_opcode_t (instruction opcode type) = enum

+ inst_t (opcode, args[0,1,2])

+ segment => page => offset => cell

## OS Simulation

+ make all
+ ./os [configure path] = [./input/os_0 | os_1]

# Implementation
## Scheduler

+ make sched
+ make test_sched

** Question **: What is the advantage of using priority feedback queue in comparison with other scheduling
algorithms you have learned?

## Memory

+ make mem
+ make test_mem

** Question **: What is the advantage and disadvantage of segmentation with paging.

## Final
+ make all
+ make test_al