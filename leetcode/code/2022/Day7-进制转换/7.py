class Solution:
    def toHex(self, num: int) -> str:
        #如果是0直接返回"0"
        if num == 0:
            return "0"
        #十六进制字符串
        s = "0123456789abcdef"
        result = []
        #因为是32位机器，所以是8个F
        num = num & 0xFFFFFFFF
        while num > 0:
            #从右至左，每4个bit位进行与操作，得到该十六进制的字符表示
            result.append(s[num & 0XF])
            #每次右移4位
            num = num >> 4
        #结果取反序返回
        return "".join(result[::-1])