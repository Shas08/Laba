#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;

typedef unsigned short Base;         //2 байта
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
        BigNumb operator *(Base);                    //оператор умножение на базу +!
        BigNumb operator *(BigNumb &b1);             //оператор умножение на большое число +!
        BigNumb input_10();                               //10-ый ввод +!
        void output_10();                                 //10-ый вывод +!
        BigNumb operator /(Base);                    //оператор деление на базу +!
        int subBN(BigNumb &A, int j);         //вычитание из части числа часть другого
        BigNumb operator /(BigNumb &b1);             //оператор деления на БЧ +!
        Base operator %(Base c);                     //остаток от деления на базу +!
        BigNumb operator %(BigNumb &b1);             //остаток от деления на БЧ +!
        //BigNumb operator %(BigNumb &b1);             //оператор остатка от деления на БЧ ?
        void cmp (const BigNumb&);                   //ф-ция сравнения больших чисел +
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

BigNumb::BigNumb()
{
    lenMax=1;
    len=lenMax;
    coef=new Base;
    coef[0]=0;
}

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
    u.Len();
    return u;
}

void BigNumb::output_10()
{
    int i;
    char *s=new char[1000];
    BigNumb u=*this, b; //b=0 для сравнения в цикле
    int n=0;
    Base x=0, c=10;
    for(i=0; u!=b; i++)
    {
        x=u%c+'0';// из числа получаем символ
        s[i]=x;// заносим его в массив
        u=u/c;// делим на базу
    }
    s[i]='\0';
    n=strlen(s);
    for(int i=n-1; i>=0; i--)
    {
        cout<<s[i];
    }
    cout<<endl;
}

BigNumb::BigNumb(int t, int lM)
{
    len=lenMax=lM;
    coef=new Base[lenMax];
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
    coef=new Base[lenMax];
    for(int i=0; i<lenMax; i++) coef[i]=b.coef[i];
    Len();
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
        len=b1.len;
        lenMax=b1.lenMax;
        delete[] coef;
        coef=new Base[lenMax];
        for(int i=0; i<lenMax; i++) coef[i]=b1.coef[i];
        Len();
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
    int maximum = max(len, b1.len);
    BigNumb sum(0, maximum+1);
    sum.len = maximum + 1;
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
    sum.Len();
    sum.lenMax = sum.len;
    return sum;
}

BigNumb BigNumb::operator -(const BigNumb &b1)
{
    if(*this<b1)
    {
        cout<<"Вычитаемое больше уменьшаемого!"<<endl; 
        exit(1);
    }
    int l=b1.len, i;
    D_Base x=0;
    Base tmp=0;
    BigNumb sub(0, len);
    sub.len=len;
    for(i=0; i<l; i++)
    {
        x = ((D_Base)1 << Base_size) + coef[i];
        sub.coef[i] = x -= (b1.coef[i]+tmp);
        tmp = !(x >> Base_size);
    }
    for(; i<len; i++) 
    {
        x = ((D_Base)1 << Base_size) + coef[i];
        sub.coef[i]= x -= tmp;
        tmp = !(x >> Base_size);
    }
    sub.Len();
    sub.lenMax = sub.len;
    //cout<<hex<<sub.coef[2]<<endl;
    return sub;
}

BigNumb BigNumb::operator -=(const BigNumb &b1)
{
    *this=*this-b1;
    cout<<"+"<<endl;
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
    mul.len=n+1;//задаём длину результатирующего большого числа
    for(j=0; j<n; j++)//непосредственно умножение большого числа на базу
    {
        mul.coef[j]=tmp=coef[j]*b1+k;//в tmp заносим результат умножения коэф на базу + возможный остаток не вошедший в память Base_size
                                     //затем присваиваем в коэф результата часть tmp содержащуюся в Base_size
        k=tmp>>Base_size;//в к заносим остаток из tmp сдвинутый на Base_size
    }
    mul.coef[j]=k;//в последний коэфициент заносим остаток, если тот есть 
    mul.Len();//Вычисляем кол-во коэф
    mul.lenMax = mul.len;
    return mul;//возвращаем большое число
}

