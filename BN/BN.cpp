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
        bool operator > (const BigNumb&);            // оператор больше +
        bool operator < (const BigNumb&);            //оператор меньше +
        bool operator >= (const BigNumb&);           //оператор больше/равно +
        bool operator <= (const BigNumb&);           //оператор меньше/равно +
        bool operator == (const BigNumb&);           //оператор сравненимы +
        bool operator != (const BigNumb&);           //оператор несравнимы +
        BigNumb operator +(const BigNumb&);          //оператор сложение +
        BigNumb operator +(Base b);
        BigNumb operator +=(const BigNumb&);         //оператор += +
        BigNumb operator +=(Base c);
        BigNumb operator -(const BigNumb&);          //оператор вычитание +
        BigNumb operator -=(const BigNumb&);         //оператор -= +
        BigNumb operator *(Base);                    //оператор умножение на базу ?
        BigNumb operator *(BigNumb &b1);             //оператор умножение на большое число ?
        BigNumb input_10();                               //10-ый ввод ?
        void output_10();                                 //10-ый вывод ?
        BigNumb operator /(Base);                    //оператор деление на базу ?
        BigNumb operator /(BigNumb &b1);             //оператор деления на БЧ ?
        Base operator %(Base c);
        //BigNumb operator %(BigNumb &b1);             //оператор остатка от деления на БЧ ?
        void cmp (const BigNumb&);                   //ф-ция сравнения больших чисел +
        void Len();                 //ф-ия вычисления кол-ва коэф +
        ~BigNumb();                 //деструктор +
        //void test();                //ТЕСТ
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
            char* b1=new char[1000];
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

BigNumb BigNumb::input_10()
{
    char *s=new char[1000];
    cout<<"Введите число: ";
    cin>>s;
    int n=strlen(s);
    Base x=0, c=10;
    BigNumb u;
    for(int i=0; i<n; i++)
    {
        x=s[i]-'0';
        u=u*c+x;
    }
    u.len=n;
    u.Len();
    return u;
}

void BigNumb::output_10()
{
    
    char *s=new char[1000];
    BigNumb u=*this, b;
    int n=0;
    Base x=0, c=10;
    for(int i=0; u!=b; i++)
    {
        x=u%c;
        s[i]=x+='0';
        u=u/c;
    }
    n=strlen(s);
    for(int i=n-1; i>=0; i--)
    {
        cout<<s[i];
    }
    cout<<endl;
}

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

BigNumb BigNumb::operator +(Base b)
{
    int j=0;
    D_Base x=b;
    BigNumb sum(0, len+1);
    for (j=0; j<len; j++)
    {
        sum.coef[j]=x+=coef[j];
        x=x>>Base_size;
    }
    sum.coef[j]=x;
    sum.len=len+1;
    sum.Len();
    return sum;
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
    sum.coef[i]=x;
    sum.len=max(len, b1.len)+1;
    sum.Len();
    return sum;
}

