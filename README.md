# Math-expression evaluator
Shunting-yard algorithm for parsing mathematical expressions specified in infix notation implemented in C language

## Changes from original version

 * Context per evaluation to provide thread-safety support
 * Support double instead of integer for all operations
 * Add TEST_MODE evaluations
 * Use error handling instead of exit calls
 * Swap variables
 * Support math operation (sqrt, sin, cos, floor, ceil) (pending)
 * Context Config: scale, decimal (pending)

### How to compile
gcc ./src/*.*  -pthread -lm -o calc

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

### How to evaluate a math expression with variables
```
./calc EVAL_MODE "1+2+width" "width=3"
Result: 6.0

./calc EVAL_MODE "area-1+2+width" "width=3;area=100"
Result: 104.0000000000

./calc EVAL_MODE "(12%5)^3-(length+width)*2.45-area+12%5" "length=12.34;width=88.78;area=33.55"
Result: 0.15
```
