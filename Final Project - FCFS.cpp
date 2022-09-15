#include <iostream>
#include <fstream>
#include <algorithm> 
#include <iomanip> 
using namespace std;

struct process {
    int pid;
    int arrivalTime;
    int burstTime;
    int startTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
};

bool compareArrival(process p1, process p2) {
    return p1.arrivalTime < p2.arrivalTime;
}

bool compareID(process p1, process p2) {
    return p1.pid < p2.pid;
}

int main() {
    int n;
    struct process p[100];
    float cpuEfficiency;
    float avgTAT;
    float avgWT;
    int totalTAT = 0;
    int totalWT = 0;
    int totalIT = 0;
    int totalET = 0;
    int totalST = 0;

    cout << setprecision(2) << fixed;

    ifstream infile;
    infile.open("input.txt");
    if (!infile) {
        cout << "Error";
        return 0;
    }

    cout << "Number of Processes: ";
    infile >> n;
    cout << n << endl << endl;

    while (!infile.eof()) {   
        for (int i = 0; i < n; i++) {   
            //p[i].pid = i + 1; 
            infile >> p[i].pid;
            //cout << "Arrival Time of Process " << i + 1 << ": ";       
            cout << "Process " << i + 1 << " Arrival Time: ";
            infile >> p[i].arrivalTime;
            cout << p[i].arrivalTime << endl;
            //cout << "Burst Time of Process " << i + 1 << ": ";
            cout << "Process " << i + 1 << " Burst Time: ";
            infile >> p[i].burstTime;
            cout << p[i].burstTime << endl;;
            cout << endl;
            totalET += p[i].burstTime;
        }
    }

    sort(p, p + n, compareArrival);

    for (int i = 0; i < n; i++) {
        p[i].startTime = (i == 0) ? p[i].arrivalTime : max(p[i - 1].completionTime, p[i].arrivalTime);
        p[i].completionTime = p[i].startTime + p[i].burstTime;
        p[i].turnaroundTime = p[i].completionTime - p[i].arrivalTime;
        p[i].waitingTime = p[i].turnaroundTime - p[i].burstTime;

        totalST += 5;
        totalTAT += p[i].turnaroundTime;
        totalWT += p[i].waitingTime;
        totalIT += (i == 0) ? (p[i].arrivalTime) : (p[i].startTime - p[i - 1].completionTime);
    }

    totalET = totalET + totalST + totalIT;
    avgTAT = (float)totalTAT / n;
    avgWT = (float)totalWT / n;
    cpuEfficiency = (totalET / float(totalET + totalST)) * 100;

    sort(p, p + n, compareID);

    cout << endl;
    cout << "PID\t" << "AT\t" << "BT\t" << "ST\t" << "CT\t" << "TAT\t" << "WT\t" << "\n" << endl;

    for (int i = 0; i < n; i++) {
        cout << p[i].pid << "\t" << p[i].arrivalTime << "\t" << p[i].burstTime << "\t" << p[i].startTime << "\t" << p[i].completionTime << "\t" << p[i].turnaroundTime << "\t" << p[i].waitingTime << "\n" << endl;
    }
    
    cout << "First Come First Serve" << endl << endl;
    cout << "Total Execution Time: " << totalET << endl;
    cout << "Average Turnaround Time: " << avgTAT << endl;
    cout << "Average Waiting Time: " << avgWT << endl;
    cout << "CPU Efficiency: " << cpuEfficiency << "%" << endl;

    system("PAUSE");
    return 0;
}