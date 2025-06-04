#include <iostream>
#include <vector>

using namespace std;

struct GF_16{
    int primitive = 19;

    int add(int x, int y){
        return x ^ y;
    }

    int mod(int x){
        int ans = x & ((1 << 4) - 1);
        for(int i=4; i < 7; i++){
            if((x >> i) & 1) ans ^= mul(3, (1 << (i-4)));
        }
        return ans;
    }

    int mul(int x, int y){
        int ans=0;
        for(int i=0; i < 4; i++){
            if((x >> i) & 1) ans ^= (y << i);
        }
        return mod(ans);
    }
};

GF_16 GF;

vector<vector<int>> create_nibbles(int n){
    vector<vector<int>> nibbles(2, vector<int>(2));
    for(int i=1; i >= 0; i--){
        for(int j=1; j >= 0; j--){
            nibbles[j][i] = n & ((1 << 4) - 1);
            n >>= 4;
        }
    }
    return nibbles;
}

int substitute_nibble(int nibble){
    vector<int> sbox_nibble = {9, 4, 10, 11, 13, 1, 8, 5, 6, 2, 0, 3, 12, 14, 15, 7};
    return sbox_nibble[nibble];
}

void substitute_nibbles(vector<vector<int>>& nibbles){
    for(int i=0; i < 2; i++){
        for(int j=0; j < 2; j++){
            nibbles[i][j] = substitute_nibble(nibbles[i][j]);
        }
    }
}

void add_round_key(vector<vector<int>>& nibbles, vector<vector<int>>& key){
    for(int i=0; i < 2; i++){
        for(int j=0; j < 2; j++){
            nibbles[i][j] ^= key[i][j];
        }
    }
}

vector<vector<int>> mix_columns(vector<vector<int>>& matriz){
    vector<vector<int>> resultado(2, vector<int>(2, 0));
    vector<vector<int>> mult={{1, 4}, {4, 1}};
    for(int i=0; i < 2; i++){
        for(int j=0; j < 2; j++){
            for(int k=0; k < 2; k++){
                resultado[i][j] ^= GF.mul(mult[i][k], matriz[k][j]);
            }
        }
    }
    return resultado;
}

void g_function(vector<int>& w){
    swap(w[0], w[1]);
    for(int i=0; i < 2; i++) w[i] = substitute_nibble(w[i]);
}

void expand_key(vector<vector<int>>& key, int ordem){
    vector<int> key1 = {key[0][1], key[1][1]};
    g_function(key1);
    key1[0] ^= GF.mod(1 << ordem+2);
    for(int i=0; i < 2; i++) key[i][0] ^= key1[i];
    for(int i=0; i < 2; i++) key[i][1] ^= key[i][0];
}

void print(vector<vector<int>>& key){
    for(int i=0; i < 2; i++){
        for(int j=0; j < 2; j++){
            cout << key[j][i] << " ";
        }
    }
    cout << endl;
}


int encript(int n, int k){
    vector<vector<int>> nibbles = create_nibbles(n);
    vector<vector<int>> key = create_nibbles(k);
    add_round_key(nibbles, key);
    cout << "add_round_key : ";
    print(nibbles);
    substitute_nibbles(nibbles);
    cout << "sub_nibbles : ";
    print(nibbles);
    swap(nibbles[1][0], nibbles[1][1]);
    cout << "swap_nibbles : ";
    print(nibbles);
    nibbles = mix_columns(nibbles);
    cout << "mix_collumns : ";
    print(nibbles);
    expand_key(key, 1);
    add_round_key(nibbles, key);
    cout << "add_round_key : ";
    print(nibbles);
    substitute_nibbles(nibbles);
    cout << "sub_nibbles : ";
    print(nibbles);
    swap(nibbles[1][0], nibbles[1][1]);
    cout << "swap_nibbles : ";
    print(nibbles);
    expand_key(key, 2);
    add_round_key(nibbles, key);
    cout << "add_round_key : ";
    print(nibbles);
    int ciphertext=0;
    for(int i=0; i < 2; i++){
        for(int j=0; j < 2; j++){
            ciphertext <<= 4;
            ciphertext |= nibbles[j][i];
        }
    }
    return ciphertext;
}