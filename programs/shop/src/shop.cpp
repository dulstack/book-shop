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
bool Shop::add_book(int seller_id, float price){

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
   if(book.seller_id!=-1){
    b_list.push_back(book);
   }
  }
 }
 return b_list;
}
std::vector<Book> Shop::list_owned_books(int uid){

}
Book Shop::get_book(int id){

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
