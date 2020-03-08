#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

struct knot
{
    vector<char> symb;
    vector<int> numb;
};

int index_of_max(vector<int> numb)
{
    int max=numb[0], k=0;
    for(int i=1; i<numb.size(); i++) 
    {
        if(max<numb[i]) 
        {
            max=numb[i];
            k=i;
        }
    }
    return k; 
}

int index_of_min(vector<int> numb)
{
    int k=index_of_max(numb), min=numb[k];
    for(int i=1;i<numb.size();i++)  
        if(min>numb[i] && numb[i]!=0) 
        {
            min=numb[i];
            k=i;
        }
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

void sort_vector(knot &noeds)
{
    knot nds2;
    int n=noeds.numb.size(), k=0;
    for(int i=0; i<n; i++)
    {
        k=index_of_min(noeds.numb);
        nds2.numb.push_back(noeds.numb[k]);
        nds2.symb.push_back(noeds.symb[k]);
        noeds.numb.erase(noeds.numb.begin()+k);
        noeds.symb.erase(noeds.symb.begin()+k);
    }
    noeds.numb=nds2.numb;
    noeds.symb=nds2.symb;
    for(int i=0; i<noeds.numb.size(); i++) cout<<noeds.numb[i]<<"    "<<noeds.symb[i]<<endl;
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
    sort_vector(noeds);
    max_grade=j*8;
    //cout<<max_grade<<endl;
    //for(int i=0; i<noeds.numb.size(); i++) cout<<noeds.numb[i]<<"    "<<noeds.symb[i]<<endl;
    return 0;
}