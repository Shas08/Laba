#include <iostream>
#include <cstdlib>
using namespace std;

typedef unsigned short int Base;         //2 байта
typedef unsigned int D_Base;
#define Base_size sizeof(Base)*8    //16 бит
class BigNumb
{
    Base *coef;                     //массив коэффициентов
    int len, lenMax;               //len-число коэффициентов, lenMax-длина выделеной памяти
    public:
        BigNumb();          //конструктор 1 (по умолчанию) +
        BigNumb(int t, int lM);        //конструктор 2,3 (с 2 параметрами) +
        BigNumb(BigNumb &);         //конструктор копирования  +
        BigNumb operator = (const BigNumb&);            //присвоение  +
        bool operator > (const BigNumb&);            //+
        bool operator < (const BigNumb&);            //+
        bool operator >= (const BigNumb&);           //+
        bool operator <= (const BigNumb&);           //+
        bool operator == (const BigNumb&);           //+
        bool operator != (const BigNumb&);           //+
        BigNumb operator +(const BigNumb&);          //+
        BigNumb operator +=(const BigNumb&);         //+
        BigNumb operator -(const BigNumb&);          //+
        BigNumb operator -=(const BigNumb&);
        void cmp (const BigNumb&);
        void Len();                 //ф-ия вычисления кол-ва коэф +
        ~BigNumb();                 //деструктор +
        friend ostream& operator << (ostream &t, BigNumb &b)    //вывод +
        {
            for (int i=b.len-1; i >= 0; i--)
            {
                cout.width(Base_size/4);
                cout.fill('0');
                cout<<hex<<b.coef[i];
            }
            return t;
        } 
        friend istream& operator >> (istream &n, BigNumb &b)    //ввод +
        {
            int nofcs=Base_size/4;
            int k;
            char* b1=new char[80];
            cin>>b1;                                            //15ab482
            int str=strlen(b1);
            int l = b.lenMax = b.len = (str + nofcs - 1) / nofcs; 
            b.coef=new Base[l];
            unsigned int x;
            for(int i=0; i<l; i++) b.coef[i]=0;
            int j=str-1;
            for(int i=0; i<l; i++)
            {
                k=0;
                for(; k<nofcs && j>=0; j--)
                {
                    if('0'<=b1[j] && b1[j]<='9') x=b1[j]-'0';
                    else if('a'<=b1[j] && b1[j]<='f') x=b1[j]-'a'+10;
                    else if('A'<=b1[j] && b1[j]<='F') x=b1[j]-'A'+10;
                    else cout<<"Ошибка!";
                    x=x<<k*4;
                    b.coef[i]|=x;
                    k++;
                }
            }
            b.Len();
            return n;
        }
}; 

BigNumb::BigNumb()
{
    lenMax=1;
    len=lenMax;
    coef=new Base;
    coef[0]=0;
}

BigNumb::BigNumb(int t, int lM)
{
    len=lenMax=lM;
    coef=new Base[len];
    if(t==0)
    {
        len=1;
        for(int i=0; i<lenMax; i++) coef[i]=0;
    }
    else if(t==1)
    {
        for(int i=0; i<lenMax; i++) coef[i]=rand();
        Len();
    }
    else cout<<"Ошибка: неверные данные"<<endl;
}

BigNumb::BigNumb(BigNumb &b)
{
    len=b.len;
    lenMax=b.lenMax;
    coef=new Base[len];
    for(int i=0; i<lenMax; i++) coef[i]=b.coef[i];
}

void BigNumb::Len()
{
    for(int i=len-1; coef[i]==0 && i>0; i--) len--;
}

bool BigNumb::operator > (const BigNumb& b1)
{
    if (this==&b1) return false;
    if(len>b1.len) return true;
    if(len<b1.len) return false;
    for (int i=len-1; i >= 0; i--)
    {
        if(coef[i]>b1.coef[i]) return true;
        if(coef[i]<b1.coef[i]) return false;
    }
    return false;
}

