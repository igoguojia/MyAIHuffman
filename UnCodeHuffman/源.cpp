#include "��ͷ.h"
#include <iostream>
#include<queue>
#include<stack>
#include<string>
using std::cout;
using namespace std;
int HuffmanTree::sum = 1;
int HuffmanTree::sourcecode_length = 0;

//��������Ա����ʵ��
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



//����������Ա����ʵ��
HuffmanTree::HuffmanTree() :tree(0, 0)
{
}

HuffmanTree::~HuffmanTree()
{
	os.close();
	is.close();
	isSource.close();
}

bool HuffmanTree::ReadFile(char * code_filename, char * SourceCode_filename)
{
	is.open(code_filename, std::ios_base::in);
	if (!is.is_open()) {
		cout << "error: " << code_filename << " is not exist!" << endl;
		return false;
	}	
	isSource.open(SourceCode_filename, std::ios_base::in);
	if (!isSource.is_open()) {
		cout << "error: " << SourceCode_filename << " is not exist!" << endl;
		return false;
	}
	string line, key, valuestr;
	char value;
	while (getline(isSource,line))
	{
		if (line[0] == '/'&&line[1] == '/')
			continue;
		stringstream lines(line);
		getline(lines, key, ';');
		getline(lines, valuestr);
		value = valuestr.c_str()[0];
		key.length() > sourcecode_length ? 
			sourcecode_length = key.length() : sourcecode_length = sourcecode_length;

		sourcecodes.insert(pair<string,char>(key,value ));
		
	}
	return true;
}


