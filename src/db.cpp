#include "db.h"
#include <stdio.h>
DB::DB(){
 db=NULL;
}
DB::~DB(){
 this->close();
}
bool DB::table_exists(const char* tbl_name){
 if(!db)return 0;
 std::string sql="SELECT * FROM '";sql+=sql_str(tbl_name)+"' LIMIT 1;";
 return this->exec(sql.c_str()).suc;
}
bool DB::open(const char* path){
 if(db)this->close();
 if(sqlite3_open(path, &db)==0){
  return 1;
 }
 db=NULL;return 0;
}
void DB::close(){
 if(db){sqlite3_close(db);db=NULL;}
}
bool DB::is_open(){
 return this->db!=NULL;
}
Query_res DB::exec(const char* sql){
 Query_res res={0, ""};
 if(!db){return res;}
 sqlite3_stmt* st;
 try{
  int status=sqlite3_prepare(db, sql, -1,&st, NULL);
  if(status==SQLITE_ERROR){
   throw(-1);
  }
  while(sqlite3_step(st)!=SQLITE_DONE){
   for(int col=0; col<sqlite3_column_count(st); col++){
    res.res+=(const char*)sqlite3_column_text(st, col);
   }
   res.res+="\n";
  }
  sqlite3_finalize(st);
  res.suc=1;
  return res;
 }
 catch(int code){
  #ifdef DEBUG
  printf("DEBUG: error in prepare\n");
  #endif
  if(st)sqlite3_finalize(st);
  return res;
 }
}
std::string DB::sql_str(const std::string& str){
 return str;
}
