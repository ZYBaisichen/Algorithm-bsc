package class32;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

/**
 * Created by Terry
 * 仅供学习使用
 * User: Administrator
 * Date: 2022-08-10
 * Time: 15:47
 * Description: AC自动机：在前缀树上玩KMP。
 * 用途：给你一篇文章，和一些敏感词。查询这篇文章中有哪些敏感词。
 *  返回的是所有的敏感词
 */
public class Code03_AC1 {
    public static void main(String[] args) {
        ACAutomation ac = new ACAutomation();
        ac.insert("dhe");
        ac.insert("he");
        ac.insert("abcdheks");
        // 设置fail指针
        ac.build();

        List<String> contains = ac.containWords("abcdhekskdjfafhasldkflskdjhwqaeruv");
        for (String word : contains) {
            System.out.println(word);
        }
    }

    // 前缀树节点
    private static class Node {
        public String end; // 以当前节点结尾，这条线路的字符串
        public boolean endUse; // 标记是否已经找到过这个敏感词。如果是end节点，并且没有被收集过答案了是true。否则是false
        public Node fail; // fail指针，匹配失败时，往上找最佳的前缀字符串的开始节点
        public Node[] nexts; // 下级节点，可以是数组，也可以是哈希表的形式，根据数据类型来定

        public Node() {
            this.end = null;
            this.endUse = false;
            this.fail = null;
            this.nexts = new Node[26]; // 假设是26个小写字母
        }
    }

    private static class ACAutomation {
        private Node root;

        public ACAutomation() {
            this.root = new Node();
        }

        public void insert(String str) {
            char[] chars = str.toCharArray();
            Node cur = root;
            for (int i = 0; i < chars.length; i++) {
                int num = chars[i] - 'a';
                if (cur.nexts[num] == null) {
                    cur.nexts[num] = new Node();
                }
                cur = cur.nexts[num];
            }
            cur.end = str; // 尾结点，记录这条线路的字符串
        }

        // 连接所有节点的fail指针 ----使用BFS
        // 根节点的fail是null，根节点的直接下级节点的fail都是 指向 根节点
        public void build() {
            Queue<Node> queue = new LinkedList<>();
            queue.add(root);
            while (!queue.isEmpty()) { // BFS
                Node cur = queue.poll();
                // 遍历nexts数组
                for (int i = 0; i < 26; i++) { // 处理他的孩子节点
                    if (cur.nexts[i] != null) { // 有孩子节点的情况
                        cur.nexts[i].fail = root; // 先指向root。后续如果有其他情况，再修改
                        Node curFail = cur.fail;
                        while (curFail != null) {
                            if (curFail.nexts[i] != null) { // 父节点的fail指向的节点 也有走向i位置的路，就连接
                                cur.nexts[i].fail = curFail.nexts[i];
                                break; // 连上之后，直接跳出了
                            }
                            curFail = curFail.fail; // 再往下一个fail节点跳转
                        }
                        queue.add(cur.nexts[i]); // 当前节点入队列
                    }
                }
            }
        }

        // 查询文章中的敏感词：content
        public List<String> containWords(String content) {
            if (content == null || content.length() == 0) {
                return new ArrayList<>();
            }
            Node cur = root;
            int length = content.length();
            List<String> ans = new ArrayList<>();
            for (int i = 0; i < length; i++) {
                int index = content.charAt(i) - 'a';
                // 没有走向index的路，但是cur又不是根节点的情况，继续沿着fail走
                while (cur.nexts[index] == null && cur != root) {
                    cur = cur.fail;
                }
                // 出了上面的while有两种可能
                // 现在来到的路径，是可以继续匹配的
                // 现在来到的节点已经是头节点了
                cur = cur.nexts[index] != null? cur.nexts[index] : root; //能往下走继续往下走，否则来到root
                // 现在cur要么是走到了下级节点，要么就还是在root位置
                // 以当前cur节点跑一遍fail指针，尝试搜集沿途的敏感词
                Node follow = cur;
                //过一圈答案
                while (follow != root) {
                    if (follow.endUse) { // 说明当前节点已经搜集过敏感词了，无需再次搜集
                        break;
                    }
                    // 不同的需求，可修改一下代码
                    if(follow.end != null) {
                        ans.add(follow.end);
                        follow.endUse = true;
                    }
                    follow = follow.fail;
                }
            }
            return ans;
        }
    }
}
