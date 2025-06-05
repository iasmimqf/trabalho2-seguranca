
#include <iostream>
#include <string>
#include <cassert>
#include <vector>

#include "s-aes.hpp"
#include "util.cpp"
#include "base64.hpp"

using namespace std;

string ecb_encript(string plainText, int key) {
    auto bytes = get_bytes_from_text(plainText);
    assert((int)bytes.size() % 2 == 0);
    
    vector<int> result;

    for(int i = 0; i < (int)bytes.size(); i += 2) {
        auto slice = get_vector_slice(bytes, i, 2);
        int num = (slice[0] << 8) + slice[1];
        int res = encript(num, key);
        result.push_back(res >> 8);
        result.push_back(res & 0xFF);
    }

    return convert_to_base64(result);
}

string ecb_decript(string cipherText, int key) {
    vector<int> encrypted_blocks = convert_from_base64(cipherText);
    vector<int> bytes;

    for(int i = 0; i < (int)encrypted_blocks.size(); i += 2) {
        int h = encrypted_blocks[i], l = encrypted_blocks[i + 1];
        int res = (h << 8) + l;
        int dec = decript(res, key);
        bytes.push_back(dec >> 8);
        bytes.push_back(dec & 0xFF);
    }

    return string(bytes.begin(), bytes.end());
}