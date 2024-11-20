#include "api.h"
#include "bank.h"
#include <nlohmann/json.hpp>
void BankApi::bind(){
 this->storage=new Bank;
 this->AccountsApi::bind();
 Rest::Routes::Post(router, "/api/deposit", Rest::Routes::bind(&BankApi::on_deposit, this));
 Rest::Routes::Post(router, "/api/transfer", Rest::Routes::bind(&BankApi::on_transfer, this));
 Rest::Routes::Get(router, "/api/get_balance", Rest::Routes::bind(&BankApi::on_getBal, this));
 this->storage->init("bank.db", "Accounts");
}
void BankApi::on_deposit(const Rest::Request& rq, Http::ResponseWriter response){
 response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
 bool suc=0;
 if(this->auth(rq)){
  int id=std::stoi(rq.cookies().get("id").value);
  std::string s_ammount=query_vars(rq)["ammount"];
  if(!s_ammount.empty()){
   float ammount=0;
   try{ammount=std::stof(s_ammount);}
   catch(...){}
   if(ammount>0)suc=((Bank*)storage)->deposit(id, ammount);
  }
 }
 print_success(response, suc);
}
void BankApi::on_transfer(const Rest::Request& rq, Http::ResponseWriter response){
 response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
 bool suc=0;
 if(this->auth(rq)){
  int sender_id=std::stoi(rq.cookies().get("id").value);
  std::unordered_map<std::string, std::string> vars=query_vars(rq);
  std::string s_ammount=vars["ammount"];
  std::string s_target_id=vars["id"];
  if(!s_ammount.empty()&&!s_target_id.empty()){
   float ammount=0;
   //prevent sending "stof" to client
   try{ammount=std::stof(s_ammount);}
   catch(...){}

   int t_id=std::stoi(s_target_id);
   if(ammount>0)suc=((Bank*)storage)->send_cash(sender_id,t_id, ammount);
  }
 }
 print_success(response, suc);
}
void BankApi::on_getBal(const Rest::Request& rq, Http::ResponseWriter response){
 response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
 Bank* storage=(Bank*)this->storage;
 int uid=-1;
 float bal=-1;
 try{uid=std::stoi(query_vars(rq)["id"]);}
 catch(...){print_success(response, 0);return;}
 bal=storage->get_bal(uid);
 if(uid==-1||bal==-1){
  response.send((Http::Code)404, R"({"response": "not found"})");return;
 }
 nlohmann::json j;
 j["response"]=bal;
 response.send(Http::Code::Ok, j.dump());
}
int main(int argc, char** argv){
  Address addr(Ipv4::any(), Port(1024));
  BankApi api;
  api.bind();
  Http::Endpoint server(addr);
  Http::Endpoint::Options opt=Http::Endpoint::options().threads(1);
  opt.flags(Tcp::Options::ReuseAddr);
  server.init(opt);
  server.setHandler(api.get_router().handler());
  server.serve();
  return 0;
}