bool BigNumb::operator >=(const BigNumb &b1)
{
    if (this==&b1) return true;
    if(len>b1.len) return true;
    if(len<b1.len) return false;
    for (int i=len-1; i >= 0; i--) 
    {
        if(coef[i]<b1.coef[i]) return false;
        if(coef[i]>b1.coef[i]) return true;
    }
    return true; 
}

bool BigNumb::operator ==(const BigNumb &b1)
{
    if (this==&b1) return true;
    if(len>b1.len) return false;
    if(len<b1.len) return false;
    for (int i=len-1; i >= 0; i--) if((coef[i]<b1.coef[i]) || (coef[i]>b1.coef[i])) return false;
    return true; 
}

bool BigNumb::operator !=(const BigNumb &b1)
{
    if (this==&b1) return false;
    if(len>b1.len) return true;
    if(len<b1.len) return true;
    for (int i=len-1; i >= 0; i--) if((coef[i]<b1.coef[i]) || (coef[i]>b1.coef[i])) return true;
    return false; 
}

bool BigNumb::operator < (const BigNumb &b1)
{
    if (this==&b1) return false;
    if(len>b1.len) return false;
    if(len<b1.len) return true;
    for (int i=len-1; i >= 0; i--)
    {
        if(coef[i]>b1.coef[i]) return false;
        if(coef[i]<b1.coef[i]) return true;
    }
    return false;
}

bool BigNumb::operator <=(const BigNumb &b1)
{
    if (this==&b1) return true;
    if(len>b1.len) return false;
    if(len<b1.len) return true;
    for (int i=len-1; i >= 0; i--)
    {
        if(coef[i]>b1.coef[i]) return false;
        if(coef[i]<b1.coef[i]) return true;
    }
    return true; 
}



void BigNumb::cmp(const BigNumb &b1)
{
    BigNumb b=*this;
    if (b>b1) cout<<"Первое число больше второго"<<endl;
    else if(b<b1) cout<<"Первое число меньше второго"<<endl;
    else cout<<"Они равны"<<endl;
}

BigNumb BigNumb::operator = (const BigNumb &b1)
{
    if(this!=&b1)
    {
        this->len=b1.len;
        this->lenMax=b1.lenMax;
        delete[](coef);
        this->coef=new Base[this->len];
        for(int i=0; i<this->lenMax; i++) this->coef[i]=b1.coef[i];
    }
    return *this;
}

BigNumb BigNumb::operator +(const BigNumb &b1)
{
    int l=min(len, b1.len), i;
    D_Base x=0;
    BigNumb sum(0, max(len, b1.len)+1);
    for(i=0; i<l; i++)
    {
        sum.coef[i]=x+=coef[i]+b1.coef[i];
        x=x>>Base_size;
    }
    for(; i<len; i++)
    {
        sum.coef[i]=x+=coef[i];
        x=x>>Base_size;
    }
    for(; i<b1.len; i++)
    {
        sum.coef[i]=x+=b1.coef[i];
        x=x>>Base_size;
    }
    sum.len=max(len, b1.len)+1;
    sum.Len();
    return sum;
}

BigNumb BigNumb::operator -(const BigNumb &b1)
{
    int l=min(len, b1.len), i;
    D_Base x=0;
    BigNumb sub(0, len);
    for(i=0; i<l; i++)
    {
        x=1<<Base_size;
        x+=coef[i];
        sub.coef[i]=x-=b1.coef[i];
        x=x>>Base_size;
        if(x==0) coef[i+1]-=1;
    }
    for(; i<len; i++) sub.coef[i]=coef[i];
    sub.len=len;
    sub.Len();
    cout<<hex<<sub.coef[2]<<endl;
    return sub;
}

BigNumb BigNumb::operator -=(const BigNumb &b1)
{
    *this=*this-b1;
    return *this;
}

BigNumb BigNumb::operator +=(const BigNumb &b1)
{
    *this=*this+b1;
    return *this;
}

BigNumb::~BigNumb()
{
    if(coef!=NULL)
    {
        delete[](coef);
        coef=NULL;
    }
}

int main()
{
    int k;
    srand(unsigned(time(0)));
    BigNumb b, b1, b2;
    cin>>b;
    cin>>b1;
    b-=b1;
    cout<<b<<endl;
    return 0;
}