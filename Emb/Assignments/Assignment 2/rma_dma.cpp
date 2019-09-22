#include<bits/stdc++.h>
#include<stdio.h>
#define ll float
using namespace std;
class Node
{
    public:
    ll arrivalTime;
    ll executionTime;
    ll period;
    ll deadline;
    ll response;
    string name;

    ll id;
    Node(ll id, ll arrivalTime , ll executionTime, ll period, ll deadline)
    {
        this->arrivalTime = arrivalTime;
        this->executionTime = executionTime;
        this->period = period;
        this->id = id;
        this->deadline = deadline;
        //this->name = "T"+to_string(id);
        this->response = 0;
    }
};

bool comparatorPeriod(const Node &s1 ,const Node &s2){

    if(s1.arrivalTime == s2.arrivalTime)
    {
        return s1.period < s2.period;
    }
    return s1.arrivalTime < s2.arrivalTime;
}
bool comparatorDeadline(const Node &s1 ,const Node &s2){

    if(s1.arrivalTime == s2.arrivalTime)
    {
        return s1.deadline < s2.deadline;
    }
    return s1.arrivalTime < s2.arrivalTime;
}
vector<ll> split(string s){

    vector<ll> arr;
    ll i=0,val = 0;
    while(i < s.length())
    {
        if(s[i] == ' ')
        {
            arr.push_back(val);
            val = 0;
        }
        else
        {
            val = val * 10 + s[i] - '0';
        }
        i++;
    }
    arr.push_back(val);
    return arr;
}

void rma(vector<Node> tasks){
    sort(tasks.begin(),tasks.end(),comparatorPeriod);
	cout << "Rate monotonic Algorithm TDA ... " << endl;
	for (int i = 0; i < tasks.size(); ++i)
	{
		float w=tasks[i].executionTime,t1=tasks[i].executionTime,t2=0;
		int k=0;
		while(t1!=t2){
			t1=w;
			w=0;
			for(k=0 ; k<i ; k++)
			{
				w=w+(ceil(t1/tasks[k].period) * tasks[k].executionTime);
			}
			w=w+tasks[i].executionTime;
			t2=w;
		}
		tasks[i].response=t2;
		if(tasks[i].response<=tasks[i].deadline)
		{
				printf("Response Time for %d = %f , yes\n",i,tasks[i].response);
		}
		else
		{
			printf("Response Time for  %d = %f , No\n",i,tasks[i].response);
		}
	}
    cout << endl << "_______________________________________________________________________________________________________________" << endl;
}
int levelInterval(int i, vector<Node> tasks)
{
    float w=tasks[i].executionTime,t1=tasks[i].executionTime,t2=0;
    int k=0;
    while(t1!=t2){
        t1=w;
        w=0;
        for(k=0 ; k<=i ; k++)
        {
            w=w+(ceil(t1/tasks[k].period) * tasks[k].executionTime);
        }
        t2=w;
    }
    int x=ceil(t2/tasks[i].period);
    return x;
}
void dma(vector<Node> tasks)
{
    sort(tasks.begin(),tasks.end(),comparatorDeadline);
    cout << "deadline monotonic Algorithm TDA ... " << endl;
    for(int i=0;i<tasks.size();i++)
    {
        int x=levelInterval(i,tasks);
        cout << "X : " << x << endl;
        float w=tasks[i].executionTime,t1=tasks[i].executionTime,t2=0;
        int k=0,index=0;
        float m=0;
        for(index=0;index<x;index++){
            w=tasks[i].executionTime;
            t1=tasks[i].executionTime;
            t2=0;
            while(t1!=t2){
                t1=w;
                w=0;
                for(k=0 ; k<i ; k++)
                {
                    w=w+(ceil(( t1+(index*tasks[i].period) )/ tasks[k].period ) * tasks[k].executionTime);
                }
                w = w + (tasks[i].executionTime*(index+1)) - (index*tasks[i].period);
                t2=w;
            }

            printf("worst case for (%d %d) is %.3f, ",i+1,index+1,t2);
            if((t2-tasks[i].deadline)<=0)
                printf("Yes\n");
            else
                printf("No\n");
        }
        printf("\n");
        tasks[i].response=t2;
    }
    cout << endl << "_______________________________________________________________________________________________________________" << endl;
}
int main(){
    ll n=0;
    vector<Node> tasks;
    ifstream fin;
    fin.open("input.txt");
    string line;
    while (fin) {
        getline(fin, line);
        if(line.length() > 0){
            vector<ll> arr = split(line);
            tasks.push_back(Node(n+1,arr[0],arr[1],arr[2],arr[3]));
            n++;
        }
    }
    fin.close();

    cout << "Task \t arrivalTime \t executionTime \t period \t deadline \t" << endl;
    for (int i = 0; i < tasks.size(); ++i)
    {
        cout << "T" << tasks[i].id << " " << tasks[i].arrivalTime << " " << tasks[i].executionTime << " " << tasks[i].period << " " << tasks[i].deadline << " " << endl;
    }
    cout << endl;
    rma(tasks);
    //dma(tasks);
    return 0;
}
