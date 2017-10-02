#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <string>

using namespace std;

class Person {
 public:
 Person(string username, string password) : username_(username), password_(password) {}
  virtual void menu() = 0;
 protected:
  string username_;
  string password_;
};
