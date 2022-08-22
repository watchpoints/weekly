@echo off          

cd /d D:\db\daily-interview\blog
start hugo
cd /d D:\db\daily-interview\blog\public
echo 当前目录是：%cd%
git add -A .
git commit -m "update"
git push 
pause



