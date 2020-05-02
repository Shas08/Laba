#include <iostream> 
#include <fstream>
#include <vector>
#include <map>
#include <list>

using namespace std;

class Uzel
{
public:
	int key;
	char s;
	Uzel *left, *right;
	Uzel() { left = NULL; right = NULL; }
	Uzel(Uzel *l, Uzel *r)
	{
		left = l;
		right = r;
		key = l->key + r->key;
	}

};

struct Sort
{
	bool operator()
		(const Uzel* l, const Uzel* r)
	{
		return l->key < r->key;
	}
};

vector<bool> code;
vector<bool> ::iterator itr;
map<char, vector<bool> > buf;
map<char, vector<bool> > ::iterator it;

void HafCode(Uzel *root) 
{
	if (root->left != NULL)
	{
		code.push_back(0);
		HafCode(root->left); 
	}
	if (root->right != NULL) 
	{
		code.push_back(1);
		HafCode(root->right);
	}
	if (root->right == NULL && root->left == NULL)
	{
		buf[root->s] = code;
		for (itr = code.begin(); itr != code.end(); itr++)
			cout << *itr;
		cout << endl;
	}
	if (!code.empty())
	code.pop_back();

}

void HafDecode(Uzel *root)
{
	ifstream F("code.txt");  
	ofstream g("output.txt");
	Uzel *p = root; 
	char byte;
	int count = 0;
	byte = F.get();
	while (!F.eof())
	{
		bool b = byte & (1 << (7 - count)); 
		if (b) 
			p = p->right;
		else
			p = p->left; 
		if (p->left == NULL && p->right == NULL)
		{
			cout << p->s;
		g << p->s; 
		p = root; 
		}
		count++;
		if (count == 8)  
		{ 
			count = 0;
			byte = F.get();  
		}
	}
	cout << endl;
	F.close();
}

void print(Uzel *root, int k = 0)
{
	if (root != NULL)
	{
		print(root->right, k + 3);
		for (int i = 0; i<k; i++)
		{
			cout << "  ";
		}
		if (root->s)
			cout << root->key << " (" << root->s << ")" << endl;
		else cout << root->key << endl;
		print(root->left, k + 3);
	}
}
int main()
{
	
	//setlocale(LC_ALL, "Russian");

	ifstream f("text.txt", ios::out | ios::binary);
	map<char, int> m;
	map<char, int> ::iterator ii;

	while (!f.eof()) 
	{
		char c = f.get();
		m[c]++;
	}


	
	for (ii = m.begin(); ii != m.end(); ii++)
		cout << ii->first << ':' << ii->second << endl; 

	list<Uzel*> L;
	list<Uzel*> ::iterator itr;
	for (ii = m.begin(); ii != m.end(); ii++) 
	{
		Uzel* p = new Uzel; 
		p->s = ii->first; 
		p->key = ii->second; 
		L.push_back(p);
	}

	
	cout << "vivod order" << endl;
	for (itr = L.begin(); itr != L.end(); itr++)
		cout << (*itr)->key << ':' << (*itr)->s << endl; 

	cout << endl;
	

	while (L.size() != 1) 
	{
		L.sort(Sort());
		Uzel *Left = L.front(); 
		L.pop_front(); 
		Uzel *Right = L.front(); 
		L.pop_front();
		Uzel *pr = new Uzel(Left, Right); 
		L.push_back(pr); 
	}

	Uzel *root = L.front(); 
	cout << "vivod dereva" << endl;
	print(root);
	cout << endl;
	HafCode(root);

	cout << "vivod coda" << endl;
	f.clear(); f.seekg(0);
	while (!f.eof())
	{
		char c = f.get();
		vector<bool> x = buf[c];
		for (int j = 0; j<x.size(); j++)
			cout << x[j];
		cout << "  ";
	}
	cout << endl;



	f.clear(); f.seekg(0);
	ofstream g("code.txt", ios::out | ios::binary);
	char tx = 0;
	int count = 0;
	while (!f.eof())
	{
		char c = f.get();
		vector<bool> x = buf[c]; 
		for (int j = 0; j<x.size(); j++)
		{
			tx = tx | x[j] << (7 - count);
			count++;
			if (count == 8)
			{ 
				count = 0; 
			g << tx;
			tx = 0;
			}
			
		}

	}

	f.close();
	g.close();

	HafDecode(root);

}