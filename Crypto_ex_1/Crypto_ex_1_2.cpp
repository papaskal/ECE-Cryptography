using namespace std;

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#define IENG 0.065
#define IRAND 0.038

char rollChar(unsigned char p, unsigned int k, char leftLim, char rightLim){
    unsigned char c = p + k;
    while (c > rightLim)
        c -= 26;
    while (c < leftLim)
        c += 26;

    return c;
}

string ceasarEncr(string msg, int k){
    string r = msg;
    for (auto i=0; i<msg.size(); i++){
        if (msg[i]>='A' && msg[i]<='Z'){
            r[i] = rollChar(msg[i], k, 'A', 'Z');
        }
        else if (msg[i]>='a' && msg[i]<='z'){
            r[i] = rollChar(msg[i], k, 'a', 'z');
        }
    }

    return r;
}

string vigenereEncr(string msg, string keyWord, bool encr){         //Encrypts for encr = true, decrypts for encr = false
    string r = msg;
    vector<int> keyInt;
    keyInt.resize(keyWord.size());
    for (auto i=0; i < keyWord.size(); i++){
        if (keyWord[i]>='A' && keyWord[i]<='Z')
            keyInt[i] = keyWord[i] - 'A';
        else if (keyWord[i]>='a' && keyWord[i]<='z')
            keyInt[i] = keyWord[i] - 'a';
        else 
            keyInt[i] = 0;

        if (!encr)
            keyInt[i] *= -1;
    }

    int k = 0;
    for (auto i=0; i<msg.size(); i++){
        if (msg[i]>='A' && msg[i]<='Z'){
            r[i] = rollChar(msg[i], keyInt[k], 'A', 'Z');
            k++;
            k %= keyInt.size();
        }
        else if (msg[i]>='a' && msg[i]<='z'){
            r[i] = rollChar(msg[i], keyInt[k], 'a', 'z');
            k++;
            k %= keyInt.size();
        } 
    }

    return r;
}

map<char, int> abFreq(string txt){
    map<char, int> freq;
    for (unsigned char c='A'; c<='Z'; c++)
        freq[c] = 0;

    for (char elem : txt)
        freq[elem]++;

    return freq;
}

double iC(string txt){
    map<char, int> freq = abFreq(txt);
    
    double sum = 0;
    for (auto elem : freq)
        sum += elem.second * (elem.second - 1);

    return sum / (txt.size() * (txt.size() - 1));
}

double iMC(string str1, string str2){
    map<char, int> freq1 = abFreq(str1);
    map<char, int> freq2 = abFreq(str2);
    
    double sum = 0;
    for (auto i='A'; i<='Z'; i++)
        sum += freq1[i] * freq2[i];
    
    return sum/(str1.size() * str2.size());
}

/*******************************************/


int main(int argc, char **argv){

    string txt = "VVTWZARYOORLVUGHRBPQFCFDDYWGFLSELQEVOEBTARTFTWLBVUUOLFXBPBSXDJHVAHTAIFUPNVZNTTLESEPRDPTIPZAGZSDQURPDHDQMNTAHTHILQMHJXIARYOVCMFUAHTSIGGVFBPVJKSLELAFCUDSTKGCAOGDLVGHNSEPRRVWTCBUGFTDZSSTVMISMCGVPAPEVNSRTECEPAOISMCGVPAPIAFZOAZVTCZMTYLVGSIQPZGADIAWVRXLREPZVUO";

    /******************* Step 1 ******************/
    cout << "STEP 1: Finding the most probable length r of the key" << endl << endl;
    cout << "We are going to test the IC of the text for various values of r" << endl;

    /*
    int r = (int) ((IENG - IRAND)/(iC(txt) - IRAND));       //This method is inaccurate in such short text
    cout << r << endl;
    */
    
    vector<string> columns;
    for (auto r=1; r<20; r++){
        columns.clear();
        columns.resize(r);
        cout << "r is " << r << endl;
        for (auto i=0; i<r; i++){
            int f = i;
            while (f < txt.size()){
                columns[i] += txt[f];
                f += r;
            }
            cout << iC(columns[i]) << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "We have the best overall IC values for r = 7" << endl << endl;


    /************************ Step 2 ************************/
    cout << "******************************" << endl << endl;
    cout << "STEP 2: Spliting the text in r columns and finding the most probable relative rolls of the columns to column 0" << endl << endl;

    int r = 7;
    cout << "r is " << r << endl;
    
    columns.clear();
    columns.resize(r);
    for (auto i=0; i<r; i++){
        int f = i;
        while (f < txt.size()){
            columns[i] += txt[f];
            f += r;
        }
    }

    vector<vector<pair<double, int>>> imcColumns;
    imcColumns.resize(r);

    for (auto i=0; i<26; i++){
        string test = ceasarEncr(columns[0], i);
        for (auto j=0; j<r; j++){
            //string s = test + columns[j];
            //imcColumns[j].push_back(make_pair(iC(s), i));
            imcColumns[j].push_back(make_pair(iMC(test, columns[j]), i));
        }
    }
 
    for (auto i=0; i<r; i++)
        sort(imcColumns[i].begin(), imcColumns[i].end());

    cout << "Best matches for relative rolls (in ASCENDING order)" << endl << endl;
    for (auto i=1; i<r; i++){
        cout << "Column " << i << ": ";
        for (auto j=0; j<26; j++)
            cout << imcColumns[i][j].second << " ";    
        cout << endl;
    }
    cout << endl;


    /************************** Step 3 *********************/
    cout << "******************************" << endl << endl;
    cout << "STEP 3: Trying out all of the 26 rolls for column 0 (and the respective rolls for the other columns)" << endl << endl;

    vector<string> columnsRes = columns;
    string key;
    key.resize(r);

    for (auto i='A'; i<='Z'; i++){
        for (auto j=0; j<r; j++){
            key[j] = i + imcColumns[j].back().second;
            while (key[j] > 'Z')
                key[j] -= 26;
        }
        cout << "Key is " << key << endl;
        cout << vigenereEncr(txt, key, false) << endl << endl;
    }

    /******************* Step 4 ****************************/
    cout << "******************************" << endl << endl;
    cout << "STEP 4: Correcting the key" << endl << endl;
    cout << "We read the resulting texts from the last step and see that the only one close to human speech is the one with key: CHOALIN" << endl;
    cout << "Combing through the text, we conclude that the keys for the columns 2 and 3 are probably wrong" << endl;
    cout << "We try out different values for those keys. We will use values from step 2" << endl;

    key = "CHOALIN";
    for (auto i=1; i<4; i++){                                                   
        key[2] = key[0] + imcColumns[2][imcColumns[2].size()-i].second;
        while (key[2] > 'Z')
            key[2] -= 26; 
        for (auto j=1; j<4; j++){
            key[3] = key[0] + imcColumns[3][imcColumns[3].size()-j].second;
            while (key[3] > 'Z')
                key[3] -= 26;

            cout << "Key is " << key << endl;
            cout << vigenereEncr(txt, key, false) << endl << endl;              //There's a bit of redundancy here. I could just change the appropriate columns
        }                                                                       //instead of decrypting the whole text every time. Still, it's fast enough so it doesn't matter
    }

    /***************** Step 5 ******************/
    cout << "******************************" << endl << endl;
    cout << "STEP 5: Presenting the decrypted text" << endl << endl;
    key = "CHAPLIN";
    cout << "Key is " << key << endl;
    cout << "Decrypted text is:" << endl;
    cout << vigenereEncr(txt, key, false) << endl << endl;
    cout << "********Celebrating************" << endl;

    return 0;
}


/*******************************************/
