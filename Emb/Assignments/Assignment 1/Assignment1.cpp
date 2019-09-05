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
    Node(ll arrivalTime , ll executionTime, ll period, ll deadline, ll quantum ,  ll id)
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
            tasks.push_back(Node(arr[0],arr[1],arr[2],arr[3],arr[4],n+1));
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
    fifo(tasks);
    lifo(tasks);

    round_robin(tasks);
    weighted_round_robin(tasks);

    return 0;
}
