#!/bin/bash

file=ex1.txt

br='=======================================================\n'
hr='-------------------------------------------------------\n'

# ===================================================

echo -e $br$br > $file

nargs=$# # get number arguments
# echo $nargs

if [ $nargs -eq 0 ] # username is null
then
	echo "your name is null"
else
	args=$* # get all params after script $0
	username=$args
	echo "your name is $username"
fi >> $file


# ==================================================

function systemInfo() {
echo -e "\n"$br$br

echo -e "System Information\n$hr$br"

echo "Kernel name		: `uname -s`"
echo "Kernel release		: `uname -r`"
echo "Kernel version		: `uname -v`"
echo "Machine			: `uname -m`"
echo "Processor		: `uname -p`"
echo "Hardware		: `uname -i`"
echo "Operator 		: `uname -o`"

echo -e "\n$hr\n"

echo -e "Current user: $USER\n$hr"
echo -e "Current time: `date`\n$hr"

echo -e $br
echo -e "Memory:\n$hr"
free

echo -e $br
echo -e "Disk space:\n$hr"
df -h

echo -e $br
echo -e "Network:\n$hr"
ifconfig

echo -e $br
echo -e "CPU information$hr"
lscpu
}

systemInfo >> $file

exit 0
