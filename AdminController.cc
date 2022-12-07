#include "AdminController.h"
#include <iostream>
#include <sqlite3.h>
#include "../components/AddUser.h"
#include "../components/GetDeleteUsers.h"
#include "../components/SetTimer.h"


using namespace std;

const string ADD_USER = "/add_user";
const string GET_USER_DELETE_USERS = "/get_delete_users";
const string SET_SECOND_PER_QSTN = "/set_seconds_per_qstn";
const string GET_SECOND_PER_QSTN = "/get_seconds_per_qstn";

void AdminController::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) {
    auto resp= drogon::HttpResponse::newHttpResponse(); 
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_PLAIN);

    if(req->path() == ADD_USER){
       AddUser add_user = AddUser(req->bodyData());
       resp->setBody(add_user.addToDatabase());
    }else if(req->path() == GET_USER_DELETE_USERS){
       // For getting users
       GetDeleteUsers get_delete_user = GetDeleteUsers(req->bodyData());
       resp->setBody(get_delete_user.operate());
    }else if(req->path() == GET_USER_DELETE_USERS){
       GetDeleteUsers get_delete_user = GetDeleteUsers(req->bodyData());
       resp->setBody(get_delete_user.operate());
    }else if(req->path() == SET_SECOND_PER_QSTN){
        resp->setBody(setTimer(req->bodyData()));
    }else if(req->path() == GET_SECOND_PER_QSTN){
        resp->setBody(getTime());
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
