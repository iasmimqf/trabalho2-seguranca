#include <vector>
#include <string>

using namespace std;

#ifndef UTILS_HPP
#define UTILS_HPP

vector<int> get_vector_slice(vector<int> bytes, int l, int size) {
    auto result = vector<int>(bytes.begin() + l, bytes.begin() + l + size);
    return result;
}

vector<int> get_bytes_from_text(string str) {
    return vector<int>(str.begin(), str.end());
}

// convert an integer to a binary string
string toBin(int n, int sz){
    string bits = "";
    for(int i=sz-1; i >= 0; i--){
        bits += (char)(((n >> i) & 1) + '0');
    } 
    return bits;
}

// convert a nibble matrix to a binary string
string toBin(vector<vector<int>>& nibble_matrix){
    string bits="";
    for(int i=0; i < 2; i++){
        for(int j=0; j < 2; j++){
            bits += toBin(nibble_matrix[j][i], 4);
        }
    }
    return bits;
}

// print a nibble matrix as a binary string
void print(vector<vector<int>>& nibble_matrix){
    for(int i=0; i < 2; i++){
        for(int j=0; j < 2; j++){
            cout << toBin(nibble_matrix[j][i], 4) << " ";
        }
    }
}

// convert a binary string to an integer
int toInt(string bits){
    int num=0;
    for(int i=0; i < (int)bits.size(); i++){
        if(bits[i] == '1') num = (num << 1) + 1;
        else if(bits[i] == '0') num <<= 1;
    } 
    return num;
}

// convert a binary string of 16 bits to a vector of bytes
vector<int> fromBinToVectorByte(string bits){
    int bite1, bite2;
    bite1 = toInt(bits.substr(0, 8));
    bite2 = toInt(bits.substr(8, 8));
    vector<int> ans = {bite1, bite2};
    return ans;
}


#endif