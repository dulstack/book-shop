#include "auth_api.h"
void AccountsApi::on_createAcc(const Rest::Request& rq, Http::ResponseWriter response){
 response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
 if(rq.method()==Http::Method::Post){
  response.send(Http::Code::Ok, rq.body());
 }
}
void AccountsApi::on_login(const Rest::Request& rq, Http::ResponseWriter response){
 response.headers().add<Http::Header::ContentType>(MIME(Application, Json));

}
void AccountsApi::on_auth(const Rest::Request& rq, Http::ResponseWriter response){
 response.headers().add<Http::Header::ContentType>(MIME(Application, Json));

}
void AccountsApi::bind(){
 Rest::Routes::Post(router, "/api/create_account", Rest::Routes::bind(&AccountsApi::on_createAcc, this));
 Rest::Routes::Post(router, "/api/log_in", Rest::Routes::bind(&AccountsApi::on_login, this));
 Rest::Routes::Post(router, "/api/auth", Rest::Routes::bind(&AccountsApi::on_auth, this));
}
Rest::Router& AccountsApi::get_router(){
 return this->router;
}
