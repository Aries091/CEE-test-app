#include "UserController.h"
#include <iostream>
#include <sqlite3.h>
#include "../components/User.h"

using namespace std;

const string USER_LOGIN = "/user_login";
const string GET_QUESTION_IMG_PATH_AND_MCQ_OPTIONS = "/question_path_and_options";
const string GET_ANSWER = "/get_answer";
const string SAVE_SCORE = "/save_score";

void UserController::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback){
    auto resp = drogon::HttpResponse::newHttpResponse(); 

    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_PLAIN);

    if(req->path() == USER_LOGIN){
        User user = User(req->bodyData());
        resp->setBody(user.userLogin());
    }else if(req->path() == GET_ANSWER){
        string req_data(req->bodyData());

        string subject;
        string q_no;

        subject = req_data.substr(0, req_data.find(','));
        q_no = req_data.substr(req_data.find(',') + 1);

        ifstream timer_file;
        string sub_path = "";
        sub_path.append("Subjects");
        sub_path.append("/");
        sub_path.append(subject);
        sub_path.append("/");
        sub_path.append(q_no);
        sub_path.append("/");
        sub_path.append(q_no);
        sub_path.append(".txt");
       
        cout << sub_path << endl;
        timer_file.open(sub_path.c_str());
        if(timer_file.is_open()){
           string line;
           getline(timer_file, line);
            timer_file.close();
           resp->setBody(line.c_str());
        }else{
           resp->setBody("0");
        }


    }else if(req->path() == SAVE_SCORE){
        string req_data(req->bodyData());
        cout << "------------------------" << endl;
        cout << req_data << endl;
        cout << "------------------------" << endl;
        string id;
        string score;
        sqlite3* db;

        id = req_data.substr(0, req_data.find(','));
        score = req_data.substr(req_data.find(',') + 1);

        int isConnected = sqlite3_open("cbt.db", &db);

        if(isConnected == SQLITE_OK){
            string query = "UPDATE user SET score = SCORE WHERE id = ID";          
            query.replace(query.find("SCORE"), strlen("SCORE"), score);
            query.replace(query.find("ID"), strlen("ID"), id);
            sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);
            sqlite3_close(db);
            resp->setBody("OK");
        }else{
            resp->setBody("ERR");
        }
    }else{
        resp->setBody("BAD_REQUEST");
    }

    auto &origin = req->getHeader("Origin");
    resp->addHeader("Access-Control-Allow-Origin", origin);
    resp->addHeader("Access-Control-Allow-Methods", "OPTIONS,POST");
    resp->addHeader("Access-Control-Allow-Headers", "x-requested-with,content-type");
    resp->addHeader("Access-Control-Allow-Credentials","true");

    callback(resp);
}
