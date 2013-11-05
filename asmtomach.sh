#!/bin/sh

# save as astomach.sh and run:
# chmod u+rwx asmtomach.sh
# then run:
# ./asmtomach.sh 'mov $1, %eax'

cd /var/tmp

echo $1 > asmtomach.s
gcc -m32 -c asmtomach.s
objdump -d asmtomach.o | grep '[0-9a-f][0-9a-f] '

cd - 2>&1 >/dev/null
