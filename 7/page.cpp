#include <iostream>
#include <vector>
#include <cstdlib> // for rand()
using namespace std;

void display(vector<int> &frames) {
    for (int x : frames)
        if (x == -1) cout << "- ";
        else cout << x << " ";
    cout << endl;
}

void fifo(vector<int> pages, int n, int f) {
    vector<int> frames(f, -1);
    int faults = 0, index = 0;
    cout << "\nFIFO Page Replacement:\n";
    for (int i = 0; i < n; i++) {
        bool hit = false;
        for (int j = 0; j < f; j++)
            if (frames[j] == pages[i]) hit = true;
        if (!hit) {
            frames[index] = pages[i];
            index = (index + 1) % f;
            faults++;
        }
        display(frames);
    }
    cout << "Total Page Faults: " << faults << endl;
}

void optimal(vector<int> pages, int n, int f) {
    vector<int> frames(f, -1);
    int faults = 0;
    cout << "\nOptimal Page Replacement:\n";
    for (int i = 0; i < n; i++) {
        bool hit = false;
        for (int j = 0; j < f; j++)
            if (frames[j] == pages[i]) hit = true;
        if (!hit) {
            int replace = -1, farthest = i+1;
            for (int j = 0; j < f; j++) {
                int k;
                for (k = i+1; k < n; k++)
                    if (frames[j] == pages[k]) break;
                if (k == n) { replace = j; break; } // not used again
                if (k > farthest) { farthest = k; replace = j; }
            }
            if (replace == -1) replace = 0;
            frames[replace] = pages[i];
            faults++;
        }
        display(frames);
    }
    cout << "Total Page Faults: " << faults << endl;
}

void mru(vector<int> pages, int n, int f) {
    vector<int> frames(f, -1);
    int faults = 0;
    int lastUsed[50] = {0};
    cout << "\nMRU Page Replacement:\n";
    for (int i = 0; i < n; i++) {
        bool hit = false;
        for (int j = 0; j < f; j++)
            if (frames[j] == pages[i]) {
                hit = true;
                lastUsed[j] = i;
            }
        if (!hit) {
            int replace = 0, maxUsed = -1;
            for (int j = 0; j < f; j++) {
                if (frames[j] == -1) { replace = j; break; }
                if (lastUsed[j] > maxUsed) {
                    maxUsed = lastUsed[j];
                    replace = j;
                }
            }
            frames[replace] = pages[i];
            lastUsed[replace] = i;
            faults++;
        }
        display(frames);
    }
    cout << "Total Page Faults: " << faults << endl;
}

void randomPage(vector<int> pages, int n, int f) {
    vector<int> frames(f, -1);
    int faults = 0;
    cout << "\nRandom Page Replacement:\n";
    for (int i = 0; i < n; i++) {
        bool hit = false;
        for (int j = 0; j < f; j++)
            if (frames[j] == pages[i]) hit = true;
        if (!hit) {
            int replace = rand() % f;
            frames[replace] = pages[i];
            faults++;
        }
        display(frames);
    }
    cout << "Total Page Faults: " << faults << endl;
}

int main() {
    int n, f, choice;
    vector<int> pages;

    cout << "Enter number of pages: ";
    cin >> n;
    cout << "Enter reference string:\n";
    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        pages.push_back(x);
    }
    cout << "Enter number of frames: ";
    cin >> f;

    do {
        cout << "\nPage Replacement Algorithms\n";
        cout << "1. FIFO\n2. Optimal\n3. MRU\n4. Random\n5. Exit\nEnter choice: ";
        cin >> choice;
        switch(choice) {
            case 1: fifo(pages, n, f); break;
            case 2: optimal(pages, n, f); break;
            case 3: mru(pages, n, f); break;
            case 4: randomPage(pages, n, f); break;
            case 5: cout << "Exiting...\n"; break;
        }
    } while(choice != 5);
    return 0;
}
