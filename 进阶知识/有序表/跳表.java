package class36;

import java.util.ArrayList;

/**
//源码来自terry，仅做学习使用
 * Created by Terry
 * User: Administrator
 * Date: 2022-08-24
 * Time: 8:15
 * Description: 跳表
 */
public class Code02_SkipList {
    // for test
    public static void printAll(SkipListMap<String, String> obj) {
        for (int i = obj.maxLevel; i >= 0; i--) {
            System.out.print("Level " + i + " : ");
            SkipListNode<String, String> cur = obj.head;
            while (cur.nextNode.get(i) != null) {
                SkipListNode<String, String> next = cur.nextNode.get(i);
                System.out.print("(" + next.key + " , " + next.value + ") ");
                cur = next;
            }
            System.out.println();
        }
    }

    public static void main(String[] args) {
        SkipListMap<String, String> test = new SkipListMap<>();
        printAll(test);
        System.out.println("======================");
        test.put("A", "10");
        printAll(test);
        System.out.println("======================");
        test.remove("A");
        printAll(test);
        System.out.println("======================");
        test.put("E", "E");
        test.put("B", "B");
        test.put("A", "A");
        test.put("F", "F");
        test.put("C", "C");
        test.put("D", "D");
        printAll(test);
        System.out.println("======================");
        System.out.println(test.containsKey("B"));
        System.out.println(test.containsKey("Z"));
        System.out.println(test.firstKey());
        System.out.println(test.lastKey());
        System.out.println(test.floorKey("D"));
        System.out.println(test.ceilingKey("D"));
        System.out.println("======================");
        test.remove("D");
        printAll(test);
        System.out.println("======================");
        System.out.println(test.floorKey("D"));
        System.out.println(test.ceilingKey("D"));
    }

    private static class SkipListNode<K extends Comparable<K>, V> {
        public K key;
        public V value;
        public ArrayList<SkipListNode<K, V>> nextNode;

        public SkipListNode(K key, V value) {
            this.key = key;
            this.value = value;
            nextNode = new ArrayList<>();
        }

        // 判断当前节点的key 是否 < otherKey，成立就返回true。如果otherKey=null，就返回false
        public boolean isKeyLess(K otherKey) {
            if (otherKey == null) {
                return false;
            }
            return key == null || key.compareTo(otherKey) < 0; // key可能为null
        }

        // 判断key和otherKey是否相等。
        public boolean isKeyEqual(K otherKey) {
            if (key == null && otherKey == null) {
                return true;
            }
            // 二者都不为null，才判断
            return (key != null && otherKey != null) && key.compareTo(otherKey) == 0;
        }
    }

    public static class SkipListMap<K extends Comparable<K>, V> {
        private static final double PROBABILITY = 0.5; // < 0.5就继续累加，>=0.5就停止。用于新节点的level高度随机产生
        private SkipListNode<K, V> head;
        private int size; // 整个跳表的节点数
        private int maxLevel; // 整个跳表最大高度

        public SkipListMap() {
            head = new SkipListNode<>(null, null);
            head.nextNode.add(null); // 表示0号指针
            size = 0;
            maxLevel = 0;
        }

        public void put(K key, V value) {
            if (key == null) {
                throw new RuntimeException("key is invalid.");
            }
            SkipListNode<K, V> less = mostRightLessNodeInTree(key);
            SkipListNode<K, V> find = less.nextNode.get(0); //再往下挑一个
            if (find != null && find.isKeyEqual(key)) { // 更新值
                find.value = value;
            } else { // find == null || fing.key != key
                size++;
                int newNodeLevel = 0; // 新建节点的高度。一定拥有第0层
                if (Math.random() < PROBABILITY) {
                    newNodeLevel++;
                }
                // newNodeLevel 有可能超过最大高度的，需要更新
                // 最左节点要和当前新节点一样高
                // 这个while是有可能走不到的，如果随机得到的层数小于最左节点的高度就不会进入
                while (maxLevel < newNodeLevel) {
                    head.nextNode.add(null);
                    maxLevel++;
                }
                SkipListNode<K, V> newNode = new SkipListNode<>(key, value);
                for (int i = 0; i <= newNodeLevel; i++) { // 新节点的0~level层都是需要添加指针的
                    newNode.nextNode.add(null);
                }
                // 从head的最高层开始，往右寻找每一层 < key的最右节点
                int level = maxLevel;
                SkipListNode<K, V> pre = head;
                while (level >= 0) {
                    // 在level层找 <key的最右节点
                    pre = mostRightLessNodeInLevel(pre, level, key);
                    if (level <= newNodeLevel) { //新节点有当前层的时候才插入
                        newNode.nextNode.set(level, pre.nextNode.get(level));
                        pre.nextNode.set(level, newNode);
                    }
                    level--;
                }
            }
        }

