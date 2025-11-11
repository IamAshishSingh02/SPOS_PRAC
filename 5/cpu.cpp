#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;

struct Process {
    int id, at, bt, ct, tat, wt, priority, rt;
};

void fcfs(Process p[], int n) {
    int time = 0;
    for(int i = 0; i < n; i++) {
        if(time < p[i].at) time = p[i].at;
        p[i].ct = time + p[i].bt;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        time = p[i].ct;
    }
    cout << "\nFCFS Scheduling:\n";
    cout << "PID\tAT\tBT\tCT\tTAT\tWT\n";
    for(int i = 0; i < n; i++)
        cout << p[i].id << "\t" << p[i].at << "\t" << p[i].bt << "\t" << p[i].ct
             << "\t" << p[i].tat << "\t" << p[i].wt << endl;
}

void sjf_preemptive(Process p[], int n) {
    int complete = 0, time = 0, min_bt, shortest = -1, finish_time;
    bool check = false;
    for(int i = 0; i < n; i++) p[i].rt = p[i].bt;
    while(complete != n) {
        min_bt = 9999;
        for(int i = 0; i < n; i++) {
            if(p[i].at <= time && p[i].rt > 0 && p[i].rt < min_bt) {
                min_bt = p[i].rt;
                shortest = i;
                check = true;
            }
        }
        if(!check) { time++; continue; }
        p[shortest].rt--;
        if(p[shortest].rt == 0) {
            complete++;
            finish_time = time + 1;
            p[shortest].ct = finish_time;
            p[shortest].tat = p[shortest].ct - p[shortest].at;
            p[shortest].wt = p[shortest].tat - p[shortest].bt;
        }
        time++;
        check = false;
    }
    cout << "\nSJF (Preemptive):\n";
    cout << "PID\tAT\tBT\tCT\tTAT\tWT\n";
    for(int i = 0; i < n; i++)
        cout << p[i].id << "\t" << p[i].at << "\t" << p[i].bt << "\t"
             << p[i].ct << "\t" << p[i].tat << "\t" << p[i].wt << endl;
}

void priority_non_preemptive(Process p[], int n) {
    int time = 0, completed = 0;
    bool done[10] = {false};
    cout << "\nPriority (Non-Preemptive):\n";
    while(completed != n) {
        int idx = -1, minPriority = 9999;
        for(int i = 0; i < n; i++) {
            if(p[i].at <= time && !done[i] && p[i].priority < minPriority) {
                minPriority = p[i].priority;
                idx = i;
            }
        }
        if(idx == -1) { time++; continue; }
        p[idx].ct = time + p[idx].bt;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        done[idx] = true;
        completed++;
        time = p[idx].ct;
    }
    cout << "PID\tAT\tBT\tPR\tCT\tTAT\tWT\n";
    for(int i = 0; i < n; i++)
        cout << p[i].id << "\t" << p[i].at << "\t" << p[i].bt << "\t"
             << p[i].priority << "\t" << p[i].ct << "\t"
             << p[i].tat << "\t" << p[i].wt << endl;
}

void round_robin(Process p[], int n, int quantum) {
    queue<int> q;
    int time = 0, completed = 0;
    for(int i = 0; i < n; i++) p[i].rt = p[i].bt;
    q.push(0);
    bool inQueue[10] = {false};
    inQueue[0] = true;
    while(!q.empty()) {
        int i = q.front(); q.pop();
        inQueue[i] = false;
        if(time < p[i].at) time = p[i].at;
        int exec = min(quantum, p[i].rt);
        p[i].rt -= exec;
        time += exec;
        for(int j = 0; j < n; j++)
            if(p[j].at <= time && p[j].rt > 0 && !inQueue[j]) {
                q.push(j);
                inQueue[j] = true;
            }
        if(p[i].rt > 0) {
            q.push(i);
            inQueue[i] = true;
        } else {
            p[i].ct = time;
            p[i].tat = p[i].ct - p[i].at;
            p[i].wt = p[i].tat - p[i].bt;
            completed++;
        }
    }
    cout << "\nRound Robin (Preemptive):\n";
    cout << "PID\tAT\tBT\tCT\tTAT\tWT\n";
    for(int i = 0; i < n; i++)
        cout << p[i].id << "\t" << p[i].at << "\t" << p[i].bt << "\t"
             << p[i].ct << "\t" << p[i].tat << "\t" << p[i].wt << endl;
}

int main() {
    Process p[10];
    int n, choice, quantum;
    cout << "Enter number of processes: ";
    cin >> n;
    for(int i = 0; i < n; i++) {
        cout << "Enter AT, BT, Priority for P" << i+1 << ": ";
        p[i].id = i+1;
        cin >> p[i].at >> p[i].bt >> p[i].priority;
    }

    do {
        cout << "\n\nCPU Scheduling Algorithms\n";
        cout << "1. FCFS\n2. SJF (Preemptive)\n3. Priority (Non-Preemptive)\n4. Round Robin\n5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: fcfs(p, n); break;
            case 2: sjf_preemptive(p, n); break;
            case 3: priority_non_preemptive(p, n); break;
            case 4: cout << "Enter Time Quantum: "; cin >> quantum; round_robin(p, n, quantum); break;
            case 5: cout << "Exiting...\n"; break;
        }
    } while(choice != 5);
}