//��ȡ�ڵ�Ĺ���������
std::string HuffmanTree::getHuffmanCode(Node *p_n)
{
	std::string huffmanCode = "";
	std::stack<Node *> stack;
	std::deque<char> code;

	//������ƣ���Ϊ���ӵ�ʱ������0����Ϊ�Һ��ӵ�ʱ������1��
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

//�ҵ����ڿ������ڵ��ŵĽڵ�
Node * HuffmanTree::findLarge(Node *p_node)
{
	std::stack<Node *> stack;
	Node *p = tree.getRoot();
	Node *large = p;
	while (p || !stack.empty()) {
		if (p != nullptr) {
			stack.push(p);
			if (p->weight == p_node->weight) {
				//���large����ͬȨ���£�����largeΪp
				if (large->weight != p->weight) {
					large = p;
				}
				//ͬȨ���µ�large��pС������largeΪp
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
	//large�������Ǹ��ڵ㣬��largeΪ���ڵ�ʱ����ԭ�ڵ�
	if (large == tree.getRoot()) {
		return p_node;
	}
	return large;
}


//���뺯��
bool HuffmanTree::uncode(char * out_filename)
{
	//ȷ���ļ�����
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
	//��ȡ�ַ�������nyt�ڵ�Ϊ���ڵ�
	char cbuffer,temp;
	std::string code;//����������
	//string nytcode;
	//bool skipnyt=false;
	Node *nyt = tree.getRoot();
	bool ableF_s = true;//�ܷ��ڳ�ʼ���в���
	/*while (code.length() <= sourcecode_length)
	{
		auto scode_find = sourcecodes.find(code);
		if (scode_find == sourcecodes.end())
		{
			continue;
		}
		else
		{
			cbuffer = scode_find->second;
			os << cbuffer;
			//s_exist = true;
			cout << code << "�ڳ�ʼ�����д��ڣ�Դ��Ϊ��" << cbuffer << endl;
			code.clear();
		}
	}*/
	while (true)
	{
		//bool exist = false;
		//bool s_exist = false;
		temp = is.get();
		if (temp == EOF&&sourcecodes.empty()) 
			break;
		//std::string code;
		code.push_back(temp);
		//nytcode = getHuffmanCode(nyt);
		/*if (nytcode == code&&!skipnyt)
		{
			code.clear();
			skipnyt = true;
			continue;
		}*/
		if (!sourcecodes.empty())
		{
			if (ableF_s)
			{
				auto scode_find = sourcecodes.find(code);
				if (scode_find == sourcecodes.end())
				{
					if (code.length() == sourcecode_length)
					{
						is.seekg(-sourcecode_length, ios::cur);//��Դ�����Ҳ���������������ƫ�ơ�
						code.clear();//���
						ableF_s = false;

					}
					//code.push_back(is.get());//�õ�ƫ�ƺ�ĵ�ǰ�ֽڡ�
					continue;
				}
				else
				{
					cbuffer = scode_find->second;
					os << cbuffer;
					cout << code << "�ڳ�ʼ�����д��ڣ�Դ��Ϊ��" << cbuffer << endl;
					sourcecodes.erase(code);//ɾ���Ѿ����ֵ��ַ��ĳ�ʼ���롣

					code.clear();

					//s_exist = true;
					//ableF_s = true;

					//���ַ�����������ʱ�����½������������ԭnyt�ڵ�
					Node *c = new Node(nullptr, nullptr, nyt);
					c->num = sum++;
					c->weight = 1;

					Node *NYT = new Node(nullptr, nullptr, nyt);
					NYT->num = sum++;
					NYT->weight = 0;

					tree.addNode(nyt, NYT, BinaryTree::LeftChild);//���nyt��������NYT
					tree.addNode(nyt, c, BinaryTree::RightChild);
					nyt->weight = 1;

					//���µ��ַ��Ž�buffers��
					charMap* new_cm = new charMap();
					new_cm->key = cbuffer;
					new_cm->p = nyt->p_right;
					new_cm->value = getHuffmanCode(nyt->p_right);
					buffers.push_back(*new_cm);

					//��������Ȩ��
					Node *root = nyt->p_parent;
					weightAdd(root);

					//�����µ�nyt�ڵ�Ϊԭnyt�ڵ������
					nyt = nyt->p_left;


				}
			}
		}
		else if(ableF_s)
		{
			ableF_s = false;
		}
		if (!ableF_s)
		{
			auto existNode = buffers.begin();	//buffers��һ������������cbuffer���������е�ʱ��existNodeָ��ýڵ�
			for (existNode; existNode != buffers.end(); existNode++)
			{
				if (existNode->value == code)
				{
					cbuffer = existNode->key;
					//for (int i = 0; '\0' != code[i]; i++)
					//{
					//	os << code[i];
					//}
					os << cbuffer;
					//exist = true;
					cout << code << " �����д��ڣ�Դ��Ϊ�� " << cbuffer << endl;
					code.clear();//���

					Node *root = existNode->p;
					weightAdd(root);

					ableF_s = true;
					break;
				}
			}
		}

		//auto scode_find = sourcecodes.begin();	//buffers��һ������������cbuffer���������е�ʱ��existNodeָ��ýڵ�
		//for (scode_find; scode_find != sourcecodes.end(); scode_find++)
		//{
		//	if (scode_find->first == code)
		//	{
		//		cbuffer = scode_find->second;
		//		os << cbuffer;
		//		s_exist = true;
		//		cout << code << "�ڳ�ʼ�����д��ڣ�Դ��Ϊ��" << cbuffer << endl;
		//		code.clear();
		//		//skipnyt = false;
		//		break;
		//	}
		//}


		/*if (exist) {
			Node *root = existNode->p;
			weightAdd(root);
		}
		else if(s_exist)
		{
			//���ַ�����������ʱ�����½������������ԭnyt�ڵ�
			Node *c = new Node(nullptr, nullptr, nyt);
			c->num = sum++;
			c->weight = 1;

			Node *NYT = new Node(nullptr, nullptr, nyt);
			NYT->num = sum++;
			NYT->weight = 0;

			tree.addNode(nyt, NYT, BinaryTree::LeftChild);//���nyt��������NYT
			tree.addNode(nyt, c, BinaryTree::RightChild);
			nyt->weight = 1;

			////������ֵ��Ϊnyt+ԭ�ַ�ֵ
			//code = getHuffmanCode(nyt);
			//for (int i = 0; '\0' != code[i]; i++) {
			//	os << code[i];
			//}
			//os << cbuffer;
			//cout << cbuffer << "�״γ��֣��趨����Ϊ��" << code << cbuffer << endl;

			//���µ��ַ��Ž�buffers��
			charMap* new_cm = new charMap();
			new_cm->key = cbuffer;
			new_cm->p = nyt->p_right;
			new_cm->value = getHuffmanCode(nyt->p_right);
			buffers.push_back(*new_cm);

			//��������Ȩ��
			Node *root = nyt->p_parent;
			weightAdd(root);

			//�����µ�nyt�ڵ�Ϊԭnyt�ڵ������
			nyt = nyt->p_left;
		}*/

	}
	return false;
}
//�ӵ�ǰ�ڵ���������Ȩ��ֵ��һ�����Ǽ�һǰ���ж��Ƿ�����ֵ�����
void HuffmanTree::weightAdd(Node * p_node)
{
	while (p_node != nullptr) {
		Node* large = findLarge(p_node);
		if (large != p_node && !tree.isAncestor(p_node, large)) {
			cout << "����Ϊ�ڵ�" << p_node->num << "��һ,����ͬ�����ڵ�Ϊ��" << large->num << ",Ȩ��ֵΪ��" << p_node->weight << endl;
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
		cout << "�ڵ�" << p_node->num << "Ȩ��ֵ��1" << "Ϊ��" << p_node->weight << endl;
		p_node = p_node->p_parent;
	}
}

int main()
{
	HuffmanTree huff;
	huff.ReadFile("..\\CodeTxt\\codes_output.txt", "..\\CodeTxt\\New_SourceEncode.txt");
	huff.uncode("..\\UnCodeTxt\\Uncode_words.txt");
	return 0;
}