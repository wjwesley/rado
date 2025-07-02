#include <iostream>
#include <set>
#include <unordered_set>
#include <random> 
#include <fstream>
#include <string> 
#include <sstream> 
#include <cmath>
#include <bits/stdc++.h>
#include <bitset>
using namespace std;

void removeLeadingZeros(vector<int>& v){
        int i = v.size()-1; 
        while ( i > 0 && v[i] ==0){
            v.pop_back(); 
            i--; 
        }
}

class IntPoly{
    private: 
    vector<int> coeffs; //written in reverse order: {3,4,5} is 3+4a+5a^2

    public: 
    IntPoly(vector<int> v = {})
    {
        // vector<int> w = v;
        // removeLeadingZeros(w);
        coeffs = v; 
    }

    

    int degree() const{
        return coeffs.size()-1; 
    }

    int leadingCoefficient() const{
        return coeffs[coeffs.size()-1];
    }

    int eval(int a){
        int total = 0; 
        for (int i = 0; i < coeffs.size() ; i++){
            total += coeffs[i]*pow(a,i); 
        }
        return total; 
    }

    bool isRoot(int a){
        return this->eval(a) == 0; 
    }

    IntPoly multiplyX(){
        vector<int> v = {0}; 
        for(int i = 0; i < coeffs.size(); i++){
            v.push_back(coeffs[i]); 
        }
        removeLeadingZeros(v); 
        return IntPoly(v); 
    }

    IntPoly multiplyXPower(int k){
        vector<int> v; 
        for(int i = 0; i < k; i++){
            v.push_back(0);
        }
        for(int i = 0; i < coeffs.size(); i++){
            v.push_back(coeffs[i]); 
        }
        removeLeadingZeros(v); 
        return IntPoly(v); 
    }

    /*
        {1,3} = 1 + 3x
        {1,3,0} = 1 + 3x 
    */
    
    IntPoly operator+(IntPoly const& obj)
    {
        vector<int> v; 
        int d = degree();  
        int d2 = obj.degree(); 
        for(int i = 0; i <= min(d,d2); i++){
            v.push_back(this->coeffs[i] + obj.coeffs[i]); 
        }
        for (int i = min(d,d2)+1; i <= max(d,d2); i++){
            int c = d > d2 ? this->coeffs[i] : obj.coeffs[i]; 
            v.push_back(c);
        }
        removeLeadingZeros(v); 
        IntPoly res(v); 
        return res;
    }

    IntPoly operator-(IntPoly const& obj)
    {
        vector<int> v; 
        int d = degree();  
        int d2 = obj.degree(); 
        for(int i = 0; i <= min(d,d2); i++){
            v.push_back(this->coeffs[i] - obj.coeffs[i]); 
        }
        for (int i = min(d,d2)+1; i <= max(d,d2); i++){
            int c = d > d2 ? this->coeffs[i] : -1*obj.coeffs[i]; 
            v.push_back(c);
        }
        removeLeadingZeros(v); 
        IntPoly res(v); 
        return res;
    }

    IntPoly operator*(int a){
        vector<int> v; 
        for (int i = 0; i < coeffs.size(); i++){
            v.push_back(a*coeffs[i]);
        }
        removeLeadingZeros(v); 
        IntPoly res(v); 
        return res; 
    }

    bool operator==(IntPoly const& p){
        return coeffs == p.coeffs; 
    }

    void print() { 
        if (coeffs.size() ==1){
            cout << coeffs[0] << endl;
            return; 
        }
        cout << coeffs[0] << "+"; 
        for (int i = 1; i < coeffs.size()-1; i++){
            cout << coeffs[i] << "x^" << i << " + "; 
        }
        cout << coeffs[coeffs.size()-1] << "x^" << coeffs.size()-1 << endl; 
     }

