package leetcode;

public class ShiftTest {
	public static void main(String args[]) {
		ShiftTest st = new ShiftTest();
		st.ShiftTest1();
	}

	public void ShiftTest1() {
	

	
		System.out.println("*****************��������(>>)****************");
		int num = 54321;
		System.out.println("��λǰ��num= " + num + "  ������Ϊ��" + Integer.toBinaryString(num));
		num = num >> 11;
		System.out.println("��λ��num= " + num + "  ������Ϊ�� " + Integer.toBinaryString(num));

		int num1 = -54321;
		System.out.println("��λǰ��num1= " + num1 + "  ������Ϊ��" + Integer.toBinaryString(num1));
		num1 = num1 >> 11;
		System.out.println("��λ��num1= " + num1 + "  ������Ϊ�� " + Integer.toBinaryString(num1));

		System.out.println("*********************************************");

		System.out.println("\n");
		System.out.println("*****************�߼����ƣ�>>>��*****************");
		int v1 = 54321;
		System.out.println("��λǰ��v1= " + v1 + "  ������Ϊ��" + Integer.toBinaryString(v1));
		v1 = v1 >>> 11;
		System.out.println("��λ��v1= " + v1 + "  ������Ϊ��" + Integer.toBinaryString(v1));

		int v2 = -54321;
		System.out.println("��λǰ��v2= " + v2 + "  ������Ϊ��" + Integer.toBinaryString(v2));
		v2 = v2 >>> 11;
		System.out.println("��λ��v2= " + v2 + "  ������Ϊ��" + Integer.toBinaryString(v2));

		System.out.println("************************************************");
	}
}