BigNumb BigNumb::operator *(BigNumb &b1)//Умножение БЧ на БЧ
{
    int i=0, j=0, m=len, n=b1.len;//i и j-счётчики для первого и второго БЧ соответственно, m и n-кол-во коэф первого и второго БЧ соответственно
    BigNumb mul(0, m+n);//Создаём новое БЧ с выделенной памятью m+n
    mul.len = m + n;
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
    //mul.coef[j+i]=k+mul.coef[j+i];//в последний коэфициент заносим остаток, если тот есть 
    mul.Len();//вычисляем кол-во коэф
    mul.lenMax = mul.len;
    return mul;//возвращаем БЧ
}

BigNumb BigNumb::operator/(Base b1)//Оператор деления на число
{
    if(b1 == 0)
    {
        cout<<"На 0 делить нельзя!"<<endl;
        return *this;
    }
    int j=0, n=len;//j-счётчик, n-длина БЧ
    Base r=0, b=Base_size;//r будет хранить остаток от деления коэф БЧ на число
    D_Base tmp=0;//tmp будет хранить результат объединения остатка(как старшего разряда) 
                 //и следующего коэф(как младшего)
                 //Пример: при делении 27 на 3 первый остаток равен 2, умножаем на 10, +7 и в tmp хранится 27
    BigNumb div(0,n);//Создаём БЧ и зануляем
    for(;j<n; j++)//цикл деления
    {  
        tmp=(r<<b)+coef[n-1-j];//заносим результат объединения остатка(как старшего разряда) и следующего коэф(как младшего)
        div.coef[n-1-j]=tmp/b1;//в результирующее БЧ в последний незаполненный коэф заносим результат 
                               //целочисленного деления на число
        r=tmp%b1;              //сохраняем остаток от деления
    }
    div.len=n;//задаём длину результатирующего большого числа
    div.Len();//Вычисляем кол-во коэф
    div.lenMax = div.len;
    return div;//возвращаем большое число
}

int BigNumb::subBN(BigNumb &A, int j) //аналогично для вычитания бч, но только для частей
{//Нужно для деления на бч
    D_Base tmp = 0;
    Base k = 0;
    int i = 0;
    if(*this < A)
    {
        return 1;
    }
    for(; i < A.len; i++)
    {
        tmp = (1 << Base_size) + coef[i+j] - (A.coef[i] + k);
        coef[i+j] = tmp;
        k=!(tmp >> Base_size);
    }
    for(; i < len; i++)
    {
        tmp = (1 << Base_size) + coef[i + j] - k;
        coef[i + j] = tmp;
        k=!(tmp >> Base_size);
    }
    Len();
    lenMax = len;
    return 0;
}

