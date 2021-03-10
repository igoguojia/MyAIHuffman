#include "HuffTree.h"
#include <iostream>
#include<queue>
#include<stack>
#include<string>
using std::cout;
using namespace std;
int HuffmanTree::sum = 1;


//二叉树成员函数实现
BinaryTree::BinaryTree(int num, int weight)
{
	p_root = new Node(nullptr, nullptr, nullptr);
	p_root->num = num;
	p_root->weight = weight;
}

BinaryTree::~BinaryTree()
{
	deleteNode(p_root);
}

bool BinaryTree::swap(Node * p_nodeA, Node * p_nodeB)
{
	if (p_nodeA == nullptr || p_nodeB == nullptr || p_nodeA == p_nodeB)
		return false;
	Node *pTemp;
	if (getBrotherState(p_nodeA) == LeftChild) {
		if (getBrotherState(p_nodeB) == LeftChild) {
			pTemp = p_nodeA->p_parent->p_left;
			p_nodeA->p_parent->p_left = p_nodeB->p_parent->p_left;
			p_nodeB->p_parent->p_left = pTemp;
		}
		else {
			pTemp = p_nodeA->p_parent->p_left;
			p_nodeA->p_parent->p_left = p_nodeB->p_parent->p_right;
			p_nodeB->p_parent->p_right = pTemp;
		}
	}
	else {
		if (getBrotherState(p_nodeB) == LeftChild) {
			pTemp = p_nodeA->p_parent->p_right;
			p_nodeA->p_parent->p_right = p_nodeB->p_parent->p_left;
			p_nodeB->p_parent->p_left = pTemp;
		}
		else {
			pTemp = p_nodeA->p_parent->p_right;
			p_nodeA->p_parent->p_right = p_nodeB->p_parent->p_right;
			p_nodeB->p_parent->p_right = pTemp;

		}
	}
	pTemp = p_nodeA->p_parent;
	p_nodeA->p_parent = p_nodeB->p_parent;
	p_nodeB->p_parent = pTemp;
	return true;

}

bool BinaryTree::addNode(Node * p_parent, Node * p_child, Brother brotherState)
{
	if (p_parent == nullptr || p_child == nullptr)
		return false;
	if (brotherState == LeftChild) {
		if (p_parent->p_left != nullptr) {
			std::cout << "error:left child exist!" << std::endl;
			return false;
		}
		p_parent->p_left = p_child;
	}
	else if (brotherState == RightChild) {
		if (p_parent->p_right != nullptr) {
			std::cout << "error:right child exist!" << std::endl;
			return false;
		}
		p_parent->p_right = p_child;
	}
	else {
		std::cout << "error:brotherState is wrong!" << std::endl;
		return false;
	}
	p_child->p_parent = p_parent;
	return true;
}

Node * BinaryTree::findNode(Node *p)
{
	Node *p_node = p_root;
	std::queue<Node *> queue;
	queue.push(p_node);
	while (!queue.empty()) {
		p_node = queue.front();
		if (p_node == p) {
			return p_node;
		}
		queue.pop();
		if (p_node->p_left != nullptr) {
			queue.push(p_node->p_left);
		}
		if (p_node->p_right != nullptr) {
			queue.push(p_node->p_right);
		}
	}
	return nullptr;
}

bool BinaryTree::setNodeNum(Node* p_node, int num)
{
	if (p_node == nullptr)
		return false;
	else {
		p_node->num = num;
		return true;
	}
}

bool BinaryTree::isAncestor(Node * p_nodeChild, Node * p_nodeAncestor)
{
	while (p_nodeChild != p_root) {
		if (p_nodeChild == p_nodeAncestor) {
			return true;
		}
		else {
			p_nodeChild = p_nodeChild->p_parent;
		}
	}
	return false;
}

void BinaryTree::deleteNode(Node *p_node)
{
	if (p_node->p_left != nullptr) {
		deleteNode(p_node->p_left);
	}
	if (p_node->p_right != nullptr) {
		deleteNode(p_node->p_right);
	}
	delete p_node;
}

BinaryTree::Brother BinaryTree::getBrotherState(Node *p_node)
{
	if (p_node->p_parent->p_left == p_node) {
		return LeftChild;
	}
	else {
		return RightChild;
	}
}



//哈夫曼树成员函数实现
HuffmanTree::HuffmanTree() :tree(0, 0)
{
}

HuffmanTree::~HuffmanTree()
{
	os.close();
	is.close();
	isSource.close();
	osNsource.close();
}

bool HuffmanTree::ReadFile(char * filename, char * SourceCode_filename)
{
	is.open(filename, std::ios_base::in);
	if (!is.is_open()) {
		cout << "error: " << filename << " is not exist!" << endl;
		return false;
	}
	isSource.open(SourceCode_filename, std::ios_base::in);
	if (!isSource.is_open()) {
		cout << "error: " << SourceCode_filename << " is not exist!" << endl;
		return false;
	}
	string line, value, keystr;
	char key;
	while (getline(isSource, line))
	{
		if (line[0] == '/'&&line[1] == '/')
			continue;
		stringstream lines(line);
		getline(lines, value, ';');
		getline(lines, keystr);
		key = keystr.c_str()[0];
		//key.length() > sourcecode_length ?
		//sourcecode_length = key.length() : sourcecode_length = sourcecode_length;

		sourcecodes.insert(pair<char, string>(key, value));

	}
	return true;
}


