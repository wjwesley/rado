if [ "$#" -ne 6 ]; then
    echo "Usage: ./rado.sh k a b c lb ub"
	echo "Calculates k-color Rado number R_k(ax+by=cz) with lower bound and upper bound guesses"
else 
STARTTIME=$(date +%s)

k=$1
a=$2
b=$3
c=$4
lb=$5
ub=$6
ubfile="rado_n"$ub"_k"$k"_"$a"_"$b"_"$c".cnf"
lbfile="rado_n"$(($lb-1))"_k"$k"_"$a"_"$b"_"$c".cnf"
./radoSolutions $ub $k $a $b $c
./radoSolutions $(($lb-1)) $k $a $b $c
echo "Verifying upper and lower bounds..."
if [ "$(../kissat/build/kissat $ubfile --relaxed -q -n)" != "s UNSATISFIABLE" ]
    then 
    echo "Upper bound too low"
    exit 
fi
if [ "$(../kissat/build/kissat $lbfile --relaxed -q -n)" != "s SATISFIABLE" ]
    then 
    echo "Lower bound too high"
    exit  
fi

echo "Determining Rado number..."

while [ $ub -ge $lb ]
do
    MID=$((($lb+$ub)/2))
    
    # Checking satisfiable or not

	./radoTrimClauses $ubfile "mid.cnf" $MID $k

    SOLVESTARTTIME=$(date +%s)
	SATString=$(../kissat/build/kissat mid.cnf --relaxed -q -n)
    SOLVEENDTIME=$(date +%s)
	if [ "$SATString" = "s SATISFIABLE" ]
			then
                echo "n= $MID : SATISFIABLE, solve time: $(($SOLVEENDTIME - $SOLVESTARTTIME)) seconds."
				lb=$(($MID+1))
               
			else
                echo "n= $MID : UNSATISFIABLE, solve time: $(($SOLVEENDTIME - $SOLVESTARTTIME)) seconds."
				ub=$(($MID-1))
	fi 
	
done

ENDTIME=$(date +%s)
echo "----------- [ Results ] ----------"
echo "Rado number is $((ub+1))."
echo "Total time: $(($ENDTIME - $STARTTIME)) seconds."
rm *.cnf
fi 