    IntPoly divideByFactor(IntPoly p){
        int plc = p.leadingCoefficient();
        int pdeg = p.degree();
        IntPoly dividend = *this; 
        IntPoly zero({0});
        vector<int> q;
        //int debugSteps = 0;
        while (!(dividend == zero)){
        // while (debugSteps <= 5){
            //cout << "dividend = "; 
            //dividend.print(); 
            int ddeg = dividend.degree();
            int dlc = dividend.leadingCoefficient();
            IntPoly s = p.multiplyXPower(ddeg-pdeg);
            //cout << "s = ";
            //s.print();
            dividend = dividend - s*(dlc/plc);
            q.push_back(dlc/plc);
           // debugSteps++;
        }
        reverse(q.begin(),q.end());
        return IntPoly(q);
    }
    };

bool properBound(IntPoly p, IntPoly l, IntPoly u, int aLower, int aUpper){ 
    //returns true if and only if  l(a) <= p(a) <= u(a) for all a with aLower <= a <= aUpper 
    for (int a = aLower; a <= aUpper; a++){
        int pa = p.eval(a); 
        int la = l.eval(a); 
        int ua = u.eval(a); 
        if (pa < la || pa > ua){
            return false; 
        }
    }
    return true;  
}

vector<IntPoly> bruteForceCubic(vector<int> L, vector<int> U){ //generates all integer coefficient cubic polynomials where the coefficient of x^i is bounded by L[i] and U[i] 
    vector<IntPoly> S;
    vector<int> v = {0,0,0,0};
    for (int i0 = L[0]; i0 <= U[0]; i0++){
        for (int i1 = L[1]; i1 <= U[1]; i1++){
            for (int i2 = L[2]; i2 <= U[2]; i2++){
                for (int i3 = L[3]; i3 <= U[3]; i3++){
                    v[0] = i0;
                    v[1] = i1;
                    v[2] = i2;
                    v[3] = i3; 
                    IntPoly p = IntPoly(v); 
                    vector<int> w = {0};
                    IntPoly q(w); 
                    IntPoly r = q+p; 
                    //r.print();
                    S.push_back(r);
                }
            }
        }
    }
    return S; 
}

