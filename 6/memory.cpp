#include <iostream>
using namespace std;

void firstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[10];
    for (int i = 0; i < n; i++) allocation[i] = -1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                break;
            }
        }
    }

    cout << "\nProcess No.\tProcess Size\tBlock No.\n";
    for (int i = 0; i < n; i++) {
        cout << i+1 << "\t\t" << processSize[i] << "\t\t";
        if (allocation[i] != -1)
            cout << allocation[i]+1;
        else
            cout << "Not Allocated";
        cout << endl;
    }
}

void bestFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[10];
    for (int i = 0; i < n; i++) allocation[i] = -1;

    for (int i = 0; i < n; i++) {
        int bestIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx])
                    bestIdx = j;
            }
        }
        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blockSize[bestIdx] -= processSize[i];
        }
    }

    cout << "\nProcess No.\tProcess Size\tBlock No.\n";
    for (int i = 0; i < n; i++) {
        cout << i+1 << "\t\t" << processSize[i] << "\t\t";
        if (allocation[i] != -1)
            cout << allocation[i]+1;
        else
            cout << "Not Allocated";
        cout << endl;
    }
}

void worstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[10];
    for (int i = 0; i < n; i++) allocation[i] = -1;

    for (int i = 0; i < n; i++) {
        int worstIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx])
                    worstIdx = j;
            }
        }
        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            blockSize[worstIdx] -= processSize[i];
        }
    }

    cout << "\nProcess No.\tProcess Size\tBlock No.\n";
    for (int i = 0; i < n; i++) {
        cout << i+1 << "\t\t" << processSize[i] << "\t\t";
        if (allocation[i] != -1)
            cout << allocation[i]+1;
        else
            cout << "Not Allocated";
        cout << endl;
    }
}

void nextFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[10];
    for (int i = 0; i < n; i++) allocation[i] = -1;

    int j = 0; // start from last allocated position
    for (int i = 0; i < n; i++) {
        int count = 0;
        while (count < m) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                break;
            }
            j = (j + 1) % m;
            count++;
        }
    }

    cout << "\nProcess No.\tProcess Size\tBlock No.\n";
    for (int i = 0; i < n; i++) {
        cout << i+1 << "\t\t" << processSize[i] << "\t\t";
        if (allocation[i] != -1)
            cout << allocation[i]+1;
        else
            cout << "Not Allocated";
        cout << endl;
    }
}

int main() {
    int m, n, blockSize[10], processSize[10], choice;
    cout << "Enter number of blocks: ";
    cin >> m;
    cout << "Enter block sizes:\n";
    for (int i = 0; i < m; i++) cin >> blockSize[i];
    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter process sizes:\n";
    for (int i = 0; i < n; i++) cin >> processSize[i];

    do {
        cout << "\nMemory Allocation Strategies\n";
        cout << "1. First Fit\n2. Best Fit\n3. Worst Fit\n4. Next Fit\n5. Exit\nEnter choice: ";
        cin >> choice;

        int tempBlock[10];
        for (int i = 0; i < m; i++) tempBlock[i] = blockSize[i]; // reset blocks

        switch(choice) {
            case 1: firstFit(tempBlock, m, processSize, n); break;
            case 2: bestFit(tempBlock, m, processSize, n); break;
            case 3: worstFit(tempBlock, m, processSize, n); break;
            case 4: nextFit(tempBlock, m, processSize, n); break;
            case 5: cout << "Exiting...\n"; break;
        }
    } while(choice != 5);
    return 0;
}
