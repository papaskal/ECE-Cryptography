using namespace std;

#include <iostream>
#include <vector>


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


/*******************************************/


int main(int argc, char **argv){

    //Part 1 : Decryption
    cout << "*************** PART 1: Decryption ***************" << endl << endl;

    string s = "Nd Dhy. A dcmgv yk ccob xsieewa svptdwn os ptp Kqg, url gz wazwry vaffu jj t mgzogk tsi os xyextrm lmb hildcmzu. B plsgp plpz oq npw dci Otikigkb usklxc. Egi ahr lrdrd zh g rcr qg wvox zwx hglpsqzw bxrunubydo os wpextrm cgb cik?";
    string key = "cryptography";

    for (unsigned int i=0; i<26; i++){
        string newKey = ceasarEncr(key, i);
        string r = vigenereEncr(s, newKey, false);
        cout << "Rolled " << i << ", current key is assumed to be: " << newKey << endl;
        cout << r << endl << endl;
    }

    cout << "**************************" << endl << endl;

    cout << "After looking at the possible decryptions of the message, the only one that makes sense is the one with the key \"gvctxskvetlc\"" << endl;
    cout << "which is the word cryptography, encrypted with ceasar cipher with key = 4" << endl << endl;
    
    key = ceasarEncr("cryptography", 4);
    cout << "Alice's decrypted message:" << endl;
    cout << vigenereEncr(s, key, false) << endl << endl;


    //Part 2 : Encryption
    cout << "*************** PART 2: Encryption ***************" << endl << endl;
    
    //key = "gvctxskvetlc";
    string response = "I wonder if anyone will bother decrypting this message or reading the source code, hail Cthulu. If we want to make our messages impossible to decrypt (thus achieving perfect secrecy), we should use keys with length (at least) the length of our message and, in addition, not reuse the same key (or select a very lengthy key that can cover many messages of our correspondance and then change it when we reach its end). That way, the key becomes an unbreakable One Time Pad. Moreover, that key should be randomised so it can't be guessed and we should also eliminate punctuation, spaces, capital letters.";
    string encrResponse = vigenereEncr(response, key, true);
    
    cout << "Bob's response, encrypted in the same way as Alice's message:" << endl;
    cout << encrResponse << endl << endl;

    //cout << vigenereEncr(encrResponse, key, false) << endl;

    return 0;
}


/*******************************************/
