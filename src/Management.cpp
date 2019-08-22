#include "Management.h"

#include <iostream>

#include <SQLAPI.h>
#include "User.h"
#include <string>

using namespace std;

const string USERDB = "testfinal";
// helps take in and validate user's entered email and password
void userInfo(string &userEmail, string &userPassword)
{
    cout << "Enter email: " << endl;
    //cin.ignore();
    getline(cin, userEmail);

    while(userEmail.length() == 0)
    {
        cout << "Invalid email. Retry!" << endl;
        cout << "Enter email: " << endl;
        getline(cin, userEmail);

    }

    cout << "Enter Password: " << endl;
    getline(cin, userPassword);

    while(userPassword.length() == 0)
    {
        cout << "Invalid Password. Retry!" << endl;
        cout << "Enter Password: " << endl;
        getline(cin, userPassword);

    }

}

Management::Management(const string HOST,const string PORT, const string DATABASE, const string USER, const string PASSWORD)
{
    try
    {
        // connect to database
        // in this example it is Oracle,
        // but can also be Sybase, Informix, DB2
        // SQLServer, InterBase, SQLBase and ODBC
        con.Connect(
            (HOST + "," + PORT + "@" + DATABASE).c_str(),     // SERVER,PORT@DB NAME
            USER.c_str(),   // user name
            PASSWORD.c_str(),   // password
            SA_PostgreSQL_Client);

        printf("We are connected!\n");

        // associate a command with connection
        // connection can also be specified in SACommand constructor
        cmd.setConnection(&con);

    }

    catch(SAException &x)
    {
        // SAConnection::Rollback()
        // can also throw an exception
        // (if a network error for example),
        // we will be ready
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message

        printf("%s\n", (const char*)x.ErrText());
    }
}

Management::~Management()
{
    con.Disconnect();
}

// takes in a User object and inserts its details into the table
void Management::createUser(User* user)
{
    string isAdmin;
    cout << "Make user admin?" << endl;
    cout << "Enter 'yes' or 'no'" << endl;

    getline(cin, isAdmin);

    while(((isAdmin.compare("yes")) != 0) && ((isAdmin.compare("no")) != 0))
    {
        cout << "Invalid input" << endl;
        cout << "Make user admin?" << endl;
        cout << "Enter 'yes' or 'no'" << endl;

        cin >> isAdmin;
    }
    try
    {
        cmd.setCommandText(
        ("INSERT INTO " + USERDB +
         "(email, password, password_salt, datecreated, isremoved, isadmin) VALUES(:1, :2, :3, :4, :5, :6)").c_str());

        // use first method of binding - param assignment
        cmd.Param(1).setAsString() = user->getEmail().c_str();
        cmd.Param(2).setAsString() = user->getPassword().c_str();
        cmd.Param(3).setAsString() = user->getPasswordSalt().c_str();
        cmd.Param(4).setAsString() = "NOW()";
        cmd.Param(5).setAsString() = "FALSE";

        if(((isAdmin.compare("yes")) == 0))
        {
            cmd.Param(6).setAsString() = "TRUE";
        }else
        {
            cmd.Param(6).setAsString() = "FALSE";
        }
        // Insert first row
        cmd.Execute();
    }

    catch(SAException &x)
    {
        // SAConnection::Rollback()
        // can also throw an exception
        // (if a network error for example),
        // we will be ready
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message

        printf("%s\n", (const char*)x.ErrText());
    }

}

// print all the users information in detail for admin level access
void Management::admin_ReadAllUsers()
{
    try
    {

        // will keep all the records but only show one which werent "removed"
        // using 'c_str()' to convert string to SAString.
        cmd.setCommandText(("SELECT * FROM " + USERDB + " WHERE IsRemoved = FALSE ORDER BY id ASC").c_str());
        cmd.Execute();

        while(cmd.FetchNext())
            {
                cout << "=============================================================" << endl;
                cout << "Id: " << cmd.Field("Id").asLong() << endl;
                cout << "Email: " << (const char*)cmd.Field("email").asString() << endl;
                cout << "Password: " << (const char*)cmd.Field("password").asString() << endl;
                SAString date = cmd.Field("DateCreated").asDateTime();
                cout << "PasswordSalt: " << (const char*)cmd.Field("password_salt").asString() << endl;
                cout << "DateCreated: " << (string)date << endl;
                cout << "IsRemoved: " << cmd.Field("IsRemoved").asBool() << endl;
                cout << "IsAdmin: " << cmd.Field("IsAdmin").asBool() << endl;
                cout << "=============================================================" << endl;
            }
            con.Commit();

            printf("Rows selected!\n");
    }
    catch(SAException &x)
    {
        // SAConnection::Rollback()
        // can also throw an exception
        // (if a network error for example),
        // we will be ready
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message

        printf("%s\n", (const char*)x.ErrText());
    }
}

