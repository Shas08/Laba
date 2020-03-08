#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

struct knot
{
    vector<char> symb;
    vector<int> numb;
};

int index_of_max(knot noeds)
{
    int max=noeds.numb[0], k=0;
    for(int i=1;i<255;i++) 
        if(max<noeds.numb[i]) k=i;
    return k; 
}

int index_of_min(knot noeds)
{
    int k=index_of_max(noeds), min=noeds.numb[k];
    for(int i=1;i<255;i++)  
        if(min>noeds.numb[i] && noeds.numb[i]!=0) k=i;    
    return k;
}

void without_0(knot &noeds)
{
    int n=noeds.numb.size(), i;
    for(i=0; i<n; i++)
        if(noeds.numb[i]==0)
        {
            noeds.numb.erase(noeds.numb.begin()+i);
            noeds.symb.erase(noeds.symb.begin()+i);
            n--;
            i--;
        }
}

int main()
{
    knot noeds;
    char symb;
    int j=0, k=0, max_grade;
    for(int i=0; i<255; i++) 
    {
        noeds.numb.push_back(0);
        noeds.symb.push_back((int)i);
    }
    ifstream txt("text.txt");
    while((symb=txt.get())!=EOF)
    {
        noeds.numb[(int)symb]=noeds.numb[(int)symb]+1;
        j++;
    }
    txt.close();
    without_0(noeds);
    max_grade=j*8;
    //cout<<max_grade<<endl;
    for(int i=0; i<noeds.numb.size(); i++) cout<<noeds.numb[i]<<"    "<<noeds.symb[i]<<endl;
    return 0;
}