#ifndef __SHOP_H
#define __SHOP_H
#include "auth.h"
#include <vector>
//LibCurl will be used to send requests to bank
#include <curl/curl.h>
struct Book{
 int seller_id;
 float price;
 std::string title;
};
class Shop:public Auth{
 public:
  Shop();
  ~Shop();
  bool init(const char* db, const char* tbl);
  bool add_book(int seller_id, float price);
  bool buy_book(int uid, int book_id, int bank_id, const char* bank_pwd);
  std::vector<Book> list_books();
  std::vector<Book> list_owned_books(int uid);
  int get_book_count();
 private:
  CURL* curl;
  bool is_book_owned(int uid, int book_id);
  float get_book_price(int book_id);
  Book get_book(int id);
};
#endif
