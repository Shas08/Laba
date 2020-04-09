#include<iostream>
#include<fstream>
#include<vector>
#include<map>
using namespace std;

struct knot
{
    vector<char> symb;
    vector<int> numb;
};

void without_0(knot &noeds)
{
    int n=noeds.numb.size();
    for(int i=0; i<n; i++)
        if(noeds.numb[i]==0)
        {
            noeds.numb.erase(noeds.numb.begin()+i);
            noeds.symb.erase(noeds.symb.begin()+i);
            n--;
            i--;
        }
}

void create_tree(knot noeds, multimap<int, char> &tree)
{
    int n=noeds.numb.size();
    for(int i=0; i<n; i++)
    {
        tree.insert(pair<int, char>(noeds.numb[i], noeds.symb[i]));
    }
    int a, b;
    for(int i=0; i<n-1; i++)
    {
        auto it=tree.begin();
        auto it2=it++;
        a=it->first;
        b=it2->first;
        tree.insert(pair<int, char>(a+b, it2->second));
        tree.insert(pair<int, char>(a+b, it->second));
        tree.erase(it);
        tree.erase(it2);
    }
    for (auto it3 = tree.begin(); it3 != tree.end(); ++it3)
    {
      cout << it3->first << " : " << it3->second << endl;
    }
}

int main()
{
    knot noeds;
    multimap<int, char> tree;
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
    create_tree(noeds, tree);
    return 0;
}