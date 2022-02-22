
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentSkipListMap;

public class ConcurrentSkipListMapTest {
    // 循环次数
    private final static int loopCount = 5;
    // 操作的HashMap和ConcurrentSkipListMap
    private final static Map<String, String> hashMap = new HashMap<>();
    private final static ConcurrentSkipListMap<String, String> concurrentSkipListMap = new ConcurrentSkipListMap();

    public static void main(String[] args) {
        // 开启两个线程同时操作
        new Thread(new ConcurrentSkipListMapTest.OperateThread(concurrentSkipListMap, loopCount), "T-1").start();
        new Thread(new ConcurrentSkipListMapTest.OperateThread(concurrentSkipListMap, loopCount), "T-2").start();

        //new Thread(new ConcurrentSkipListMapTest.OperateThread(hashMap, loopCount), "T-3").start();
        //new Thread(new ConcurrentSkipListMapTest.OperateThread(hashMap, loopCount), "T-4").start();
    }

    // 遍历方法
    private static void iterate(Map<String, String> map) {
        StringBuilder stringBuilder = new StringBuilder();
        for (Map.Entry<String, String> entry : map.entrySet()) {
            stringBuilder.append(entry.getKey() + " => " + entry.getValue());
            stringBuilder.append(", ");
        }
        // 删除最后多余的字符
        stringBuilder.delete(stringBuilder.length() - 2, stringBuilder.length() - 1);
        // 打印
        System.out.println(Thread.currentThread().getName() + " iterate: " + stringBuilder.toString());
    }

    private static class OperateThread implements Runnable {

        private Map map;
        private int loopCount;

        public OperateThread(Map map, int loopCount) {
            this.map = map;
            this.loopCount = loopCount;
        }

        @Override
        public void run() {
            // 循环添加并遍历打印
            while (loopCount >= 0) {
                map.put(Thread.currentThread().getName() + " - " + loopCount, "" + loopCount);
                iterate(map);
                loopCount--;
            }
        }
    }
}