#!/bin/bash
# shell mà script sẽ chạy

# echo
echo 'hello world'
# dung -e de hieu \t, \a, \n
echo -e 'hello world. Tab: \t.Alert \a\.Endline\n'

# khai bao bien
# dau bang (=) phai dinh lien, no moi hieu
a=0; # not use: a = 5
echo $a; # lay gia tri

# dung let
let 'b = 1'
echo $b
let 'b += 1'
echo $b
# dung $(()), trong co the co hoac ko co $bien
b=$(($b*b))
echo $b
c=$((b*2))
echo $c

# shell script trong string
# date la 1 shell script, in ra ngay hien tai
n='hi hi'
echo 'hom nay la date, $n' # date, $n
echo "hom nay la date, $n" # date, hihi 
echo 'hom nay la `date`, $n' # `date`, $n
echo "hom nay la `date`, $n" # ngay hien tai, hihi

# exit status => dung $?
rm unknownfile
echo $? # khac 0 => unsuccess
touch existfile # tao mot file, de xoa no thanh cong
rm -rf existfile
echo $? # = 0 => success

# read
echo 'Nhap cai gi do:'
read n
echo "Ban vua nhap: $n"

echo 'Nhap tiep di:'
read
echo "Ban moi nhap: $REPLY"

# read file
echo 'Doc file text.txt, hien ra nhu sau:'
while read line 
do 
echo $line
done < text.txt

# doi so dong lenh
echo 'Chay script co doi so dong lenh: ./ex1.sh one two'
echo "Co $# doi so dong lenh:"
echo 'dung $@, hoac $* de lay het'
echo 'dung $@: ' 
echo $@
echo 'dung $*: ' 
echo $* 
echo 'dung $0 de lay ten script, $1, $2, ... de lay cac doi so dong lenh'
echo $1

# xuat file, doc file
echo 'dung > de tao moi / ghi de file'
echo 'dung >> de ghi tiep file'
echo 'dung < de lay luong file input'

# exit status cua script, 0 => success
exit 0
