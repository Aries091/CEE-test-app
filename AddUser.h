#pragma once
#ifndef _ADDUSER_H_
#define _ADDUSER_H_

#include <iostream>
#include <sqlite3.h>
#include <fstream>
#include <filesystem>
#include <string.h>

using namespace std;

int sqlite_exec_callback(void* isIDTaken, int argc, char **argv, char **azColName){
   bool* _isIDTaken = (bool*)isIDTaken;
   int i;
   for(i = 0; i < argc; i++){
       *_isIDTaken = true;
   }
   return 0;
}

// Accepts a JSON data for Add Admin and then deserializes that
// JSON data to the fields below in the class
class AddUser{
    public:
        string full_name;
        int id;
        string password;
        
        // Data should come in CSV format
        // Eg. Rishad Baniya, 20, rishad123
        // Where first one is the full name, second is the id and third is the password
        AddUser(const char* req){
            cout << req << endl;
            string request(req);
            full_name = request.substr(0, request.find(','));
             
            request = request.substr(request.find(',') + 1);

            cout << request << endl;
            id = stoi(request.substr(0, request.find(',')));

            password = request.substr(request.find(',') + 1);
        }

         bool isIDTaken(int id, sqlite3* db){
             string query = "SELECT id FROM user WHERE id=ID";
             query.replace(query.find("ID"), strlen("ID"), to_string(id));

             bool isIDTaken = false;
             cout << 
             sqlite3_exec(db, query.c_str(), sqlite_exec_callback, &isIDTaken, NULL);
             return isIDTaken;
         
         }

         // Tries to add the new user to the database and returns true if succes
         string addToDatabase(){
             // In order to add any new admin to the database, one must check if a admin with same username
             // exists there in the database already or not
             //
             // If an admin with same username exists there on the database then one must respond with error to
             // the client

             sqlite3* db;
             int isConnected = sqlite3_open("cbt.db", &db);

             if(isConnected == SQLITE_OK){
                 createTableIfNotExist(db);

                 if(!isIDTaken(id, db)){
                    cout << "WAS HERE" << endl;
                     string query = "INSERT INTO user VALUES ('FULL_NAME', ID, 'PASSWORD', 0)";          
                     query.replace(query.find("FULL_NAME"), strlen("FULL_NAME"), full_name);
                     query.replace(query.find("ID"), strlen("ID"), to_string(id));
                     query.replace(query.find("PASSWORD"), strlen("PASSWORD"), password);
         
                     sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);
                     sqlite3_close(db);
                     return "OK";
                 }else{
                     sqlite3_close(db);
                     cout << "The id was taken" << endl;
                     return "ID_TAKEN";
                 }
             }else{
                 sqlite3_close(db);
                 // Can't add to the database and returns false
                 return "DATABASE_ERROR";
             }
        
         }

         void createTableIfNotExist(sqlite3* db){
             string query = "CREATE TABLE IF NOT EXISTS user(full_name TEXT, id INT, password TEXT, score INT)";                            
             sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);
         }
};

#endif
