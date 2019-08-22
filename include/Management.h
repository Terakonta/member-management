#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include "User.h"
#include <SQLAPI.h>

#include <string>

class User;

using namespace std;

void userInfo(string&, string&);
class Management
{
    public:
        Management(const string = "localhost", const string = "", const string = "", const string = "", const string = "");
        virtual ~Management();

        bool loginWithStatus(string&, string, string);
        void idExists(string&);

        // methods with admin restrictions
        void createUser(User*);
        void delUser(string);
        void admin_UpdateUser(string, int);
        void admin_ReadAllUsers();
        void admin_ReadUser(string);

        // methods with no restrictions
        void user_ReadAllUsers();
        void user_ReadUser(string);
        void user_UpdateUser(string, int);


    private:
        SAConnection con;
        SACommand cmd;

};

#endif // MANAGEMENT_H
