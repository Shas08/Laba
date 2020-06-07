#include <iostream> 
#include <fstream>
#include <vector>
#include <map>
#include <list>

using namespace std;

struct Range
{
    char s;
    int numb;
    int lb;
    int rb;
};

struct Sort//сравнивает 2, если Sort=1 то левый меньше правого и если 0, то наоборот
{
	bool operator() (Range l, Range r)
	{
		return l.numb > r.numb;
	}
};


int main()
{
    int count=0;
    ifstream f("code.txt", ios::out | ios::binary);//открываем файл для чтения, который нужно декодировать
	ofstream g("output.txt", ios::out | ios::binary);//открываем файл для записи декодированного текста
    int x1, x2;
	char s;
	map<char, int> m;
    map <char, int> ::iterator ii;														//мапа для таблицы кодирования
	// считывание из файла code.txt и декодирование
	f.read((char*)&x1, sizeof(x1));												//считываем первое число для расшифровки таблциы																//отнимаем значение int
	while (x1>0) 
	{
		f.read((char*)&s, sizeof(s));											//считываем символ
		f.read((char*)&x2, sizeof(x2));											//считываем его значение
		x1-=40;																//отнимаем 8 бит символа и 32 его значения
		m[s]=x2;																//строим таблицу
        count+=x2;
    }
    list<Range> L;
    for(ii=m.begin(); ii!=m.end(); ii++)
    {
        Range p;
        p.s=ii->first;
        p.numb=ii->second;
        L.push_back(p);
    }
    L.sort(Sort());
    L.begin()->rb=L.begin()->numb;
    L.begin()->lb=0;
    list<Range>::iterator it=L.begin(), i2;
    i2=it;
    it++;
    for(; it!=L.end(); it++)
    {
        it->lb=i2->rb;
        it->rb=it->lb+it->numb;
        i2++;
    }
    x1=0;
    int *l=new int[count], *h=new int[count], i=1, delitel=L.back().rb; 
    l[0]=0; h[0]=65535;
    int F_q=(h[0]+1)/4, Half=F_q*2, T_q=F_q*3;
    short int value=f.get();
    value=(value<<8) | f.get();
    while(!f.eof())
    {
        int freq=((value-l[i-1]+1)*delitel-1)/(h[i-1]-l[i-1]+1);
        for(it=L.begin(); it->rb<=freq; it++);
        l[i]=l[i-1]+(it->lb)*(h[i-1]-l[i-1]+1)/delitel;
        h[i]=l[i-1]+(it->rb)*(h[i-1]-l[i-1]+1)/delitel-1;
        for(;;)
        {
            if(h[i]<Half);
            else if(l[i]>=Half)
            {
                l[i]-=Half; h[i]-=Half; value-=Half;
            }
            else if((l[i]>=F_q) && (h[i]<T_q))
            {
                l[i]-=F_q; h[i]-=F_q; value-=F_q;
            } else break;
            l[i]+=l[i]; h[i]+=h[i]+1;
            value+=value+((f.get()<<x1)>>7);
            x1++;
            if(x1!=8) f.seekg(ios_base::cur-1);
            else x1=0;
        }
        i++;
        g<<it->s;
    }
    f.close();
    g.close();
    return 0;
}