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
			//ʮ���Ʊ�ʾ -->ʮ�����Ʊ�ʾ
			//System.out.println(Integer.toBinaryString(num));
			int decIndex = num & lowBit; 
			// ���ʣ���ȡ�ĺ���λ����ô����������ͣ��������أ�
			//�۾����� ���Ǵ��ڴ��ȡ���Ĵ������ݡ��Զ��������ת����
			output.append(mapHex[decIndex]);
			//n = n >>4; //��ѭ���ˡ�
			num >>>= 4;
			//System.out.println(Integer.toBinaryString(num));
		}
		//O4 bug -1 ;
		//�������ƣ�>>�����з��ŵ���λ��,����֮�����Ŀ�λ������0���䣬������1����
		//11111111111111111111111111111111 ��ѭ���ˡ�
		//�߼����ƣ�>>>���޷�����λ,����֮�����Ŀ�λ��0����
		
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

//�߼����ƣ���λ�������λ��0
//�������ƣ���λ�������λ�÷���λ��ֵ��
