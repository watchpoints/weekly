#!/bin/bash
#
# root@20200427

num=1
master_max=5
slave_max=8
rtmp="rtmp://cctvcncs.v.wscdns.com/live/news10_1_3600"
nohup ./ffmpeg -y -i $rtmp -vcodec copy -acodec copy -f mpegts ./$(date "+%Y%m%d%H%M%S").ts >>./ffmpeg_ts.log 2>&1 &
pidffmpeg=$!
echo "[ts] First ffmpeg PID is ${pidffmpeg}"
sleep 2

while :
do
    pid=$(ps x | grep $pidffmpeg | grep -v grep | awk '{print $1}')

    if [ "$pid" = "" ]; then
        if (("$num" < "$master_max" )); then
                num=$(($num+1))
                 nohup ./ffmpeg -y -i $rtmp -vcodec copy -acodec copy -f mpegts ./$(date "+%Y%m%d%H%M%S").ts >>./ffmpeg_ts.log 2>&1 &
		pidffmpeg=$!
		echo "[ts] ffmpeg PID is ${pidffmpeg}"
	else
		num=$(($num+1))
	         nohup ./ffmpeg -y -i $rtmp -vcodec copy -acodec copy -f mpegts ./$(date "+%Y%m%d%H%M%S").ts >>./ffmpeg_ts.log 2>&1 &
		pidffmpeg=$!
                echo "[MeiDian] ffmpeg PID is ${pidffmpeg}"
		if (("$num" > "$slave_max"));then
                        num=1
                fi
	fi
    else
	num=1
        echo "[ts] ffmpeg server[${pid}] is running..."
    fi

    sleep 3
done
