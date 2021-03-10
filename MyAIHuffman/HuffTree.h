#pragma once
#include <fstream>
#include<vector>
#include<map>
#include<sstream>
using std::ifstream;
using std::ofstream;
using std::endl;

struct Node {
	int weight;
	int num;
	Node* p_left;
	Node* p_right;
	Node* p_parent;
	Node(Node* p_left_, Node* p_right_, Node* p_parent_) : p_left(p_left_), p_right(p_right_), p_parent(p_parent_) {};
};

class BinaryTree
{
public:

	enum Brother { LeftChild, RightChild };
	BinaryTree(int num = 0, int weight = 0);
	~BinaryTree();
	bool swap(Node* p_nodeA, Node* p_nodeB);
	bool addNode(Node* p_parent, Node* p_child, Brother brotherState);
	Node* findNode(Node *p);
	void deleteNode(Node *p_node);
	Node* getRoot() { return p_root; }
	bool setNodeNum(Node* p_node, int num);
	Brother getBrotherState(Node *p_node);
	bool isAncestor(Node* p_nodeChild, Node* p_nodeAncestor);
private:
	Node *p_root;

};


class HuffmanTree
{
public:

	HuffmanTree();
	~HuffmanTree();
	//bool ReadFile(char * filename);

	bool encode(char * out_filename, char * out_Source_filename);
	bool ReadFile(char * code_filename, char * SourceCode_filename);

	//bool encode(char * out_filename);
private:
	void weightAdd(Node* p_node);

	static int sum;
	BinaryTree tree;

	//�洢�Ѵ����ַ��Ĺ���������Ľṹ
	struct charMap {
		char key;
		std::string value;
		Node* p;
	};
	std::string getHuffmanCode(Node *p);
	Node * findLarge(Node *);

	//һ���洢��Щ�ַ��Ѿ����������еĻ���
	std::vector<charMap> buffers;
	std::map<char,std::string>sourcecodes;//���� �ַ� �ɱ������ַ�

	ifstream is, isSource;
	ofstream os,osNsource;
};