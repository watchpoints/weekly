#!/bin/bash
#
# root@20200427
rtmp="rtmp://cctvcncs.v.wscdns.com/live/news10_1_3600"
nohup ./ffmpeg -i $rtmp -acodec copy -vcodec copy -movflags faststart -f mp4 ./$(date "+%Y%m%d%H%M%S").mp4  >>./ffmpeg_mp4.log 2>&1 &
pidffmpeg=$!
echo "[mp4] First ffmpeg PID is ${pidffmpeg}"
sleep 2

while :
do
    pid=$(ps x | grep $pidffmpeg | grep -v grep | awk '{print $1}')

    if [ "$pid" = "" ]; then
        	echo "[mp4] record ffmpeg server is not running, restart up master server now..."
		nohup ./ffmpeg -i $rtmp -acodec copy -vcodec copy -movflags faststart -f mp4 ./$(date "+%Y%m%d%H%M%S").mp4  >>./ffmpeg_mp4.log 2>&1 &
		pidffmpeg=$!
		echo "[mp4] record ffmpeg PID is ${pidffmpeg}"
    else
        echo "[mp4] record ffmpeg server[${pid}] is running..."
    fi

    sleep 3
done
