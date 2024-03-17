package class36;

/**
 * 原作者为Terry，这里仅做阅读使用
 * Created by Terry
 * User: Administrator
 * Date: 2022-08-18
 * Time: 14:54
 * Description: SBT
 */
public class Code01_SizeBalanceTree {
    // for test
    public static void printAll(SBTNode<String, Integer> head) {
        System.out.println("Binary Tree:");
        printInOrder(head, 0, "H", 17);
        System.out.println();
    }

    // for test
    public static void printInOrder(SBTNode<String, Integer> head, int height, String to, int len) {
        if (head == null) {
            return;
        }
        printInOrder(head.right, height + 1, "v", len);
        String val = to + "(" + head.key + "," + head.value + ")" + to;
        int lenM = val.length();
        int lenL = (len - lenM) / 2;
        int lenR = len - lenM - lenL;
        val = getSpace(lenL) + val + getSpace(lenR);
        System.out.println(getSpace(height * len) + val);
        printInOrder(head.left, height + 1, "^", len);
    }

    // for test
    public static String getSpace(int num) {
        String space = " ";
        StringBuffer buf = new StringBuffer("");
        for (int i = 0; i < num; i++) {
            buf.append(space);
        }
        return buf.toString();
    }
    public static void main(String[] args) {
        SizeBalanceTree<String, Integer> sbt = new SizeBalanceTree<String, Integer>();
        sbt.put("d", 4);
        sbt.put("c", 3);
        sbt.put("a", 1);
        sbt.put("b", 2);
        // sbt.put("e", 5);
        sbt.put("g", 7);
        sbt.put("f", 6);
        sbt.put("h", 8);
        sbt.put("i", 9);
        sbt.put("a", 111);
        System.out.println(sbt.get("a"));
        sbt.put("a", 1);
        System.out.println(sbt.get("a"));
        for (int i = 0; i < sbt.size(); i++) {
            System.out.println(sbt.getIndexKey(i) + " , " + sbt.getIndexValue(i));
        }
        printAll(sbt.root);
        System.out.println(sbt.firstKey());
        System.out.println(sbt.lastKey());
        System.out.println(sbt.floorKey("g"));
        System.out.println(sbt.ceilingKey("g"));
        System.out.println(sbt.floorKey("e"));
        System.out.println(sbt.ceilingKey("e"));
        System.out.println(sbt.floorKey(""));
        System.out.println(sbt.ceilingKey(""));
        System.out.println(sbt.floorKey("j"));
        System.out.println(sbt.ceilingKey("j"));
        sbt.remove("d");
        printAll(sbt.root);
        sbt.remove("f");
        printAll(sbt.root);
    }

    private static class SBTNode<K extends Comparable<K>, V> {
        public K key;
        public V value;
        public int size;
        public SBTNode<K, V> left;
        public SBTNode<K, V> right;

        public SBTNode(K key, V value) {
            this.key = key;
            this.value = value;
            size = 1;
        }
    }

    private static class SizeBalanceTree<K extends Comparable<K>, V> {
        private SBTNode<K, V> root;

        private SBTNode<K, V> getIndex( SBTNode<K, V> cur, int kth) {
            if (kth == (cur.left != null ? cur.left.size : 0) + 1) {
                return cur;
            } else if (kth <= (cur.left != null ? cur.left.size : 0)) {
                return getIndex(cur.left, kth);
            } else {
                return getIndex(cur.right, kth - (cur.left != null ? cur.left.size : 0) - 1);
            }
        }

        public K getIndexKey(int index) {
            if (index < 0 || index >= this.size()) {
                throw new RuntimeException("invalid parameter.");
            }
            return getIndex(root, index + 1).key;
        }

        public V getIndexValue(int index) {
            if (index < 0 || index >= this.size()) {
                throw new RuntimeException("invalid parameter.");
            }
            return getIndex(root, index + 1).value;
        }

        public int size() {
            return root == null ? 0 : root.size;
        }

