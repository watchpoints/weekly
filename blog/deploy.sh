
#!/bin/bash
# 进入打包文件夹
hugo
cd public
git add -A

git commit -m "update"

git push -f  

# 回到原文件夹
cd ..
