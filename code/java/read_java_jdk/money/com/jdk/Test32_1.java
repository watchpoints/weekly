
public class Test32_1 {
    public static void main(String[] args) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 32; i++) {
            sb.append("1");
        }
        System.out.println(sb);
        int i = Integer.parseUnsignedInt(sb.toString(),2);
        System.out.println(i);

        System.out.println("MIN_VALUE = "+Integer.MIN_VALUE+", binary = "+Integer.toBinaryString(Integer.MIN_VALUE));
        System.out.println("MAX_VALUE = "+Integer.MAX_VALUE+" , binary = "+Integer.toBinaryString(Integer.MAX_VALUE));
    }
}
/** 
11111111111111111111111111111111
-1
MIN_VALUE = -2147483648, binary = 10000000000000000000000000000000
MAX_VALUE = 2147483647 , binary = 1111111111111111111111111111111

作者：Alex Wang
链接：https://www.zhihu.com/question/50098632/answer/119740171
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
*/