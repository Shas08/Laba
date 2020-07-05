#include <iostream>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <algorithm>
using namespace std; //rasmer and proverki v delenii.

typedef unsigned char BASE; // 8 бит 
typedef unsigned short DBASE;

class BigNumber {
    BASE* number; //в одном коэффициенте - 2 символа
    int len, maxlen; // длинна и максимальная длинна
public:
    BigNumber(int n = 0, int mode = 0) { // конструктор по умолчанию
        maxlen = n;
        number = new BASE[maxlen + 1];

        switch (mode) {//нулями или рандомно
        case 0: {
            for (int i = 0; i < maxlen + 1; i++) number[i] = 0;
            len = 1;
            break;
        }
        case 1: { // рандомно
            for (int i = 0; i < maxlen; i++) number[i] = rand();
        }
              len = maxlen;
              zerominus(); //избавление от нулей, идущих впереди
        }
    }

    BigNumber(BigNumber& A) { //конструктор копирования
        number = new BASE[A.maxlen]; 
        len = A.len;
        for (maxlen = 0; maxlen < A.maxlen; number[maxlen] = A.number[maxlen], maxlen++);
    }

    ~BigNumber() { //Деструктор
        if (number != NULL)
            delete[] number;
    }

    void obnulen() { // Обнуление БЧ
        for (int i = 0; i < len; i++) number[i] = 0;
        len = 1;
    }

    BigNumber& operator =(const BigNumber& A) { // Оператор присвоения
        if (this != &A) { 
            delete[] number; 
            number = new BASE[A.maxlen]; 
            len = A.len;
            for (maxlen = 0; maxlen < A.maxlen; number[maxlen] = A.number[maxlen], maxlen++); 
        }
        return *this;
    }

    void zerominus() {//функция избавления от впереди идущих нулей
        while ((number[len - 1] == 0) && (len > 0)) //сокращаем длинну, пока не встретим ненулевой символ или не пройдём БЧ
            len--;

        if (len == 0) //Если все нули, то это 0
            len++;

    }

    friend istream& operator >>(istream& r, BigNumber& A) {
        string str; //создаём переменную для ввода с клавиатуры
        cin >> str;
        BigNumber B(str.size(), 0); //создаём БЧ длинной нашей строки и заполняем его нулями
        int k = 0, l = 0;
        BASE tmp;
        for (int i = str.size() - 1; i >= 0; i--) {         //длинна строки без '\0'
            try { //выясняем в каком диапозоне символ из строки
                if ((str[i] >= '0') && (str[i] <= '9'))         
                    tmp = (int)str[i] - '0';                    //переводим из строки в число
                else if (str[i] >= 'A' && str[i] <= 'F')
                    tmp = (int)str[i] - 'A' + 10;               
                else if (str[i] >= 'a' && str[i] <= 'f')
                    tmp = (int)str[i] - 'a' + 10;
                else
                    throw - 1; //Если не выполняется try, то выдаёт ошибку и программа завершается
            }
            catch (int j) { //catch-обработка исключения определённого типа данных (в нашем случае типа int)
                cout << "Некорректные данные" << endl;
                break;
            }
            k = (str.size() - 1 - i) / (sizeof(BASE) * 2);//пока не получим 2 символа из строки, заносим их в 1 коэффициент
            l = (str.size() - 1 - i) % (sizeof(BASE) * 2) * 4;//вычисляем, 1-ый ли это символ коэффициента или 2-ой
            B.number[k] |= tmp << l; // через дизъюнкцию бит добавляем в коэффициент 1 символ за другим, если тот имеется
        }
        B.len = k + 1;// присваеваем длинну
        A = B; // присваемваем значение локального БЧ к нашему
        A.zerominus(); // избавляемся от нулей
        return r;
    }

    friend ostream& operator <<(ostream& r, BigNumber& A) {//потоковый вывод
        int j = A.len - 1;
        cout << hex << (int)A.number[j]; // выводим старший коэффициент
        for (int i = j - 1; i >= 0; i--) {
            cout.width(sizeof(BASE) * 2);// устанавливаем ширину поля вывода
            cout.fill('0'); //заполняем нулями пустые знакоместа нулями 
            cout << hex << (int)A.number[i];//выводим коэффициенты на экран 
        }
        return r;
    }

