#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class AdminController : public drogon::HttpSimpleController<AdminController>
{
  public:
    void asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) ;
    PATH_LIST_BEGIN
      PATH_ADD("/add_user", HttpMethod::Post);
      PATH_ADD("/get_delete_users", HttpMethod::Post);
      PATH_ADD("/set_seconds_per_qstn", HttpMethod::Post);
      PATH_ADD("/get_seconds_per_qstn", HttpMethod::Post);
    PATH_LIST_END
};
