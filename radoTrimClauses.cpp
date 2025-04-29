#include <iostream>
#include <fstream>
#include <string> 
#include <sstream> 
#include <cmath>
using namespace std;

void radoTrim(string oldFilename, string newFilename, int n, int k){ //write only clauses that use integers up to n from oldFilename to newFilename
    ifstream oldfile(oldFilename); 
    ofstream newfile(newFilename);
    newfile << "p cnf " << n*k << " 1\n";
    for (string line;getline(oldfile,line);){
            stringstream ss(line);
            bool keepLine = true; 
            if (line[0] == 'p'){
                keepLine = false; 
            }
            for (int i = 0; ss >> i;){
                if (abs(i) > n*k){
                    keepLine = false; 
                    //newfile << i << " ";
                }
            }
            if (keepLine){
                newfile << line << endl; 
            }
        
    }
}

int main(int argc, char** argv){
    radoTrim(argv[1],argv[2],stoi(argv[3]),stoi(argv[4]));
}

