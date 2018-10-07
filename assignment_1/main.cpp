#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <regex>
using namespace std;


map<string, vector<string> > parse_grammar(string filename);

int main(int argc, char* argv[]){
    // There should be one additional argument beyond the program name, and
    // no more.
    if (argc != 2) {
        cerr << "Incorrect number of arguments provided." << endl;
        cerr << "usage: ./main <grammarfile>" << endl;
        return 1;
    }
    
    string infile_name(argv[1]);
    map<string, vector<string> > grammar;
    grammar = parse_grammar(infile_name);

    map<string, vector<string> >::const_iterator it;
    vector<string>::const_iterator iv;
    for (it = grammar.begin(); it != grammar.end(); it++) {
        cout << "Rule for " << it->first << endl;
        vector<string> value = it->second;
        int i = 0;
        for (iv = value.begin(); iv != value.end(); iv++) {
            cout << i << ": " << *iv << endl;
            i++;
        }
        cout << endl;
    }
}

map<string, vector<string> > parse_grammar(string filename) {
    ifstream file(filename);
    string line;
    map<string, vector<string> > result;
    while (getline(file, line)) {
        //istringstream iss(line);
        // Left brace means we start a new non-terminal
        if (line[0] == '{') {
            string key;
            vector<string> value;
            // The next line will be the name of the nonterminal
            getline(file, line);
            // We want just the nonterminal, without < and >
            line.erase(std::remove(line.begin(), line.end(), '<'), line.end());
            line.erase(std::remove(line.begin(), line.end(), '>'), line.end());
            key = string(line);
            // The following lines, until '}', will be expansions
            getline(file, line);
            while (line[0] != '}') {
                // Remove leading and trailing spaces, and ;
                line = regex_replace(line, regex("^ +| +$|( ) +"), "$1");
                line.erase(std::remove(line.begin(), line.end(), ';'), line.end());
                value.push_back(line);
                getline(file, line);
            }
            result[key] = value;
        }
    }
    return result;
}
