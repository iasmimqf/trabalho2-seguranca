
#include <iostream>
#include <string>
#include <cassert>
#include <vector>

#include "s-aes.hpp"
#include "util.hpp"
#include "base64.hpp"

using namespace std;

/*  ECB (Electronic Codebook) mode encryption class for S-AES.
    This class provides methods to encrypt and decrypt data using the S-AES cipher in ECB mode.
    The class uses Base64 encoding for output and input of ciphertext.
*/

class ECB {
public:
    int key;
    SAES saes;

    ECB(int key_) : key(key_), saes(key_) {}


    /*  Encrypts a plaintext string using S-AES in ECB mode.
        The plaintext is converted to bytes, split into 2-byte blocks, and each block is encrypted (since S-AES.Encrypt works with 16 bits).
        The resulting ciphertext blocks are concatenated and encoded in Base64.
    */

    string encript(string plainText) {
        auto bytes = get_bytes_from_text(plainText);
        assert((int)bytes.size() % 2 == 0);
        
        vector<int> result;

        for(int i = 0; i < (int)bytes.size(); i += 2) {
            auto slice = get_vector_slice(bytes, i, 2);
            int num = (slice[0] << 8) + slice[1];
            int res = saes.encript(num);
            result.push_back(res >> 8);
            result.push_back(res & 0xFF);
        }

        return Base64::convert_to(result);
    }


    /*  Decrypts a Base64-encoded ciphertext string using S-AES in ECB mode.
        The ciphertext is decoded from Base64, split into 2-byte blocks, and each block is decrypted.
        The resulting plaintext bytes are concatenated and returned as a string.
    */

    string decript(string cipherText) {
        vector<int> encrypted_blocks = Base64::convert_from(cipherText);
        vector<int> bytes;

        for(int i = 0; i < (int)encrypted_blocks.size(); i += 2) {
            int h = encrypted_blocks[i], l = encrypted_blocks[i + 1];
            int res = (h << 8) + l;
            int dec = saes.decript(res);
            bytes.push_back(dec >> 8);
            bytes.push_back(dec & 0xFF);
        }

        return string(bytes.begin(), bytes.end());
    }

};