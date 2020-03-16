#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

struct knot
{
    vector<char> symb;
    vector<int> numb;
    vector<char> *array_of_pointers=new vector<char>;
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

void sort_knot(knot &noeds)
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
}

void create_tree(knot noeds, knot &tree)
{
    knot tree=noeds;
    for(int i=0, i<n-1, i++)
    {
        tree.numb.push_back(noeds.numb[0]+noeds.numb[1]);
        tree.numb.erase(tree.numb.begin(), tree.numb.begin()+1);
        sort_knot(tree);

    }
}

int main()
{
    knot noeds, tree;
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
    max_grade=j*8;
    without_0(noeds);
    sort_knot(noeds);
    create_tree(noeds, tree);
    return 0;
}