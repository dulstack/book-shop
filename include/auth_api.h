#ifndef __API_H
#define __API_H
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <unordered_map>
#include "auth.h"
using namespace Pistache;
class AccountsApi{
 public:
    AccountsApi();
    ~AccountsApi();
    void on_createAcc(const Rest::Request& rq, Http::ResponseWriter response);
    void on_login(const Rest::Request& rq, Http::ResponseWriter response);
    void on_auth(const Rest::Request& rq, Http::ResponseWriter response);
    void on_user_info(const Rest::Request& rq, Http::ResponseWriter response);
    virtual void bind();
    Rest::Router& get_router();
  protected:
    std::unordered_map<std::string, std::string> query_vars(const Http::Request& rq);
    bool auth(const Http::Request& rq);
    void print_success(Http::ResponseWriter&, bool suc);
    Rest::Router router;
    Auth* storage;
};
#endif