BigNumb BigNumb::operator /(BigNumb& b1) 
{
    BigNumb Null(0, 1);
    if(b1 == Null)
    {
        cout<<"На 0 делить нельзя"<<endl;
        exit(1);
    }
    if(b1.len == 1)
    {
        BigNumb q(0, len);
        q.len = q.lenMax;
        q = *this / b1.coef[0];
        q.Len();
        return q;
    }
    BigNumb u=*this;//u-копия делимого
    BigNumb u2=*this;//u2-вторая копия
    BigNumb v=b1;//копия делителя
    int flag=0;// отвечает за то, был займ или нет
    int m=len-b1.len; //разница длин
    BigNumb q(0, m + 1);//частное от деления
    q.len = q.lenMax;
    D_Base qi=0, ri=0;//Для нормализации
    Base b=Base_size;//2 байта
    Base d=((D_Base)1 << b)/(b1.coef[b1.len-1] + 1);//нормализатор d
    if(u<b1)
    {
        q.Len();
        q.lenMax = q.len;
        return q;//если делимое меньше делителя, частное=0
    }
    else if(u==b1)//если оба БЧ равны, частное=1
    {
        q.coef[0]=1;
        q.Len();
        q.lenMax = q.len;
        return q;
    }
    if(d!=1)//Если нормализатор не равен 1, то нужно умножить оба числа на нормализатор
    {
        u = u * d;
        u2 = u;
        v = b1 * d;
    }
    if(len==u.len)//Если после нормализации длины совпадают, нужно увеличить разряд у делимого
    {
        delete[] u.coef;
        u.coef=new Base[u2.len+1];
        u.lenMax++;
        u.len++;
        int i;
        for(i=0; i<u2.len; i++) u.coef[i]=u2.coef[i];
        u.coef[i]=0;
    }
    int j=m, n=v.len;//j-для цикла, к-длина делителя
    BigNumb v2;
    while(j>=0)//пока j не выйдет за 0
    {
        qi=(((D_Base)u.coef[j+n]<<b)+u.coef[j+n-1])/v.coef[n-1];
        ri=(((D_Base)u.coef[j+n]<<b)+u.coef[j+n-1]) % v.coef[n-1];
        while((qi == ((D_Base)1<<b)) || (qi*v.coef[n-2]>((ri<<b)+u.coef[j+n-2])))
        {
            qi--;// где b*r'=u.coef[j+k]<<b)+u.coef[j+k-1]-qi*v.coef[k-1])<<b)
            ri += v.coef[n - 1];
            if(ri >= ((D_Base)1<<b)) break;
        }
        v2=v*qi;
        if(v2 != Null) flag=u.subBN(v2, j);//делаем вычитание частей 2-ух бч.
        q.coef[j]=qi;
        if(flag)//если был займ при вычитании
        {
            v2 -= v;
            flag = u.subBN(v2, j);//компенсация сложением
            q.coef[j]--;//коррекция q'
        }
        j--;
        flag=0;//обнуляем флаг
    }
    q.Len();//убираем лишние нули впереди, если они есть.
    q.lenMax = q.len;
    return q;
}

//3178ea8ecdde8385 8932e05f 1111111111111111 12321ffca
BigNumb BigNumb::operator %(BigNumb &b1) 
{
    BigNumb Null(0, 1);
    if(b1 == Null)
    {
        cout<<"На 0 делить нельзя"<<endl;
        exit(1);
    }
    if(b1.len == 1)
    {
        BigNumb ost(0, 1);
        ost.len = 1;
        ost.coef[0] = *this % b1.coef[0];
        ost.Len();
        ost.lenMax = ost.len;
        return ost;
    }
    BigNumb u=*this;//u-копия делимого
    BigNumb u2=*this;//u2-вторая копия
    BigNumb v=b1;//копия делителя
    int flag=0;// отвечает за то, был займ или нет
    int m=len-b1.len; //разница длин
    D_Base qi=0, ri=0;//Для нормализации
    Base b=Base_size;//2 байта
    Base d=((D_Base)1 << b)/(b1.coef[b1.len-1] + 1);//нормализатор d = 2
    if(u<b1) return *this;//если делимое меньше делителя
    else if(u==b1) return Null;//если оба БЧ равны
    if(d!=1)//Если нормализатор не равен 1, то нуэно умножить оба числа на нормализатор
    {
        u=u*d;
        u2=u2*d;
        v=b1*d;
    }
    if(len==u.len)//Если после нормализации длины совпадают, нужно увеличить разряд у делимого
    {
        delete[] u.coef;
        u.coef=new Base[u2.len+1];
        u.lenMax++;
        u.len++;
        for(int i=0; i<u2.len; i++) u.coef[i]=u2.coef[i];
        u.coef[u.len-1]=0;
    }
    int j=m, n=v.len;//j-для цикла, к-длина делителя
    BigNumb v2;
    while(j>=0)//пока j не выйдет за 0
    {
        qi=(((D_Base)u.coef[j+n]<<b)+u.coef[j+n-1])/v.coef[n-1];
        ri=((D_Base)u.coef[j+n]<<b)+u.coef[j+n-1]-qi*v.coef[n-1];
        while((qi == ((D_Base)1<<b)) || (qi*v.coef[n-2]>((ri<<b)+u.coef[j+n-2])))
        {
            qi--;// где b*r'=u.coef[j+k]<<b)+u.coef[j+k-1]-qi*v.coef[k-1])<<b)
            ri += v.coef[n - 1];
            if(ri >= ((D_Base)1<<b)) break;
        }
        v2=v*qi;
        if(v2 != Null) flag=u.subBN(v2, j);//делаем вычитание частей 2-ух бч.
        if(flag)//если был займ при вычитании
        {
            v2 -= v;
            flag = u.subBN(v2, j);//компенсация сложением
        }
        j--;
        flag=0;//обнуляем флаг
    }
    u = u / d;
    u.Len();
    u.lenMax = u.len;
    return u;
}

