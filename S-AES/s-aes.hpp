#include <iostream>
#include <vector>
#include "gf16.hpp"

using namespace std;

#ifndef SAES_H
#define SAES_H

class SAES {
public:

    int key;
    bool debug;
    
    SAES(int key_, bool debug_ = false) : key(key_), debug(debug_) { 

    }

    int encript(int n) {

        vector<vector<int>> nibbles = split_to_nibbles(n);
        vector<vector<int>> key_nibbles = split_to_nibbles(this->key);
        add_round_key(nibbles, key_nibbles);
        // cout << "add_round_key : ";
        // print(nibbles);
        sub_nibbles(nibbles);
        // cout << "sub_nibbles : ";
        // print(nibbles);
        swap(nibbles[1][0], nibbles[1][1]);
        // cout << "swap_nibbles : ";
        // print(nibbles);
        nibbles = mix_columns(nibbles);
        // cout << "mix_collumns : ";
        // print(nibbles);
        expand_key(key_nibbles, 1);
        add_round_key(nibbles, key_nibbles);
        // cout << "add_round_key : ";
        // print(nibbles);
        sub_nibbles(nibbles);
        // cout << "sub_nibbles : ";
        // print(nibbles);
        swap(nibbles[1][0], nibbles[1][1]);
        // cout << "swap_nibbles : ";
        // print(nibbles);
        expand_key(key_nibbles, 2);
        add_round_key(nibbles, key_nibbles);
        // cout << "add_round_key : ";
        // print(nibbles);
        int ciphertext=0;
        for(int i=0; i < 2; i++){
            for(int j=0; j < 2; j++){
                ciphertext <<= 4;
                ciphertext |= nibbles[j][i];
            }
        }
        return ciphertext;

    }

    int decript(int n){
        vector<vector<int>> nibbles = split_to_nibbles(n);
        vector<vector<int>> key0_nibbles = split_to_nibbles(this->key);
        vector<vector<int>> key1_nibbles = key0_nibbles;
        expand_key(key1_nibbles, 1);
        vector<vector<int>> key2_nibbles = key1_nibbles;
        expand_key(key2_nibbles, 2);

        add_round_key(nibbles, key2_nibbles);
        // cout << "add_round_key : ";
        // print(nibbles);
        swap(nibbles[1][0], nibbles[1][1]);
        // cout << "swap_nibbles : ";
        // print(nibbles);
        sub_nibbles(nibbles, true);
        // cout << "sub_nibbles : ";
        // print(nibbles);
        add_round_key(nibbles, key1_nibbles);
        // cout << "add_round_key : ";
        // print(nibbles);
        nibbles = mix_columns(nibbles, true);
        // cout << "mix_collumns : ";
        // print(nibbles);
        swap(nibbles[1][0], nibbles[1][1]);
        // cout << "swap_nibbles : ";
        // print(nibbles);
        sub_nibbles(nibbles, true);
        // cout << "sub_nibbles : ";
        // print(nibbles);
        add_round_key(nibbles, key0_nibbles);
        // cout << "add_round_key : ";
        // print(nibbles);
        int plaintext=0;
        for(int i=0; i < 2; i++){
            for(int j=0; j < 2; j++){
                plaintext <<= 4;
                plaintext |= nibbles[j][i];
            }
        }
        return plaintext;
    }

    
private:

    GF_16 GF; // Instance of GF(2⁴) field operations with primitive x⁴ + x + 1

    /*  Converts a 16-bit integer into a 2x2 matrix of 4-bit values (nibbles)
        The input integer is split into four nibbles, starting from the most significant
        Layout:
            nibbles[0][0] ← most significant nibble (bits 15-12)
            nibbles[1][0] ← bits 11-8
            nibbles[0][1] ← bits 7-4
            nibbles[1][1] ← least significant nibble (bits 3-0) 
    */
    vector<vector<int>> split_to_nibbles(int n){
        vector<vector<int>> nibbles(2, vector<int>(2));
        // Extract 4-bit chunks from least to most significant
        for(int i=1; i >= 0; i--){
            for(int j=1; j >= 0; j--){
                nibbles[j][i] = n & 0b1111; // Get the last 4 bits
                n >>= 4; // Shift right to get the next nibble
            }
        }
        return nibbles;
    }

    // Applies the S-box substitution on a 4-bit nibble
    int apply_sbox(int nibble, bool decript = false){
        // Row 0: encryption S-box | Row 1: decryption S-box
        vector<vector<int>> sbox_nibble = {
            {9, 4, 10, 11, 13, 1, 8, 5, 6, 2, 0, 3, 12, 14, 15, 7},
            {10, 5, 9, 11, 1, 7, 8, 15, 6, 0, 2, 3, 12, 4, 13, 14}
        };
        return sbox_nibble[decript][nibble];
    }

    // Applies the S-box transformation to each nibble in the 2×2 state matrix
    // Uses encryption S-box by default; decryption if 'decrypt' is true
    void sub_nibbles(vector<vector<int>>& nibbles, bool decript = false){
        for(int i=0; i < 2; i++){
            for(int j=0; j < 2; j++){
                nibbles[i][j] = apply_sbox(nibbles[i][j], decript);
            }
        }
    }

    // XORs each nibble in the state matrix with the corresponding nibble in the round key
    void add_round_key(vector<vector<int>>& nibbles, vector<vector<int>>& key_vector){
        for(int i=0; i < 2; i++){
            for(int j=0; j < 2; j++){
                nibbles[i][j] ^= key_vector[i][j];
            }
        }
    }

    /*  Performs the MixColumns step on the 2×2 state matrix
        Multiplies the matrix by a fixed basis matrix in GF(2⁴)
        Uses different matrices for encryption and decryption 
    */
    vector<vector<int>> mix_columns(vector<vector<int>>& matrix, bool decript = false){
        vector<vector<int>> ans(2, vector<int>(2, 0));
        vector<vector<vector<int>>> mult={{{1, 4}, {4, 1}}, {{9, 2}, {2, 9}}};
        for(int i=0; i < 2; i++){
            for(int j=0; j < 2; j++){
                for(int k=0; k < 2; k++){
                    ans[i][j] ^= GF.mul(mult[decript][i][k], matrix[k][j]);
                }
            }
        }
        return ans;
    }

    /*  Applies the g-function from the S-AES key expansion:
        - Rotates the 2-byte word (swap)
        - Applies the S-box to both nibbles
    */
    void g_function(vector<int>& w){
        swap(w[0], w[1]);
        for(int i=0; i < 2; i++) w[i] = apply_sbox(w[i]);
    }

    /*
        Expands the round key using the g-function and a round constant.
        'round' is the current round number (0-based).
    */
    void expand_key(vector<vector<int>>& key_vector, int round){
        vector<int> key1 = { key_vector[0][1], key_vector[1][1] };
        g_function(key1);

        // Add round constant: 2^(round+2) reduced mod the primitive polynomial.
        key1[0] ^= GF.mod(1 << (round+2));

        // First new column = previous column XOR g_function result
        for(int i=0; i < 2; i++) key_vector[i][0] ^= key1[i];

        // Second new column = first new column XOR previous column
        for(int i=0; i < 2; i++) key_vector[i][1] ^= key_vector[i][0];
    }

    void print(vector<vector<int>>& key_vector){
        for(int i=0; i < 2; i++){
            for(int j=0; j < 2; j++){
                cout << key_vector[j][i] << " ";
            }
        }
        cout << endl;
    }

};

#endif