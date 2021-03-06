/*
* MIT License
*
* Copyright (c) 2018 Georgi Goranov
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <string.h>

#define MAXOPSTACK 64
#define MAXNUMSTACK 64
#define DECIMAL_SEPARATOR '.'
#define ERR_MSG_LEN 256

typedef double DOUBLE;
/*
 * Terminology:
 *
 * Unary operators expect one (1) operand.
 * Binary operators expect two (2) operands.
 * Ternary operators expect three (3) operands.
 *
 * LHS is informal shorthand for the left-hand side of an equation.
 * Similarly, RHS is the right-hand side.
 */

/* Evaluate a math expression without any variables
 * returns result as double if parsing and calculations do pass successfully
 * returns DBL_MIN if failure occur. err is populated too
 * Thread-safe support
 */
DOUBLE evaluate(const char* expression, char* err);