// print data of a particular user in detail for admin level access
void Management::admin_ReadUser(string id)
{
    try
    {

        // will keep all the records but only show one which werent "removed"
        // using 'c_str()' to convert string to SAString.
        cmd.setCommandText(("SELECT * FROM " + USERDB + " WHERE IsRemoved = FALSE AND id = '" + id +"';").c_str());
        cmd.Execute();

        while(cmd.FetchNext())
            {
                cout << "=============================================================" << endl;
                cout << "Id: " << cmd.Field("Id").asLong() << endl;
                cout << "Email: " << (const char*)cmd.Field("email").asString() << endl;
                cout << "Password: " << (const char*)cmd.Field("password").asString() << endl;
                SAString date = cmd.Field("DateCreated").asDateTime();
                cout << "PasswordSalt: " << (const char*)cmd.Field("password_salt").asString() << endl;
                cout << "DateCreated: " << (string)date << endl;
                cout << "IsRemoved: " << cmd.Field("IsRemoved").asBool() << endl;
                cout << "IsAdmin: " << cmd.Field("IsAdmin").asBool() << endl;
                cout << "=============================================================" << endl;
            }
            con.Commit();

            printf("Rows selected!\n");
    }
    catch(SAException &x)
    {
        // SAConnection::Rollback()
        // can also throw an exception
        // (if a network error for example),
        // we will be ready
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message

        printf("%s\n", (const char*)x.ErrText());
    }
}

// (soft)delete a user from the table.
void Management::delUser(string id)
{
    try
    {
        cmd.setCommandText(("SELECT * FROM " + USERDB + " WHERE Id = " + id).c_str());
        cmd.Execute();

        if(cmd.isResultSet())
        {
            cmd.setCommandText(("UPDATE " + USERDB + " SET IsRemoved = TRUE WHERE Id = " + id).c_str());
            cmd.Execute();
        }
        else
        {
            cout << "Invalid ID!" << endl;
        }
    }
    catch(SAException &x)
    {
        // SAConnection::Rollback()
        // can also throw an exception
        // (if a network error for example),
        // we will be ready
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message

        printf("%s\n", (const char*)x.ErrText());
    }
}

// allows admin to update data of any user based on id
void Management::admin_UpdateUser(string id, int option)
{
    try
    {
        //can declare in 'if' blocks to save memory space
        string newEmail, newPassword;

        if(option == 1)
        {
            cout << "Enter new email: ";
            //cin.ignore();
            getline(cin, newEmail);

            while(newEmail.length() == 0)
            {
                cout << "Invalid input. Retry!" << endl;
                cout << "Enter new email: ";
                getline(cin, newEmail);
            }

            cmd.setCommandText(("SELECT * FROM " + USERDB + " WHERE isRemoved = FALSE AND id = " + id + ";").c_str());
            cmd.Execute();

            if(cmd.isResultSet())
            {
                cmd.setCommandText(("UPDATE " + USERDB + " SET email = '" + newEmail + "' WHERE Id = " + id).c_str());
                cmd.Execute();
            }else
            {
                cout << "Invalid ID!" << endl;
            }

        }else if(option == 2)
        {
            cout << "Enter new password: ";
            //cin.ignore();
            getline(cin, newPassword);

            while(newPassword.length() == 0)
            {
                cout << "Invalid input. Retry!" << endl;
                cout << "Enter new password: ";
                getline(cin, newPassword);

            }
            cout << ("SELECT * FROM " + USERDB + " WHERE isRemoved = FALSE AND Id = " + id + ";").c_str() << endl;

            cmd.setCommandText(("SELECT * FROM " + USERDB + " WHERE Id = " + id + ";").c_str());
            cmd.Execute();

            if(cmd.isResultSet())
            {
                cmd.setCommandText(("UPDATE " + USERDB + " SET password = '" + newPassword + "', password_salt = '" +
                                    encrypt(newPassword) + "' WHERE Id = " + id + ";").c_str());
                cmd.Execute();
            }else
            {
                cout << "Invalid ID!" << endl;
            }

        }else
        {
            cout << "Enter new email: ";
            //cin.ignore();
            getline(cin, newEmail);

            while(newEmail.length() == 0)
            {
                cout << "Invalid input. Retry!" << endl;
                cout << "Enter new email: ";
                getline(cin, newEmail);
            }

            cout << "Enter new password: ";
            getline(cin, newPassword);

            while(newPassword.length() == 0)
            {
                cout << "Invalid input. Retry!" << endl;
                cout << "Enter new password: ";
                getline(cin, newPassword);
            }

            cmd.setCommandText(("SELECT * FROM " + USERDB + " WHERE isRemoved = FALSE AND Id = " + id + ";").c_str());
            cmd.Execute();

            if(cmd.isResultSet())
            {
                cmd.setCommandText(("UPDATE " + USERDB + " SET email = '" + newEmail + "', password = '" + newPassword +
                                    "', password_salt = '" + encrypt(newPassword) + "' WHERE Id = " + id + ";").c_str());
                cmd.Execute();
            }else
            {
                cout << "Invalid ID!" << endl;
            }
        }
    }
    catch(SAException &x)
    {
        // SAConnection::Rollback()
        // can also throw an exception
        // (if a network error for example),
        // we will be ready
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message

        printf("%s\n", (const char*)x.ErrText());
    }
}

