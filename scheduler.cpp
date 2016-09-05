//pranjalr34

#include<bits/stdc++.h>

#define PB push_back
#define MP make_pair
#define F first
#define S second
#define all(a) a.begin(),a.end()
#define SZ(a) (int)(a.size())
#define fi(i,a,b) for(int i=a;i<b;i++)
#define fd(i,a,b) for(int i=a;i>=b;i--)
#define n_p next_permutation
#define b_s binary_search
#define min(a,b) ((a<b)?a:b)
#define max(a,b) ((a<b)?b:a)
#define gcd __gcd
#define INF INT_MAX
#define init(a,b) memset(a,b,sizeof(a))
#define INFL LLONG_MAX
#define trace1(x)                cerr <<#x<<": "<<x<<endl;
#define trace2(x, y)             cerr <<#x<<": "<<x<<" | "<<#y<<": "<<y<< endl;
#define trace3(x, y, z)          cerr <<#x<<": "<<x<<" | "<<#y<<": "<<y<<" | "<<#z<<": "<<z<<endl;
#define trace4(a, b, c, d)       cerr <<#a<<": "<<a<<" | "<<#b<<": "<<b<<" | "<<#c<<": "<<c<<" | "<<#d<<": "<<d<<endl;
#define trace5(a, b, c, d, e)    cerr <<#a<<": "<<a<<" | "<<#b<<": "<<b<<" | "<<#c<<": "<<c<<" | "<<#d<<": "<<d<<" | "<<#e<<": "<<e<<endl;

using namespace std;
typedef pair<int,int> PII;
typedef vector<int> VI;
typedef vector< PII > VPII;
typedef long long int LL;
typedef pair<LL,LL> PLL;
typedef vector<LL> VLL;
typedef vector< PLL > VPLL;
typedef set<int> SI;
typedef set<LL> SLL;

void FastIO(){ios_base::sync_with_stdio(0);cin.tie(NULL);cout.tie(NULL);cout.precision(15);}
//LL modpow(LL a,LL b,LL m){LL r = 1;while (b > 0){if (b % 2 == 1)r = (r * a) % m;b = b >> 1;a = (a * a) % m;}return r%m;}
//LL power(LL a, LL p){LL ret=1;while(p){if(p&1)ret=(ret*a);a=(a*a);p/=2;}return ret;}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

//FCFS: First Come First Serve

vector<pair<int,pair<int,int> > >  FCFS(vector<int> &arrivalTime, vector<int> &processLength)
{
    int n=arrivalTime.size();
    int temp=arrivalTime[0];
    vector<pair<int,pair<int,int> > > gantt;
    for(int i=0;i<n;i++)
    {
        temp=max(temp,arrivalTime[i]);
        gantt.push_back(make_pair(i,make_pair(temp,temp+processLength[i])));
        temp+=processLength[i];
    }
    return gantt;
}

//SJS: Shortest Job First

vector<pair<int,pair<int,int> > >  SJS(vector<int> &arrivalTime, vector<int> &processLength)
{
    int n=arrivalTime.size();
    vector<int> processLengthCpy;
    processLengthCpy=processLength;
    vector<pair<int,pair<int,int> > > gantt;
    int time=arrivalTime[0],start=time,end,minTime,minProcess=0;
    int Process=0;
    while(1)  //while there is a process still in the queue
    {
        minTime=INT_MAX;
        for(int i=0;i<n;i++)
        {
            if(arrivalTime[i]<=time&&processLengthCpy[i]>0)
            {
                if(processLengthCpy[i]+time<minTime)
                {
                    minTime=processLengthCpy[i]+time;
                    minProcess=i;
                }
            }
        }
        if(minTime==INT_MAX)
        {
            gantt.push_back(make_pair(Process,make_pair(start,time)));
            break;
        }
        if(minProcess!=Process)
        {
            gantt.push_back(make_pair(Process,make_pair(start,time)));
            start=time;
            Process=minProcess;
        }
        processLengthCpy[Process]--;
        time++;
    }
    return gantt;
}

//Round Robin

