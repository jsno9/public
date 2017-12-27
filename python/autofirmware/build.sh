#!/bin/bash

function clean_all()
{
    ls |grep -Eiv ".py|.xlsx|.sh|fwconfig" |xargs rm -rf
}

function helpinfo()
{
    echo "useage"
    echo "./build.sh argv[1]"
    echo "argv[1]="
    echo "      clean:clean the dir"
    echo "      tv:tv fw test"
    echo "      tablet: tablet fw test"
}

if [ $1 == "clean" ]
then
    clean_all;
elif [ $1 == "tv" ]
then
    ./fw_for_qc_test.sh "tv"
elif [ $1 == "tablet" ]
then
    ./fw_for_qc_test.sh "tablet"
else
    helpinfo;
fi
