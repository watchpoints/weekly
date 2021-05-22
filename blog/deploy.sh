
#!/bin/bash
# 进入打包文件夹
cd public

git add -A

git commit -m "update"

git push 

# 回到原文件夹
cd ..
