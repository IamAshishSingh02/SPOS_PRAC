#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    ifstream fin("input.txt");
    ofstream mnt("mnt.txt");
    ofstream mdt("mdt.txt");
    ofstream ala("ala.txt");
    ofstream intermediate("intermediate.txt");

    string line;
    int mdtp = 1;

    cout << "--- PASS 1 of Macro Processor ---\n";

    while (getline(fin, line)) {
        if (line == "MACRO") {
            getline(fin, line); // Macro prototype line
            string macroName = line.substr(0, line.find(" "));
            mnt << macroName << "\t" << mdtp << endl;

            // Extract and write ALA
            int pos = line.find(" ");
            string args = line.substr(pos + 1);
            int count = 1;
            for (int i = 0; i < args.length(); i++) {
                if (args[i] == '&') {
                    string param;
                    while (i < args.length() && args[i] != ',' && args[i] != ' ')
                        param += args[i++];
                    ala << "#" << count++ << "\t" << param << endl;
                }
            }

            // Copy body to MDT until MEND
            while (getline(fin, line) && line != "MEND") {
                for (int i = 0; i < line.length(); i++)
                    if (line[i] == '&') line[i] = '#';
                mdt << mdtp++ << "\t" << line << endl;
            }
            mdt << mdtp++ << "\tMEND\n";
        } else {
            // Write non-macro lines to intermediate file
            intermediate << line << endl;
        }
    }

    cout << "MNT, MDT, ALA, and Intermediate file generated successfully.\n";
    return 0;
}
