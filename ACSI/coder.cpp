#include <iostream> 
#include <fstream>
#include <vector>
#include <map>
#include <list>

using namespace std;

class Uzel
{
public:
	int key;//кол-во повторений
	char s;//символ
	Uzel *left, *right;//ссылки на левого и правого потомков
	Uzel() { left = NULL; right = NULL; }//констрктор по умолчанию
	Uzel(Uzel *l, Uzel *r)//конструктор с известными потомками
	{
		left = l;
		right = r;
		key = l->key + r->key;//при слиянии 2-ух символов, суммируется их кол-во повторений
	}

};

struct Sort//сравнивает 2 узла, если sort=1 то левый меньше правого и если 0, то наоборот
{
	bool operator() (const Uzel* l, const Uzel* r)
	{
		return l->key < r->key;
	}
};

vector<bool> code;//создаётся булев вектор для кодирования символа (для сжатия)
map<char, vector<bool> > buf;//создаём мап из символа и соответствующего ему в дальнейшем кода

void TreeHaf(Uzel *root)
{
	if (root->left != NULL)//проверяем наличие левого потомка
	{
		code.push_back(0);//когда идём влево добавляем в код символа 0
		TreeHaf(root->left); //рекурсивно идём дальше
	}
	if (root->right != NULL) //проверяем наличие правого потомка
	{
		code.push_back(1);//когда идём вправо добавляем в код символа 1
		TreeHaf(root->right);//рекурсивно идём дальше
	}
	if (root->right == NULL && root->left == NULL)//если у узла нет потомков->узел содержит символ
	{
		buf[root->s] = code;//в мап соответствующего символа передаём получившийся код
	}
	if (!code.empty())//если вектор кода не пуст
		code.pop_back();//удаляем последний элемент(чтобы исключить повторения кода с началом других кодов)
}

int main()
{
	ifstream f("text.txt", ios::out | ios::binary);//открываем файл для чтения, который нужно закодировать
	ofstream g("code.txt", ios::out | ios::binary);//открываем файл для записи закодированного текста
	map<char, int> m;//создаём, мап символ и соответствующее число повторений
	map<char, int> ::iterator ii;
	while (!f.eof()) //пока файл не закончился
	{
		char c = f.get();//считываем символ из файла в переменную с
		m[c]++;//число повторений этого символа увеличиваем
	}
	list<Uzel*> L;//создаём список узлов
	for (ii = m.begin(); ii != m.end(); ii++) //от начала мапы до конца
	{
		Uzel* p = new Uzel;//создаём узел
		p->s = ii->first;//в узел записываем символ соответствующий символу из мапы
		p->key = ii->second;//аналогично в этот же узел записываем число повторений этого символа 
		L.push_back(p);//заносим узел в список узлов
	}
	while (L.size() != 1) //пока не построим дерево с единственным корнем
	{
		L.sort(Sort());//сортируем список
		Uzel *Left = L.front(); //левый потомок-первый узел списка
		L.pop_front();//удаляем его из списка 
		Uzel *Right = L.front();//правый потомок-первый узел нынешнего списка 
		L.pop_front();//удаляем его из списка
		Uzel *pr = new Uzel(Left, Right);//создаём новый узел с соответствующими левым и правым потомками
		L.push_back(pr);//в конец списка добавляем получившийся узел (кол-во повторений=сумме повторений символов)
	}
	Uzel *root = L.front();//корнем является единственный оставшийся узел в списке 
	TreeHaf(root);
	int count=0;
	for (ii = m.begin(); ii != m.end(); ii++) //проходим по всей мапе и считаем сколько символов будет кодироваться(разных)
	{		
		if (ii->second != 0) count += 40;	  //добавляем к таблице 40 символов (сумма бит char и int)		
	}
	g.write((char*)(&count), sizeof(count));										//записываем информацию о разновидностях символов и их частот
	for (int i=0; i<256; i++) 
	{
		if (m[char(i)]>0) {
			char c=char(i);
			g.write((char*)(&c), sizeof(c));									//забивка символа
			g.write((char*)(&m[char(i)]), sizeof(m[char(i)]));					//забивка значения символа
		}
	}
	f.clear();//после чтения за концом файла, поток перейдет в ошибочное состояние, clear сбрасывает ошибки
	f.seekg(0);//вернем позицию чтения в начало файла
	count=0;
	char tx = 0;
	while (!f.eof())//заполняем бинарник
	{
		char c = f.get();//считываем символ из файла с текстом
		vector<bool> x = buf[c];//булев вектор символа
		for (int j = 0; j<x.size(); j++)//из булева вектора переводим в соответствующий символ
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
	f.close();//закрываем файл с текстом
	g.close();//закрываем файл с бинарником
	return 0;
}