        //（key, value） 加入到有序表，新增、修改value
        public void put(K key, V val) {
            if (key == null) {
                throw new RuntimeException("key is invalid.");
            }
            SBTNode<K, V> lastNode = findLastIndex(key); //找到key或其父节点
            if (lastNode != null && key.compareTo(lastNode.key) == 0) { // 更新值的情况
                lastNode.value = val;
            } else { // 没找到，则新插入值。从root开始加
                root = add(root, key, val);
            }
        }

        // 返回等于key的，或者key的父节点。
        private SBTNode<K, V> findLastIndex(K key) {
            SBTNode<K, V> pre = root;
            SBTNode<K, V> cur = root;
            while (cur != null) {
                pre = cur;
                if (key.compareTo(cur.key) == 0) {
                    break;
                } else if (key.compareTo(cur.key) < 0) {
                    cur = cur.left;
                } else {
                    cur = cur.right;
                }
            }
            return pre; //返回最近的不空的节点，有可能是key，也有可能是key的父节点
        }

        //返回新头部
        private SBTNode<K, V> add(SBTNode<K, V> node, K key, V val) {
            if (node == null) {
                return new SBTNode<K, V>(key, val);
            } else {
                node.size++; //划的同时，每个节点的size++
                if (key.compareTo(node.key) < 0) {
                    node.left = add(node.left, key, val); //左树也有可能因为这个add行为换头结点
                } else {
                    node.right = add(node.right, key, val); //右树也有可能因为这个add行为换头结点
                }
                // 维持平衡.
                // 调用到底层后，会依次调整平衡返回到上游。每一层都会调整，整体方向是从上往下add，从下往上maintain
                // 返回值是新的头部？
                return maintain(node);
            }
        }

        public void remove(K key) {
            if (key == null) {
                throw new RuntimeException("key is invalid.");
            }
            //有这个key了，才会递归去删除
            if (containsKey(key)) {
                root = delete(root, key);
            }
        }
        //只在add的时候做平衡性调整，delete的时候不调整
        //返回新头部。
        private SBTNode<K, V> delete(SBTNode<K, V> node, K key) {
            node.size--;
            int compare = key.compareTo(node.key);
            if (compare < 0) {
                node.left = delete(node.left, key); //左右子树的新头部被自己的左节点抓住
            } else if (compare > 0) {
                node.right = delete(node.right, key);
            } else { // ==0的情况
                if (node.right == null) { // 有左孩子的情况，或者左右孩子都没有
                    node = node.left; //让自己的左孩子接替自己
                } else if (node.left == null) { // 有右孩子的情况
                    node = node.right;
                } else { // 左右孩子都有的情况。让后继节点代替自己
                    SBTNode<K, V> pre = null;
                    SBTNode<K, V> cur = node.right;
                    cur.size--;
                    //因为要删除右树上的一个节点，所以沿途路径上的节点个数减一
                    while (cur.left != null) {
                        pre = cur;
                        cur = cur.left;
                        cur.size--;
                    }
                    if (pre != null) {
                        pre.left = cur.right; //让后继节点的右孩子代替它
                        cur.right = node.right; //将后继节点代替自己的位置，right
                    }
                    cur.left = node.left; //将后继节点代替自己的位置，left
                    cur.size = cur.left.size + (cur.right != null ? cur.right.size : 0) + 1; //更新新头部的size
                    node = cur; //新头部，彻底舍弃node了。但C++里面要有个delete释放内存操作
                }
            }
            // 维持平衡---可以不用维持平衡，在add的时候再维护
            // node = maintain(node);
            return node;
        }

        public boolean containsKey(K key) {
            if (key == null) {
                throw new RuntimeException("key is invalid.");
            }
            SBTNode<K, V> lastNode = findLastIndex(key);
            return lastNode != null && key.compareTo(lastNode.key) == 0;
        }

