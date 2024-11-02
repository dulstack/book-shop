#include "db.h"
#ifndef __AUTH_H
#define __AUTH_H
//abstract class for managing accounts
//the database will have the table with the following structure:
//uid,email,hash,date_created
//the databse should be open and table should exist
class Auth{
 public:
  Auth();
  ~Auth();
  virtual bool init(const char* db_path, const char* accounts_table)=0;
  bool log_in(const std::string& email, const std::string& password);
  bool create_acc(const std::string& email, const std::string& password);
  int get_id(const std::string& email);
  bool auth(int id, const std::string& hash);		//verify the login details
  std::string get_email(int id);
 protected:
  bool account_exists(int id);
  std::string hash(const std::string& pasword);			//not implemented yet
  std::string get_hash(int id);
  std::string acc_tbl;			//the string containing the table name what stores the data of accounts
  DB db;
};
#endif
