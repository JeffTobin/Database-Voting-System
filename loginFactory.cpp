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

#include "User.cpp"
#include "Admin.cpp"

using namespace std;

class loginFactory {
public:
  //returns singelton instance                                                                
  static loginFactory& instance(){
    static loginFactory s;
    return s;
  }
  //login function                                                                            
  Person* login(string username, string password) {
    try{
      int login_status = 0;

      pstmt.reset(con->prepareStatement("SELECT user_login(?,?)"));
      pstmt->setString(1, username);
      pstmt->setString(2, password);
      res.reset(pstmt->executeQuery());
      res->next();
      login_status = res->getInt(1);

      if(login_status == 2)
        return new Admin(username, password);
      else if(login_status == 1)
        return new User(username, password);
      else{
        cout << "USER DOES NOT EXSISTS: register new user: " << endl;
        return register_user();
      }
    }
    catch (sql::SQLException &e){
      cout << "====REQUEST ERROR====" << endl;
    }
  }

  Person* register_user (){
    string username, full_name, password, email;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter full name: ";
    cin.ignore();
    getline(cin, full_name);
    cout << "Enter password: ";
    cin >> password;
    cout << "Enter email: ";
    cin >> email;

    try{
      pstmt.reset(con->prepareStatement("CALL register_user(?,?,?,?)"));
      pstmt->setString(1, username);
      pstmt->setString(2, full_name);
      pstmt->setString(3, password);
      pstmt->setString(4, email);
      pstmt->executeQuery();
    
      return login(username, password);
    }   

    catch (sql::SQLException &e){
      if(e.getErrorCode() == 1136 || e.getErrorCode() == 1062)
        cout << "====DUPLICATE USERNAME OR FULLNAME====" << endl;
      else
	cout << "====REQUEST ERROR====" << endl;
    }
  }

private:
  sql::Driver                     *driver;
  sql::Connection                 *con;
  auto_ptr<sql::Statement>         stmt;
  auto_ptr<sql::PreparedStatement> pstmt;
  auto_ptr<sql::ResultSet>         res;

  // copying and creation prohibited                                                          
  loginFactory(const loginFactory&){}
  loginFactory& operator=(const loginFactory&){}
  loginFactory(){
    //Create a connection                                                                     
    driver = get_driver_instance();
    con = driver->connect("dbdev.cs.kent.edu", "jtobin6", "4RkbbfT5");
    con->setSchema("jtobin6");
  }
};
