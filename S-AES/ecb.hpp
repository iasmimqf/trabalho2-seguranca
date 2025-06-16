
#include <iostream>
#include <string>
#include <cassert>
#include <vector>

#include "s-aes.hpp"
#include "util.hpp"
#include "base64.hpp"

using namespace std;

/**
 * @class ECB
 * @brief ECB (Electronic Codebook) mode encryption class for S-AES.
 * 
 * @details This class provides methods to encrypt and decrypt data using the S-AES
 * cipher in ECB mode. The encryption and decryption processes operate on 16-bit blocks,
 * and the class uses Base64 encoding for output and input of ciphertext.
 */
class ECB {
public:
    int key;
    SAES saes;

    /**
     * @brief Constructs an ECB mode encryption object with the given key.
     * 
     * @param key_ The 16-bit integer key used to initialize the S-AES cipher.
     */
    ECB(int key_) : key(key_), saes(key_, false, true) {}

    /**
     * @brief Encrypts a plaintext string using S-AES in ECB mode.
     * 
     * @details The plaintext is first converted into bytes, 
     * then split into 16-bit (2-byte) blocks.
     * Each block is encrypted using S-AES, and the resulting 
     * ciphertext is concatenated and encoded in Base64.
     * 
     * @param plaintext The input string to be encrypted.
     * @return A Base64-encoded string representing the ciphertext.
     */
    string encrypt(string plainText) {
        auto bytes = get_bytes_from_text(plainText);
        assert((int)bytes.size() % 2 == 0);
        
        vector<int> result;

        for(int i = 0; i < (int)bytes.size(); i += 2) {
            auto slice = get_vector_slice(bytes, i, 2);
            int num = (slice[0] << 8) + slice[1];
            int res = saes.encrypt(num);
            result.push_back(res >> 8);
            result.push_back(res & 0xFF);
        }

        return Base64::convert_to(result);
    }


   /**
     * @brief Decrypts a Base64-encoded ciphertext using the S-AES in ECB mode.
     * 
     * @details The input ciphertext is first decoded from Base64 to obtain the raw encrypted bytes.
     * These bytes are grouped into 16-bit (2-byte) blocks, and each block is decrypted using S-AES.
     * The resulting plaintext bytes are concatenated and returned as a standard string.
     * 
     * @param cipherText The Base64-encoded ciphertext string to be decrypted.
     * @return The decrypted plaintext as a string.
     */
    string decrypt(string cipherText) {
        vector<int> encrypted_blocks = Base64::convert_from(cipherText);
        vector<int> bytes;

        for(int i = 0; i < (int)encrypted_blocks.size(); i += 2) {
            int h = encrypted_blocks[i], l = encrypted_blocks[i + 1];
            int res = (h << 8) + l;
            int dec = saes.decrypt(res);
            bytes.push_back(dec >> 8);
            bytes.push_back(dec & 0xFF);
        }

        return string(bytes.begin(), bytes.end());
    }

};