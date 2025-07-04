#include <iostream>
#include <vector>
#include "gf16.hpp"
#include "base64.hpp"
#include "util.hpp"

using namespace std;

#ifndef SAES_H
#define SAES_H

/**
 * @class SAES
 * @brief Implements the S-AES (Simplified AES) encryption and decryption algorithm.
 * 
 * @details S-AES operates on 16-bit plaintext and ciphertext blocks using a 16-bit key.
 * The algorithm includes SubNibbles (S-box substitution), ShiftRows (permutation), 
 * MixColumns (matrix multiplication in GF(2⁴)), AddRoundKey (XOR with round key), 
 * and key expansion with round constants. The class supports optional debugging output
 * and integrates with ECB mode through an external wrapper.
 */
class SAES {
public:

    int key;
    bool debug;
    bool ecb;
    
    /**
     * @brief Constructs an instance of the S-AES cipher with the specified key and flags.
     * 
     * @param key_ The 16-bit encryption key.
     * @param debug_ Optional flag to enable debugging output (default: false).
     * @param ecb_ Optional flag to indicate ECB mode usage (default: false).
     */
    SAES(int key_, bool debug_ = false, bool ecb_ = false) : key(key_), debug(debug_), ecb(ecb_) { 

    }

    /**
     * @brief Encrypts a 16-bit plaintext block using the S-AES algorithm.
     * 
     * @details The plaintext is processed through 2 encryption rounds:
     * - Round 0: AddRoundKey
     * - Round 1: SubNibbles, ShiftRows, MixColumns, AddRoundKey
     * - Round 2: SubNibbles, ShiftRows, AddRoundKey
     * 
     * Intermediate results may be displayed if debugging is enabled.
     * 
     * @param n The 16-bit plaintext block to encrypt.
     * @return The 16-bit ciphertext block resulting from encryption.
     */
    int encrypt(int n) {
        if(!ecb){
            cout << "\n====================== S-AES - Encryption ======================\n\n";
            cout << "Plaintext:         " << toBin(n, 16) << endl;
            cout << "Initial key:       " << toBin(key, 16) << "\n\n";
        }   

        vector<vector<int>> nibbles = split_to_nibbles(n);
        vector<vector<int>> key_nibbles = split_to_nibbles(this->key);

        if(debug) cout << "------------- Round 0 --------------\n\n";
        add_round_key(nibbles, key_nibbles);

        if(debug) cout << "------------- Round 1 --------------\n\n";
        sub_nibbles(nibbles);
        shift_rows(nibbles);
        mix_columns(nibbles);
        expand_key(key_nibbles, 1);
        add_round_key(nibbles, key_nibbles);

        if(debug) cout << "------------- Final Round (2) --------------\n\n";
        sub_nibbles(nibbles);
        shift_rows(nibbles);
        expand_key(key_nibbles, 2);
        add_round_key(nibbles, key_nibbles);

        // Converts the nibbles into a 16-bit integer (ciphertext)
        int ciphertext=0;
        for(int i=0; i < 2; i++){
            for(int j=0; j < 2; j++){
                ciphertext <<= 4;
                ciphertext |= nibbles[j][i];
            }
        }

        if(!ecb){
            cout << "-------------------------------------------------------------\n\n";
            cout << "-> Ciphertext\n\n";
            cout << "Bin:               ";
            print(nibbles);
            printf("\nHex:               %X\n", ciphertext);
            auto bytes = fromBinToVectorByte(toBin(nibbles));
            cout << "Base64:            " << Base64::convert_to(bytes);
            cout << "\n=============================================================\n\n";
        }

        return ciphertext;
    }

