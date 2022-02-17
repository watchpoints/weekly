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



   callgrind ------> 它主要用来检查程序中函数调用过程中出现的问题。

​    cachegrind ------> 它主要用来检查程序中缓存使用出现的问题。

​    helgrind ------> 它主要用来检查多线程程序中出现的竞争问题。

​    massif ------> 它主要用来检查程序中堆栈使用中出现的问题
