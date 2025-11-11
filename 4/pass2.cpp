#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct MNT {
    string name;
    int mdtp;
};

struct MDT {
    string def;
};

struct ALA {
    string index;
    string arg;
};

int main() {
    ifstream fin("macro_input.txt");
    ifstream fmnt("mnt.txt");
    ifstream fmdt("mdt.txt");
    ofstream fout("expanded_output.txt");

    MNT mnt[10];
    MDT mdt[20];
    ALA ala[10];
    string line;

    int mntCount = 0, mdtCount = 0;

    // Load MNT
    while (fmnt >> mnt[mntCount].name >> mnt[mntCount].mdtp)
        mntCount++;

    // Load MDT
    while (getline(fmdt, line))
        if (!line.empty()) mdt[mdtCount++].def = line;

    cout << "--- PASS 2 of Macro Processor ---\n";

    while (getline(fin, line)) {
        if (line == "MACRO") {
            // Skip macro definition (already processed)
            while (getline(fin, line) && line != "MEND");
            continue;
        }

        string word;
        stringstream ss(line);
        ss >> word;

        bool isMacro = false;
        int macroIndex = -1;

        // Check if line is macro invocation
        for (int i = 0; i < mntCount; i++) {
            if (word == mnt[i].name) {
                isMacro = true;
                macroIndex = i;
                break;
            }
        }

        if (isMacro) {
            // Read actual arguments
            string args[5];
            int argCount = 0;
            string temp;
            while (getline(ss, temp, ',')) {
                // Remove spaces
                if (temp[0] == ' ') temp = temp.substr(1);
                args[argCount++] = temp;
            }

            // Expand macro using MDT
            int j = mnt[macroIndex].mdtp - 1;
            while (mdt[j].def.find("MEND") == string::npos) {
                string expanded = mdt[j].def;

                // Replace #1, #2 with actual arguments
                for (int k = 0; k < argCount; k++) {
                    string key = "#" + to_string(k + 1);
                    size_t pos = expanded.find(key);
                    if (pos != string::npos)
                        expanded.replace(pos, key.length(), args[k]);
                }

                fout << expanded << endl;
                j++;
            }
        } else {
            // Not a macro call → copy as is
            fout << line << endl;
        }
    }

    cout << "Macro expansion done. Check expanded_output.txt\n";

    fin.close();
    fmnt.close();
    fmdt.close();
    fout.close();

    return 0;
}
