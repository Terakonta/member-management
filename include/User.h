#ifndef USER_H
#define USER_H

#include <string>

using std::string;

string encrypt(string);

class User
{
    public:
        User(string = "", string = "");
        virtual ~User();

        void setEmail(string);
        void setPassword(string);
        void setPasswordSalt(string);

        string getEmail() const;
        string getPassword() const;
        string getPasswordSalt() const;

    private:
        string email;
        string password;
        string passwordSalt;
};

#endif // USER_H