BigNumb BigNumb::operator -(const BigNumb &b1)
{
    //if(*this<b1) {cout<<1<<endl; exit(1);}
    int l=min(len, b1.len), i;
    D_Base x=0;
    Base tmp=0;
    BigNumb sub(0, len);
    for(i=0; i<l; i++)
    {
        x=1<<Base_size;
        x+=coef[i];
        sub.coef[i]=x-=(b1.coef[i]+tmp);
        tmp=!(x>>Base_size);
    }
    for(; i<len; i++) 
    {
        x=1<<Base_size;
        x+=coef[i];
        sub.coef[i]=x-=tmp;
        tmp=!(x>>Base_size);
    }
    sub.len=len;
    sub.Len();
    //cout<<hex<<sub.coef[2]<<endl;
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

BigNumb BigNumb::operator *(Base b1) //умножение на базу
{
    int n=len, j; //n-кол-во коэф большого числа, j-счётчик
    D_Base tmp=0; //tmp-временная переменная, которая будет хранить в себе результат умножения коэф на базу, а затем с помощью сдвига будем получать остаток,
                  //который нужно будет прибавить к следующему коэф
    Base k=0;     //к будет хранить этот остаток и прибавлять к следующему коэф
    BigNumb mul(0, n+1);//создаём большое число из нулей с lenMax=n+1, чтобы хватило выделенной памяти для результата
    for(j=0; j<n; j++)//непосредственно умножение большого числа на базу
    {
        mul.coef[j]=tmp=coef[j]*b1+k;//в tmp заносим результат умножения коэф на базу + возможный остаток не вошедший в память Base_size
                                     //затем присваиваем в коэф результата часть tmp содержащуюся в Base_size
        k=tmp>>Base_size;//в к заносим остаток из tmp сдвинутый на Base_size
    }
    mul.coef[j]=k;//в последний коэфициент заносим остаток, если тот есть 
    mul.len=n+1;//задаём длину результатирующего большого числа
    mul.Len();//Вычисляем кол-во коэф
    return mul;//возвращаем большое число
}

BigNumb BigNumb::operator *(BigNumb &b1)//Умножение БЧ на БЧ
{
    int i=0, j=0, m=len, n=b1.len;//i и j-счётчики для первого и второго БЧ соответственно, m и n-кол-во коэф первого и второго БЧ соответственно
    BigNumb mul(0, m+n);//Создаём новое БЧ с выделенной памятью m+n
    D_Base tmp=0;//tmp-временная переменная, которая будет хранить в себе результат умножения коэф-ов, а затем с помощью сдвига будем получать остаток,
                 //который нужно будет прибавить к следующим коэф-ам
    Base k=0; //к будет хранить этот остаток и прибавлять к следующему коэф
    for(j=0; j<n; j++)//цикл, где j отвечает за коэф второго БЧ
    {
        if(b1.coef[j]==0)//Если коэф 2-го БЧ =0, то умножение на него даст 0, так что просто сдвигаемся на следующий коэф
        {
            j++;
            if(j>=n) break;//Если предпоследний коэф 2-го БЧ был равен 0, выходим из цикла
        }
        for(i=0, k=0; i<m; i++)//цикл, где i отвечает за коэф 1-го БЧ
        {
            mul.coef[i+j]=tmp=coef[i]*b1.coef[j]+mul.coef[i+j]+k;//в tmp заносим результат умножения коэф на базу + возможный остаток не вошедший в память Base_size
            //+ уже имеющийся коэф в результирующем БЧ от предыдущих умножений затем присваиваем в коэф результата часть tmp содержащуюся в Base_size
            k=tmp>>Base_size;//в к заносим остаток из tmp сдвинутый на Base_size
        }
        mul.coef[j+i]=tmp=k+mul.coef[j+i];//когда заканчивается массив коэф 1-го БЧ заносим остатки в след. коэф результирующего БЧ
        k=tmp>>Base_size;//Если есть остаток, то он в к
    }
    mul.coef[j+i]=k+mul.coef[j+i];//в последний коэфициент заносим остаток, если тот есть 
    mul.len=n+m;//задаём длину БЧ
    mul.Len();//вычисляем кол-во коэф
    return mul;//возвращаем БЧ
}

BigNumb BigNumb::operator/(Base b1)//Оператор деления на число
{
    int j=0, n=len;//j-счётчик, n-длина БЧ
    Base r=0, b=0;//r будет хранить остаток от деления коэф БЧ на число
    b=~b;//b-максимальное число базы
    D_Base tmp=0;//tmp будет хранить результат объединения остатка(как старшего разряда) 
                 //и следующего коэф(как младшего)
                 //Пример: при делении 27 на 3 первый остаток равен 2, умножаем на 10, +7 и в tmp хранится 27
    BigNumb div(0, n);//Создаём БЧ и зануляем
    for(;j<n; j++)//цикл деления
    {  
        tmp=r*b+coef[n-1-j];//заносим результат объединения остатка(как старшего разряда) и следующего коэф(как младшего)
        div.coef[n-1-j]=tmp/b1;//в результирующее БЧ в последний незаполненный коэф заносим результат 
                               //целочисленного деления на число
        r=tmp%b1;              //сохраняем остаток от деления
    }
    div.len=n;//задаём длину результатирующего большого числа
    div.Len();//Вычисляем кол-во коэф
    return div;//возвращаем большое число
}

BigNumb BigNumb::operator /(BigNumb& b1) 
{
    BigNumb u=*this, u2, q, r;//u2 для увеличения u, q для частного, r для остатка
    int n=b1.len;//длина b1
    int m=len-n; //разница длин
    cout<<m<<" "<<n<<" "<<len<<endl;
    q.coef=new Base[m+1]; q.len=m+1;//частное будет длины не больше m
    r.coef=new Base[n]; r.len=n;//остаток будет длины не больше n
    int j, k=0;//j-счётчик от D3-D7
    D_Base b=(Base)~0+1;//65536
    D_Base d=b/(b1.coef[n-1] + 1), qi=0, ri=0;//Для нормализации
    cout<<d<<endl;
    if(d!=1)
    {
        u=u*d;//нормализуем
        b1=b1*d;//нормализуем
    }
    else
    {
        u2.coef=new Base[n+m+1];//создаём новое бч, чтобы добавить нуль
        u2+=u;//копируем значение исходного
        u2.coef[n+m]=0;//добавляем нуль
        u2.len=n+m+1;//увеличиваем длину
        u=u2;//присваиваем u2 (len=n+m+1)
    }
    for(j=m; j>=0; j--)//цикл от Д3-Д7
    {
        qi=(u.coef[n+j]*b+u.coef[n+j-1])/b1.coef[n-1];//приблизительное частное(разряд)
        ri=(u.coef[j+n]*b+u.coef[n+j-1])%b1.coef[n-1];//остаток
        if((qi==b) || (qi*b1.coef[n-2]>(b*ri+u.coef[j+n-2])))
        {
            qi--;
            ri=ri+b1.coef[n-1];
        }
        if(ri<b)
           if((qi==b) || (qi*b1.coef[n-2]>(b*ri+u.coef[j+n-2])))
            {
                qi--;
                ri=ri+b1.coef[n-1];
            }
        u2=u2*0;
        u2.coef=new Base[n];
        u2.len=n;
        for(int i=j; i<n+j; i++) u2.coef[i-j]=u.coef[i]; 
        cout<<u2<<endl;
        u2.Len();
        u2-=(b1*qi);
        if(u2<(b1*qi)) k=1;
        else k=0;
        q.coef[j]=qi;
        if(k==1) 
        {
            q.coef[j]--;
            u2+=b1;
        }
        for(int i=0; i<n+1; i++) u.coef[i+j]=u2.coef[i];
    }
    cout<<q<<endl;
    u=u/d;
    u.len=n;
    u.Len();
    cout<<u<<endl;
    q.len=m+1;
    q.Len();
    return q;
}

Base BigNumb::operator %(Base c)
{
    int j=0, n=len;//j-счётчик, n-длина БЧ
    Base r=0, b=0;//r будет хранить остаток от деления коэф БЧ на число
    b=~b;//b-максимальное число базы
    D_Base tmp=0;//tmp будет хранить результат объединения остатка(как старшего разряда) 
                 //и следующего коэф(как младшего)
                 //Пример: при делении 27 на 3 первый остаток равен 2, умножаем на 10, +7 и в tmp хранится 27
    BigNumb div(0, n);//Создаём БЧ и зануляем
    for(;j<n; j++)//цикл деления
    {  
        tmp=r*b+coef[n-1-j];//заносим результат объединения остатка(как старшего разряда) и следующего коэф(как младшего)
        div.coef[n-1-j]=tmp/c;//в результирующее БЧ в последний незаполненный коэф заносим результат 
                               //целочисленного деления на число
        r=tmp%c;              //сохраняем остаток от деления
    }
    return r;//возвращаем большое число
}

BigNumb::~BigNumb()
{
    if(coef!=NULL)
    {
        delete[](coef);
        coef=NULL;
    }
}

/*void BigNumb::test()
{
    BigNumb A, B, C, D;
    int M=1000, T=1000, n, m;
    do
    {
        n=rand()%M+1;
        m=rand()%M+1;
        A.cin(n);
        B.cin(m);
        C=A/B;
        D=A%B;
    } while (A==C*B+D && A-D==C*B && D<B && T--);
}*/

int main()
{
    srand(unsigned(time(0)));
    BigNumb b, b1, b2;
    cin>>b;
    cin>>b1;
    //b.input_10();
    //b1.input_10();
    b2=b/b1;
    cout<<b2<<endl;
    return 0;
}