//获取节点的哈夫曼编码
std::string HuffmanTree::getHuffmanCode(Node *p_n)
{
	std::string huffmanCode = "";
	std::stack<Node *> stack;
	std::deque<char> code;

	//逆向后推，当为左孩子的时候则置0，当为右孩子的时候则置1。
	while (p_n != tree.getRoot()) {
		if (tree.getBrotherState(p_n) == tree.LeftChild) {
			code.push_back('0');
		}
		else {
			code.push_back('1');
		}
		p_n = p_n->p_parent;
	}

	while (!code.empty()) {
		huffmanCode += code.back();
		code.pop_back();
	}
	return huffmanCode;
}

//找到所在块中最大节点编号的节点
Node * HuffmanTree::findLarge(Node *p_node)
{
	std::stack<Node *> stack;
	Node *p = tree.getRoot();
	Node *large = p;
	while (p || !stack.empty()) {
		if (p != nullptr) {
			stack.push(p);
			if (p->weight == p_node->weight) {
				//如果large不在同权重下，则置large为p
				if (large->weight != p->weight) {
					large = p;
				}
				//同权重下的large比p小，则置large为p
				else if (large->num > p->num) {
					large = p;
				}
			}
			p = p->p_left;
		}
		else {
			p = stack.top();
			stack.pop();
			p = p->p_right;
		}
	}
	//large不可能是根节点，当large为根节点时返回原节点
	if (large == tree.getRoot()) {
		return p_node;
	}
	return large;
}


//编码函数
bool HuffmanTree::encode(char * out_filename, char * out_Source_filename)
{
	//确认文件存在
	if (!is.is_open()) {
		cout << "error: no file read!" << endl;
		return false;
	}
	if (!isSource.is_open()) {
		cout << "error: no Source_file read!" << endl;
		return false;
	}
	os.open(out_filename, ios::out | ios::trunc);
	if (!os.is_open()) {
		cout << "error: can not open file to write!" << endl;
	}	
	osNsource.open(out_Source_filename, ios::out | ios::trunc);
	if (!osNsource.is_open()) {
		cout << "error: can not open file to write!" << endl;
	}

	//读取字符，设置nyt节点为根节点
	char cbuffer;
	Node *nyt = tree.getRoot();
	while (!is.eof())
	{
		cbuffer = is.get();
		if (cbuffer == EOF) break;
		bool exist = false;
		std::string code;
		auto existNode = buffers.begin();	//buffers的一个迭代器，当cbuffer存在于树中的时候，existNode指向该节点
		for (existNode; existNode != buffers.end(); existNode++) 
		{
			if (existNode->key == cbuffer) 
			{
				code = existNode->value;
				for (int i = 0; '\0' != code[i]; i++) 
				{
					os << code[i];
				}
				exist = true;
				cout << cbuffer << " 在树中存在，编码为： "<< existNode->value << endl;
				break;
			}
		}
		if (exist) {
			Node *root = existNode->p;
			weightAdd(root);
		}
		else {
			//当字符不存在树中时，则新建子树，并替代原nyt节点
			Node *c = new Node(nullptr, nullptr, nyt);
			c->num = sum++;
			c->weight = 1;

			Node *NYT = new Node(nullptr, nullptr, nyt);
			NYT->num = sum++;
			NYT->weight = 0;

			tree.addNode(nyt, NYT, BinaryTree::LeftChild);
			tree.addNode(nyt, c, BinaryTree::RightChild);
			nyt->weight = 1;

			//将编码值设为nyt+原字符值
			//code = sourcecodes.find(cbuffer)->second;
			code = getHuffmanCode(nyt).append(sourcecodes.find(cbuffer)->second);
			for (int i = 0; '\0' != code[i]; i++) {
				os << code[i];
				//osSource << code[i];
			}
			//os << cbuffer;
			osNsource << code;
			osNsource << ';';
			osNsource << cbuffer;
			osNsource << endl;
			//cout << cbuffer << "首次出现，设定编码为：" << code << cbuffer << endl;
			cout << cbuffer << "首次出现，new和编码为：" << code << endl;

			//将新的字符放进buffers中
			charMap* new_cm = new charMap();
			new_cm->key = cbuffer;
			new_cm->p = nyt->p_right;
			new_cm->value = getHuffmanCode(nyt->p_right);
			buffers.push_back(*new_cm);

			//依次增加权重
			Node *root = nyt->p_parent;
			weightAdd(root);

			//设置新的nyt节点为原nyt节点的左孩子
			nyt = nyt->p_left;
		}

	}
	return false;
}
//从当前节点往上依次权重值加一，但是加一前先判断是否符合兄弟属性
void HuffmanTree::weightAdd(Node * p_node)
{
	while (p_node != nullptr) {
		Node* large = findLarge(p_node);
		if (large != p_node && !tree.isAncestor(p_node, large)) {
			cout << "即将为节点" << p_node->num << "加一,但是同块最大节点为：" << large->num << ",权重值为：" << p_node->weight << endl;
			tree.swap(large, p_node);
			int temp;
			temp = large->num;
			large->num = p_node->num;
			p_node->num = temp;
			for (auto iterator = buffers.begin(); iterator != buffers.end(); iterator++) {
				iterator->value = getHuffmanCode(iterator->p);
			}
		}
		p_node->weight++;
		cout << "节点" << p_node->num << "权重值加1" << "为：" << p_node->weight << endl;
		p_node = p_node->p_parent;
	}
}

int main()
{
	HuffmanTree huff;
	huff.ReadFile("..\\CodeTxt\\words.txt", "..\\CodeTxt\\SourceEncode.txt");
	huff.encode("..\\CodeTxt\\codes_output.txt","..\\CodeTxt\\New_SourceEncode.txt");
	return 0;
}