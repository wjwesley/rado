echo "Generating .cnf file"
./radoSolutions $1 $2 $3 $4 $5
cnfFilename="rado_n"$1"_k"$2"_"$3"_"$4"_"$5".cnf"
echo "Solving $cnfFilename"
SATOutputFilename="SAToutput.txt"
isSAT="true"
while [ $isSAT = "true" ] 
do
	echo $isSAT
	../kissat/build/kissat $cnfFilename --relaxed > $SATOutputFilename
        SATString=$(./radoCertReader $SATOutputFilename $2 $cnfFilename)
	echo $SATString
        if [ "$SATString" = "s UNSATISFIABLE" ]
		then 
		isSAT="false"
	fi	
done
