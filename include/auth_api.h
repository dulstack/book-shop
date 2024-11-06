#ifndef __API_H
#define __API_H
#include <pistache/endpoint.h>
#include <pistache/router.h>
using namespace Pistache;
class AccountsApi{
 public:
    void on_createAcc(const Rest::Request& rq, Http::ResponseWriter response);
    void on_login(const Rest::Request& rq, Http::ResponseWriter response);
    void on_auth(const Rest::Request& rq, Http::ResponseWriter response);
    virtual void bind();
    Rest::Router& get_router();
  protected:
    Rest::Router router;
};
#endif
