#include "bank.h"
#include "auth_api.h"
class BankApi:public AccountsApi{
 public:
  void on_deposit(const Rest::Request& rq, Http::ResponseWriter response);
  void on_transfer(const Rest::Request& rq, Http::ResponseWriter response);
  void on_getBal(const Rest::Request& rq, Http::ResponseWriter response);
  void bind() override;
};
