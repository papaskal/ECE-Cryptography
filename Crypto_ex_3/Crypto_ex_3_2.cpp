using namespace std;

#include <iostream>
#include <fstream>
#include <vector>

#define RADIUS  127.5
#define DADIUS  255
#define SADIUS  16256.25

class prCircle{
    private:
        unsigned long long int succ = 0, total = 0;
        unsigned int x = 0, y = 0; 
        int cnt = 0;
        vector<vector<bool>> circle;
    
    public:
        prCircle(const vector<vector<bool>> &circle){
            this->circle = circle;
        }

        unsigned long long int getSucc(){
            return succ;
        }

        double getApprox(){
            return double (4 * succ) / total;
        }

        void addPoint(unsigned int x, unsigned int y){
            total++;
            if (circle[x][y] == true)
                succ++;
        }

        void incBit(unsigned int bitOut){
            if (cnt < 8)
                x = (x << 1) + bitOut;
            else {
                y = (y << 1) + bitOut;
                if (cnt == 15){
                    addPoint(x,y);
                    x = y = 0;
                    cnt = -1;
                }
            }
            cnt++;
        }
};

vector<vector<bool>> calcInside();

void bbs(unsigned long long int, unsigned long long int, unsigned long long int, const vector<vector<bool>> &);

unsigned long long int powerModulo(unsigned long long int a, unsigned long long int n, unsigned long long int m);

unsigned long long int squareMod(unsigned long long int, unsigned long long int);

unsigned long long int mulMod(unsigned long long int a, unsigned long long int b, unsigned long long int n);

unsigned int xOR(unsigned long long int);

/*******************************************/


int main(int argc, char **argv){
    //freopen("output.txt","w",stdout);
    
    const unsigned long long int p = 600239;  
    const unsigned long long int q = 600983;                        //n = p*q = 360733434937
    const unsigned long long int seed = squareMod(20749, p*q);      //seed = 430521001;

    vector<vector<bool>> circle;
    circle = calcInside();

    bbs(p, q, seed, circle);
    return 0;
}


/*******************************************/


vector<vector<bool>> calcInside(){
    vector<vector<bool>> result;

    result.resize(DADIUS + 1);
    for (auto i=0; i<result.size(); i++)
        result[i].resize(DADIUS + 1);

    for (auto i=0; i<result.size(); i++)
        for (auto j=0; j<result[i].size(); j++)
            result[i][j] = false;


    int cnt = 0;
    for (auto i=0; i<result.size(); i++)
        for (auto j=0; j<result[i].size(); j++)
            if (i*i + j*j + SADIUS <= DADIUS * (i+j)){
                result[i][j] = true;
                cnt++;
            }
    
    double total = 256 * 256;
    cout << "There are " << cnt << " points in the circle, out of a total of " << total << " points in the square.\n";
    cout << "The best possible approximation for pi is " << ((double) (cnt * 4))/(double) total << "\n\n";
/*
    for (auto i=0; i<result.size(); i++){
        for (auto j=0; j<result[i].size(); j++)
            cout << result[i][j];
        cout << "\n";
    }
*/
    return result;
}


unsigned long long int calcS(unsigned long long int s, unsigned long long int n, unsigned long long int lambdaN, unsigned long long int i){     //calculates s(i) for given s(0), n, lambda(n), i
    return powerModulo(s, powerModulo(2, i, lambdaN), n);
}

unsigned int xOR(unsigned long long int x){
    return __builtin_popcount(x) & 1;
}

void bbs(unsigned long long int p, unsigned long long int q, unsigned long long int seed, const vector<vector<bool>> &circle){

    unsigned long long int k = 0, n = p*q, aceCnt = 0, aceCnt2 = 0, succ = 0, succ2 = 0;
    unsigned int bitOut = 0, bitOut2 = 0;
    
    prCircle cir1(circle);
    prCircle cir2(circle);

    seed %= n;

    cout << "p: " << p << "\tq: " << q << "\tn = p*q: " << n << " \tseed: " << seed << "\n\n";
    //cout << calcS(seed, n, 180366116858, 0) << " " << calcS(seed, n, 180366116858, 45091228910) << endl;    //lambda(n) = 180366116858, lambda(lambda(n)) = 45091228910
    
    unsigned long long int s = seed;
    
    do{
        bitOut = s & 1;
        //cout << bitOut;
        bitOut2 = xOR(s);
        //cout << bitOut2;
        aceCnt += bitOut;
        aceCnt2 += bitOut2;
        
        
        cir1.incBit(bitOut);
        cir2.incBit(bitOut2);
        succ = cir1.getSucc();
        succ2 = cir2.getSucc();
        
           
        
        if (k % 10000000 == 0) {
            cout << "k: " << k << "\ts: " << s << "\t aceCnt: " << aceCnt << "\t succ: " << succ << "\t  approx: " << (double) (64 * succ) / k; 
            cout << "\t aceCnt2: " << aceCnt2 << "\t succ2: " << succ2 << "\t  approx2: " << (double) (64 * succ2) / k << "\n";
        }

        k++;
        s = squareMod(s, n);        //s = (s*s) % n; 
        //if (k == 1000000000) break;
    }
    while (s != seed);

    cout << "k: " << k << "\ts: " << s << " (same as original seed)\n";
    cout << "Final results (k = : " << k-1 << ")\t aceCnt: " << aceCnt << "\t succ: " << succ << "\t  approx: " << (double) (64 * succ) / k; 
    cout << "\t aceCnt2: " << aceCnt2 << "\t succ2: " << succ2 << "\t  approx2: " << (double) (64 * succ2) / k << "\n";


}

unsigned long long int powerModulo(unsigned long long int a, unsigned long long int n, unsigned long long int m){
    unsigned long long int x = a % m;
    unsigned long long int y = 1;
    
    while (n > 0) {
        if (n % 2 != 0)
            y = mulMod(x, y, m);
        x = squareMod(x, m);
        n = n / 2;
    }
    return y;
}



unsigned long long int mulMod(unsigned long long int a, unsigned long long int b, unsigned long long int n){           //a*b mod n, avoiding overflow
    const unsigned long long int safe = 0x7FFFFF;
    if (a <= safe || b <= safe)
        return a*b % n;
    
    return (2 * mulMod(a, b/2, n) + (b % 2) * a) % n;
}



unsigned long long int squareMod(unsigned long long int a, unsigned long long int n){           //a*a mod n, avoiding overflow
    const static unsigned long long int safe = 0x7FFFFFFF;
    if (a <= safe)
        return (a*a) % n;

    int b = a % 2;
    
    return ((4 * squareMod(a/2, n) + b * (2*a - 1)) % n);    
}