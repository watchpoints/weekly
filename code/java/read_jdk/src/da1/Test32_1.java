package da1;

public class Test32_1 {
    public static void main(String[] args) {
        StringBuilder sb = new StringBuilder();
        //for (int i = 0; i < 64; i++) {
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