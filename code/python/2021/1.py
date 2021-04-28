from collections.abc import Iterable, Iterator, Generator

def test(N):
    index = 0
    while index < N:
        # 通过send()发送的信息将赋值给jump
        jump = yield index
        if jump is None:
            jump = 1
        index += jump

if __name__ == '__main__':
    a = test(5)
    print(next(a)) # 0 < 5 在yield处阻塞
    print(a.send(2)) # 继续往下执行 此时jump == 2。则index = 0 + 2 = 2，然后阻塞
    print(next(a)) # 没有给jump传值，此时jump == 1。index = 2 + 1 = 3，然后阻塞
    print(a.send(-3)) # 给jump传值为-3，继续往下执行，index = 3 - 3 = 0，此时继续阻塞在yield处，程序结束。