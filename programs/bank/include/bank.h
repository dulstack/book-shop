#ifndef __BANK_H
#define __BANK_H
#include "auth.h"
//bank will have the same table structure as described in file above, but with user balance
class Bank:public Auth{
 public:
  bool init(const char* db, const char* tbl);
  bool send_cash(int sender_id, int receiver_id, float ammount);
  bool deposit(int uid, float ammount);
  float get_bal(int uid);
 private:
  bool set_bal(int uid, float bal);
};
#endif
