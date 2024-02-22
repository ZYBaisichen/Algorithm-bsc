	
//摘自：https://github.com/algorithmzuo/algorithm-journey/blob/main/src/class025/Code01_HeapSort.java
public class Code01_HeapSort {
	
	public static int MAXN = 501;

	public static int[] arr = new int[MAXN];

	public static int n;
    // i位置的数，向上调整大根堆
	public static void heapInsert(int i) {
		while (arr[i] > arr[(i - 1) / 2]) {
			swap(i, (i - 1) / 2);
			i = (i - 1) / 2;
		}
	}

	// i位置的数，向下调整大根堆
	// 当前堆的大小为size
	public static void heapify(int i, int size) {
		int l = i * 2 + 1;
		while (l < size) {
			int best = l + 1 < size && arr[l + 1] > arr[l] ? l + 1 : l;
			best = arr[best] > arr[i] ? best : i;
			if (best == i) {
				break;
			}
			swap(best, i);
			i = best;
			l = i * 2 + 1;
		}
	}

	public static void swap(int i, int j) {
		int tmp = arr[i];
		arr[i] = arr[j];
		arr[j] = tmp;
	}
}