#!/bin/bash
echo `pwd`
test_lex_count=`find "../testy/" -type f | grep  "ELEX-" | wc -l`
i=0
lex_count_s=0
lex_count_f=0
echo ""
echo "Start TESTING..."
echo ""
while [ $i != $test_lex_count ]
do
	echo ""
	echo "---------------------------------------------------"
	echo "---------------------------------------------------"
	echo ""
	echo "ELEX - " $i

	./ifj17 ../testy/ELEX-$i
	program_output="$?"
	if [ $program_output = 1 ]
	then
		echo "---------------------------------------------------"
		echo "Expected output - 1"
		echo "Program output - " $program_output
		echo "Test was succesful"
		echo "---------------------------------------------------"
		lex_count_s=`expr $lex_count_s + 1`
	else
		echo "---------------------------------------------------"
		echo "Expected output - 1"
		echo "Program output - " $program_output
		echo "Test failed"
		echo "---------------------------------------------------"
		lex_count_f=`expr $lex_count_f + 1`
	fi
	i=`expr $i + 1`
done


test_parser_count=`find "../testy/" -type f | grep  "ESYN-" | wc -l`
i=0
parser_count_s=0
parser_count_f=0
while [ $i != $test_parser_count ]
do
	echo ""
	echo "---------------------------------------------------"
	echo "---------------------------------------------------"
	echo ""
	echo "ESYN - " $i

	./ifj17 ../testy/ESYN-$i
	program_output="$?"
	if [ $program_output = 2 ]
	then
		echo "---------------------------------------------------"
		echo "Expected output - 2"
		echo "Program output - " $program_output
		echo "Test was succesful"
		echo "---------------------------------------------------"
		parser_count_s=`expr $parser_count_s + 1`
	else
		echo "---------------------------------------------------"
		echo "Expected output - 2"
		echo "Program output - " $program_output
		echo "Test failed"
		echo "---------------------------------------------------"
		parser_count_f=`expr $parser_count_f + 1`
	fi
	i=`expr $i + 1`
done

test_sem_count=`find "../testy/" -type f | grep  "ESEM-" | wc -l`
i=0
sem_count_s=0
sem_count_f=0
while [ $i != $test_sem_count ]
do
	echo ""
	echo "---------------------------------------------------"
	echo "---------------------------------------------------"
	echo ""
	echo "ESEM - " $i

	./ifj17 ../testy/ESEM-$i
	program_output="$?"
	if [ $program_output = 3 ]
	then
		echo "---------------------------------------------------"
		echo "Expected output - 3"
		echo "Program output - " $program_output
		echo "Test was succesful"
		echo "---------------------------------------------------"
		sem_count_s=`expr $sem_count_s + 1`
	else
		echo "---------------------------------------------------"
		echo "Expected output - 3"
		echo "Program output - " $program_output
		echo "Test failed"
		echo "---------------------------------------------------"
		sem_count_f=`expr $sem_count_f + 1`
	fi
	i=`expr $i + 1`
done

test_ok_count=`find "../testy/" -type f | grep  "EOK-" | wc -l`
i=0
ok_count_s=0
ok_count_f=0
while [ $i != $test_ok_count ]
do
	echo ""
	echo "---------------------------------------------------"
	echo "---------------------------------------------------"
	echo ""
	echo "EOK - " $i

	./ifj17 ../testy/EOK-$i
	program_output="$?"
	if [ $program_output = 0 ]
	then
		echo "---------------------------------------------------"
		echo "Expected output - 0"
		echo "Program output - " $program_output
		echo "Test was succesful"
		echo "---------------------------------------------------"
		ok_count_s=`expr $ok_count_s + 1`
	else
		echo "---------------------------------------------------"
		echo "Expected output - 0"
		echo "Program output - " $program_output
		echo "Test failed"
		echo "---------------------------------------------------"
		ok_count_f=`expr $ok_count_f + 1`
	fi
	i=`expr $i + 1`
done


echo "-------------------------------------------------"
echo "END"
echo "-------------------------------------------------"
echo ""

echo "Lex_tests:"
echo "-------------------------------------------------"
echo "Succesful: $lex_count_s"
echo "Failed: $lex_count_f"
echo "Summary: $lex_count_s / $test_lex_count"

echo ""
echo ""

echo "Parser_tests:"
echo "-------------------------------------------------"
echo "Succesful: $parser_count_s"
echo "Failed: $parser_count_f"
echo "Summary: $parser_count_s / $test_parser_count"

echo ""
echo ""

echo "Sem_tests:"
echo "-------------------------------------------------"
echo "Succesful: $sem_count_s"
echo "Failed: $sem_count_f"
echo "Summary: $sem_count_s / $test_sem_count"

echo ""
echo ""

echo "OK_tests:"
echo "-------------------------------------------------"
echo "Succesful: $ok_count_s"
echo "Failed: $ok_count_f"
echo "Summary: $ok_count_s / $test_ok_count"