// helps user log in and returns true if user is 'admin'
// initially, the table should be given a admin account manually for logging in for the first time
// modifies id to match with the user whos email and password was passed in
bool Management::loginWithStatus(string &id, string email, string password)
{
    while(true)
    {
        try
        {
            cmd.setCommandText(("SELECT * FROM " + USERDB + " WHERE email = '" + email +
                              "' AND password = '" + password + "';").c_str());

            cmd.Execute();

            // this means we found a result
            // we know that the result returned is the user since we assume all emails are unique
            // and both email and password match
            if(cmd.FetchFirst())
            {
                if(cmd.Field("isRemoved").asBool() == false)
                {
                    cout << "Account found!" << endl;
                    id = (const char*)cmd.Field("id").asString();
                    return cmd.Field("isAdmin").asBool();
                }else
                {
                    cout << "Account no longer exists! Try a different account." << endl;
                    userInfo(email, password);
                }

            }else
            {
                cout << "No account with this info. Please retry!" << endl;
                // ask for user email and password to retry logging in
                userInfo(email, password);
            }

        }
        catch(SAException &x)
        {
            try
            {
                // on error rollback changes
                con.Rollback();
            }
            catch(SAException &)
            {
            }
            // print error message

            printf("%s\n", (const char*)x.ErrText());
        }
    }
}

// to make sure id exists. Otherwise, ask user to reenter until found
void Management::idExists(string &id)
{
    while(true)
    {

        try
        {
            cmd.setCommandText(("SELECT * FROM " + USERDB + "  WHERE isRemoved = FALSE AND id = :1 ;").c_str());

            cmd.Param(1).setAsString() = id.c_str();

            cmd.Execute();

            if(cmd.FetchFirst())
            {
                return;
            }else
            {
                cout << "ID not found! Retry." << endl;
                cout << "Enter ID: " << endl;
                getline(cin, id);

                while(id.length() == 0)
                {
                    cout << "Invalid user ID. Retry!";
                    getline(cin, id);
                }
            }
        }
        catch(SAException &x)
            {
                try
                {
                    // on error rollback changes
                    con.Rollback();
                }
                catch(SAException &)
                {
                }
                // print error message

                printf("%s\n", (const char*)x.ErrText());
            }
    }
}

// user methods with limited access to information

// print all users information in lesser detail for non-admin level access
void Management::user_ReadAllUsers()
{
    try
    {

        // will keep all the records but only show one which werent "removed"
        // using 'c_str()' to convert string to SAString.
        cmd.setCommandText(("SELECT * FROM " + USERDB + " WHERE IsRemoved = FALSE ORDER BY id ASC").c_str());
        cmd.Execute();

        while(cmd.FetchNext())
            {
                cout << "=============================================================" << endl;
                cout << "Id: " << cmd.Field("Id").asLong() << endl;
                cout << "Email: " << (const char*)cmd.Field("email").asString() << endl;
                SAString date = cmd.Field("DateCreated").asDateTime();
                cout << "DateCreated: " << (string)date << endl;
                cout << "=============================================================" << endl;
            }
            con.Commit();

            printf("Rows selected!\n");
    }
    catch(SAException &x)
    {
        // SAConnection::Rollback()
        // can also throw an exception
        // (if a network error for example),
        // we will be ready
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message

        printf("%s\n", (const char*)x.ErrText());
    }
}

