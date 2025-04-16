if [ "$#" -ne 5 ]; then
    echo "Usage: ./rado.sh n k a b c"
	echo "Outputs k-coloring witness that R_k(ax+by=cz) > n" 
	echo "n = number of integers"
	echo "k = number of colors"
else 
echo "Generating .cnf file" 
./radoSolutions $1 $2 $3 $4 $5
filename="rado_n"$1"_k"$2"_"$3"_"$4"_"$5".cnf"
echo "Solving $filename"
SATOutputFilename="SAToutput.txt"
../kissat/build/kissat $filename --relaxed > $SATOutputFilename
./radoCertReader $SATOutputFilename $2
fi 

