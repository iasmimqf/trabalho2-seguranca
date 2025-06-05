#include "utils.hpp"

int main(){
    int x = encript(28523, 42811);
    cout << x << endl;
    cout << decript(x, 42811) << endl;
}