    //оператор +
    BigNumber operator +(BigNumber& A) {
        int j = 0;
        DBASE tmp;
        DBASE k = 0;
        BigNumber B(max(len, A.len) + 1, 0); //создаём БЧ максимальной размерности, чтобы влезла сумма чисел
        while (j < min(len, A.len)) {//пока меньшее число не закончилось, складываем их коэффициенты
            tmp = k + number[j] + A.number[j];//сумма коэффициентов и займа из будущего коэффициента
            B.number[j] = tmp;//присваеваем типу BASE элемент типа DBASE, тогда младший элемент суммы попадёт в результат
            k = tmp >> (int)sizeof(BASE) * 8; // то что не попало в результат переходит в займ, для дальнейшего сложения
            j++; // след. коэф
            B.len++;//увеличиваем длинну БЧ
        }
        while (j < len) {//пока большее число не закончилось, прибавляем к его коэффициентам оставшийся займ
            tmp = k + number[j];
            B.number[j] = tmp;
            k = tmp >> (int)sizeof(BASE) * 8;
            j++;
            B.len++;
        }
        while (j < A.len) {// если второе БЧ больше первого, аналогично
            tmp = k + A.number[j];
            B.number[j] = tmp;
            k = tmp >> (int)sizeof(BASE) * 8;
            j++;
            B.len++;
        }
        B.len--; // лишнюю длинну убираем
        if (k) {//если займ остался от последнего действия, увеличиваем нашу длинну и присваеваем старшему коэф. наш займ
            B.len++;
            B.number[j] = k;
        }
        B.zerominus();//избавляемся от лишних нулей
        return B;
    }

    BigNumber operator +=(BigNumber& A) {//оператор + и =
        *this = *this + A;
        return *this;
    }

    BigNumber operator -(BigNumber& A) {//минус
        int j = 0;
        int l = min(len, A.len), i;
        DBASE tmp;
        BASE k = 0;//////DBASE
        BigNumber B(len, 0); //�������� ����� � �������� len � ���������� 0
        B.len = len;
        A.zerominus();
                for (i = 0; i < l; i++) {   //пока меньшее число не закончилось, вычитаем их коэффициенты
                tmp = 1 << (sizeof(BASE) * 8); //делаем займ (т.к. не проверяем >= между коэфф.)
                tmp += number[i]; //получаем уменьшаемое
                B.number[i] = tmp -= (A.number[i] + k); // получаем разность
                k = !(tmp >> sizeof(BASE) * 8);// если займ был или его не было
            }
            for (; i < len; i++) { // вычитаем займ из следующего коэффициента, если он есть, а дальше присваеваем оставш.БЧ
                tmp = 1 << (sizeof(BASE) * 8);
                tmp += number[i];
                B.number[i] = tmp-= k;
                k = !(tmp >> sizeof(BASE) * 8);
            }
            B.zerominus();
            return B;
     }

    BigNumber operator -=(BigNumber& A) {
        *this = *this - A;
        return *this;
    }

    //сравнение <
    bool operator <(BigNumber& A) {
        if (len < A.len)
            return true;
        if (len > A.len)
            return false;
        for (int i = len - 1; i >= 0; i--) {//пока коэф. сравнимы, идём по БЧ
            if (number[i] > A.number[i])
                return false;
            if (number[i] < A.number[i])
                return true;
        }
        return false;
    }

    bool operator >(BigNumber& A) {
        if (len > A.len)
            return true;
        if (len < A.len)
            return
            false;
        for (int i = len - 1; i >= 0; i--) { //���������� �����������
            if (number[i] > A.number[i])
                return true;
            if (number[i] < A.number[i])
                return false;
        }
        return false;
    }

