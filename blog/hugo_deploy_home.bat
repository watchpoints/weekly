@echo off             #从本行开始关闭回显。一般批处理第一行都是这个

cd /d G:\money\eye\blog
start hugo
cd /d G:\money\eye\blog\public
echo 当前目录是：%cd%

git add -A .

git commit -m "update"

git push 



pause
