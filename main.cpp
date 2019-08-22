#include <stdio.h>  // for printf
#include <iostream>
#include <SQLAPI.h> // main SQLAPI++ header
#include <string>

#include "User.h"
#include "Management.h"
#include "settings.h"

using namespace std;

enum Options { READ_ALL_USERS = 1 , CREATE_USER = 2, UPDATE_USER = 3, DELETE_USER = 4, READ_USER = 5, END_PROGRAM = 6,
               LOGOUT = 7};

// display main menu and return choice made by the user
int mainMenu()
{
   cout << "\nMenu Options" << endl
        << "1 - Print All Users"
        << endl << "2 - Create New User" << endl
        << "3 - Update" << endl << "4 - Delete"
        << endl << "5 - Search user" << endl
        << endl << "6 - End Management" << endl
        << endl << "7 - Logout" << endl;

    // stores the choice the user made
   int ch;
   cin >> ch;

   while (ch > 7 || ch < 1)
   {
       cout << "INVALID INPUT!" << endl;

       cout << "\nMenu Options" << endl
        << "1 - Print All Users"
        << endl << "2 - Create New User" << endl
        << "3 - Update" << endl << "4 - Delete"
        << endl << "5 - Search user" << endl
        << endl << "6 - End Management" << endl
        << endl << "7 - Logout" << endl;

        cin >> ch;
   }
   return ch;
}

int main(int argc, char *argv[])
{
    // using database variables from settings file
    Management* manage = new Management(HOST, PORT, DATABASE, DB_USER, DB_PASSWORD);

    int choice;
    // data of the user logged in
    string userId, userEmail, userPassword, tableName;
    bool isAdmin;
    bool loggedIn = false;

    // for filtering purposes
    string id, email, password;

    cout << "=========" << endl;
    cout << "LOGIN" << endl;
    cout << "=========" << endl;

    // function to ask for user information, do validation checks, and store data in variables
    // passed by reference
    userInfo(userEmail, userPassword);


    isAdmin = manage->loginWithStatus(userId, userEmail, userPassword);
    cout << userId << endl;
    if(isAdmin)
    {
        cout << "=========" << endl;
        cout << "ADMIN MODE" << endl;
        cout << "Your ID: " << userId << endl;
        cout << "=========" << endl;


    }else
    {
        cout << "==========================" << endl;
        cout << "USER MODE (limited access)" << endl;
        cout << "Your ID: " << userId << endl;
        cout << "==========================" << endl;
    }

    loggedIn = true;
    while(1)
    {
        if(loggedIn)
        {
            choice = mainMenu();
            if (choice == END_PROGRAM)
            {
                break;
            }
            switch(choice)
            {
                // more information visible if user is also an admin
                case READ_ALL_USERS:
                    if(isAdmin)
                    {
                        manage->admin_ReadAllUsers();
                    }else
                    {
                        manage->user_ReadAllUsers();
                    }

                    break;

                case CREATE_USER:
                    // only user with admin privilege can create users
                    if(isAdmin)
                    {
                        cout << "=========" << endl;
                        cout << "CREATE USER" << endl;
                        cout << "=========" << endl;
                        cin.ignore();
                        userInfo(email, password);

                        manage->createUser(new User(email, password));
                        cout << "Entry made!" << endl;
                    }else
                    {
                        cout << "=================" << endl;
                        cout << "RESTRICTED ACCESS" << endl;
                        cout << "=================" << endl;
                        cout << "Need admin permission!" << endl;
                    }
                    break;

                case UPDATE_USER:
                    // admin can update record of ANY user
                    // normal users can only update their own record
                    cout << "=========" << endl;
                    cout << "UPDATE" << endl;
                    cout << "=========" << endl;


                    int option;

                    cout << "Enter '1' to change email, '2' to change password and '3' to change both." << endl;
                    cin >> option;

                    while(option < 1 || option > 3)
                    {
                        cout << "Invalid option entered. Retry!" << endl;
                        cout << "Enter '1' to change email, '2' to change password and '3' to change both." << endl;
                        cin >> option;
                    }

                    if(isAdmin)
                    {
                        cout << "Enter Id of user you want to update data of: " << endl;
                        // to ignore the \n entered earlier which is still in the buffer
                        cin.ignore();
                        getline(cin, id);

                        while(id.length() == 0)
                        {
                            cout << "Invalid user id. Retry!" << endl;
                            getline(cin, id);
                        }

                        // isExists() makes sure id exists in table before we proceed
                        manage->idExists(id);
                        manage->admin_UpdateUser(id, option);

                    }else
                    {
                        cout << "You will now be able to update your account" << endl;
                        manage->user_UpdateUser(userId, option);
                    }
                    break;

                case DELETE_USER:
                    // only admin can delete users
                    if(isAdmin)
                    {
                        cout << "Enter Id of user you want to (soft)delete: " << endl;
                        // to ignore the \n entered earlier which is still in the buffer
                        cin.ignore();
                        getline(cin, id);
                        while(id.length() == 0)
                        {
                            cout << "Invalid Id. Retry!";
                            cout << "Enter Id of user you want to (soft)delete: " << endl;
                            getline(cin, id);
                        }
                        manage->idExists(id);
                        manage->delUser(id);
                    }else
                    {
                        cout << "=================" << endl;
                        cout << "RESTRICTED ACCESS" << endl;
                        cout << "=================" << endl;
                        cout << "Need admin permission!" << endl;
                    }
                    break;

                case READ_USER:
                    // looking up a single user through their id
                    cout << "Enter the id of the user you want to look up:" << endl;
                    cin.ignore();
                    getline(cin, id);

                    while(id.length() == 0)
                    {
                        cout << "Invalid ID!" << endl;
                        cout << "Enter the id of the user you want to look up:" << endl;
                        //cin.ignore();
                        getline(cin, id);
                    }

                    manage->idExists(id);
                    if(isAdmin)
                    {
                        manage->admin_ReadUser(id);
                    }else
                    {
                        manage->user_ReadUser(id);
                    }
                    break;

                case LOGOUT:
                    cout << "=================" << endl;
                    cout << "LOGGING OUT" << endl;
                    cout << "=================" << endl;

                    loggedIn = false;
                    break;

                default:
                    cerr << "Invalid choice!" << endl;
                    break;
            }
        }
        else
        {
            cout << "=========" << endl;
            cout << "LOGIN AGAIN" << endl;
            cout << "=========" << endl;

            cin.ignore();
            userInfo(userEmail, userPassword);

            isAdmin = manage->loginWithStatus(userId, userEmail, userPassword);
            if(isAdmin)
            {
                cout << "=========" << endl;
                cout << "ADMIN MODE" << endl;
                cout << "Your ID: " << userId << endl;
                cout << "=========" << endl;


            }else
            {
                cout << "==========================" << endl;
                cout << "USER MODE (limited access)" << endl;
                cout << "Your ID: " << userId << endl;
                cout << "==========================" << endl;
            }

            loggedIn = true;
        }
    }
    return 0;
}
