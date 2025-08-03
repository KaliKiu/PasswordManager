#include <iostream>
#include <fstream>
#include <cstdio>
#include "../include/login.hpp"
#include "../include/key_derivation.hpp"
#include "../include/utils.hpp"
#include "../include/user_account_managing.hpp"
//compile: g++ -std=c++17 main.cpp login.cpp utils.cpp -o password_manager -largon2
//  g++ -std=c++17 main.cpp login.cpp utils.cpp key_derivation.cpp -o password_manager -largon2
//g++ -std=c++17 main.cpp login.cpp utils.cpp key_derivation.cpp user_account_managing.cpp vault.cpp -o password_manager -largon2
using namespace std;

int main(){
    User::LoggedInStatus session;
    KeyDerivation kdf = Login::log();
        cout << "\n----Logged in----\n";
        session.isLoggedin = true;
    while(session.isLoggedin){
        cout <<"input:\n";
        User::parseUserInput(kdf,utils::input_word(),session);
        if(kdf.isderivedKeyempty()){
            session.isLoggedin=false;
        }
    }
    cout <<"Logged out successfully\n";
}