vector<pair<int,pair<int,int> > >  RR(vector<int> &arrivalTime, vector<int> &processLength,int quantum)
{
    int n=arrivalTime.size();
    vector<int> processLengthCpy;
    processLengthCpy=processLength;
    vector<pair<int,pair<int,int> > > gantt;
    int Process=0,start=arrivalTime[0],cnt=0,time=start,idx=0;
    while(cnt<n)
    {
        if(arrivalTime[Process]>time)
        {
            int f=0;
            for(int i=idx;i<Process;i++)
            {       
                if(processLengthCpy[i]!=0)
                {
                    Process=i,idx=i,f=1;
                    break;
                }
            }
            if(!f)
                time=arrivalTime[Process];
        }
        if(processLengthCpy[Process]==0)
        {
            cnt++;
            Process=(Process+1)%n;
            continue;
        }
        cnt=0;
        start=time;
        int tmp=min(processLengthCpy[Process],quantum);
        time=time+tmp;
        processLengthCpy[Process]-=tmp;
        gantt.push_back(make_pair(Process,make_pair(start,time)));
        Process=(Process+1)%n;
    }
    return gantt;
}

//given the gantt chart calculate response time

double responseTime(int numProcess,vector<pair<int,pair<int,int> > > &ganttChart,vector<int> &arrivalTime)
{
    int n=ganttChart.size();
    vector<int> flag(numProcess,0);
    int ans=0,start,end,processID;
    for(int i=0;i<n;i++)
    {
        processID=ganttChart[i].first;
        start=ganttChart[i].second.first;
        end=ganttChart[i].second.second;
        if(!flag[processID])
        {
            flag[processID]=1;
            ans=ans+(start-arrivalTime[processID]);
        }
    }
    return ans/(1.0*numProcess);
}

//given the gantt chart calculate turnaround time

double turnaroundTime(int numProcess,vector<pair<int,pair<int,int> > > &ganttChart,vector<int> &arrivalTime)
{
    int n=ganttChart.size();
    vector<int> flag(numProcess,0);
    int ans=0,start,end,processID;
    //start from end of the gantt chart
    for(int i=n-1;i>=0;i--)
    {
        processID=ganttChart[i].first;
        start=ganttChart[i].second.first;
        end=ganttChart[i].second.second;
        if(!flag[processID])
        {
            flag[processID]=1;
            ans=ans+(end-arrivalTime[processID]);
        }
    }
    return ans/(1.0*numProcess);
}

//given the gantt chart calculate waiting time

double waitingTime(int numProcess,vector<pair<int,pair<int,int> > > &ganttChart,vector<int> &arrivalTime)
{
    int n=ganttChart.size();
    int ans=0,start,end,processID;
    vector<int> arrivalTimeCpy;
    arrivalTimeCpy=arrivalTime;
    for(int i=0;i<n;i++)
    {
        processID=ganttChart[i].first;
        start=ganttChart[i].second.first;
        end=ganttChart[i].second.second;
        ans=ans+(start-arrivalTimeCpy[processID]);
        arrivalTimeCpy[processID]=end;
    }
    return ans/(1.0*numProcess);
}