    /**
     * @brief Decrypts a 16-bit ciphertext block using the S-AES algorithm.
     * 
     * @details The ciphertext is processed through 2 decryption rounds (inverse operations):
     * - Round 2: AddRoundKey, InverseShiftRows, InverseSubNibbles
     * - Round 1: AddRoundKey, InverseMixColumns, InverseShiftRows, InverseSubNibbles
     * - Round 0: AddRoundKey
     * 
     * Round keys are expanded before decryption. Intermediate results may be shown if debugging is enabled.
     * 
     * @param n The 16-bit ciphertext block to decrypt.
     * @return The 16-bit plaintext block resulting from decryption.
     */
    int decrypt(int n){
        if(!ecb){
            cout << "\n====================== S-AES - Decryption ======================\n\n";
            cout << "Ciphertext:        " << toBin(n, 16) << endl;
            cout << "Initial key:       " << toBin(key, 16) << "\n\n";
        }

        vector<vector<int>> nibbles = split_to_nibbles(n);

        if(debug)
            cout << "------------- Key Generation --------------\n\n";
        vector<vector<int>> key0_nibbles = split_to_nibbles(this->key);
        vector<vector<int>> key1_nibbles = key0_nibbles;
        expand_key(key1_nibbles, 1);
        vector<vector<int>> key2_nibbles = key1_nibbles;
        expand_key(key2_nibbles, 2);
        if(debug){
            cout << "Key0 = ";
            print(key0_nibbles);
            cout << "\nKey1 = ";
            print(key1_nibbles);
            cout << "\nKey2 = ";
            print(key2_nibbles);
            cout << "\n\n";
        }

        if(debug) cout << "------------- Initial Round (2) --------------\n\n";
        add_round_key(nibbles, key2_nibbles);
        shift_rows(nibbles);
        sub_nibbles(nibbles, true);

        if(debug) cout << "------------- Round 1 --------------\n\n";
        add_round_key(nibbles, key1_nibbles);
        mix_columns(nibbles, true);
        shift_rows(nibbles);
        sub_nibbles(nibbles, true);

        if(debug) cout << "------------- Final Round (0) --------------\n\n";
        add_round_key(nibbles, key0_nibbles);

        // Converts the nibbles into a 16-bit integer (plaintext)
        int plaintext=0;
        for(int i=0; i < 2; i++){
            for(int j=0; j < 2; j++){
                plaintext <<= 4;
                plaintext |= nibbles[j][i];
            }
        }
        
        if(!ecb){
            cout << "-------------------------------------------------------------\n\n";
            cout << "-> Plaintext\n\n";
            cout << "Bin:               ";
            print(nibbles);
            printf("\nHex:               %X\n", plaintext);
            auto bytes = fromBinToVectorByte(toBin(nibbles));
            cout << "Base64:            " << Base64::convert_to(bytes);
            cout << "\n=============================================================\n\n";
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
    
    // XORs each nibble in the state matrix with the corresponding nibble in the round key
    void add_round_key(vector<vector<int>>& nibbles, vector<vector<int>>& key_vector){
        if(debug){ 
            cout << "Adding round key:\n= ";
            print(nibbles);
            cout << "  XOR  ";
            print(key_vector);
        }
        for(int i=0; i < 2; i++){
            for(int j=0; j < 2; j++){
                nibbles[i][j] ^= key_vector[i][j];
            }
        }
        if(debug){
            cout << "\n= ";
            print(nibbles);
            cout << "\n\n";
        }
    }
    
    // Applies the S-box substitution on a 4-bit nibble
    int apply_sbox(int nibble, bool decrypt = false){
        // Row 0: encryption S-box | Row 1: decryption S-box
        vector<vector<int>> sbox_nibble = {
            {9, 4, 10, 11, 13, 1, 8, 5, 6, 2, 0, 3, 12, 14, 15, 7},
            {10, 5, 9, 11, 1, 7, 8, 15, 6, 0, 2, 3, 12, 4, 13, 14}
        };
        return sbox_nibble[decrypt][nibble];
    }
    
    // Applies the S-box transformation to each nibble in the 2×2 state matrix
    // Uses encryption S-box by default; decryption if 'decrypt' is true
    void sub_nibbles(vector<vector<int>>& nibbles, bool decrypt = false){
        for(int i=0; i < 2; i++){
            for(int j=0; j < 2; j++){
                nibbles[i][j] = apply_sbox(nibbles[i][j], decrypt);
            }
        }
        if(debug){
            cout << "Substituting nibbles:\n= ";
            print(nibbles);
            cout << "\n\n";
        }
    }
    
    // Shift the second row (swapping the first and second nibble)
    void shift_rows(vector<vector<int>>& nibbles){
        swap(nibbles[1][0], nibbles[1][1]);
        if(debug){
            cout << "Shifting rows:\n= ";
            print(nibbles);
            cout << "\n\n";
        }
    }

    /*  Performs the MixColumns step on the 2×2 state matrix
        Multiplies the matrix by a fixed basis matrix in GF(2⁴)
        Uses different matrices for encryption and decryption 
    */
    void mix_columns(vector<vector<int>>& matrix, bool decrypt = false){
        vector<vector<int>> ans(2, vector<int>(2, 0));
        vector<vector<vector<int>>> mult={
            {{1, 4}, {4, 1}},  // encription matrix
            {{9, 2}, {2, 9}}   // decription matrix
        };
        for(int i=0; i < 2; i++){
            for(int j=0; j < 2; j++){
                for(int k=0; k < 2; k++){
                    ans[i][j] ^= GF.mul(mult[decrypt][i][k], matrix[k][j]);
                }
            }
        }
        swap(ans, matrix);
        if(debug){
            cout << "Mixing Columns:\n= ";
            print(matrix);
            cout << "\n\n";
        }
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
        if(debug){
            cout << "Expanding Key " << round-1 << " (";
            print(key_vector);
            cout << "):\nKey " << round << " = ";
        }
        
        vector<int> key1 = { key_vector[0][1], key_vector[1][1] };
        g_function(key1);

        // Add round constant: 2^(round+2) reduced mod the primitive polynomial.
        key1[0] ^= GF.mod(1 << (round+2));

        // First new column = previous column XOR g_function result
        for(int i=0; i < 2; i++) key_vector[i][0] ^= key1[i];

        // Second new column = first new column XOR previous column
        for(int i=0; i < 2; i++) key_vector[i][1] ^= key_vector[i][0];

        if(debug){
            print(key_vector);
            cout << "\n\n";
        }
    }
};

#endif