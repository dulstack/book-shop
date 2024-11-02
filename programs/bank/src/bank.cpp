#include "bank.h"
bool Bank::init(const char* db_path, const char* acc_tbl){
 if(!db.open(db_path))return 0;
 this->acc_tbl=acc_tbl;
 if(db.table_exists(acc_tbl))return 1;
 std::string sql="CREATE TABLE '"+DB::sql_str(acc_tbl)+"' (id INTEGER PRIMARY KEY, email text NOT NULL, hash text NOT NULL,balance FLOAT DEFAULT 0.00, date_created DATE CURRENT_DATE);";
 if(!db.exec(sql.c_str()).suc){
  #ifdef DEBUG
  fprintf(stderr, "DEBUG: failed to create the table\n");
  #endif
  return 0;
 }
 return 1;
}
bool Bank::send_cash(int s_id, int r_id, float ammount){
 if(!account_exists(s_id) || !account_exists(r_id) ||ammount<=0)return 0;
 float s_bal=get_bal(s_id), r_bal=get_bal(r_id);
 if(s_bal<ammount){
  return 0;
 }
 s_bal-=ammount;
 r_bal+=ammount;
 bool suc = set_bal(s_id, s_bal);
 if(suc){
  suc=set_bal(r_id, r_bal);
  if(!suc){
   //Transaction failed, return the money back
   set_bal(s_id, s_bal+ammount);
  }
 }
 else{
  return 0;
 }
 return suc;
}
bool Bank::deposit(int uid, float ammount){
 if(!account_exists(uid))return 0;
 float bal=get_bal(uid);
 bal+=ammount;
 return set_bal(uid, bal);
}
float Bank::get_bal(int uid){
 if(!account_exists(uid))return -1;
 std::string s_id=std::to_string(uid);
 std::string sql="SELECT balance FROM '";
 sql+=acc_tbl+"' WHERE id=";
 sql+=s_id+";";
 Query_res res=db.exec(sql.c_str());
 if(!res.suc||res.res.size()<=0)return -1;
 return std::stof(res.res);
}
bool Bank::set_bal(int uid, float bal){
 if(!account_exists(uid))return 0;
 std::string sql="UPDATE '";
 sql+=acc_tbl+"' SET balance=";
 sql+=std::to_string(bal)+" WHERE id=";
 sql+=std::to_string(uid)+";";
 return db.exec(sql.c_str()).suc;
}