    bool operator <=(BigNumber& A) {
        if (len < A.len)
            return true;
        if
            (len > A.len)
            return false;
        for (int i = len - 1; i >= 0; i--) { //���������� �����������
            if (number[i] > A.number[i])
                return false;
            if (number[i] < A.number[i])
                return true;
        }
        return true;
    }

    bool operator >=(BigNumber& A) {
        if (len > A.len)
            return true;
        if (len < A.len)
            return false;
        for (int i = len - 1; i >= 0; i--) { //���������� �����������
            if (number[i] > A.number[i])
                return true;
            if (number[i] < A.number[i])
                return false;
        }
        return true;
    }

    bool operator ==(BigNumber& A) {
        if (len == A.len) {
            for (int i = len - 1; i >= 0; i--) { //���������� �����������
                if (number[i] != A.number[i])
                    return false;
            }
        }
        else
            return false;
        return true;
    }

    bool operator !=(BigNumber& A) {
        if (len == A.len) {
            for (int i = len - 1; i >= 0; i--) { //���������� �����������
                if (number[i] != A.number[i])
                    return true;
            }
        }
        else
            return true;
        return false;
    }

    //умножение
    BigNumber operator *(BigNumber& A) {
        int j = 0, i = 0;
        DBASE k;
        DBASE tmp;
        BigNumber B(len + A.len, 0);// создаём БЧ размера, в который поместится наше произведение
        B.len = len + A.len;//создаём длинну
        while (j < A.len) { // идём по умножителю
            i = 0;
            k = 0;
            while (i < len) {//умножаем коэффициенты умножаемого на коэф. j-ый умножителя 
                tmp = (DBASE)number[i] * A.number[j] + k + B.number[i + j];
                B.number[i + j] = tmp;  //заносим в коэф. результирующего БЧ результат умножения
                k = tmp >> (int)sizeof(BASE) * 8; //назовём займом старший коэффициен результата для дальнейших вычислений 
                i++;
            }
            B.number[i + j] = k;//присваеваем старшему коэффициенту займ если он есть
            j++;
        }
        B.zerominus();
        return B;
    }

    // умножение на базу
    BigNumber mul(BASE a) {//��������� ����� �� ����
        int i = 0;
        DBASE tmp = 0;
        DBASE r = 0;
        BigNumber B(len + 1, 0);
        B.len = len + 1;
        while (i < len) {
            tmp = (DBASE)r + number[i] * a;
            B.number[i] = tmp;
            r = tmp >> sizeof(BASE) * 8;
            i++;
        }
        if (r != 0) {
            B.number[i] = r;
        }
        B.zerominus();
        return B;
    }

    BigNumber div(BASE a, BASE& R) { //деление на базу
        int j = len - 1;        //j-индекс последнего элемента
        BASE r = 0;             // остаток
        DBASE tmp;              //временная переменная
        BigNumber B(len, 0);
        B.len = len;
        while (j >= 0) {//идём со старших коэф. к младщим
            tmp = (r << (int)sizeof(BASE) * 8) + number[j];
            B.number[j] = tmp / a;                                  //высчитываем коэф.
            r = (DBASE)tmp % a;                                     //высчитываем остаток
            j--;
        }
        R = r;
        B.zerominus();
        return B;
    }

    BigNumber sumBASE(BASE A) {// прибавление базы
        int j = 0;
        DBASE k = 0;
        DBASE tmp = 0;
        BigNumber B(len + 1, 0);
        B.len = len + 1;
        tmp = (DBASE)k + A + number[j];
        B.number[j] = tmp;
        k = tmp >> sizeof(BASE) * 8;
        j++;
        while (j < len) {
            tmp = k + number[j];
            B.number[j] = tmp;
            k = tmp >> (int)sizeof(BASE) * 8;
            j++;
        }
        B.number[j] = k;
        B.zerominus();
        return B;
    }

