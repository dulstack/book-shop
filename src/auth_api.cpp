#include "auth_api.h"
#include <stdio.h>
#include <string.h>
#include <nlohmann/json.hpp>
void AccountsApi::on_createAcc(const Rest::Request& rq, Http::ResponseWriter response){
 response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
 std::unordered_map<std::string, std::string> vars=query_vars(rq);
 std::string email, password;
 email=vars["email"];
 password=vars["pwd"];
 bool suc=false;
 if(!email.empty()&&!password.empty()&&this->storage){
  suc=storage->create_acc(email,password);
 }
 print_success(response, suc);
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
  print_success(response, suc);
 }
}
void AccountsApi::on_auth(const Rest::Request& rq, Http::ResponseWriter response){
 response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
 print_success(response, this->auth(rq));
}
void AccountsApi::print_success(Http::ResponseWriter& res, bool suc){
 if(suc)res.send(Http::Code::Ok, R"({"response":"success"})");
 else res.send(Http::Code::Ok, R"({"response":"fail"})");
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
   nlohmann::json j=nlohmann::json::parse(rq.body());
   nlohmann::json::iterator it=j.begin();
   while(it!=j.end()){
    res[it.key()]=it.value().get<std::string>();
    it++;
   }
  }
 }
 return res;
}
bool AccountsApi::auth(const Http::Request& rq){
 Http::CookieJar cookies=rq.cookies();
 std::string id, s_hash;
 bool suc=0;
 if(cookies.has("id")&&cookies.has("hash")){
  id=cookies.get("id").value;
  s_hash=cookies.get("hash").value;
  if(this->storage&&id.size()>0&&s_hash.size()>0){
   suc=storage->auth(std::stoi(id), s_hash);
  }
 }
 return suc;
}
AccountsApi::AccountsApi(){
 storage=NULL;
}
AccountsApi::~AccountsApi(){
 if(storage)delete storage;
}
