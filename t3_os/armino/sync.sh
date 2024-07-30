#! /bin/bash
#
# sync.sh
# Copyright (C) 2023 cc <cc@tuya>
#
# Distributed under terms of the MIT license.
#

co_path="/home/share/bk_sdk/bk_idk/"
find . -name __pycache__ -type d -exec rm -rf {} \;
#make clean

file_list=($(find . \( -path ./docs -o -path ./tuya_app -o -path ./tuya_build -o -path ./bk_idk/aboot-main -o -path ./bk_idk/tools/env_tools/beken_utils/scripts -o -path ./bk_idk/tools/build_tools/kconfig_new -o -path ./bk_idk/tools/build_tools/part_table_tools/otherScript \) -prune , -type f))

ingore_file_list=($(grep -nr "Modified by TUYA" | awk -F':' '{print $1}' | sort -u))

test_cnt=0

function __md5_cmp()
{
    md5_f1=$(md5sum $1 |  awk '{print $1}')
    md5_f2=$(md5sum $2 |  awk '{print $1}')
    if [ "x$md5_f1" == "x$md5_f2" ]; then
        echo 1
    else
        echo 0
    fi
}

#set -x
diff_file_cnt=0
ignore_file_cnt=0
total_file_cnt=0
for cur_file in ${file_list[@]}; do

    equal_flag=0
    ignore_flag=0

    if [ ! -f ${cur_file} ]; then
        continue
    fi

    let total_file_cnt++

    for ignore_file in ${ingore_file_list[@]}; do
        if [ "x${cur_file}" == "x./${ignore_file}" ]; then
            ignore_flag=1
            break
        fi
    done

    if [ $ignore_flag -eq 1 ]; then
        echo "ignore $cur_file"
        continue
    fi

    md5_cur_file=$(md5sum $cur_file | awk '{print $1}')

    co_files=${co_path}/$cur_file
    for co_file in ${co_files}; do
        res=$(__md5_cmp $cur_file $co_file)
        if [ $res -eq 1 ]; then
            equal_flag=1
            break;
        fi
    done

    if [ $equal_flag -ne 1 ]; then
        let diff_file_cnt++
        echo "no same found, $co_files $cur_file"
        cp $co_files $cur_file
    fi

    let test_cnt++
    if [ $test_cnt -ge 500 ]; then
        test_cnt=0
        echo $(date +%Y%m%H%M%S)
    fi
done

echo "end, total: $total_file_cnt, diff file cnt: $diff_file_cnt"

