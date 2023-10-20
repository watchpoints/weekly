---
title: "day1:obs"
date: 2023-09-24
description: ""
draft: false
tags: ["money"]

---







# 规则

1. 歌曲类版权限制，不使用 
2. 相关组织，比如大家说的影视审核群？
3. 读书类的 ok

# 基本用啊


提问1：how-to-concatenate-two-mp4-files-using-ffmpeg


https://stackoverflow.com/questions/7333232/how-to-concatenate-two-mp4-files-using-ffmpeg

提问2：/usr/local/qcloud/YunJing/YDEyes/YDService

https://www.aiunk.com/1315/

## 功能
1. 用 FFmpeg 给 mp4 添加封面
ffmpeg -i input.mp4 -i cover.png -map 0 -map 1 -c copy -disposition:v:1 attached_pic output.mp4

2. 提取MP3

~~~~ 
ffmpeg -i .\2023-P01-如何做月目标.mp4 -vn   -f mp3 1.mp3
~~~~

-vn表示no video，

https://ffmpeg.org/ffmpeg.html

-vn (*input/output*)

As an input option, blocks all video streams of a file from being filtered or being automatically selected or mapped for any output. See `-discard` option to disable streams individually.



