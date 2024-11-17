#include "shop.h"
#include <nlohmann/json.hpp>
Shop::Shop(){
 curl=NULL;
}
Shop::~Shop(){
 if(curl){curl_easy_cleanup(curl);curl=NULL;}
}
bool Shop::init(const char* db_path, const char* tbl){
 if(!(curl=curl_easy_init()))return 0;
 if(!db.open(db_path))return 0;
 this->acc_tbl=tbl;
 if(db.table_exists(tbl)&&db.table_exists("books")&&db.table_exists("book_inventory"))return 1;
 std::string sql[3];
 sql[0]="CREATE TABLE '"+DB::sql_str(tbl)+"' (id INTEGER PRIMARY KEY, email text NOT NULL, hash text NOT NULL, date_created DATE CURRENT_DATE);";
 sql[1]="CREATE TABLE books(book_id INTEGER PRIMARY KEY,seller_id INTEGER NOT NULL, seller_bank_id INTEGER NOT NULL ,title TEXT, price float NOT NULL, FOREIGN KEY(seller_id) REFERENCES '";
 sql[1]+=DB::sql_str(tbl)+"'(id));";
 sql[2]="CREATE TABLE book_inventory (uid INTEGER NOT NULL, book_id INTEGER NOT NULL, FOREIGN KEY(uid) REFERENCES '";
 sql[2]+=DB::sql_str(tbl)+"'(id), FOREIGN KEY(book_id) REFERENCES books(book_id));";
 bool suc=1;
 for(int i=0; i<sizeof(sql)/sizeof(std::string); i++){
  if(!(db.exec(sql[i].c_str()).suc)){
   #ifdef DEBUG
   fprintf(stderr, "DEBUG: failed to create the tables\n");
   #endif
   suc=0;
  }
 }
 return suc;
}
bool Shop::add_book(int seller_id, int seller_bank_id, const char* title, float price){
 std::string sql="INSERT INTO books (seller_id, seller_bank_id, title, price) values(";
 sql+=std::to_string(seller_id)+", ";
 sql+=std::to_string(seller_bank_id)+", '";
 sql+=DB::sql_str(title)+"', ";
 sql+=std::to_string(price)+");";
 return db.exec(sql.c_str()).suc;
}
bool Shop::buy_book(int uid, int book_id, int bank_id, const char* bank_pwd){
 if(!curl)return 0;
 std::string response;
 size_t(*writefn)(void*, size_t,size_t,std::string*)= [](void* data, size_t bytes, size_t count, std::string* trg)->size_t{*trg=(char*)data;return count*bytes;};
 std::string url=std::string(BANK_URL)+"/api/transfer";
 std::string h=Auth::hash(bank_pwd);
 std::string cookies="id=";cookies+=std::to_string(uid)+"; hash=";
 cookies+=h+";";
 nlohmann::json data;
 Book book=get_book(book_id);
 if(book.id==-1)return 0;
 data["id"]=book.seller_bank_id;
 data["ammount"]=book.price;

 std::string rq=data.dump();
 curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);      //where the bank's server response will be stored
 curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
 curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefn);
 curl_easy_setopt(curl, CURLOPT_COOKIE, cookies.c_str());
 curl_easy_setopt(curl, CURLOPT_POSTFIELDS, rq.c_str());
 struct curl_slist *headers=NULL;
 headers=curl_slist_append(headers, "Content-Type: application/json");
 curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
 CURLcode r=curl_easy_perform(curl);
 if(headers)curl_slist_free_all(headers);
 if(r==CURLE_OK){
  //TODO: process the response
  nlohmann::json j_res=nlohmann::json::parse(response);
  std::string res=j_res["response"];
  if(!res.empty()&&res=="success"){return add_book_to_inv(book_id, uid);}
 }
 return 0;
}
std::vector<Book> Shop::list_books(){
 std::vector<Book> b_list;
 b_list.reserve(get_book_count());
 for(int i=0; i<get_book_count(); i++){
  std::string sql="SELECT book_id FROM books LIMIT 1 OFFSET ";
  sql+=std::to_string(i);
  Query_res res=db.exec(sql.c_str());
  if(res.suc){
   int id=std::stoi(res.res);
   Book book=get_book(id);
   if(book.id!=-1){
    b_list.push_back(book);
   }
  }
 }
 return b_list;
}
std::vector<Book> Shop::list_owned_books(int uid){
 std::vector<Book> books, owned_books;
 if(db.is_open()&&account_exists(uid)){
  books=list_books();
  for(int i=0; i<books.size(); i++){
   if(is_book_owned(uid, books[i].id)){
    owned_books.push_back(books[i]);
   }
  }
 }
 return owned_books;
}
Book Shop::get_book(int id){
 Book book={id,-1,-1, -1, ""};
 enum{
  T_SELLER_ID,
  T_SELLER_BANK_ID,
  T_TITLE,
  T_PRICE
 };
 std::string col[]={"seller_id","seller_bank_id" ,"title", "price"};
 std::string sql=" FROM books WHERE book_id=";
 sql+=std::to_string(id)+";";
 for(int i=0; i<sizeof(col)/sizeof(std::string); i++){
  std::string query=std::string("SELECT ")+col[i]+sql;
  Query_res res=db.exec(query.c_str());
  //stop the loop if there is an error
  if(!res.suc||res.res.empty()){book.id=-1; break;}
  switch(i){
   case T_SELLER_ID:{
    book.seller_id=std::stoi(res.res);
    break;
   }
   case T_SELLER_BANK_ID:{
    book.seller_bank_id=std::stoi(res.res);
    break;
   }
   case T_TITLE:{
    book.title=res.res;
    break;
   }
   case T_PRICE:{
    book.price=std::stof(res.res);
    break;
   }
  }
 }
 return book;
}
bool Shop::add_book_to_inv(int book_id, int uid){
 //check whether book and account exist
 if(!account_exists(uid)||get_book(book_id).id==-1)return 0;
 std::string sql="INSERT INTO book_inventory(uid, book_id) values(";
 sql+=std::to_string(uid)+", ";sql+=std::to_string(book_id)+");";
 return db.exec(sql.c_str()).suc;
}
int Shop::get_book_count(){
 int count=-1;
 if(!db.is_open()||!db.table_exists("books"))return -1;
 std::string sql="SELECT COUNT(book_id) FROM books;";
 Query_res res=db.exec(sql.c_str());
 if(!res.suc)return -1;
 count = std::stoi(res.res);
 return count;
}
bool Shop::is_book_owned(int uid, int book_id){
 std::string sql="SELECT book_id FROM book_inventory WHERE uid=";
 sql+=std::to_string(uid)+" AND book_id=";
 sql+=std::to_string(book_id)+";";
 Query_res res=db.exec(sql.c_str());
 if(!res.suc)return 0;
 return !res.res.empty();
}