int main(){
    IntPoly f({24,42,27,18,15,6});
    IntPoly g({4,5,2});
    f.divideByFactor(g).print();

//Upper bound: goal is to show that R_3(a(x-y) = (a+1)z) <= a^3+5a^2+8a for a>=7
//Generation tests: generate a set U of polynomials that

//*are bounded between 1 and the conjectured Rado number a^3 + 5a^2 + 8a for all a >= 7
//*do this by calling properBound

//Problem: How to generate U? 
/*Naive way: brute force: generate ALL polynomials with coefficients bounded by some constant, e.g. 
    1,2,3,4,a,2a,3a,4a,a+1,2a+1,3a+1,4a+1,a+2,2a+2,... 

    can do this with bruteForceCubic method

    problem: get too many that aren't "useful" i.e. they don't appear in solutions to a(x-y) = (a+1)z 
*/

/*Alternative: generate many polynomials that appear in solutions together. 

    if we have a set S of polynomials, then take all possible pairs (x,y) and look at the LHS of the equation

    a(x-y) is another polynomial. 
    
    If this is divisible by a+1, then we can divide by a+1 and get a new polynomial z which we can add to our set S. 

    Example: If x = 2a^2 + 2, y = 2a^2 - a + 1, then a(x-y) = a(a+1), so then z = a gives a solution, and we can add the polynomial z = a to S

    iterate some number of times to get more solutions
*/

/*Experiments: 

brute force with L = {-5,-5,-5,0}, U = {5,5,5,1} gave some conflicts during SAT solve 
alternative method starting with smaller brute force set (L = {0,-1,-3,0}, U = {0,1,3,1}) gave zero conflicts (easy) 
alternative method starting with "relevant" polynomials (multiples of a^2 {0,0,1,0}, {0,0,2,0},{0,0,3,0},..., multiples of a, a^3+5a^2+8a, etc.) also gave zero conflicts 

Want to find sets with many conflicts in the SAT solver output (harder to solve) 
The Rado number itself should always be in the set (otherwise instance is necessarily satisfiable) 

*/

//*enumerate the solutions to a(x-y) = (a+1)z where x,y,z are polynomials (in the variable a) in the set U 
//*want UNSATISFIABLE cnf file 


    //brute force a set of polynomials S 
    vector<IntPoly> S = bruteForceCubic({0,-1,-3,0},{0,1,3,1});

    //throw out polynomials in S that are not bounded properly 
    vector<IntPoly> T; 
    for (IntPoly p : S){ 
         if (properBound(p,IntPoly({1,0,0,0}),IntPoly({0,8,5,1}),10,100)){
            T.push_back(p);
         }
    }

    //conjectured rado number upper bound polynomial 
    vector<int> radoUB = {0,8,5,1};

    //some polynomials that appear in the proof 
    vector<vector<int>> relevant = {{0,8,5,1},{-1,8,5,1},{-2,8,5,1},{-3,8,5,1},{0,7,5,1},{0,6,5,1},{0,5,5,1},{0,8,4,1},{0,0,1,0},{0,0,2,0},{0,0,3,0},{0,0,4,0},{0,1,0,0},{0,2,0,0}
,{0,3,0,0},{0,0,1,1},{0,0,2,1},{0,0,3,1},{1,3,3,1},{1,2,3,1}};

    for (auto p : relevant){
        if (properBound(p,IntPoly({1,0,0,0}),IntPoly({0,8,5,1}),10,100))
        U.push_back(IntPoly(p));
    }
    
    
    


    /* "alternative method" iteratively generate more polynomials that appear in solutions and add to the set U 
    */
    
    int iter = 0;
    int maxIter = 2;
    while (iter < maxIter){
        vector<IntPoly> newU = U;
        for (int i = 0; i < U.size(); i++){
            for (int j = 0; j < U.size(); j++){
                //cout << "i j = " << i << " " << j << endl; 
                IntPoly x = U[i];
                IntPoly y = U[j];
                //x.print(); 
                //y.print(); 
                IntPoly LHS = (x-y).multiplyX();
                if (LHS.isRoot(-1)){
                    IntPoly z = LHS.divideByFactor(IntPoly({1,1}));
                    if (properBound(z,IntPoly({1,0,0,0}),IntPoly({0,8,5,1}),10,100)){ //whenever we add z to the set, need to check that it's bounded correctly 
                        newU.push_back(z);
                    }
                }
               
            }
        }
        U = newU; 
        cout << "U size: " << U.size() << endl; 
        iter++;
    }

     //test different possibilities for the set U


    /* should make separate method
    Given a set U, generate the CNF file for the SAT solver 

    Currently a bit inefficient (should be able to rewrite so that it runs in quadratic rather than cubic time)
    Should instead iterate over all pairs (x,y), then check if divisible by a+1, then check if a(x-y)/(a+1) belongs to the set U 
    */

    ofstream cnffile("redDiagTest2.cnf"); 
    cnffile << "p cnf " << U.size()*3 << " " << 1 << endl;  

    for (int i = 0; i < U.size(); i++){
        //cout << i << endl; 
        cnffile << 3*(i+1) << " " << 3*(i+1) - 1 << " " << 3*(i+1) -2 << " 0" << endl; //pos clauses
        for (int j = 0; j < U.size(); j++){
            for (int k = 0; k < U.size(); k++){
                IntPoly x = U[i]; 
                IntPoly y = U[j];
                IntPoly z = U[k];
                if ((x-y).multiplyX() == z.multiplyX()+z){
                    // cout << "sol: " << endl;
                    // x.print();
                    // y.print();
                    // z.print();
                    for (int c = 0; c <=2; c++){
                        cnffile << -1*(3*(i+1)-c) << " " << -1*(3*(j+1)-c) << " " << -1*(3*(k+1)-c) << " 0" << endl; 
                    }
                }
            }
        }
    }


    cnffile.close(); 
    return 0; 
}