// print data of a particular user in lesser detail for admin level access
void Management::user_ReadUser(string id)
{
    try
    {

        // will keep all the records but only show one which werent "removed"
        // using 'c_str()' to convert string to SAString.
        cmd.setCommandText(("SELECT * FROM " + USERDB + " WHERE IsRemoved = FALSE AND id = '" + id +"';").c_str());
        cmd.Execute();

        while(cmd.FetchNext())
            {
                cout << "=============================================================" << endl;
                cout << "Id: " << cmd.Field("Id").asLong() << endl;
                cout << "Email: " << (const char*)cmd.Field("email").asString() << endl;
                SAString date = cmd.Field("DateCreated").asDateTime();
                cout << "DateCreated: " << (string)date << endl;
                cout << "=============================================================" << endl;
            }
            con.Commit();

            printf("Rows selected!\n");
    }
    catch(SAException &x)
    {
        // SAConnection::Rollback()
        // can also throw an exception
        // (if a network error for example),
        // we will be ready
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message

        printf("%s\n", (const char*)x.ErrText());
    }
}

// user can update only his/her own data
void Management::user_UpdateUser(string id, int option)
{
    try
    {
        //can declare in 'if' blocks to save memory space
        string newEmail, newPassword;

        if(option == 1)
        {
            cout << "Enter new email: ";
            cin.ignore();
            getline(cin, newEmail);

            while(newEmail.length() == 0)
            {
                cout << "Invalid input. Retry!" << endl;
                cout << "Enter new email: ";
                getline(cin, newEmail);
            }

            cmd.setCommandText(("SELECT * FROM " + USERDB + " WHERE isRemoved = FALSE AND id = " + id + ";").c_str());
            cmd.Execute();

            if(cmd.isResultSet())
            {
                cmd.setCommandText(("UPDATE " + USERDB + " SET email = '" + newEmail + "' WHERE Id = " + id).c_str());
                cmd.Execute();

                cout << "Email updated!" << endl;
            }else
            {
                cout << "Invalid ID!" << endl;
            }

        }else if(option == 2)
        {
            cout << "Enter new password: ";
            cin.ignore();
            getline(cin, newPassword);

            while(newPassword.length() == 0)
            {
                cout << "Invalid input. Retry!" << endl;
                cout << "Enter new password: ";
                getline(cin, newPassword);

            }
            cout << ("SELECT * FROM " + USERDB + " WHERE isRemoved = FALSE AND Id = " + id + ";").c_str() << endl;

            cmd.setCommandText(("SELECT * FROM " + USERDB + " WHERE Id = " + id + ";").c_str());
            cmd.Execute();

            cout << "Password updated!" << endl;

            if(cmd.isResultSet())
            {
                cmd.setCommandText(("UPDATE " + USERDB + " SET password = '" + newPassword + "', password_salt = '" +
                                    encrypt(newPassword) + "' WHERE Id = " + id + ";").c_str());
                cmd.Execute();
            }else
            {
                cout << "Invalid ID!" << endl;
            }

        }else
        {
            cout << "Enter new email: ";
            cin.ignore();
            getline(cin, newEmail);

            while(newEmail.length() == 0)
            {
                cout << "Invalid input. Retry!" << endl;
                cout << "Enter new email: ";
                getline(cin, newEmail);
            }

            cout << "Enter new password: ";
            getline(cin, newPassword);

            while(newPassword.length() == 0)
            {
                cout << "Invalid input. Retry!" << endl;
                cout << "Enter new password: ";
                getline(cin, newPassword);
            }

            cmd.setCommandText(("SELECT * FROM " + USERDB + " WHERE isRemoved = FALSE AND Id = " + id + ";").c_str());
            cmd.Execute();

            if(cmd.isResultSet())
            {
                cmd.setCommandText(("UPDATE " + USERDB + " SET email = '" + newEmail + "', password = '" + newPassword +
                                    "', password_salt = '" + encrypt(newPassword) + "' WHERE Id = " + id + ";").c_str());
                cmd.Execute();

                cout << "Email and Password updated!" << endl;
            }else
            {
                cout << "Invalid ID!" << endl;
            }
        }
    }
    catch(SAException &x)
    {
        // SAConnection::Rollback()
        // can also throw an exception
        // (if a network error for example),
        // we will be ready
        try
        {
            // on error rollback changes
            con.Rollback();
        }
        catch(SAException &)
        {
        }
        // print error message

        printf("%s\n", (const char*)x.ErrText());
    }
}




