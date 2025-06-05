#include <string>
#include <vector>
#include <algorithm>

using namespace std;

string get_binary_string(int n) {
    string result = "";

    while(n > 0) {
        result = to_string(n % 2) + result;
        n /= 2;
    }

    while((int)result.size() < 8)
        result = "0" + result;

    return result;
}

string convert_to_base64(vector<int> bytes) {
    auto alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
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

string convert_to_base64(string str) {

    vector<int> bytes;

    for(auto &c : str) {
        bytes.push_back(c);
    }

    return convert_to_base64(bytes);

}