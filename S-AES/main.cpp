#include "assert.h"

#include "s-aes.hpp"
#include "base64.hpp"
#include "ecb.hpp"
#include "util.hpp"

void input16(int& key, int& message, bool encrypt=true, bool ecb_=false){
    int type;
    while(true){
        cout << "Choose the INPUT" << (ecb_ ? " key " : " ") << "format:\n";
        cout << "1 - Binary\n";
        cout << "2 - Hexadecimal\n";
        cout << "3 - Base64\n";
        cout << "-> ";
        cin >> type;
        cin.ignore();
        cout << endl;

        string sKey, sMessage;
        vector<int> vKey, vMessage;
        switch (type){
            case 1:
                cout << "Enter the 16-bit key :\n-> ";
                getline(cin, sKey);
                key = toInt(sKey);
                if(!ecb_){
                    cout << "\nEnter the 16-bit " << (encrypt ? "plaintext" : "ciphertext") << ":\n-> ";
                    getline(cin, sMessage);
                    message = toInt(sMessage);
                }
                cout << endl;
                return;
            case 2:
                printf("Enter the 16-bit key (hexadecimal, e.g., 3A94):\n-> ");
                scanf("%x", &key);
                if(!ecb_){
                    printf("\nEnter the 16-bit %s (hexadecimal):\n-> ", (encrypt? "plaintext" : "ciphertext"));
                    scanf("%x", &message);
                }
                cin.ignore();
                printf("\n");
                return;
            case 3:
                cout << "Enter the 16-bit key (base64, e.g., JOw=):\n-> ";
                getline(cin, sKey);
                vKey = Base64::convert_from(sKey);
                key = toInt(vKey);
                if(!ecb_){
                    cout << "\nEnter the 16-bit (base64)" << (encrypt ? "plaintext" : "ciphertext") << ":\n-> ";
                    getline(cin, sMessage);
                    vMessage = Base64::convert_from(sMessage);
                    message = toInt(vMessage);
                }
                cout << endl;
                return;
            default: cout << "Invalid Option.\n\n";
        }
    }
}

void s_aes(bool encrypt=true){
    int key, message;
    input16(key, message, encrypt);

    char choice;
    bool debug=true;
    cout << "Do you want to enable debug mode? (y/n): ";
    cin >> choice;
    cin.ignore();
    cout << endl;
    if(choice == 'n' || choice == 'N') debug = false;

    SAES saes(key, debug);
    if(encrypt) saes.encrypt(message);
    else saes.decrypt(message);
}

void ecb(bool encrypt=true){
    int key, nulll;
    input16(key, nulll, encrypt, true);

    string message;

    if(encrypt) {
        cout << "\nEnter the plaintext:\n-> ";
        getline(cin, message);

    } else {
        cout << "\nEnter the ciphertext in base64:\n-> ";
        getline(cin, message);
        if((int)message.size() % 4 != 0) {
            cout << "Error: ";
            cout << "The length of the base64 string must be a multiple of 4.\n";
            exit(0);
        }
    }

    ECB ecb(key);
    if(encrypt){
        cout << "\n====================== S-AES (ECB) - Encryption ======================\n\n";
        cout << "Ciphertext :   " << ecb.encrypt(message);
        cout << "\n\n======================================================================\n\n";
    }   
    else{
        cout << "\n====================== S-AES (ECB) - Decryption ======================\n\n";
        cout << "Plaintext :   " << ecb.decrypt(message);
        cout << "\n\n======================================================================\n\n";
    } 
}


int main(){
    while (true) {
        cout << "-------------- S-AES ------------------\n\n";
        cout << "1 - Encrypt 16-bit block with S-AES\n";
        cout << "2 - Decrypt 16-bit block with S-AES\n";
        cout << "3 - Encrypt full message (ECB)\n";
        cout << "4 - Decrypt full message (ECB)\n";
        cout << "0 - Exit\n\nChoose an option: ";
        int op;
        cin >> op;
        cout << endl;
        
        switch (op) {
            case 1: s_aes(); break;
            case 2: s_aes(false); break;
            case 3: ecb(); break;
            case 4: ecb(false); break;
            case 0: return 0;
            default: cout << "Invalid option.\n";
        }
        cout << "\nPress enter to continue...\n";
        string enter;
        getline(cin, enter);
    }
}