#include "User.h"

#include <string>
#define ENCRYPT_KEY 5

string encrypt(string pass)
{
    string password = pass;

    int key = ENCRYPT_KEY;

    string encrypted = "";
    char c;
    int ascii;
    //65 to 90 ascii for capital letters
    //97 to 122 ascii for lower case letters
    for(string::size_type i = 0; i < password.length(); i++)
    {
        c = password[i];

        ascii = c;

        if(ascii >= 65 && ascii <= 90)
        {
            ascii += key;

            if(ascii > 90)
            {
                ascii = ascii - 90 + 64;
            }
        }else if(ascii >= 97 && ascii <= 122)
        {
            ascii += key;

            if(ascii > 122)
            {
                ascii = ascii - 122 + 96;
            }
        }else{

        ascii += key;
        }

        c = ascii;
        encrypted += c;
    }

    return encrypted;
}

string decrypt(string ePass)
{
    string encrypted_pass = ePass;

    int key = ENCRYPT_KEY;

    string decrypted = "";
    char c;
    int ascii;
    //65 to 90 ascii for capital letters
    //97 to 122 ascii for lower case letters
    for(string::size_type i = 0; i < encrypted_pass.length(); i++)
    {
        c = encrypted_pass[i];

        ascii = c;

        if(ascii >= 65 && ascii <= 90)
        {
            ascii -= key;

            if(ascii < 65)
            {
                ascii = ascii + 90 - 64;
            }
        }else if(ascii >= 97 && ascii <= 122)
        {
            ascii -= key;

            if(ascii < 97)
            {
                ascii = ascii + 122 - 96;
            }
        }else{

        ascii -= key;
        }

        c = ascii;
        decrypted += c;
    }

    return decrypted;
}

User::User(string email, string password)
{
    setEmail(email);
    setPassword(password);
    setPasswordSalt(password);
}

User::~User()
{
}

void User::setEmail(string e)
{
    email = e;
}

void User::setPassword(string p)
{
    password = p;
}

void User::setPasswordSalt(string ps)
{
    passwordSalt = encrypt(ps);
}

// const at the end since it isnt suppose to modify data members
string User::getEmail() const
{
    return email;
}

string User::getPassword() const
{
    return password;
}

string User::getPasswordSalt() const
{
    return passwordSalt;
}



