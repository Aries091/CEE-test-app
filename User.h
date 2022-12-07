#pragma once

#ifndef _USER_H
#define _USER_H

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

int sqlite_isValidUserCallback(void* isValidUser, int argc, char **argv, char **azColName){
   bool* _isValidUser = (bool*)isValidUser;
   *_isValidUser = argc > 0;
   return 0;
}

int sqlite_getFullNameCallback(void* full_name, int argc, char **argv, char **azColName){
   string* _full_name = (string*)full_name;
   *_full_name = argv[0];
   return 0;
}



// Accepts a JSON data for Add Admin and then deserializes that
// JSON data to the fields below in the class
class User{
    public:
        int id;
        string password;
        
        // Data should come in CSV format
        // Eg. Rishad Baniya, 20, rishad123
        // Where first one is the full name, second is the id and third is the password
        User(const char* req){
            cout << req << endl;
            string request(req);
            id = stoi(request.substr(0, request.find(',')));
            password = request.substr(request.find(',') + 1);
        }

         vector<string> getAllSubjects(){
            vector<string> subjects(0);
            string path = "Subjects";
            for (const auto & entry : std::filesystem::directory_iterator(path)){
                string sub_path = entry.path();
                subjects.push_back(sub_path.substr(sub_path.find('/') + 1));
            }
            return subjects;
         }

         int questionsCount(string subject){
            string path = "Subjects/" + subject;
            int questions = 0;
            for (const auto & entry : std::filesystem::directory_iterator(path)){
                ++questions;
            }
            return questions;
         }

         bool isUserValid(){
            sqlite3* db;
            int isConnected = sqlite3_open("cbt.db", &db);
            if(isConnected == SQLITE_OK){
                string query = "SELECT * FROM user WHERE id = ID AND password = 'PASSWORD'";
                query.replace(query.find("ID"), strlen("ID"), to_string(id));
                query.replace(query.find("PASSWORD"), strlen("PASSWORD"), password);
                bool doesUserCredentialMatch = false;
                sqlite3_exec(db, query.c_str(), &sqlite_isValidUserCallback, &doesUserCredentialMatch, NULL);
                sqlite3_close(db);
                return doesUserCredentialMatch;
            }else{
                return false;
            }
         }

         string getFullName(){
            sqlite3* db;
            int isConnected = sqlite3_open("cbt.db", &db);
            if(isConnected == SQLITE_OK){
                string query = "SELECT full_name FROM user WHERE id = ID";
                query.replace(query.find("ID"), strlen("ID"), to_string(id));
                string full_name = "";
                sqlite3_exec(db, query.c_str(), &sqlite_getFullNameCallback, &full_name, NULL);
                return full_name;
            }else{
                return "ERR";
            }
         }

         string getScore(){
            sqlite3* db;
            int isConnected = sqlite3_open("cbt.db", &db);
            if(isConnected == SQLITE_OK){
                string query = "SELECT score FROM user WHERE id = ID";
                query.replace(query.find("ID"), strlen("ID"), to_string(id));
                string score = "";
                sqlite3_exec(db, query.c_str(), &sqlite_getFullNameCallback, &score, NULL);
                return score;
            }else{
                return "ERR";
            }
         }

         string userLogin(){
             if(isUserValid()){
                bool doesContain = false;
                string respJSON = getFullName() + "," + getScore() + "[";
                vector<string> subjects = getAllSubjects();
                for (const string &subject : subjects){
                    StringBuffer s;
                    Writer<StringBuffer> writer(s);
                    writer.StartObject();
                    writer.Key("subject");
                    writer.String(subject.c_str());
                    writer.Key("total_questions");
                    string _totalQuestions = to_string(questionsCount(subject));
                    writer.String(_totalQuestions.c_str());
                    writer.EndObject();
                    respJSON.append(s.GetString());
                    respJSON.append(",");
                    doesContain = true;
                }
                if(doesContain){
                    respJSON.pop_back(); // Removes trailing comma
                }
                respJSON.append("]");
                return respJSON;
             }else{
                 return "ERR";
             }
         }
    
        // string questionAndAnswer(const char* subjectAndQstNo){
        //     string subAndNo(subjectAndQstNo);
        //     string sub = subAndNo.substr(0, subAndNo.find(','));
        //     string q_no = subAndNo.substr(subAndNo.find(',') + 1);
        // }
};

#endif
