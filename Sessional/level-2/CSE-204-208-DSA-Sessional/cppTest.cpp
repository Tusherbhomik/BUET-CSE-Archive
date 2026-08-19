#include <bits/stdc++.h>
using namespace std;
#define ll long long int
#define N 1000001

void dbug()
{
    cout<<"Ok till"<<endl;
}
bool prime[N + 1];
void SieveOfEratosthenes()
{
    memset(prime, true, sizeof(prime));
    for (int p = 2; p * p <= N; p++) {
        // If prime[p] is not changed, then it is a prime--> true means prime
        if (prime[p] == true) {
            for (int i = p * p; i <= N; i += p)
                prime[i] = false;
        }
    }
    prime[0]=false;
    prime[1]=false;
}//o(nloglogn)
vector<int> no_factor(int n)
{
    vector<int>factors;
    for(int i=1;i<=n/2;i++)if(n%i==0)factors.push_back(i);
    factors.push_back(n);
    return factors;
}//returns the totals vector of the factors
int sumOf_factors(int n)
{
    int sum=0;
    vector<int >factors;
    factors=no_factor(n);
    for(auto c:factors)sum+=c;
    return sum;
}
int factorial(int n)
{
    //factorial beshi boro dile ans ashbe na
    ll fact[20];
    memset(fact,0,sizeof(fact));
    fact[0]=1;
    for(int i=1;i+1<20;i++)fact[i]=i*fact[i-1];
    return fact[n];
}
bool perfect_Number(int n)
{
    int sum=sumOf_factors(n);
    if(n==sum-n)cout<<"Perfect"<<endl;
    else cout<<"NO"<<endl;
}
int densityPrime(int n)//--->n/ln(n)
{//prime is a global array
    SieveOfEratosthenes();
    int count =0;
    for(int i=2;i<=n;i++)if(prime[i])count++;
    return count;
}
int gcd(int a,int b)
{
    if(b==0)return a;
    else return gcd(b,a%b);
}//ologn
//built in function  __gcd(10,23)
int lcm(int a ,int b)
{
    return (a*b)/gcd(a,b);
}
vector<int> primeFactorization(int n)
{
    vector<int> pf;
    for(int i=2;i*i<=n;i++)
    {
        while(n%i==0)
        {
            pf.push_back(i);
            n/=i;
        }
    }
    if(n>1)pf.push_back(n);
    return pf;
}
int coprime(int n)//co prime of n between 1 to n
{
    vector<int>factors=primeFactorization(n);
    int prod =1;
    int i=0;
    while(i<factors.size())
    {
        if(i==0)prod *=factors[i];
        else
        {
            if(factors[i]!=factors[i-1])
            {
                prod/=factors[i-1];
                prod*=(factors[i-1]-1);
            }
                prod*=factors[i];
        }
        i++;
    }
    prod/=factors[i-1];
    prod*=(factors[i-1]-1);
    return prod;
}

int main()
{
    ll n;
    // cin>>n;
    // perfect_Number(n);
    // cout<<densityPrime(n)/log10(n)<<endl;
    // cout<<gcd(13,8)<<endl;
    // cout<<lcm(24,36);
    cout<<coprime(12)<<endl;
    return 0;
}

/*#include<bits/stdc++.h>
using namespace std;
void topologicalSort(int i,vector<int>adj[],vector<bool>&visited,stack<int> &myStack)
{
    visited[i]=true;
    for(int j=0;j<adj[i].size();j++)
    {
        int g=adj[i][j];
        if(!visited[g]) topologicalSort(j,adj,visited,myStack);
    }
    myStack.push(i);
}
int DFS(int i,vector<bool>visited,vector<int>adj[],int count )
{
    visited[i]=true;
    for(int j=0;j<adj[i].size();j++)
    {
        if(!visited[adj[i][j]])
            return count=DFS(adj[i][j],visited,adj,count)+1;
    }
    return count;


}
int main()
{

    int n,m;
    cin>>n>>m;
    vector<int>adj[n];
    vector<bool> visited;
    for(int i=0;i<n;i++)
    {
        visited.push_back(false);
    }
    int x,y;
    for(int i=0;i<m;i++)
    {
        cin>>x>>y;
        adj[x].push_back(y);
    }

    //stack<int> myStack;

    /*for(int i=0;i<n;i++)
    {
        if(!visited[i])
        {
            topologicalSort(i,adj,visited,myStack);
        }
        while(!myStack.empty())
            cout<<myStack.top()<<" ";
            myStack.pop();
    }*/
    bool parent[n];
    for(int i=0;i<n;i++)
    {
        parent[i]=false;
    }
    for(int i=0;i<n;i++)
    {

       for(int j=0;j<adj[i].size();j++)
       {
           parent[adj[i][j]]=true;

       }
    }
    int count=0;
    for(int i=0;i<n;i++)//gm found out
    {
        if(!parent[i])
            cout<<DFS(i,visited,adj,count)<<endl;
    }

}
*/
