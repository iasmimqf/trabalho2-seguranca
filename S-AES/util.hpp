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


#endif