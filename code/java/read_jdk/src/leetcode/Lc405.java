package leetcode;

public class Lc405 {
	public String toHex(int num) {

		// 01 check
		if (num == 0)
			return "0";

		// 02 data
		StringBuilder output = new StringBuilder();
		int lowBit = 0x000f;// 15
		char[] mapHex = new char[] { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

		// 03 time
		while (num !=0) {
			//十进制表示 -->十六进制表示
			//System.out.println(Integer.toBinaryString(num));
			int decIndex = num & lowBit; 
			// 疑问：获取的后四位吗？怎么变成整数类型，二进制呢？
			//眼睛看到 就是从内存读取到寄存器数据。自动完成数据转换。
			output.append(mapHex[decIndex]);
			//n = n >>4; //死循环了。
			num >>>= 4;
			//System.out.println(Integer.toBinaryString(num));
		}
		//O4 bug -1 ;
		//算术右移（>>）：有符号的移位操,右移之后左侧的空位正数用0补充，负数用1补充
		//11111111111111111111111111111111 死循环了。
		//逻辑右移（>>>，无符号移位,右移之后左侧的空位用0补充
		
		return output.reverse().toString();

	}
	public static void main(String[] args)
	{       
		   Lc405 lc =new Lc405();
		  // System.out.println(lc.toHex(-5));
		   System.out.println(lc.toHex(-1));
		   System.out.println(lc.toHex(26));
	}
	//https://blog.csdn.net/u013399093/article/details/70331979
}

//逻辑右移：低位溢出，高位补0
//算术右移：低位溢出，高位用符号位的值补
