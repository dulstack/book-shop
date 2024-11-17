#ifndef __SHOP_H
#define __SHOP_H
#include "auth.h"
#define BANK_URL "127.24.23.22:1024"
#include <vector>
//LibCurl will be used to send requests to bank
#include <curl/curl.h>
struct Book{
 int id;
 int seller_id;
 int seller_bank_id;
 float price;
 std::string title;
};
class Shop:public Auth{
 public:
  Shop();
  ~Shop();
  bool init(const char* db, const char* tbl);
  bool add_book(int seller_id,int seller_bank_id,const char* title, float price);
  bool buy_book(int uid, int book_id, int bank_id, const char* bank_pwd);
  std::vector<Book> list_books();
  std::vector<Book> list_owned_books(int uid);
  int get_book_count();
  Book get_book(int id);
 private:
  bool add_book_to_inv(int book_id, int uid);
  CURL* curl;
  bool is_book_owned(int uid, int book_id);
  float get_book_price(int book_id);
};
#endif
