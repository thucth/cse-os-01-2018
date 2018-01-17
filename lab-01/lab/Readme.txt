Chapter 1:
=====================================================================================
Introduce about OS and OS distribution, Unix, Linux

BASIC:
Log in, Log out
syntax: <command> <option> <argument>
example: $ ls -l /etc
basis command: $ cd, $ ls, $ pwd, $ mkdir, $ vi, $ whoami
view option: <command> -h (—-help), $ man <command>

IDENTIFIER:
UID (user identifier)
GID (group identifier)
command: $ whoami, $ who am i

HOME FOLDER, CURRENT FOLDER:
command: $ echo $HOME
	 $ pwd

ALL USER:
command: $ who

PASSWORD:
password requirement: …
command: $ passed
		new password
		confirm new password

TREE FOLDER:
important folder: / , /bin , /boot , /dev , /etc , /lib , /sbin , /tmp , /usr , /var …

FILE TYPES:
directory
ordinary file
special file
command: $ file path_name

ACCESS FOLDER:
$ cd <pathname>
current folder: $ .
parent folder: $ ..
home folder: $ ~ , ~username

LIST FILE IN FOLDER
ls <option> <path_name>
example: …
$ ls -l
	result:
	<filetype><permissions> <links> <owner> <group_owner> <size> <date_of_last_modification> <file_name>

WILD CARDS
example: * , ? , [..]
$ ls *.txt

EXECUTION COMMAND
mkdir, rm, cp, cd
example:
$ mkdir path_name
$ mkdir -p a/b/c
$ rm -r <path_name>
$ cp <option> <from> <to>
$ mv <option> <file_path> <destination_path>

FILE
create, show detail, search, search by content
$ cat <file_path>
$ touch <new_file>
$ head -n <file_name>
$ last -n <file_name>
$ find <path_name> -name “file_name” -print
$ grep “pattern” file_name(s)

PERMISSION
r-w-x = read - write - execute
u-g-o = owner - group - other

MODIFY PERMISSION
$ chmod <permission> <file(s)>

Homeworks:
Exercise 1:
write a script : ex1.sh
execute it by command: ./ex1.sh <your name>
obtained result: Your name + System information in file ex1.txt
Check null of <your name>
(Hint: use if statements)

Exercise 2:
write makefile
main.c depends on sum.o, sub.o
sum.o depends on sum.c, sum.h
sub.o depends on sub.c, sub.h
clean after make

Note:
Unix: distinguish lowercase and Uppercase.
turn off unikey
Nohup stands for no hang up, which can be executed as shown below.
nohup syntax:
# nohup command-with-options &
if statements in shell script:
https://linuxacademy.com/blog/linux/conditions-in-bash-scripting-if-statements/
Tutorial Make file
http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/