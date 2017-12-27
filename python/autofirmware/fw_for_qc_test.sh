#!/bin/bash

#clean

function clean_dir()
{
    ls |grep -Eiv ".zip|.py|.xlsx|.sh|fwconfig" |xargs rm -rf
}

function copy_file()
{
    cd $dir
    for file in ./*
    do
        if test -f $file
        then
            #echo ${file##*[}
            filename=${file##*[}
            filename=${filename%%_*}"_fw"
            echo $filename
            cp $file ../$tmpdir/$filename
            #echo "000" > ../$tmpdir/$filename
        fi
        if test -d $file
        then
            echo $file 是目录
        fi
    done
    cd ..
}

function copy_diff_file()
{
    cd ../../Firmware/TV/
    status=$(git status .)
    for x in ` git status .| grep -E 'modified:*' | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//'`;
    do
    echo $x;
    cp $x ../../scripts/autofirmware/$qctestdir
    done
    cd -
}

function check_diff_fw()
{
    cd $qctestdir
    filenum=$(ls |wc -l)
    echo $filenum
    if [ $filenum == 0 ]
    then
        echo "Different fw file is not found!"
        clean_dir;
        exit 0
    else
        echo "have $filenum diff fw files"
    fi
    cd -
}

function gettvjira()
{
    python getfile.py "****" "tv" $dir $tmpdir
    copy_file;
}

function gettabletjira()
{
    python getfile.py "****" "tv" $dir $tmpdir
    copy_file;
}

date=$(date +%Y%m%d)
rm -rf *$date*
dir="BT_TV_Firmware_From_"$date"_src"
changedir="BT_TV_Firmware_From_"$date
qctestdir="BT_TV_Firmware_From_"$date"_for_test"
tmpdir="tmp"$date
mkdir $dir
mkdir $tmpdir
mkdir $qctestdir
mkdir $changedir

echo $1
if [ $1 == "tv" ]
then
    gettvjira;
elif [ $1 == "tablet" ]
then
    gettabletjira;
else
    echo "please use tv or tablet";
    clean_dir;
    exit 0
fi
echo $1
cp -a $tmpdir/* ../../Firmware/TV/
copy_diff_file;
check_diff_fw;
if [[ $1 == "tv" ]]
then
    cp -a **** $qctestdir/
elif [[ $1 == "tablet" ]]
then
    cp -a **** $qctestdir/
else
    echo "please use tv or tablet";
    clean_dir;
    exit 0
fi
cp -a $qctestdir/* $changedir/
 
zip $dir.zip $dir
zip $qctestdir.zip $qctestdir
zip $changedir.zip $changedir

#python submitjira.py $dir.zip $qctestdir.zip $changedir.zip 
