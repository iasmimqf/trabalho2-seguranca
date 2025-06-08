#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

#ifndef BASE64_HPP
#define BASE64_HPP

class Base64 {
public:

    static inline const string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


    /*  Converts a vector of bytes (integers 0-255) to a Base64-encoded string.
        Each byte is first converted to its 8-bit binary representation and concatenated into a bit stream.
        The bit stream is then split into 6-bit segments, each mapped to a Base64 character using the alphabet.
        If the final segment is less than 6 bits, it is padded with zeros.
        The output string is padded with '=' characters to ensure its length is a multiple of 4.
    */

    static string convert_to(const vector<int>& bytes) {
        string output = "", stream = "";

        for(auto &num : bytes) {
            stream += get_binary_string(num);
        }

        while((int)stream.size() % 6 != 0) {
            stream += "0";
        }

        for(int i = 0; i < (int)stream.size(); i += 6) {
            string segment = stream.substr(i, 6);
            int index = stoi(segment, nullptr, 2);
            output += alphabet[index];
        }

        int padding = (4 - ((int)output.size() % 4)) % 4;
        
        output += string(padding, '=');
        
        return output;
    }

    /*  Decodes a Base64-encoded string back into a vector of bytes (integers 0-255).
        Each Base64 character is mapped back to its 6-bit binary value using the alphabet.
        The resulting bit stream is concatenated and split into 8-bit segments to recover the original bytes.
        Any '=' padding characters at the end of the input are removed before decoding.
    */

    static vector<int> convert_from(string input) {
        map<char, int> charPos;

        for(int i = 0; i < (int)alphabet.size(); i++) {
            charPos[alphabet[i]] = i;
        }

        string stream = "";
        vector<int> bytes;
        
        // Remove padding
        while (!input.empty() && input.back() == '=') {
            input.pop_back();
        }

        for (auto &c : input) {
            int idx = charPos[c];
            string bits = "";
        
            for (int i = 5; i >= 0; --i) {
                bits += ((idx >> i) & 1) ? '1' : '0';
            }
        
            stream += bits;
        }

        // Split into bytes (8 bits)
        for (int i = 0; i + 8 <= (int)stream.size(); i += 8) {
            string byte_str = stream.substr(i, 8);
            int byte = stoi(byte_str, nullptr, 2);
            bytes.push_back(byte);
        }

        return bytes;
    }

    static string get_binary_string(int n) {
        string result = "";

        while(n > 0) {
            result = to_string(n % 2) + result;
            n /= 2;
        }
        
        while((int)result.size() < 8)
            result = "0" + result;
        
        return result;
    }

};

#endif