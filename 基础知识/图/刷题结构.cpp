#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
#include<map>
#include<string>
#include<queue>
#include <sstream>
#include <unordered_set>
#include <set>
using namespace std;

//ͼ�Ĵ�ṹ
public class Graph	{
	public HashMap<Integer,Node> nodes;   //�㼯
	public HashSet<Edge> edges;           //�߼�
	
	public Graph(){
		nodes = new HashMap<>();
		edges = new HashSet<>();
	}
}

//��ṹ
class Node {
public:
	int value; //valueֵ
	int in; //���
	int out; //����
	Vector<Node> nexts; //ֱ���ھӣ��ӵ�ǰnode��������ӵ��ھ�
	Vector<Edge> edges; //�ӵ�ǰnode������ı߶�����edges��
	
	Node(int value){
		this.value = value;
		this.in = 0;
		this.out = 0;
		this.nexts = new ArrayList<>();
		this.edges = new ArrayList<>();
	}
}

//�߽ṹ
public class Edges {
	public int value;
	public Node from;
	public Node to;

	public 	Edge(int weight, Node from, Node to){
		this.value = weight;
		this.from = from;
		this.to = to;
	}
}

//ת���ӿ����ӣ���һ����ά��������ʾͼ����ʽΪ: Ȩֵ = matrix[0][0]  from�� = matrix[0][1]  to�� = matrix[0][2]
public class Graph createGraph(Integer[][] matrix){
	Graph graph = new Graph();
	for(int i = 0; i < matrix.length; i++){
		//�ȴ������л��form�㡢to�㡢Ȩֵ��ֵ
		Integer weight = matrix[i][0];
		Integer from = matrix[i][1];
		Integer to = matrix[i][2];

		//��from���to��ӵ�ͼ��
		if(!graph.nodes.containskey(from)){
			graph.nodes.put(from,new Node(from));
		}
		if(!graph.nodes.containsket(from)){
			graph.nodes.put(to,new Node(to));
		}
		//��ȡfrom���to��
		Node fromNode = graph.nodes.get(from);
		Node toNode = graph.nodes.get(to);
		//form�㡢to���Ȩ�����һ����
		Edge newEdge = new Edge(weight,fromNode,toNode);
		//from����ڽӵ㼯����to��
		fromNode.nexts.add(toNode);
		//from����ȼ�һ
		formNode.out++;
		//to����ȼ�һ
		toNode.in++;
		//�������߼���form�����ڵı߼���
		formNode.edges.add(newEdge);
		//�������߼���ͼ�ı߼���
		graph.edges.add(newEdge);
	}
	return graph;
}
		
