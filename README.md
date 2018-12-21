# Math-expression evaluator
Shunting-yard algorithm for parsing mathematical expressions specified in infix notation implemented in C language

## Changes from original version

 * Context per evaluation to provide thread-safety support
 * Context Config: scale, decimal (pending)
 * Use double instead of integer for all operations
 * Use fmod in eval_mod
 * Add TEST_MODE evaluations
 * Support math operation (sqrt, sin, cos, floor, ceil) (pending)
 * Use error handling instead of exit calls
 * variables: length, width (pending)

### How to compile
gcc ./src/*.*  -lm -o calc

### How to run all tests
./calc TEST_MODE

#### Expected output
```
[TEST] Sample expressions ... 
[TEST] Sample expressions completed 
[TEST] Failure expressions ... 
[TEST] Failure expressions completed 
[TEST] Advanced expressions ... 
[TEST] Advanced expressions completed 
```

### How to evaluate a math expression
```
./calc EVAL_MODE "12*0.0125"
Result: 0.15

./calc EVAL_MODE "12*0.0125+10^2"
Result: 100.1500000000

./calc EVAL_MODE "(12*0.0125+10^2)%45"
Result: 10.15

 ./calc EVAL_MODE "0.00012 - 0.0000012"
Result: 0.0001188
```
