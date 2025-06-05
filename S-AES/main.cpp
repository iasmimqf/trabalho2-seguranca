#include "utils.hpp"
#include "base64.hpp"

int main(){
    int x = encript(28523, 42811);
    cout << x << endl;
    cout << decript(x, 42811) << endl;

    // cout << convert_to_base64(vector<int>{155}) << endl;                // -> mw==
    // cout << convert_to_base64(vector<int>{155, 162}) << endl;           // -> m6I=
    // cout << convert_to_base64(vector<int>{155, 162, 233}) << endl;      // -> m6Lp
    
    // cout << convert_to_base64(vector<int>{1}) << endl;                  // -> AQ==
    // cout << convert_to_base64(vector<int>{1, 2}) << endl;               // -> AQI=
    // cout << convert_to_base64(vector<int>{1, 2, 3}) << endl;            // -> AQID

    // cout << convert_to_base64(vector<int>{255}) << endl;                // -> /w==
    // cout << convert_to_base64(vector<int>{255, 255}) << endl;           // -> //8=
    // cout << convert_to_base64(vector<int>{255, 255, 255}) << endl;      // -> ////

    // cout << convert_to_base64(vector<int>{0}) << endl;                  // -> AA==
    // cout << convert_to_base64(vector<int>{0, 0}) << endl;               // -> AAA=
    // cout << convert_to_base64(vector<int>{0, 0, 0}) << endl;            // -> AAAA
 
    // cout << convert_to_base64(vector<int>{17, 41, 78, 149}) << "\n";    // ESlOlQ==

    // cout << convert_to_base64("Man is distinguished, not only by his reason, but ...") << "\n";
    // TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCAuLi4=

}