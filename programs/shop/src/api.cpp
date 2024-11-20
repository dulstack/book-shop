#include "api.h"
#include <stdio.h>
void ShopApi::bind(){
 storage=new Shop;
 storage->init("shop.db", "accounts");
 AccountsApi::bind();
 Rest::Routes::Post(router, "/api/add_book", Rest::Routes::bind(&ShopApi::on_add_book, this));
 Rest::Routes::Post(router, "/api/buy_book", Rest::Routes::bind(&ShopApi::on_buy_book, this));
 Rest::Routes::Get(router, "/api/list_books", Rest::Routes::bind(&ShopApi::on_list_books, this));
 Rest::Routes::Get(router, "/api/list_owned_books", Rest::Routes::bind(&ShopApi::on_list_owned_books, this));
 Rest::Routes::Get(router, "/api/book_info/:id", Rest::Routes::bind(&ShopApi::on_book_info, this));
}
void ShopApi::on_add_book(const Rest::Request& rq, Http::ResponseWriter response){
 response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
 bool suc=0;
 if(auth(rq)){
  int seller_id=-1;
  seller_id=std::stoi(rq.cookies().get("id").value);
  std::unordered_map<std::string, std::string> vars=query_vars(rq);
  std::string title=vars["title"], s_price=vars["price"], s_bank_id=vars["bank_id"];
  if(!title.empty()&&!s_price.empty()&&!s_bank_id.empty()){
   try{
    suc=((Shop*)storage)->add_book(seller_id, std::stoi(s_bank_id), title.c_str(), std::stof(s_price));
   }
   catch(...){}
  }
 }
 print_success(response, suc);
}
void ShopApi::on_buy_book(const Rest::Request& rq, Http::ResponseWriter response){
 response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
}
void ShopApi::on_list_books(const Rest::Request& rq, Http::ResponseWriter response){
 response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
}
void ShopApi::on_list_owned_books(const Rest::Request& rq, Http::ResponseWriter response){
 response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
}
void ShopApi::on_book_info(const Rest::Request& rq, Http::ResponseWriter response){
 response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
 int book_id=-1;
 try{
  book_id=rq.param(":id").as<int>();
 }
 catch(...){
  response.send((Http::Code)404, R"({"response":"Not found"})");
  return;
 }
 Book book=((Shop*)storage)->get_book(book_id);
 if(book.id==-1){
  response.send((Http::Code)404, R"({"response":"Not found"})");
  return;
 }
 nlohmann::json j=get_book_json(book);
 response.send(Http::Code::Ok, j.dump(1));
}
nlohmann::json ShopApi::get_book_json(const Book& book){
 nlohmann::json j;
 j["id"]=book.id;
 j["seller_id"]=book.seller_id;
 j["seller_bank_id"]=book.seller_bank_id;
 j["price"]=book.price;
 j["title"]=book.title;
 return j;
}
int main(){
 ShopApi api;
 api.bind();
 Address addr(Ipv4::any(), Port(1080));
 Http::Endpoint serv(addr);
 Http::Endpoint::Options opts=Http::Endpoint::options().threads(1);
 opts.flags(Tcp::Options::ReuseAddr);
 serv.init(opts);
 serv.setHandler(api.get_router().handler());
 serv.serve();
 return 0;
}
