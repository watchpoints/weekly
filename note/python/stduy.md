- subprocess.run 函数调用参数很重要需要【】包含起来才有返回一个类对象



~~~python
# Migrate one file from fast pool to slow pool
def doMigrate(path, rank_id, opt):
  
    process = subprocess.run([cmdMigrate], shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE)
    if process.returncode == 0:
      
    else:
        logging.error(" stderr:" + str(process.stderr))
        logging.error(" stdout:" + str(process.stdout))
        logging.error("doMigrate err:" + cmdMigrate + " returncode=" + str(process.returncode))

    return ret
~~~



https://docs.python.org/3.9/library/subprocess.html#subprocess.call

```
run(...).returncode

 process = subprocess.run([cmdMigrate], shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE)
    if process.returncode == 0:
        body = str(process.stdout, encoding='utf-8')
        
```

### 第一天 安装python3环节



```powershell
docker pull python
docker run   -itd --name python3  python:latest bash

docker exec -it 93052d64ad1c bash

```