        public V get(K key) {
            if (key == null) {
                return null;
            }
            SkipListNode<K, V> less = mostRightLessNodeInTree(key); // 先找<key的最右节点
            SkipListNode<K, V> find = less.nextNode.get(0);
            return find != null && find.isKeyEqual(key) ? find.value : null;
        }

        public void remove(K key) {
            if (containsKey(key)) { //有这个key的时候才会删除
                size--;
                SkipListNode<K, V> pre = head;
                int level = maxLevel; //从最高层开始
                while (level >= 0) {
                    pre = mostRightLessNodeInLevel(pre, level, key);
                    SkipListNode<K, V> next = pre.nextNode.get(level);
                    // 分为两种情况
                    // 1、pre的下一个节点就=被删除的key
                    // 2、pre的下一个节点的key 是 > 被删除的key。（只有需要删除的节点高度不够时才会有这种情况 ）
                    if (next != null && next.isKeyEqual(key)) {
                        // C++的，需要在整个跳表的第0层处，要释放当前节点。
                        pre.nextNode.set(level, next.nextNode.get(level));
                    }
                    // 还需判断在head节点处，最上层的指针是null的情况、除了第0层
                    // 也就是删除整个跳表最上面那指向null的层数
                    //不能缩0层
                    if (level != 0 && pre == head && head.nextNode.get(level) == null) {
                        head.nextNode.remove(level);
                        maxLevel--;
                    }
                    level--;
                }
            }
        }

        public K firstKey() {
            return head.nextNode.get(0) != null ? head.nextNode.get(0).key : null;
        }

        public K lastKey() {
            int level = maxLevel;
            SkipListNode<K, V> cur = head;
            //从高层开始找到最后一个
            while (level >= 0) {
                SkipListNode<K, V> next = cur.nextNode.get(level);
                while (next != null) {
                    cur = next;
                    next = next.nextNode.get(level);
                }
                level--;
            }
            return cur.key; // 有可能返回null
        }

        // 返回 >=key的，最左的节点
        public K ceilingKey(K key) {
            if (key == null) {
                return null;
            }
            SkipListNode<K, V> less = mostRightLessNodeInTree(key);
            SkipListNode<K, V> find = less.nextNode.get(0);
            return find != null ? find.key : null;
        }

        // 返回 <=key的最右节点
        public K floorKey(K key) {
            if (key == null) {
                return null;
            }
            SkipListNode<K, V> less = mostRightLessNodeInTree(key);
            SkipListNode<K, V> find = less.nextNode.get(0);
            // 还需判断find节点就是=key的情况
            return find != null && find.isKeyEqual(key) ? find.key : less.key;
        }

        public int size() {
            return size;
        }

        public boolean containsKey(K key) {
            if (key == null) {
                return false;
            }
            SkipListNode<K, V> less = mostRightLessNodeInTree(key);
            SkipListNode<K, V> find = less.nextNode.get(0);
            return find != null && find.isKeyEqual(key);
        }

        // 在level层，找 < key 的最右节点。整体顺序是从左往右递增的。
        private SkipListNode<K, V> mostRightLessNodeInLevel(SkipListNode<K, V> node, int level, K key) {
            SkipListNode<K, V> next = node.nextNode.get(level);
            while (next != null && key.compareTo(next.key) > 0) { // 不可能存在 =0的情况，因为在上层就已经跑过一遍，=0的话，就是更新值
                node = next;
                next = node.nextNode.get(level);
            }
            return node;
        }

        private SkipListNode<K, V> mostRightLessNodeInTree(K key) {
            if (key == null) {
                return null;
            }
            int level = maxLevel;
            SkipListNode<K, V> cur = head;
            while (level >= 0) {
                cur = mostRightLessNodeInLevel(cur, level, key);
                level--;
            }
            return cur;
        }

    }
}
