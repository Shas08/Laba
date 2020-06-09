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
    int count=0;//будем считать кол-во символов в тексте в общем, чтобы высчитывать вероятность символов
    ifstream f("text.txt", ios::out | ios::binary);
    ofstream g("code.txt", ios::out | ios::binary);
    map <char, int> m;//создаём мапу с символами и сооветствующими кол-вами повторений
    map <char, int> ::iterator ii;
    list<Range> L;
    while(!f.eof())
    {
        char c=f.get();
        m[c]++;
        count++;
    }
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
    int ik=0;
    for (ii = m.begin(); ii != m.end(); ii++) //проходим по всей мапе и считаем сколько символов будет кодироваться(разных)
	{		
		if (ii->second != 0) ik += 40;	  //добавляем к таблице 40 символов (сумма бит char и int)		
    }
	g.write((char*)(&ik), sizeof(ik));										//записываем информацию о разновидностях символов и их частот
	for (int k=0; k<256; k++) 
	{
		if (m[char(k)]>0) 
		{
			char c=char(k);
			g.write((char*)(&c), sizeof(c));									//забивка символа
			g.write((char*)(&m[char(k)]), sizeof(m[char(k)]));					//забивка значения символа
		}
	}
    f.clear();//после чтения за концом файла, поток перейдет в ошибочное состояние, clear сбрасывает ошибки
	f.seekg(0);//вернем позицию чтения в начало файла
    int l=0, h=65535, i=0, delitel=L.back().rb; 
    int F_q=(h+1)/4, Half=F_q*2, T_q=F_q*3, bits_to_follow=0;
    char tx=0;
    count=0;
    while(!f.eof())
    {
        char c=f.get(); i++;
        for(it=L.begin(); it!=L.end(); it++)
        {
            if(c==it->s) break;
        }
        if(c!=it->s)
        {
            cout<<"Error!"<<endl;
            break;
        }
        int l2=l;
        l=l+it->lb*(h-l+1)/delitel;
        h=l2+it->rb*(h-l2+1)/delitel-1;
        for(;;)
        {
            if(h<Half)
            {
                count++;
                if(count==8)
                    {
                        count = 0;
				        g << tx;
				        tx = 0;
                    }
                for(; bits_to_follow>0; bits_to_follow--)
                {
                    tx=tx | (1<<(7-count));
                    count++;
                    if(count==8)
                    {
                        count = 0;
				        g << tx;
				        tx = 0;
                    }
                }
            }
            else if(l>=Half)
            {
                tx=tx | (1<<(7-count));
                count++;
                if(count==8)
                    {
                        count = 0;
				        g << tx;
				        tx = 0;
                    }
                for(; bits_to_follow>0; bits_to_follow--)
                {
                    count++;
                    if(count==8)
                    {
                        count = 0; 
				        g << tx;
				        tx = 0;
                    }
                }
                l-=Half;
                h-=Half;
            }
            else if((l>=F_q) && (h<T_q))
            {
                bits_to_follow++;
                l-=F_q;
                h-=F_q;
            }
            else break;
            l+=l;
            h+=h+1;
        }
    }
    g<<tx;
    f.close();
    g.close();
    return 0;
}