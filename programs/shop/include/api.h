#ifndef __SHOP_API
#define __SHOP_API
#include "auth_api.h"
class ShopApi:public AccountsApi{
 public:
  void on_add_book(const Rest::Request& rq, Http::ResponeWriter response);
  void on_list_books(const Rest::Request& rq, Http::ResponeWriter response);
  void on_list_owned_books(const Rest::Request& rq, Http::ResponeWriter response);
  void on_buy_book(const Rest::Request& rq, Http::ResponeWriter response);
  void bind() override;
};
#endif
