#include "shop.h"
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
 sql[1]="CREATE TABLE books(book_id INTEGER PRIMARY KEY,seller_id INTEGER NOT NULL,title TEXT, price float NOT NULL, FOREIGN KEY(seller_id) REFERENCES '";
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
bool Shop::add_book(int seller_id, const char* title, float price){
 std::string sql="INSERT INTO books (seller_id, title, price) values(";
 sql+=std::to_string(seller_id)+", '";
 sql+=DB::sql_str(title)+", ";
 sql+=std::to_string(price)+");";
 return db.exec(sql.c_str()).suc;
}
bool Shop::buy_book(int uid, int book_id, int bank_id, const char* bank_pwd){

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
 Book book={id,-1, -1, ""};
 enum{
  T_SELLER_ID,
  T_TITLE,
  T_PRICE
 };
 std::string col[]={"seller_id", "title", "price"};
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
