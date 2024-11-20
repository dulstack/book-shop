#ifndef __SHOP_API
#define __SHOP_API

#include <nlohmann/json.hpp>
#include "auth_api.h"
#include "shop.h"
class ShopApi:public AccountsApi{
 public:
  void on_add_book(const Rest::Request& rq, Http::ResponseWriter response);
  void on_list_books(const Rest::Request& rq, Http::ResponseWriter response);
  void on_list_owned_books(const Rest::Request& rq, Http::ResponseWriter response);
  void on_buy_book(const Rest::Request& rq, Http::ResponseWriter response);
  void on_book_info(const Rest::Request& rq, Http::ResponseWriter response);
  void bind() override;
 private:
   nlohmann::json get_book_json(const Book& book);
};
#endif
