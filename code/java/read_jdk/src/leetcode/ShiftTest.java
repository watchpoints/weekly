package leetcode;

public class ShiftTest {
	public static void main(String args[]) {
		ShiftTest st = new ShiftTest();
		st.ShiftTest1();
	}

	public void ShiftTest1() {
	

	
		System.out.println("*****************算术右移(>>)****************");
		int num = 54321;
		System.out.println("移位前：num= " + num + "  二进制为：" + Integer.toBinaryString(num));
		num = num >> 11;
		System.out.println("移位后：num= " + num + "  二进制为： " + Integer.toBinaryString(num));

		int num1 = -54321;
		System.out.println("移位前：num1= " + num1 + "  二进制为：" + Integer.toBinaryString(num1));
		num1 = num1 >> 11;
		System.out.println("移位后：num1= " + num1 + "  二进制为： " + Integer.toBinaryString(num1));

		System.out.println("*********************************************");

		System.out.println("\n");
		System.out.println("*****************逻辑右移（>>>）*****************");
		int v1 = 54321;
		System.out.println("移位前：v1= " + v1 + "  二进制为：" + Integer.toBinaryString(v1));
		v1 = v1 >>> 11;
		System.out.println("移位后：v1= " + v1 + "  二进制为：" + Integer.toBinaryString(v1));

		int v2 = -54321;
		System.out.println("移位前：v2= " + v2 + "  二进制为：" + Integer.toBinaryString(v2));
		v2 = v2 >>> 11;
		System.out.println("移位后：v2= " + v2 + "  二进制为：" + Integer.toBinaryString(v2));

		System.out.println("************************************************");
	}
}
