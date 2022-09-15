#include <iostream>
#include <fstream>
#include <algorithm> 
#include <iomanip>
#include <queue>
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
    int tq;
    struct process p[100];
    float cpuEfficiency;
    float avgTAT;
    float avgWT;
    int totalTAT = 0;
    int totalWT = 0;
    int totalIT = 0;
    int totalET = 0;
    int totalST = 0;
    int remainingBurst[100];
    int idx;

    cout << setprecision(2) << fixed;

    ifstream infile;
    infile.open("input.txt");
    if (!infile) {
        cout << "Error";
        return 0;
    }

    cout << "Number of Processes: ";
    infile >> n;
    cout << n << endl;

    cout << "Enter time quantum: ";
    cin >> tq;
    cout << endl;

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
            remainingBurst[i] = p[i].burstTime;
            cout << p[i].burstTime << endl;
            cout << endl;
        }
    }

    sort(p, p + n, compareArrival);

    queue<int> q;
    q.push(0);
    int current = 0;
    int completed = 0;
    int mark[100];
    memset(mark, 0, sizeof(mark));
    mark[0] = 1;

    while (completed != n) {
        idx = q.front();
        q.pop();
        totalST += 5;

        if (remainingBurst[idx] == p[idx].burstTime) {
            p[idx].startTime = max(current, p[idx].arrivalTime);
            totalIT += p[idx].startTime - current;
            current = p[idx].startTime + 5;
        }

        if (remainingBurst[idx] - tq > 0) {
            remainingBurst[idx] -= tq;
            current += tq;
        }
        else {
            current += remainingBurst[idx];
            remainingBurst[idx] = 0;
            completed++;

            p[idx].completionTime = current;
            p[idx].turnaroundTime = p[idx].completionTime - p[idx].arrivalTime;
            p[idx].waitingTime = p[idx].turnaroundTime - p[idx].burstTime;

            totalTAT += p[idx].turnaroundTime;
            totalWT += p[idx].waitingTime;
        }

        for (int i = 1; i < n; i++) {
            if (remainingBurst[i] > 0 && p[i].arrivalTime <= current && mark[i] == 0) {
                q.push(i);
                mark[i] = 1;
            }
        }

        if (remainingBurst[idx] > 0) {
            q.push(idx);
        }

        if (q.empty()) {
            for (int i = 1; i < n; i++) {
                if (remainingBurst[i] > 0) {
                    q.push(i);
                    mark[i] = 1;
                    break;
                }
            }
        }
    }

    totalET = current + totalST + totalIT;
    avgTAT = (float)totalTAT / n;
    avgWT = (float)totalWT / n;
    cpuEfficiency = (totalET / float(totalET + totalST)) * 100;

    sort(p, p + n, compareID);

    cout << endl;
    cout << "PID\t" << "AT\t" << "BT\t" << "ST\t" << "CT\t" << "TAT\t" << "WT\t" << "\n" << endl;

    for (int i = 0; i < n; i++) {
        cout << p[i].pid << "\t" << p[i].arrivalTime << "\t" << p[i].burstTime << "\t" << p[i].startTime << "\t" << p[i].completionTime << "\t" << p[i].turnaroundTime << "\t" << p[i].waitingTime << "\n" << endl;
    }

    cout << "Round Robin" << endl << endl;
    cout << "Time Quantum: " << tq << endl;
    cout << "Total Execution Time: " << totalET << endl;
    cout << "Average Turnaround Time: " << avgTAT << endl;
    cout << "Average Waiting Time: " << avgWT << endl;
    cout << "CPU Efficiency: " << cpuEfficiency << "%" << endl;

    system("PAUSE");
    return 0;
}