#include "auth.h"
Auth::Auth(){
 acc_tbl="";
}
Auth::~Auth(){
 db.close();
}
bool Auth::log_in(const std::string& email, const std::string& password){
 return auth(get_id(email), hash(password));
}
bool Auth::create_acc(const std::string& email, const std::string& password){
 if(!db.is_open())return 0;
 if(account_exists(get_id(email)))return 0;
 std::string q="INSERT INTO '";
 std::string p_hashed=std::string("'")+hash(password)+"'";
 q+=DB::sql_str(acc_tbl)+"' (email, hash) values('";
 q+=DB::sql_str(email)+"',";
 q+=p_hashed+(");");;
 return db.exec(q.c_str()).suc;
}
int Auth::get_id(const std::string& email){
 if(!db.is_open())return -1;
 std::string s_email="'";s_email+=DB::sql_str(email)+"'";
 std::string sql=std::string("SELECT id FROM '")+acc_tbl+"' WHERE email=";
 sql+=s_email+" LIMIT 1;";
 Query_res res=db.exec(sql.c_str());
 if(res.suc==0)return -1;
 if(res.res.size()<=0)return -1;
 return std::stoi(res.res);
}
bool Auth::auth(int id, const std::string& hash){
 if(!account_exists(id))return 0;
 if(!db.is_open())return 0;
 std::string sql=std::string("SELECT id,hash FROM '")+acc_tbl+"' WHERE id=";
 sql+=std::to_string(id)+" AND hash='";
 sql+=hash+"' LIMIT 1;";
 Query_res res=db.exec(sql.c_str());
 if(res.suc&&res.res.size()>0){return 1;}
 return 0;
}
bool Auth::account_exists(int uid){
 if(uid==-1)return 0;
 std::string sql=std::string("SELECT id FROM '")+acc_tbl+"' WHERE id=";
 sql+=std::to_string(uid)+" LIMIT 1;";
 return db.exec(sql.c_str()).res.size()>0;
}
std::string Auth::hash(const std::string& password){
 std::string p_hashed=password;
 //TODO: encrypt the password
 return p_hashed;
}
std::string Auth::get_email(int uid){
 if(!account_exists(uid))return std::string("");
 std::string sql=std::string("SELECT email FROM '")+acc_tbl+"' WHERE id=";
 sql+=std::to_string(uid)+" LIMIT 1;";
 return db.exec(sql.c_str()).res;
}
