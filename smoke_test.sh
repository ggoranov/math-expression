# Use this script as a smoke test to compile, run and profile
# before any commit

VALGRIND="valgrind --leak-check=full -v"

# Compile
gcc ./src/*.*  -lm -o calc

# Run example expressions
expressions=( "12*0.0125" "12*0.0125+10^2" "(12*0.0125+10^2)%45" "0.00012 - 0.0000012" )
for i in "${expressions[@]}"
do 
	echo -e "$i: \t`$VALGRIND ./calc EVAL_MODE $i`"
done

# Run example expressions with variables
$VALGRIND ./calc EVAL_MODE "area-1+2+width" "width=3;area=100"
$VALGRIND ./calc EVAL_MODE "(12%5)^3-(length+width)*2.45-area+12%5" "length=12.34;width=88.78;area=33.55"

rm -f calc