Base BigNumb::operator %(Base c)
{
    int j=0, n=len;//j-счётчик, n-длина БЧ
    D_Base r=0;//r будет хранить остаток от деления коэф БЧ на число
    Base b=Base_size;
    //b=~b;//b-максимальное число базы
    D_Base tmp=0;//tmp будет хранить результат объединения остатка(как старшего разряда) 
                 //и следующего коэф(как младшего)
                 //Пример: при делении 27 на 3 первый остаток равен 2, умножаем на 10, +7 и в tmp хранится 27
    for(;j<n; j++)//цикл деления
    {  
        tmp=(r<<b)+coef[n-1-j];//заносим результат объединения остатка(как старшего разряда) и следующего коэф(как младшего)
        r=tmp%c;              //сохраняем остаток от деления
    }
    return r;//возвращаем большое число
}

BigNumb::~BigNumb()
{
    if(coef!=NULL)
    {
        delete[] coef;
        //coef=NULL;
    }
}

void test()
{
    int M=1000, T=1000, n, m;
    BigNumb Numb_1, Numb_2, Div, Ost, Mul, Sum,Test_1, Test_2;
    do
    {
        n=rand()%M+1;
        m=rand()%M+1;
        BigNumb a(1, n), b(1,m);
        if(a >= b)
        {
            Numb_1 = a;
            Numb_2 = b;
        }
        else
        {
            Numb_2 = a;
            Numb_1 = b;
        }
        Div = Numb_1 / Numb_2;
        Ost = Numb_1 % Numb_2;
        Mul = Div * Numb_2;
        Sum = Mul + Ost;
        Test_2 = Numb_1 - Ost;
        if(Numb_1 != Sum) cout<<"Test_1 is false"<<endl;
        if(Test_2 != Mul) cout<<"Test_2 is false"<<endl;
        if(Ost >= Numb_2) cout<<"Test_3 is false"<<endl;
        T--;
        if(T == 0) cout<<"test was finished"<<endl;
        cout<<T<<endl;
    } while ((Numb_1 == Sum) && (Test_2 == Mul) && (Ost < Numb_2) && (T > 0));
}

int main()
{
    srand(time(0));
    test();
    /*int M=1000, T=1000, n, m, i;
    BigNumb Numb_1, Numb_2, Div, Ost, Mul, Sum,Test_1, Test_2;
    for(i = 0;;i++)
    {
        n=rand()%M+1;
        m=rand()%M+1;
        BigNumb a(1, n), b(1,m);
        if(a >= b)
        {
            Numb_1 = a;
            Numb_2 = b;
        }
        else
        {
            Numb_2 = a;
            Numb_1 = b;
        }
        //cout<<"NUMB_1 = "<<Numb_1<<endl;
        //cout<<"NUMB_2 = "<<Numb_2<<endl;
        Div = Numb_1 / Numb_2;
        //cout<<"Деление = "<<Div<<endl;
        Ost = Numb_1 % Numb_2;
        //cout<<"остаток = "<<Ost<<endl;
        Mul = Div * Numb_2;
        //cout<<"Умножение 1 = "<<Mul<<endl;
        Sum = Mul + Ost;
        //cout<<"Число А = "<<Sum<<endl;
        Test_2 = Numb_1 - Ost;
        //cout<<"Умножение 2 = "<<Test_2<<endl;
        if(Numb_1 != Sum) 
        {
            cout<<"Test_1 is false"<<endl;
            break;
        }
        if(Test_2 != Mul)
        {
            cout<<"Test_2 is false"<<endl;
            break;
        }
        if(Ost >= Numb_2)
        {
            cout<<"Test_3 is false"<<endl;
            break;
        }
        cout<<i<<endl;
    }
    cout<<endl<<i<<endl;*/
    return 0;
}