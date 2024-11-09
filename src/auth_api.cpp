#include "auth_api.h"
#include <stdio.h>
#include <string.h>
void AccountsApi::on_createAcc(const Rest::Request& rq, Http::ResponseWriter response){
 response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
 if(rq.method()==Http::Method::Post){
  std::unordered_map<std::string, std::string> vars=query_vars(rq);
  std::string email, password;
  email=vars["email"];
  password=vars["pwd"];
  bool suc=false;
  if(!email.empty()&&!password.empty()&&this->storage){
   suc=storage->create_acc(email,password);
  }
  if(suc){
   response.send(Http::Code::Ok, "{\"reponse\":\"success\"}");
  }
  else{
   response.send(Http::Code::Ok, "{\"response\":\"fail\"}");
  }
 }
}
void AccountsApi::on_login(const Rest::Request& rq, Http::ResponseWriter response){
 response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
  if(rq.method()==Http::Method::Post){
  std::unordered_map<std::string, std::string> vars=query_vars(rq);
  std::string email, password;
  email=vars["email"];
  password=vars["pwd"];
  bool suc=false;
  if(!email.empty()&&!password.empty()&&this->storage){
   suc=storage->log_in(email,password);
  }
  if(suc){
   response.send(Http::Code::Ok, "{\"reponse\":\"success\"}");
  }
  else{
   response.send(Http::Code::Ok, "{\"response\":\"fail\"}");
  }
 }
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
std::unordered_map<std::string, std::string> AccountsApi::query_vars(const Http::Request& rq){
 std::unordered_map<std::string, std::string> res;
 if(rq.method()==Http::Method::Get){
 //Store all uri variables
  auto param=rq.query().parameters_begin();
  while(param!=rq.query().parameters_end()){
   res[param->first]=param->second;
   param++;
  }
 }
 else if(rq.method()==Http::Method::Post){
  auto ct=rq.headers().tryGet<Http::Header::ContentType>();
  if(!ct||ct->mime()==MIME(Application, FormUrlEncoded)){
   std::string q=rq.body();
   char* ptr=(char*)q.c_str();
   char* var_def=NULL;
   while(var_def=strtok_r(NULL, "&", &ptr)){
    std::string s_var_def=var_def;
    int del_pos=s_var_def.find("=");
    if(del_pos!=-1){
     res[s_var_def.substr(0,del_pos)]=s_var_def.substr(del_pos+1, s_var_def.size()-del_pos-1);
    }
    else{
     res[s_var_def]="";
    }
   }
  }
  else if(ct&&ct->mime()==MIME(Application, Json)){
   //TODO: parse JSON
  }
 }
 return res;
}
AccountsApi::AccountsApi(){
 storage=NULL;
}
AccountsApi::~AccountsApi(){
 if(storage)delete storage;
}
