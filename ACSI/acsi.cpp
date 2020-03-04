#include<iostream>
#include<fstream>
using namespace std;
int main()
{
    char symb;
    int *b1= new int[255], j=0;
    for(int i=0; i<255; i++) b1[i]=0;
    ifstream txt("text.txt");
    while((symb=txt.get())!=EOF)
    {
        b1[(int)symb]++;
        j++;
    }
    txt.close();
    Piramid(b1, 255);
    for(int i=0; i<255; i++) cout<<b1[i]<<endl;
    return 0;
}