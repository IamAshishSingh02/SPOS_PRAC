#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

struct Symbol {
    char name[10];
    int addr;
};

struct Literal {
    char value[10];
    int addr;
};

int main() {
    ifstream fin("intermediate.txt");
    ifstream fsym("symtab.txt");
    ifstream flit("littab.txt");
    ofstream fmc("machinecode.txt");

    Symbol symtab[20];
    Literal littab[20];
    int symcount = 0, litcount = 0;

    if (!fin || !fsym || !flit) {
        cout << "Error: One or more input files missing!" << endl;
        return 0;
    }

    // Load Symbol Table
    while (fsym >> symtab[symcount].name >> symtab[symcount].addr)
        symcount++;

    // Load Literal Table
    while (flit >> littab[litcount].value >> littab[litcount].addr)
        litcount++;

    cout << "\n--- PASS 2 ---\n";
    cout << "Generating machine code...\n";

    string word;

    while (getline(fin, word)) {
        if (word.find("(AD,") != string::npos) {
            // Assembler directives — skip them
            continue;
        } 
        else if (word.find("(IS,") != string::npos) {
            // Instruction statement
            size_t pos = word.find("(IS,");
            string opcode = word.substr(pos + 4, 2); // extract opcode

            fmc << opcode << "\t"; // write opcode

            // Register part
            pos = word.find("(");
            pos = word.find(",", pos);
            string reg = word.substr(pos + 1, 4);
            fmc << reg << "\t";

            // Symbol or Literal part
            if (word.find("(S,") != string::npos) {
                int index = word[word.find("(S,") + 3] - '0';
                fmc << symtab[index - 1].addr;
            } 
            else if (word.find("(L,") != string::npos) {
                int index = word[word.find("(L,") + 3] - '0';
                fmc << littab[index - 1].addr;
            } 
            else if (word.find("(C,") != string::npos) {
                int value = 0;
                sscanf(word.c_str(), "%*[^C]C,%d", &value);
                fmc << value;
            }

            fmc << endl;
        } 
        else if (word.find("(DL,") != string::npos) {
            // Declarative statement (e.g., DS, DC)
            int value = 0;
            sscanf(word.c_str(), "%*[^C]C,%d", &value);
            fmc << "00\t00\t" << value << endl;
        }
    }

    fin.close();
    fsym.close();
    flit.close();
    fmc.close();

    cout << "Machine code generated successfully in machinecode.txt\n";
    return 0;
}