    int subBN(BigNumber& A, int i, int t) {//вычитаем из части одного БЧ второе БЧ
        DBASE tmp = 0;
        DBASE k = 0;
        int j = i;                              //      (u[j+n].......u[j])-(v[n-1],...,v[0])q
        int l = 0;
        int Len;
        if((i + t) > len){
            Len = len - 1;
        }
        else  Len = i + t;
        while (j <= Len) {
            tmp = (DBASE)1 << sizeof(BASE) * 8;
            tmp += number[j];
            tmp -= A.number[l] + k;
            number[j] = tmp;
            k = !(tmp >> sizeof(BASE) * 8);
            j++;
            l++;
        }
        zerominus();
        int f = k;
        return f;
    }

    void sumBN(BigNumber& A, int i) { // складываем часть одного БЧ с другим БЧ
        int j = i;
        int l = 0;
        DBASE k = 0;
        DBASE tmp;
        int Len;
        if((i + A.len - 1) > len){
            Len = len - 1;
        }
        else Len = i + A.len - 1;
        while (j <= Len) {
            tmp = (DBASE)k + number[j] + A.number[l]; 
            number[j] = tmp; 
            k = tmp >> sizeof(BASE) * 8;
            j++;
            l++;
        }
        number[j] += k;
        zerominus();
    }

     //деление БЧ на БЧ
     BigNumber operator /(BigNumber& A) {        
        BigNumber B(len, 0); // результирующее число
        BigNumber copyTHIS;  // копируем 1-ое БЧ
        BigNumber copyTHIS2; // копируем 1-ое БЧ
        BigNumber copyA;     // копируем 2-ое БЧ
        int flag = 0;
        int del = len - A.len;//разница длин
        if (A.len >= 1 && A != B && *this >= A) {    //проверки, чтобы деление было корректно

            DBASE qz = 0; //всоздаём предположительное частное
            DBASE d = ((DBASE)1 << sizeof(BASE) * 8) / (A.number[A.len - 1] + 1);//вычисление нормализатора
            copyTHIS = *this;
            copyTHIS2 = *this;
            copyA = A;
            if (d != 1) {
                copyTHIS = copyTHIS.mul(d);             // Нормализуем
                copyA = A.mul(d);
                copyTHIS2=copyTHIS;
            }
            
            if (len == copyTHIS.len) {//если разрядность 1-ого БЧ не изменилась, добавляем нуль как старший коэф.
                delete[] copyTHIS.number;
                copyTHIS.number = new BASE[copyTHIS2.len + 1];//нам нужно увеличить разрядность
                copyTHIS.maxlen++;
                copyTHIS.len++;
                for (int i = 0; i < copyTHIS2.len; i++) {//присваеваем коэф.
                    copyTHIS.number[i] = copyTHIS2.number[i];
                }
                copyTHIS.number[copyTHIS.len - 1] = 0; // последний коэф. = 0
            }
            int j = del; // индекс, позволяющий делить БЧ по частям
            int k = copyA.len;
            while (j >= 0) {// вычисляем приблизительное частное
                qz = (((DBASE)(copyTHIS.number[j + k] << sizeof(BASE) * 8)) + copyTHIS.number[j + k - 1]) / copyA.number[k - 1];
                while (qz * copyA.number[k - 2] > (copyTHIS.number[j + k] * ((DBASE)1 << sizeof(BASE) * 8) + copyTHIS.number[j + k - 1] - qz * copyA.number[k - 1]) * ((DBASE)1 << sizeof(BASE) * 8) + copyTHIS.number[j + k - 2]) {
                     qz--;
                }// от самого старшего предположительного частного спускаемся до возможного частного

                BigNumber copyA2;
                copyA2 = copyA;
                int t = copyA.len;
                copyA2 = copyA.mul(qz);// умножаем делитель на приближенное частное
                flag = copyTHIS.subBN(copyA2, j, t);// flag - возвращает 1 или 0. Если уменьшаемое меньше вычитаемого, то 1
                B.number[j] = qz; // присваеваем приближенное частное как коэф. результирующ частного
                B.len++;
                if (flag) {//коррекция приближенного частного
                    copyTHIS.sumBN(copyA, j);// добавляем один раз делитель
                    B.number[j]--;//уменьшаем коэф.
                }
                j--;
                flag = 0;
            }
            B.zerominus();
        }
        return B;
    }

