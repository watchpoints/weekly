##  Valgrind  

### 一、这个技术出现的背景、初衷和要达到什么样的目标或是要解决什么样的问题





### 二、这个技术的优势和劣势分别是什么 







### 三、这个技术适用的场景。









### 四、技术的组成部分和关键点。

- Your program is then run on a synthetic CPU provided by the Valgrind core  
- Valgrind simulates every single instruction your program executes.  
- Valgrind serialises execution so that only one (kernel) thread is running at a time  
- A thread executes code only when it holds the abovementioned lock  

> Valgrind doesn't schedule the threads itself
>
> . It merely ensures that only one thread runs at once, using a simple locking scheme  
>
> , but it does mean that threaded apps never use more than one CPU simultaneously, 
>
> even if you have a multiprocessor or multicore machine.  

-  if you have some kind of concurrency,critical race, locking, or similar, bugs. In that case you might consider using the tools Helgrind  



### 五、技术的底层原理和关键实现

### 六、已有的实现和它之间的对比




