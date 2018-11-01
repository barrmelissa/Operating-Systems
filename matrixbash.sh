#!/bin/bash

#Code from hints section of Project1
#Allows us to use one variable to hold the path to a file with the passed in contents, if it came from stdin or a file

datafilepath="datafile$$"
if [[ $# -eq 1 ]]
then
   # echo "No file specified, read from stdin"
    cat > "$datafilepath"
elif [ $# -eq 2 ]
then
   # echo "Using file specified"
#    datafilepath=$2
  if [ ! -r $2 ] || [ ! -f $2 ]
  then
       	echo "This file is not readable" 1>&2
   	exit 2
  fi
  cat $2 > "$datafilepath"
#	datafilepath="$2"
fi

#dims function, calculates the dimensions of the matrix
dims(){

#checks if the file is readable, if not sends to standard error
if [ ! -r $datafilepath ] || [ ! -f $datafilepath ]
then
        echo "This file is not readable" 1>&2
        exit 2
fi

#Code from Piazza
#checks if the function has the correct number of arguments, if not sends to standard error
if [ "$#" -gt "1" ]
then
	echo "Wrong number of arguments." 1>&2
	exit 2
fi

#code from professor after lecture
#calculates the columns and rows by using head, taking the top and using cut, cutting into columns
cols=$(cat $datafilepath | head -n 1 | wc -w)
rows=$(cat $datafilepath | cut -f 1 | wc -w)

#printing out rows and columns
echo "$rows $cols"

}

#transpose function, takes in a matrix and transposes it to the opposite
transpose(){

#checks if the file is readable, if not sends to standard error
if [ ! -r $1 ] || [ ! -f $1 ]
then
        echo "This file is not readable" 1>&2
	exit 2
fi

#again, using the same code above to calculate the rows and columns
rows=$(cat $1 | head -n 1 | wc -w)
cols=$(cat $1 | cut -f 1 | wc -w)

tempCol="tempcolfile"

i=1
rows=$(expr $rows + 1)

#while loop to loop through i while it is less than the number of rows in m1
while [ $i -lt $rows ]
do
	#cuts argument 1 and uses tr to change the new lines to tabs, cut is used to turn a line into a column, the whole thing is stored in $tempCol
	cut -f $i $1 | tr '\n' '\t' > $tempCol
	#reading it back so that it is easy to echo out
	read temp2 < $tempCol
	echo -e "$temp2"
	#incrementing i
	i=$(expr $i + 1)
done
#removing the temp files created
rm -f tempCol
rm -f tempcolfile 

}

#add function, takes m1 and m2 as arguments and adds them together using matrix additon rules
add(){

#checks to make sure the file is readable for m1, if not sends to standard error
if [ ! -r "$1" ]
then
	echo "This file is not readable" 1>&2
	exit 2
	#checks to make sure the file is readable for m2, if not sends to standard error
	if [ ! -r "$2" ]
	then
		echo "This file is not readable" 1>&2
		exit 2
	fi
fi

#checks to make sure the function is given the correct number of arguments, if not sends to standard error
if [ "$#" -ne "2" ]
then
        echo "Wrong number of arguments." 1>&2
        exit 2
fi

#again, using the same code in dims to find the columns and rows for m1 and m2
col1=$(cat $1 | head -n 1 | wc -w)
row1=$(cat $1 | cut -f 1 | wc -w)
col2=$(cat $2 | head -n 1 | wc -w)
row2=$(cat $2 | cut -f 1 | wc -w)

#checks to make sure the matrix can be multiplied together meaning the columns of m1 are equal to the columns of m2, if not sends to standard error
if [ "$col1" -ne "$col2" ]
then
	echo "Matrix mismatch" 1>&2
	exit 2
fi

#same thing, checking that the rows of m1 are qual to the rows of m2
if [ "$row1" -ne "$row2" ]
then
	echo "Matrix mismatch" 1>&2
	exit 2
fi

i=0
j=0
#while loop to read in m1
while read m1 
do
	j=$(( j + 1 ))
	i=1
	#while loop for while the value of i is less than or equal to the value of columns in m2
	while [ "$i" -le "$col2" ]
	do
		sum=0
#		j=1
	#	m2Col=`cat $2 | cut -f $i`
		#cutting m2 into a column and setting it to m2Col
		m2Col=`head -n $j $2 | tail -n 1 | cut -f $i`
		for num2 in $m2Col
		do
			#piping the value of m1 into tr which takes the empty spaes and replaces with tabs, this is then put into the file ./line
			echo $m1 | tr ' ' '\t'>./line
			#using cut to get the line identified above into a column format
			num1=`cut -f $i ./line`
			#removing the line
			rm ./line
			#calculating the sum
			sum=$(($num1 + $num2))
		done
		
		#this if statment prints out the sum of the for loop above but prints a new line character at the end if it is the last one in the column and a tab if not
		if [ "$i" -eq "$col2" ];
		then
			echo -ne "$sum\n" >> ./tempfile1
			i=$(( i + 1 ))
		else
			echo -ne "$sum\t" >> ./tempfile1
			i=$(( i + 1))
		fi
	done
done < $1
	#prints out what is stored in tempfile1 and removes it
	cat ./tempfile1
	rm ./tempfile1

}

#multiply function, the one I had the most issues with, it takes two matrix's and multiplies them together using matrix multiplication rules
multiply(){

#checks to make sure the file is readable for m1, if not sends to standard error
if [ ! -r "$1" ]
then
      	echo "This file is not readable" 1>&2
	exit 2
	#checks to make sure the files is readable for m1, if not sends to standard error
	if [ ! -r "$2" ]
	then
		echo "This file is not readable" 1>&2
		exit 2
	fi
fi

#checks to make sure the function has the correct number of argumets, if not sends to standard error
if [ "$#" -ne "2" ]
then
        echo "Wrong number of arguments." 1>&2
        exit 2
fi

#again, code from dims to find the columns and rows for m1 and m2
col1=`cat $1 | head -n 1 | wc -w`
row1=`cat $1 | cut -f 1 | wc -w`
col2=`cat $2 | head -n 1 | wc -w`
row2=`cat $2 | cut -f 1 | wc -w`

#checks to make sure the matrix is able to be multiplied by checking that the rows of m2 are equal to the columns of m1
if [ "$row2" -ne "$col1" ]
then
        echo "Matrix mismatch." 1>&2
        exit 2
fi

#Sorry, I had a few tries at the multiplication function

#transpose $2 > m1t
#i=1
#while read curline1 <&3 && read curline2 <&4
#do
#	echo "Inside function"
#	i=1
#	j=0
#	sum=0
#	while [ "$i" -le $col1 ]
#	do
#		echo "I is: $i"
#		echo "Curline is: $curline1"
#		echo "Curline is: $curline2"
	#	echo $curline1 | tr ' ' '\t'>./line1
	#	num1=`cut -f $i ./line1`
	#	echo $curline2 | tr ' ' '\t' > ./line2
		#num2=`cut -f $i ./line2`
#		m2Col=`cat $2 | cut -f $i`
#		for num2 in m2Col
#		do
#			echo $1 | tr ' ' '\t'>./line1
#              		num1=`cut -f $i ./line1`
#			echo "Num 1 is: $num1"
#			echo "Num 2 is: $num2"
#			product=$((num1 * num2))
#     			sum=$((sum + product))
#			(( i++ ))
#		done
#	done

#	if [ "$i" -eq "$col2" ];
#       then
#            echo -e "$sum" #>> ./tempfile1
            # (( i++ ))
#        else
#             echo -ne "$sum\t" #>> ./tempfile1
             #(( i++ ))
#        fi
	
	#done

#done 3<$1 4<m1t

#this is my working one, taking a while loop to read in m1
while read m1
do
	i=1
	#Old code that works
	#while loop for while the value of i is less than or equal to the number of columns in m2
	while [ "$i" -le "$col2" ]
	do
		sum=0
		j=1
		#cutting m2 into columns and setting it to m2Col
		m2Col=`cat $2 | cut -f $i`
 	      	for num2 in $m2Col
        	do
			#piping m1 to tr which replaces the spaces with tabs, then send to ./line
			echo $m1 | tr ' ' '\t'>./line
			#cuts line to a column and sets that to num1
			num1=`cut -f $j ./line` 
              		#removes the line
			rm ./line
			#calculates the product and sum
			product=$(($num1 * $num2))
			sum=$((sum + product))
			(( j++ ))
		done
		#same if statement, makes sure the sum has a new line at the end if it's at the end of a column, otherwise a tab
		if [ "$i" -eq "$col2" ];
		then
			echo -e "$sum" #>> ./tempfile1
		#	i=$(( i + 1 ))
			(( i++ ))	
		else
			echo -ne "$sum\t" #>> ./tempfile1
		#	i=$(( i + 1 ))
			(( i++ ))
		fi
	done
done < $1
#done < $1
	#cat ./tempfile1
	#rm ./tempfile1
	

}


#mean function, takes one argument and calculatest the mean
mean(){

#checks to make sure the file is readale
if [ ! -r $datafilepath ] || [ ! -f $datafilepath ]
then
        echo "This file is not readable" 1>&2
        exit 2
fi

#Code from lecture, again same code from dims to calculate the rows of m1
#cols=$(cat $datafilepath | head -n 1 | wc -w)
rows=$(cat $datafilepath | cut -f 1 | wc -w)

counter=1
i=0
#calling transpose function and passing in datafilepath and sending the output to temptranspose
transpose $datafilepath > temptranspose
#cat $temptranspose
#while loop to read one line in
while read line
do
	rowSum=0
	count=0
		#for loop that takes i in the m1 and sets the rowSum to be incremented by i and count to be incremented by 1
		for i in $line
		do
			rowSum=`expr $rowSum + $i`
			count=`expr $count + 1`
		done
		#code from Piazza, calculates the mean
	mean=$((($rowSum + ($count/2)*( ($rowSum>0)*2-1)) / $count))
	counter=`expr $counter + 1`
	#if statement to print the mean with a new line if it's the last one or a tab
	if [ "$rows" -eq "$counter" ]
	then
		echo -e "$mean"
	else
		echo -ne "$mean\t"
	fi	
done< temptranspose
#removing temptranspose
rm -f temptranspose
}

#checking for bad command
#if [ "$1" -eq "dims" ] || [ "$1" -eq "multiply" ] || [ "$1" -eq "transpose" ] || [ "$1" -eq "add" ] || [ "$1" -eq "mean" ]
#then
	#calling the functions
	$1 "${@:2}"
#else
#	echo "Bad command" 1>&2
#	exit 2
#fi
#$@

files="datafilepath, tempCol, temp2"
#Delete the file at the end
#trap "rm -f $files; echo 'SIGINT received: Deleting temp files then exiting!"
rm -f $files