int main(int argc,char **argv)
{
    FastIO();
    if(argc<3)
        return 0;
    string filename(argv[1]);
    string timeQuantum_temp(argv[2]);
    int Quantum=stoi(timeQuantum_temp);
    ifstream fp;
    fp.open(filename.c_str());
    vector<int> arrivalTime,processLength;
    int x,y;
    while(fp>>x>>y)
    {
        arrivalTime.push_back(x);
        processLength.push_back(y);
    }

    int numProcess=arrivalTime.size();
    
    //FCFS
    //cout <<"FCFS"<<"\n";
    vector<pair<int,pair<int,int> > > ganttChartFCFS;
    ganttChartFCFS=FCFS(arrivalTime,processLength);
    //for(int i=0;i<ganttChartFCFS.size();i++)
    //    cout <<ganttChartFCFS[i].F<<" "<<ganttChartFCFS[i].S.F<<" "<<ganttChartFCFS[i].S.S<<"\n";

    double responseTimeFCFS=responseTime(numProcess,ganttChartFCFS,arrivalTime);
    //cout <<responseTimeFCFS<<"\n";

    double turnaroundTimeFCFS=turnaroundTime(numProcess,ganttChartFCFS,arrivalTime);
    //cout <<turnaroundTimeFCFS<<"\n";

    double waitingTimeFCFS=waitingTime(numProcess,ganttChartFCFS,arrivalTime);
    //cout <<waitingTimeFCFS<<"\n";

    //SJS
    //cout <<"SJS"<<"\n";
    vector<pair<int,pair<int,int> > > ganttChartSJS;
    ganttChartSJS=SJS(arrivalTime,processLength);
    //for(int i=0;i<ganttChartSJS.size();i++)
    //    cout <<ganttChartSJS[i].F<<" "<<ganttChartSJS[i].S.F<<" "<<ganttChartSJS[i].S.S<<"\n";
    double responseTimeSJS=responseTime(numProcess,ganttChartSJS,arrivalTime);
    //cout <<responseTimeSJS<<"\n";

    double turnaroundTimeSJS=turnaroundTime(numProcess,ganttChartSJS,arrivalTime);
    //cout <<turnaroundTimeSJS<<"\n";

    double waitingTimeSJS=waitingTime(numProcess,ganttChartSJS,arrivalTime);
    //cout <<waitingTimeSJS<<"\n";
      
    //RR
    //cout <<"RR"<<"\n";
    vector<pair<int,pair<int,int> > > ganttChartRR;
    ganttChartRR=RR(arrivalTime,processLength,Quantum);
    for(int i=0;i<ganttChartRR.size();i++)
        cout <<ganttChartRR[i].F<<" "<<ganttChartRR[i].S.F<<" "<<ganttChartRR[i].S.S<<"\n";
    double responseTimeRR=responseTime(numProcess,ganttChartRR,arrivalTime);
    //cout <<responseTimeRR<<"\n";

    double turnaroundTimeRR=turnaroundTime(numProcess,ganttChartRR,arrivalTime);
    //cout <<turnaroundTimeRR<<"\n";

    double waitingTimeRR=waitingTime(numProcess,ganttChartRR,arrivalTime);
    //cout <<waitingTimeRR<<"\n";
    
    vector<pair<double,int> > responseTime_vec,turnaroundTime_vec,waitingTime_vec;

    string arr[]={"FCFS","SJS","RR"};
    responseTime_vec.push_back({responseTimeFCFS,0});
    responseTime_vec.push_back({responseTimeSJS,1});
    responseTime_vec.push_back({responseTimeRR,2});
    turnaroundTime_vec.push_back({turnaroundTimeFCFS,0});
    turnaroundTime_vec.push_back({turnaroundTimeSJS,1});
    turnaroundTime_vec.push_back({turnaroundTimeRR,2});
    waitingTime_vec.push_back({waitingTimeFCFS,0});
    waitingTime_vec.push_back({waitingTimeSJS,1});
    waitingTime_vec.push_back({waitingTimeRR,2});
    sort(responseTime_vec.begin(),responseTime_vec.end());
    sort(turnaroundTime_vec.begin(),turnaroundTime_vec.end());
    sort(waitingTime_vec.begin(),waitingTime_vec.end());
    
    char c1=(char)40;
    char c2=(char)41;
    
    cout <<"ART:";
    for(int i=0;i<3;i++)
        cout <<" "<<arr[responseTime_vec[i].second]<<c1<<responseTime_vec[i].first<<c2;
    cout <<"\n";

    cout <<"ATT:";
    for(int i=0;i<3;i++)
        cout <<" "<<arr[turnaroundTime_vec[i].second]<<c1<<turnaroundTime_vec[i].first<<c2;
    cout <<"\n";

    cout <<"AWT:";
    for(int i=0;i<3;i++)
        cout <<" "<<arr[waitingTime_vec[i].second]<<c1<<waitingTime_vec[i].first<<c2;
    cout <<"\n";
    return 0;
}