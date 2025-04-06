@echo off          

cd /d D:\code\weekly\blog
start hugo
cd /d D:\code\weekly\blog\public
echo 当前目录是：%cd%
git pull
git add -A .
git commit -m "update"
git push 




