#include<iostream>
#include<fstream>
using namespace std;

void Piramid(int *alp, int len)
{
  int i, j, k, f, a, n=len;
  for(k=len/2-1; k>=0; k--) 
  {  
    i=k; f=1;
    j=2*i+1;
    a=alp[i];
    while(i<n)
    {
      if(j<n && f)
      {
        if(j+1<n)
          if(alp[j+1]>alp[j]) j=j+1;
        if(alp[j]>a)
        {
          alp[i]=alp[j];
          i=j;
          j=2*i+1;
        }
        else f=0;
      }
      else
      {
        alp[i]=a;
        break;
      }
    }
  }
  for(k=n-1; k>0; k--)
  {
    swap(alp[0], alp[k]);
    i=0; f=1;
    j=2*i+1;
    a=alp[i];
    while(i<k)
    {
      if(j<k && f)
      {
        if(j+1<k)
          if(alp[j+1]>alp[j]) j=j+1;
        if(alp[j]>a)
        {
          alp[i]=alp[j];
          i=j;
          j=2*i+1;
        }
        else f=0;
      }
      else
      {
        alp[i]=a;
        break;
      }
    }
  }
}

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