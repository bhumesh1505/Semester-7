#include<bits/stdc++.h>
#define ll long long int
using namespace std;
class Node
{
    public:
    ll arrivalTime;
    ll executionTime;
    ll period;
    ll deadline;
    ll quantum;
    ll tempQuantum;
    string name;
    ll id;
    Node(ll id, ll arrivalTime , ll executionTime, ll period, ll deadline ,  ll quantum = 3)
    {
        this->arrivalTime = arrivalTime;
        this->executionTime = executionTime;
        this->period = period;
        this->id = id;
        this->deadline = deadline;
        this->quantum = quantum;
        this->tempQuantum = quantum;
        this->name = "T"+to_string(id);
    }
};
bool comparator(const Node &s1 ,const Node &s2)
{
    if(s1.arrivalTime == s2.arrivalTime)
    {
        return s1.executionTime < s2.executionTime;
    }
    return s1.arrivalTime < s2.arrivalTime;
}
bool comparatorPeriod(const Node &s1 ,const Node &s2)
{
    if(s1.arrivalTime == s2.arrivalTime)
    {
        return s1.period < s2.period;
    }
    return s1.arrivalTime < s2.arrivalTime;
}
bool comparatorDeadline(const Node &s1 ,const Node &s2)
{
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

void fifo(vector<Node> tasks){

    cout << endl << "First Come First Served " << endl;
    sort(tasks.begin(), tasks.end(), comparator);
    ll t = 0;
    ll i = 0;
    cout << "(" << t << ")" ;
    while(i < tasks.size())
    {
        if(tasks[i].executionTime > 0 && tasks[i].arrivalTime <= t)
        {
            cout << "--" << tasks[i].name << "--";
            tasks[i].executionTime--;
        }
        else if(tasks[i].arrivalTime > t)
        {
            cout << "--*--" ;
        }
        if(tasks[i].executionTime == 0)
        {
            i++;
        }
        t++;
        cout << "(" << t << ")";
    }
    cout << endl << "_______________________________________________________________________________________________________________" << endl;
}

void lifo(vector<Node> tasks){

    cout << endl <<  "Last Come First Served " << endl;
    sort(tasks.begin(), tasks.end(), comparator);
    ll t = 0;
    ll i = tasks.size()-1;
    cout << "(" << t << ")" ;
    while(i >= 0)
    {
        if(tasks[i].executionTime > 0 )
        {
            cout << "--" << tasks[i].name << "--";
            tasks[i].executionTime--;
        }
        if(tasks[i].executionTime == 0)
        {
            i--;
        }
        t++;
        cout << "(" << t << ")";
    }
    cout << endl << "_______________________________________________________________________________________________________________" << endl;
}

void round_robin(vector<Node> tasks){

    cout << endl << "Round Robin " << endl;
    ll quantum;
    cout << "Enter Quantum : " << endl;
    //cin >> quantum;
    quantum = 3;
    sort(tasks.begin(), tasks.end(), comparator);

    queue<Node> readyQueue;

    ll t = 0;
    ll i = 0,completed=0;
    ll tempQuantum = quantum;
    cout << "(" << t << ")" ;
    while( completed < tasks.size() || !readyQueue.empty())
    {
        while(i < tasks.size() && tasks[i].arrivalTime <= t) // insert all available job at time t in readyQueue
        {
            readyQueue.push(tasks[i]);
            i++;
        }
        if(readyQueue.empty()) // If no jobs available at time t .. then idle
        {
            cout << "--*--" ;
        }
        else // jobs are available, so need to execute
        {
            if(tempQuantum > 0 && readyQueue.front().executionTime > 0) //
            {
                cout << "--" << readyQueue.front().name << "--";
                readyQueue.front().executionTime--;
                tempQuantum--;
            }
            if( readyQueue.front().executionTime > 0 && tempQuantum == 0) // quatum time completed for first job ... but job has not completed
            {
                // task is preemted and placed at end of the ready queue
                tempQuantum = quantum;
                Node node = readyQueue.front();
                readyQueue.pop();
                readyQueue.push(node);
            }
            if(readyQueue.front().executionTime == 0) // job completed
            {
                completed++;
                tempQuantum = quantum;
                readyQueue.pop();
            }
        }
        t++;
        cout << "(" << t << ")";
    }
    cout << endl << "_______________________________________________________________________________________________________________" << endl;
}

void weighted_round_robin(vector<Node> tasks){

    cout << endl << "Weighted Round Robin " << endl;
    sort(tasks.begin(), tasks.end(), comparator);

    queue<Node> readyQueue;

    ll t = 0;
    ll i = 0,completed=0;

    cout << "(" << t << ")" ;
    while( completed < tasks.size() || !readyQueue.empty())
    {
        while(i < tasks.size() && tasks[i].arrivalTime <= t) // insert all available job at time t in readyQueue
        {
            readyQueue.push(tasks[i]);
            i++;
        }
        if(readyQueue.empty()) // If no jobs available at time t .. then idle
        {
            cout << "--*--" ;
        }
        else // jobs are available, so need to execute
        {
            if(readyQueue.front().tempQuantum > 0 && readyQueue.front().executionTime > 0) //
            {
                cout << "--" << readyQueue.front().name << "--";
                readyQueue.front().executionTime--;
                readyQueue.front().tempQuantum--;
            }
            if( readyQueue.front().executionTime > 0 && readyQueue.front().tempQuantum == 0) // quatum time completed for first job ... but job has not completed
            {
                // task is preemted and needs to placed at end of the ready queue
                readyQueue.front().tempQuantum = readyQueue.front().quantum;
                Node node = readyQueue.front();
                readyQueue.pop();
                readyQueue.push(node);
            }
            if(readyQueue.front().executionTime == 0) // job completed
            {
                completed++;
                readyQueue.front().tempQuantum = readyQueue.front().quantum;
                readyQueue.pop();
            }
        }
        t++;
        cout << "(" << t << ")";
    }
    cout << endl << "_______________________________________________________________________________________________________________" << endl;
}

void rma(vector<Node> tasks){

}

void cyclic(vector<Node> tasks){

	cout << "\nCyclic " << endl;
	ll f=0;
	ll h;
	sort(tasks.begin(),tasks.end(),comparatorPeriod);
	for(int i=0;i<tasks.size();i++)
	{
		f = max(tasks[i].executionTime,f);
	}
	h = tasks[0].period * tasks[1].period / (__gcd(tasks[0].period,tasks[1].period));
	for(int i=2;i<tasks.size();i++)
	{
		h = h*tasks[i].period / (__gcd(h,tasks[i].period));
	}
	f++;
	while(f <= h )
	{
		if(h % f == 0)
		{
			bool found = true;
			for(ll i=0;i<tasks.size();i++)
			{
				if(2*f - __gcd(tasks[i].period, f) > tasks[i].deadline)
				{
					found = false;
				}
			}
			if(found == true)
			{
				break;
			}
		}
		f++;
	}
	//f = 4;
	cout << "Frame Size : " << f << endl;
	vector<Node> subtasks;
	for(ll i=0;i<tasks.size();i++)
	{
		ll arrivalTime = 0;
		ll deadline = tasks[i].period;
		for(ll j=0;j<h/tasks[i].period;j++)
		{
			subtasks.push_back(Node((i+1)*10+(j+1),arrivalTime,tasks[i].executionTime,tasks[i].period,deadline));
			arrivalTime = deadline;
			deadline = deadline + tasks[i].period;
		}
	}
	sort(subtasks.begin(),subtasks.end(),comparatorDeadline);
	for (ll i = 0; i < subtasks.size(); ++i)
	{
		cout << subtasks[i].name << " " << subtasks[i].executionTime << " " << subtasks[i].arrivalTime << " " << subtasks[i].deadline << endl;
	}

	ll t = 0;
    ll i = 0;
    cout << "(" << t << ")" ;
    ll remainingFrameSize = f ;
    while(i < subtasks.size())
    {
        if(subtasks[i].executionTime > 0 && subtasks[i].executionTime <= remainingFrameSize && subtasks[i].arrivalTime <= t)
        {
            cout << "--" << subtasks[i].name << "--";
            subtasks[i].executionTime--;
        }
        else if(subtasks[i].arrivalTime > t || subtasks[i].executionTime > remainingFrameSize)
        {
            cout << "--*--" ;
        }
        if(subtasks[i].executionTime == 0)
        {
            i++;
        }
        remainingFrameSize--;
        if(remainingFrameSize == 0)
        {
        	remainingFrameSize = f;
        }
        t++;
        cout << "(" << t << ")";
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
            tasks.push_back(Node(n+1,arr[0],arr[1],arr[2],arr[3],arr[4]));
            n++;
        }
    }
    fin.close();

    cout << "Task \t arrivalTime \t executionTime \t period \t deadline \t quantum" << endl;
    for (int i = 0; i < tasks.size(); ++i)
    {
        cout << "T" << tasks[i].id << " " << tasks[i].arrivalTime << " " << tasks[i].executionTime << " " << tasks[i].period << " " << tasks[i].deadline << " " << tasks[i].quantum << endl;
    }
    cout << endl;
    //fifo(tasks);
    //lifo(tasks);

    //round_robin(tasks);
    //weighted_round_robin(tasks);

    //rma(tasks);
    //dma(tasks);


    cyclic(tasks);
    return 0;
}