        private SBTNode<K, V> maintain(SBTNode<K, V> node) {
            if (node == null) {
                return null;
            }
            // 计算node的下一级节点数和 下下一级节点数
            int leftSize = node.left != null ? node.left.size : 0;
            int rightSize = node.right != null ? node.right.size : 0;
            int leftLeftSize = node.left != null && node.left.left != null ? node.left.left.size : 0; // LL
            int leftRightSize = node.left != null && node.left.right != null ? node.left.right.size : 0; // LR
            int rightLeftSize = node.right != null && node.right.left != null ? node.right.left.size : 0; // RL
            int rightRightSize = node.right != null && node.right.right != null ? node.right.right.size : 0; // RR
            if (leftLeftSize > rightSize) { // LL型
                node = rightRotate(node); //右旋
                node.right = maintain(node.right); // 先调整node下级节点的平衡
                node = maintain(node); //最后调整上方节点的平衡性
            } else if (leftRightSize > rightSize) { // LR型
                node.left = leftRotate(node.left); // 先左旋转。返回新头部
                node = rightRotate(node); // 再右旋转
                node.left = maintain(node.left);  //注意这里node已经是左右旋后的新头部了。
                node.right = maintain(node.right); // 先维持node的下级节点
                node = maintain(node);
            } else if (rightLeftSize > leftSize) { // RL型旋转
                node.right = rightRotate(node.right); //右旋
                node = leftRotate(node); //左旋
                node.left = maintain(node.left); //依次调整
                node.right = maintain(node.right);
                node = maintain(node);
            } else if (rightRightSize > leftSize) { // LL型旋转
                node = leftRotate(node);
                node.left = maintain(node.left);
                node = maintain(node);
            }
            return node;
        }

        private SBTNode<K, V> leftRotate(SBTNode<K, V> node) {
            SBTNode<K, V> newHead = node.right;
            node.right = newHead.left;
            newHead.left = node;
            newHead.size = node.size;
            node.size = (node.left != null ? node.left.size : 0) + (node.right != null ? node.right.size : 0) + 1;
            return newHead;
        }

        private SBTNode<K, V> rightRotate(SBTNode<K, V> node) {
            SBTNode<K, V> newHead = node.left;
            node.left = newHead.right;
            newHead.right = node;
            newHead.size = node.size; //新上来的size等于旧的size
            node.size = (node.left != null ? node.left.size : 0) + (node.right != null ? node.right.size : 0) + 1; //左右子树的和+1
            return newHead; //返回新头部
        }

        public V get(K key) {
            if (key == null) {
                throw new RuntimeException("key is invalid.");
            }
            SBTNode<K, V> lastNode = findLastIndex(key);
            if (lastNode != null && key.compareTo(lastNode.key) == 0) {
                return lastNode.value;
            }
            return null;
        }

        public K firstKey() {
            if (root == null) {
                return null;
            }
            SBTNode<K, V> node = root;
            while (node.left != null) {
                node = node.left;
            }
            return node.key;
        }

        public K lastKey() {
            if (root == null) {
                return null;
            }
            SBTNode<K, V> node = root;
            while (node.right != null) {
                node = node.right;
            }
            return node.key;
        }

        // <= key的，最接近key的
        public K floorKey(K key) {
            if (key == null) {
                throw new RuntimeException("key is invalid.");
            }
            SBTNode<K, V> ans = null;
            SBTNode<K, V> cur = root;
            while (cur != null) {
                if (key.compareTo(cur.key) == 0) {
                    ans = cur;
                    break;
                } else if (key.compareTo(cur.key) < 0) { // key < cur.key
                    cur = cur.left;
                } else { // key >= cur.key
                    ans = cur;
                    cur = cur.right;
                }
            }
            return ans == null ? null : ans.key;
        }


        // >= key的，最接近key的
        public K ceilingKey(K key) {
            if (key == null) {
                throw new RuntimeException("key is invalid.");
            }
            SBTNode<K, V> ans = null;
            SBTNode<K, V> cur = root;
            while (cur != null) {
                if (key.compareTo(cur.key) == 0) {
                    ans = cur;
                    break;
                } else if (key.compareTo(cur.key) < 0) { // key < cur.key
                    ans = cur;
                    cur = cur.left;
                } else { // key >= cur.key
                    cur = cur.right;
                }
            }
            return ans == null? null : ans.key;
        }
    }
}
