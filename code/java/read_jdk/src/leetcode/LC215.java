package leetcode;

import java.util.Arrays;
import java.util.PriorityQueue;
import java.util.Random;
//215. Kth Largest Element in an Array
public class LC215 {
	
	public int findKthLargest2(int[] nums, int k) {
        final int N = nums.length;
        Arrays.sort(nums);
        return nums[N - k];
}
	
	//O(N) guaranteed running time + O(1) space
	public int findKthLargest(int[] nums, int k) {
		shuffle(nums);
		k = nums.length - k;
		int lo = 0;
		int hi = nums.length - 1;
		while (lo < hi) {
			final int j = partition(nums, lo, hi);
			if (j < k) {
				lo = j + 1;
			} else if (j > k) {
				hi = j - 1;
			} else {
				break;
			}
		}
		return nums[k];
	}

	private int partition(int[] a, int lo, int hi) {

		int i = lo;
		int j = hi + 1;
		while (true) {
			while (i < hi && less(a[++i], a[lo]))
				;
			while (j > lo && less(a[lo], a[--j]))
				;
			if (i >= j) {
				break;
			}
			exch(a, i, j);
		}
		exch(a, lo, j);
		return j;
	}

	private void shuffle(int a[]) {

		final Random random = new Random();
		for (int ind = 1; ind < a.length; ind++) {
			final int r = random.nextInt(ind + 1);
			exch(a, ind, r);
		}
	}

	private void exch(int[] a, int i, int j) {
		final int tmp = a[i];
		a[i] = a[j];
		a[j] = tmp;
	}

	private boolean less(int v, int w) {
		return v < w;
	}
	
    //O(N lg K) running time + O(K) memory
	public int findKthLargest1(int[] nums, int k) {

	    final PriorityQueue<Integer> pq = new PriorityQueue<>();
	    for(int val : nums) {
	        pq.offer(val);

	        if(pq.size() > k) {
	            pq.poll();
	        }
	    }
	    return pq.peek();
	}

	public static void main(String[] args) {
		//int[] arr = { 3,2,3,1,2,4,5,5,6 };
		int[] arr = { 1,1,1,1,1,1,1,1,1 };
		System.out.println("≈≈–Ú÷Æ«∞£∫");
		for (int i = 0; i < arr.length; i++) {
			System.out.print(arr[i] + " ");
		}
         
		System.out.println();
		// ∂—≈≈–Ú
		LC215 lc =new LC215();
		System.out.print(lc.findKthLargest1(arr,4));
 
		System.out.println();
		System.out.println("≈≈–Ú÷Æ∫Û£∫");
		for (int i = 0; i < arr.length; i++) {
			System.out.print(arr[i] + " ");
		}


	}

}
