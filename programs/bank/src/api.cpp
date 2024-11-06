#include "api.h"
#include "bank.h"

void BankApi::bind(){
 this->AccountsApi::bind();
 Rest::Routes::Post(router, "/api/deposit", Rest::Routes::bind(&BankApi::on_deposit, this));
 Rest::Routes::Post(router, "/api/transfer", Rest::Routes::bind(&BankApi::on_transfer, this));
 Rest::Routes::Get(router, "/api/get_balance", Rest::Routes::bind(&BankApi::on_getBal, this));
 bank.init("bank.db", "Accounts");
}
void BankApi::on_deposit(const Rest::Request& rq, Http::ResponseWriter response){
 response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
 response.send(Http::Code::Ok, R"({
  "response": "success",
  "code": "200"
})");
}
void BankApi::on_transfer(const Rest::Request& rq, Http::ResponseWriter response){

}
void BankApi::on_getBal(const Rest::Request& rq, Http::ResponseWriter response){

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
