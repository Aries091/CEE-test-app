#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class UserController : public drogon::HttpSimpleController<UserController>
{
  public:
    void asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    PATH_LIST_BEGIN

    PATH_ADD("/user_login", HttpMethod::Post);
    PATH_ADD("/question_path_and_options", HttpMethod::Post);
    PATH_ADD("/get_answer", HttpMethod::Post);
    PATH_ADD("/save_score", HttpMethod::Post);

    PATH_LIST_END
};