    BigNumber operator %(BigNumber& A) {
        BigNumber R;
        BigNumber a;
        a = *this / A;
        R = a * A;
        R =  *this - R;
        R.zerominus();
        return R;
    }
   
    void printlen() {
        cout << dec << (int)len << " " << (int)maxlen << endl;
        for (int i = len - 1; i >= 0; i--) {
            cout << hex << (int)number[i] << " ";
        }
    }

    void change(int n) {// меняем местами 2 БЧ
        BigNumber A(n, 1);
        *this = A;
    }

    BigNumber decCin(string str) {
        DBASE tmp;
        int j = 0;
        BigNumber A(str.length(), 0);
        A.len = str.length();
        while (j < str.length()) {
            tmp = str[j] - '0'; // перевод из символа в число
            A = A.mul((BASE)10);
            A = A.sumBASE(tmp);
            j++;
        }
        A.zerominus();
        return A;
    }

    void decCout() {
        int j = 0;
        BASE R = 0;
        string str;
        BigNumber A;
        int y;
        A = *this;
       
        char* s = new char[1000];
         while ((int)A.number[0] != 0) {
            A = A.div((BASE)10, R);
            y = (int)R + '0';
            s[j] = y;
            j++;
        }
        
        for (int i = j - 1; i >= 0; i--) {
            cout << s[i];
        }
        cout << endl;
    }
};

int main() {
  /* setlocale(LC_ALL, "Russian");
    setlocale(LC_ALL, "Russian");
    srand(time(0));
    BigNumber A, B, C, D, P, L;

    cout << "A = ";
    cin >> A;
    P = A; L = P;
    cout << "B = ";
    cin >> B;
    C = A + B;
    cout << "A + B = " << C << endl;
    A += B;
    cout << "A+=B = " <<C << endl;

    C = P - B;
    cout << "A - B = " << P << " - " << B << C << endl;
    P -= B;
    cout << "A-=B = " << C << endl;
    C = A * B;
    cout << "A * B = " << C << endl;
    C = P / B;
    cout << "A / B = " << P << " / " << B <<" = " << C << endl;
    C = A % B;
    cout << "A % B = " << C << endl;
    if (A == B) cout << "A == B" << endl;
    else if (A > B) cout << "A > B" << endl;
    else if (A < B) cout << "A < B" << endl;

    cout << endl << "BASE" << endl;
    int p;
    cin >> p;
    C = A.mul(p);
    cout << "A * p = " << C << endl;
    BASE R = 0;
    C = A.div((BASE)p, R);
    cout << "A / p = " << C << endl;

    BigNumber X;
    string x1;
    cin >> x1;
    size_t found = x1.find_first_not_of("1234567890");
    if (found != string::npos) {
        std::cout << "�� ����� ����������� 10������ ����� " << std::endl;
    }
    else {
        X = X.decCin(x1);
        cout << "����� � 16 �� - " << X << endl;
        X.decCout();
    }
    return 0;*/
    
    srand(unsigned(time(0)));
    int M = 1000;
    int T = 1000;
    BigNumber F;
    BigNumber E;
    BigNumber C;
    BigNumber R;
    BigNumber D;
    int n = rand() % M + 1;
    int m = rand() % M + 1;
    BigNumber A(n + 1, 1);
    BigNumber B(m + 1, 1);
    
   do {
        n = rand() % M + 1;
        m = rand() % M + 1;
        A.change(n + 1);
        B.change(m + 1);
        if (A > B) {
            C = A / B;
            //cout << C;
            R = A % B;
            F = C * B;
            E = F + R;
            D = A - R;
        }
        if (B > A) {
            C = B / A;
            //cout << C;
            R = B % A;
            F = C * A;
            E = F + R;
            D = B - R;
        }
        T--;
        cout << endl;
        //cout << "T = ";
        cout << dec << T;
        cout << endl;
    } while (((A == E) && (D == F) && (B > R) && (T > 0)) || ((B == E) && (D == F) && (A > R) && (T > 0)));
    return 0;
}
