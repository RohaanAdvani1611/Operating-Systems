#!/bin/bash
touch /tmp/test.txt
chmod 644 /tmp/test.txt

./testread100 /tmp/test.txt
if [ $? -eq 0 ]
then
	echo "read100 pass"
else
	echo "read100 fail"
fi

./testread510 /tmp/test.txt
if [ $? -eq 0 ]
then
	echo "read510 pass"
else
	echo "read510 fail"
fi

./testwrite10end /tmp/test.txt
if [ $? -eq 0 ]
then
	echo "write10end pass"
else
	echo "write10end fail"
fi

./testcopy /tmp/test.txt
if [ $? -eq 0 ]
then
	echo "copy pass"
else
	echo "copy fail"
fi

./testrename /tmp/test.txt
if [ $? -eq 0 ]
then
	echo "rename pass"
else
	echo "rename fail"
fi
