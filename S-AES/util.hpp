#include <vector>
#include <string>

using namespace std;

#ifndef UTILS_HPP
#define UTILS_HPP

/**
 * @brief Extracts a slice (subvector) from a vector of integers.
 *
 * @param bytes The source vector.
 * @param l The starting index of the slice.
 * @param size The length of the slice.
 * @return A vector containing the slice bytes[l..l+size-1].
 */
vector<int> get_vector_slice(vector<int> bytes, int l, int size) {
    auto result = vector<int>(bytes.begin() + l, bytes.begin() + l + size);
    return result;
}

/**
 * @brief Converts a string to a vector of bytes (integers).
 *
 * Each character's ASCII value is converted to an integer in the vector.
 *
 * @param str The input string.
 * @return Vector of integer byte values corresponding to the characters.
 */
vector<int> get_bytes_from_text(string str) {
    return vector<int>(str.begin(), str.end());
}

/**
 * @brief Converts an integer to its binary representation as a string of fixed size.
 *
 * The binary string is padded with leading zeros to match the requested size.
 *
 * @param n Integer to convert.
 * @param sz Number of bits in the output string.
 * @return Binary string representing the integer n with length sz.
 */
string toBin(int n, int sz){
    string bits = "";
    for(int i=sz-1; i >= 0; i--){
        bits += (char)(((n >> i) & 1) + '0');
    } 
    return bits;
}

/**
 * @brief Converts a 2x2 matrix of 4-bit nibbles into a concatenated binary string.
 *
 * The nibbles are concatenated in column-major order.
 *
 * @param nibble_matrix 2x2 matrix of integers (each 4 bits).
 * @return A binary string of length 16 representing all nibbles.
 */
string toBin(vector<vector<int>>& nibble_matrix){
    string bits="";
    for(int i=0; i < 2; i++){
        for(int j=0; j < 2; j++){
            bits += toBin(nibble_matrix[j][i], 4);
        }
    }
    return bits;
}

/**
 * @brief Prints the 2x2 nibble matrix as binary strings, each nibble separated by space.
 *
 * @param nibble_matrix 2x2 matrix of integers (each 4 bits).
 */
void print(vector<vector<int>>& nibble_matrix){
    for(int i=0; i < 2; i++){
        for(int j=0; j < 2; j++){
            cout << toBin(nibble_matrix[j][i], 4) << " ";
        }
    }
}

/**
 * @brief Converts a binary string to its integer value.
 *
 * The string must contain only '0' and '1'.
 *
 * @param bits Binary string.
 * @return Integer corresponding to the binary number.
 */
int toInt(string bits){
    int num=0;
    for(int i=0; i < (int)bits.size(); i++){
        if(bits[i] == '1') num = (num << 1) + 1;
        else if(bits[i] == '0') num <<= 1;
    } 
    return num;
}

/**
 * @brief Concatenates a vector of integers into a single integer.
 *
 * Each integer is assumed to represent 8 bits (byte). The resulting integer
 * has bytes concatenated from left to right.
 *
 * @param bits Vector of integers representing bytes.
 * @return Single integer concatenating all bytes.
 */
int toInt(vector<int>& bits){
    int num = 0;
    for (int val : bits) {
        num <<= 8;         // Make room for the next 8 bits
        num |= (val & 0xFF); // Mask to ensure only 8 bits are used
    }
    return num;
}

/**
 * @brief Converts a 16-bit binary string into a vector of two bytes (integers).
 *
 * The input string must have length 16.
 *
 * @param bits Binary string of length 16.
 * @return Vector with two integers representing the high and low byte.
 */
vector<int> fromBinToVectorByte(string bits){
    int bite1, bite2;
    bite1 = toInt(bits.substr(0, 8));
    bite2 = toInt(bits.substr(8, 8));
    vector<int> ans = {bite1, bite2};
    return ans;
}


#endif