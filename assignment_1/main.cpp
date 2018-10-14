#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <regex>
#include <random>
using namespace std;


map<string, vector<string> > parse_grammar(string filename);
string fill_nonterminal(string nonterminal, map<string, vector<string> > grammar);

int main(int argc, char* argv[]){
    // There should be one additional argument beyond the program name, and
    // no more.
    if (argc != 2) {
        cerr << "Incorrect number of arguments provided." << endl;
        cerr << "usage: ./main <grammarfile>" << endl;
        return 1;
    }
    
    // Read in the infile and parse the grammar.
    string infile_name(argv[1]);
    map<string, vector<string> > grammar;
    grammar = parse_grammar(infile_name);

    map<string, vector<string> >::const_iterator it;
    vector<string>::const_iterator iv;
    // Recursively generate a sentence starting with nonterminal <start>.
    string random_sentence = fill_nonterminal("start", grammar);
    cout << random_sentence << endl;
}

map<string, vector<string> > parse_grammar(string filename) {
    ifstream file(filename);
    string line;
    map<string, vector<string> > result;
    // Iterate through the file.
    while (getline(file, line)) {
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
                // Remove leading and trailing spaces, and ';'.
                line = regex_replace(line, regex("^ +| +$|( ) +"), "$1");
                line.erase(std::remove(line.begin(), line.end(), ';'), line.end());
                value.push_back(line);
                getline(file, line);
            }
            // "Save" the rule in the map.
            result[key] = value;
        }
    }
    return result;
}

string fill_nonterminal(string nonterminal, map<string, vector<string> > grammar) {
    // First check that the nonterminal is in the grammar.
    assert(grammar.count(nonterminal) == 1);
    // Get the productions for the nonterminal.
    vector<string> productions = grammar[nonterminal];
    vector<string>::const_iterator iv;
    string result;
    // Choose a production at random.
    int n_productions = productions.size();
    // Only used once to initialise (seed) engine.
    std::random_device rd;
    // Random-number engine (Mersenne-Twister in this case).
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0,n_productions-1);
    int choice = uni(rng);
    string prod = productions[choice];
    // Iterate through the production and deal with nonterminals as we find them.
    string::const_iterator it;
    for (it = prod.begin(); it != prod.end(); it++ ) {
        if (*it == '<') {
        // If we encounter a '<', we need to parse a nonterminal.
            string nonterm;
            // Catch all the letters of the nonterminal.
            it++;
            while (*it != '>') {
                nonterm += *it;
                it++;
            }
            // Call fill_nonterminal (recursion!) on this nonterminal.
            result += fill_nonterminal(nonterm, grammar);
        } else {
            // If what we found wasn't a nonterminal, just print it and move on.
            result += *it;
        }
    }
    return result;
}
