# member-management
A simple console application made using C++ to connect to a database to manage members of any group/community. It can be residents in a 
society, members in a club, or even players of a team.

## Setup
Need to download [SQLAPI++](http://www.sqlapi.com) and [PostgreSQL](https://www.postgresql.org) and set required environment variables
for PostgreSQL.

To create the database and add an admin, use the commands mentioned in **extra** folder. 

Before running the program, make sure to edit the database variables accordingly in the **settings** header file based on your database
setup.

## Reflection

### Context behind the project
This was a 2 week long mini-project I did at my internship in Arpatech which is a information technology service provider. This was to 
familiarize myself with C++, its libraries, features and database management systems, as this was the first time I studied and used C++.

### Goal
I set out to build a simple console application that implemented CRUD(Create, Read, Update, Delete). I later added a few other features 
like adding restrictions based on whether a user is an admin or not, what information they can access and what functions he/she can
perform on the database.


### Challanges
The biggest challange for me was moving from a language like Python to C++. Setting up the project, its dependencies and then having to 
write way more code to perform simple tasks definitely caused confusion. Due to time constraints, I couldn't make the code as 
efficient and clean as I wanted it to be, I couldn't add enough validation and security checks, and other features that might have been 
useful.

### Tools
The tools I used were PostgreSQL as a database system, SQLAPI++ to access the database and C++ to write the program in.
