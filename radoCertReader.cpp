#include <iostream>
#include <set>
#include <unordered_set>
#include <random> 
#include <fstream>
#include <string> 
#include <sstream> 
#include <cmath>
using namespace std;

void printVec(vector<int> S){
    cout << '{';
    for (auto i : S) {
        cout << i << ' ';
    }
    cout << '}' << endl; 
}

vector<vector<int>> getColorClasses(vector<int> cert, int k){
    vector<vector<int>> colors; 
    for (int i = 0; i < k ; i++){
        colors.push_back({}); 
    }
    for (int i = 0; i < cert.size(); i++){
        if (cert[i] > 0){
            int color = i % k;
            colors[color].push_back(i/k+1);
        }
    }
    // for (int i = 0; i < k ; i++){
    //     printVec(colors[i]); 
    // }
    return colors; 
}

vector<int> processCertificateFromOutput(string filename){
    ifstream file(filename); 
    vector<int> cert; 
    for (string line;getline(file,line);){
        //cout << line[0] << endl; 
        if (line[0] == 's'){
            cout << line << endl; 
        }
        if (line[0] == 'v'){
            line.erase(0,1); 
            //cout << line << endl; 
            stringstream ss(line);
            for (int i = 0; ss >> i;){
                //cout << i << endl;
                cert.push_back(i); 
            }
        }
    }
    //printVec(cert); 
    return cert; 
}

void blockCert(vector<int> cert, string filename){
    ofstream file(filename,ios_base::app); 
    for (int i = 0; i < cert.size(); i++){
        file << to_string(-1*cert[i]) << " "; 
    }
    file << "\n";
    file.close(); 
}

void printColorClasses(vector<vector<int>> colors){
    for (int i = 0; i< colors.size(); i++){
        cout << "Color " + to_string(i+1) + ":"; 
        printVec(colors[i]); 
    }
}

int main(int argc, char** argv){
    //cout << argv[1] << " " << argv[2] << endl;
    string filename = argv[1]; 
    int k = stoi(argv[2]); 
    vector<int> cert = processCertificateFromOutput(filename); 
    vector<vector<int>> colors = getColorClasses(cert,k); 
    printColorClasses(colors);
    // if (argc >= 3){
    //     string cnfFile = argv[3]; 
    //     blockCert(cert,cnfFile); 
    // }
    return 0; 
}