#include <iostream>
#include <map>
#include <vector>
#include <fstream>
using namespace std;

static void readFile(ifstream& infile, vector<string>& lines);

int main(int argc, char* argv[]) {
    // make a map
    // map<string, int> mymap;
    // mymap.insert(make_pair(string("apple"), 400));
    cout << "hello" << endl;
    ifstream *x = NULL;
    vector<string> l;
    readFile(*x, l);
    return 0;
}

static void readFile(ifstream& infile, vector<string>& lines){
    assert(lines.size() == 0);
    assert(infile.good());

    string line;
    while (infile.peek() != EOF) {
        getline(infile, line); //reassign line to be the next line of file.
        lines.push_back(line); //append
    }

    cout << "Done! (Number of lines: " << lines.size() << ")" << endl;
}
