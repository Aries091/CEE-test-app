#pragma once
#ifndef _GETDELETEUSERS_H_
#define _GETDELETEUSER_H_

#include <iostream>
#include <sqlite3.h>
#include <fstream>
#include <filesystem>
#include <string.h>
#include "../libs/rapidjson/document.h"
#include "../libs/rapidjson/writer.h"
#include "../libs/rapidjson/stringbuffer.h"

using namespace std;
using namespace rapidjson;

int getUsersCallback(void* _all_users_JSON, int argc, char **argv, char **azColName){
   string* all_users_JSON = (string*)_all_users_JSON;
   StringBuffer s;
   Writer<StringBuffer> writer(s);
   writer.StartObject();
   int i;
   for(i = 0; i < argc; i++){
       bool isPass = strcmp(azColName[i], "password") == 0;
       if(!isPass){
           writer.Key(azColName[i]);
           if(argv[i] != NULL){
               writer.String(argv[i]);
           }else{
               writer.String("");
           }
       }
   }
   writer.EndObject();
   all_users_JSON->append(s.GetString());
   all_users_JSON->append(",");
   return 0;
}

// Accepts a JSON data for Add Admin and then deserializes that
// JSON data to the fields below in the class
class GetDeleteUsers{
    public:
        string operation;
        int id;
        
        // Data should come in the following format
        // Let's say we want to get all the users, then it should send a request as follows
        // GET_USERS as the body data
        // and if we want to delete a certain user then we must send the body data as
        // DELETE_USER, 10
        GetDeleteUsers(const char* req){
            string request(req);
            cout << request.length() << endl;
            if(request == string("GET_USERS")){
                operation = "GET_USERS";
            }else{
                operation = "DELETE_USER";
                id = stoi(request.substr(request.find(',') + 1));
            }
        }

        string getUsers(){
            sqlite3* db;
            int isConnected = sqlite3_open("cbt.db", &db);
            if(isConnected == SQLITE_OK){
                string query = "SELECT * FROM user";
                string all_users_JSON = "[";
                sqlite3_exec(db, query.c_str(), getUsersCallback, &all_users_JSON, NULL);
                if(all_users_JSON.length() > 1){
                    all_users_JSON.pop_back();
                }
                all_users_JSON.append("]");
                sqlite3_close(db);
                return all_users_JSON;
            }else{
                return "DATABASE_ERROR";
            }
        }

        string deleteUser(){
            sqlite3* db;
            int isConnected = sqlite3_open("cbt.db", &db);
            if(isConnected == SQLITE_OK){
                string query = "DELETE FROM user WHERE id = ID";
                query.replace(query.find("ID"), strlen("ID"), to_string(id));

                sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);
                sqlite3_close(db);

                return "OK";
            }else{
                return "DATABASE_ERROR";
            }
        }

        string operate(){
            if (operation == "GET_USERS"){
                return getUsers();
            }else{
                return deleteUser();
            }
        }
};

#endif
