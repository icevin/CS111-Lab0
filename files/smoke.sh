#!/bin/bash

# SEGFAULT
let errors=0


./lab0 --segfault > /dev/null  2>/dev/null
if [ $? -ne 139 ]
then
    let errors+=1
    echo "Error: with --segfault"
fi

# CATCH

./lab0 --segfault --catch > /dev/null  2>/dev/null
if [ $? -ne 4 ]
then
    let errors+=1
    echo "Error: with --segfault & --catch combination"
fi

dd if=/dev/urandom of=junk.test bs=1024 count=1 

# CATCH, STDIN, STDOUT
./lab0 --catch < junk.test > /dev/null  2>/dev/null
if [ $? -ne 0 ]
then
    let errors+=1
    echo "Error: with --catch"
fi

# STDIN, STDOUT
./lab0 < junk.test > test1.test  2>/dev/null
cmp junk.test test1.test
if [ $? -ne 0 ]
then
    let errors+=1
    echo "Error: Data comparison failure - stdin -> stdout"
fi

./lab0 --input=junk.test > test2.test  2>/dev/null
if [ $? -ne 0 ]
then
    let errors+=1
    echo "Error: with --input from file"
fi

cmp junk.test test2.test
if [ $? -ne 0 ]
then
    let errors+=1
    echo "Error: Data comparison failure - file -> stdout"
fi

./lab0 --input=junk.test --output=test3.test  2>/dev/null
if [ $? -ne 0 ]
then
    let errors+=1
    echo "Error: with --input from file and --output to file"
fi

cmp junk.test test3.test
if [ $? -ne 0 ]
then
    let errors+=1
    echo "Error: Data comparison failure - file -> file"
fi

./lab0 --output=test4.test < junk.test  2>/dev/null
if [ $? -ne 0 ]
then
    let errors+=1
    echo "Error: with stdin and --output to file"
fi

cmp junk.test test4.test
if [ $? -ne 0 ]
then
    let errors+=1
    echo "Error: Data comparison failure - stdin -> file"
fi

echo "FOUND $errors ERRORS."
exit 0
