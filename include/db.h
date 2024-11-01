#ifndef __DB_H
#define __DB_H
#include <sqlite3.h>
#include <string>
typedef struct{
 bool suc;
 std::string res;
}Query_res;
class DB{
 public:
  DB();
  ~DB();
  bool table_exists(const char* tbl_name);
  bool open(const char* path);
  void close();
  static std::string sql_str(const std::string&);		//replaces ' with \' and \ with \\(not implemented yet)
  
  Query_res exec(const char* sql);
  bool is_open();
 private:
  sqlite3* db;
};
#endif
