#include<bits/stdc++.h>
using namespace std;

struct Symbol {
    char name[10];
    int addr;
};

struct Literal {
    char value[10];
    int addr;
};

struct Opcode {
    char mnemonic[10];
    int code;
    char type[5];  // IS, AD, DL
};

int main() {
    ifstream fin("input.txt");
    ofstream fout("intermediate.txt");
    ofstream fsym("symtab.txt");
    ofstream flit("littab.txt");
    ofstream fpool("pooltab.txt");

    Symbol symtab[20];
    Literal littab[20];
    Opcode optab[] = {
        {"ADD", 1, "IS"}, {"MOVER", 4, "IS"}, {"START", 1, "AD"}, {"END", 2, "AD"}, {"LTORG", 5, "AD"},
        {"DS", 1, "DL"}, {"DC", 2, "DL"}
    };

    int lc = 0, symcount = 0, litcount = 0, pooltab[10], poolcount = 0;
    char word1[10], word2[10], word3[10];
    pooltab[poolcount++] = 0;

    if (!fin) {
        cout << "Error: input.txt not found!" << endl;
        return 0;
    }

    cout << "\n--- PASS 1 ---\n";

    while (!fin.eof()) {
        fin >> word1;
        if (strcmp(word1, "START") == 0) {
            fin >> word2;
            lc = atoi(word2);
            fout << "(AD,01)\t(C," << lc << ")\n";
            continue;
        }

        if (strcmp(word1, "END") == 0 || strcmp(word1, "LTORG") == 0) {
            // Assign addresses to all literals
            for (int i = pooltab[poolcount - 1]; i < litcount; i++) {
                littab[i].addr = lc;
                lc++;
            }
            fpool << pooltab[poolcount - 1] + 1 << endl;
            pooltab[poolcount++] = litcount;
            fout << "(AD," << (strcmp(word1, "END") == 0 ? "02" : "05") << ")\n";
            if (strcmp(word1, "END") == 0) break;
            continue;
        }

        // check if it is DS or DC (declarative)
        if (strcmp(word2, "DS") == 0) {
            strcpy(symtab[symcount].name, word1);
            symtab[symcount].addr = lc;
            symcount++;
            fout << "(DL,01)\t(C," << word3 << ")\n";
            lc += atoi(word3);
            continue;
        } else if (strcmp(word2, "DC") == 0) {
            strcpy(symtab[symcount].name, word1);
            symtab[symcount].addr = lc;
            symcount++;
            fout << "(DL,02)\t(C," << word3 << ")\n";
            lc++;
            continue;
        }

        // if mnemonic found in OPTAB
        for (int i = 0; i < 18; i++) {
            if (strcmp(word1, optab[i].mnemonic) == 0) {
                fout << "(" << optab[i].type << "," << optab[i].code << ")\t";
                fin >> word2 >> word3;
                fout << "(" << word2 << ")";
                if (word3[0] == '=') { // literal
                    strcpy(littab[litcount].value, word3);
                    littab[litcount].addr = -1;
                    fout << "(L," << litcount + 1 << ")\n";
                    litcount++;
                } else { // symbol
                    strcpy(symtab[symcount].name, word3);
                    symtab[symcount].addr = lc;
                    fout << "(S," << symcount + 1 << ")\n";
                    symcount++;
                }
                lc++;
                break;
            }
        }
    }

    // Write SYMTAB
    for (int i = 0; i < symcount; i++)
        fsym << symtab[i].name << "\t" << symtab[i].addr << endl;

    // Write LITTAB
    for (int i = 0; i < litcount; i++)
        flit << littab[i].value << "\t" << littab[i].addr << endl;

    fin.close();
    fout.close();
    fsym.close();
    flit.close();
    fpool.close();

    cout << "All tables generated successfully!" << endl;
    cout << "Check: symtab.txt, littab.txt, pooltab.txt, intermediate.txt\n";
    return 0;
}
