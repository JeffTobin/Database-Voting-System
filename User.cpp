#ifndef _Person_H
#define _Person_H

#include "Person.h"

#endif

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

class User : public Person {
public:
  User(string username = "", string password = "") : Person(username, password) {
    //connect to database
    driver = get_driver_instance();
    con = driver->connect("dbdev.cs.kent.edu", "jtobin6", "4RkbbfT5");
    con->setSchema("jtobin6");
  }

  virtual void menu() {
    //display & loop menu 
    string menu_choice = "";
    while(menu_choice != "l") {
      cout << "CHOOSE AN OPTION: [v]ote, [l]ogout: ";
      cin >> menu_choice;

      if(menu_choice == "v")  //cast vote                                                            
        cast_vote();

      else if(menu_choice != "l")
        cout <<"Command not recognized" << endl;
    }
  }

private:
  void cast_vote (){
    try{
      //display candidates                                                                          
      stmt.reset(con->createStatement());
      res.reset(stmt->executeQuery("SELECT * FROM candidates"));

      cout << "CANDIDATES: ";
      while (res->next())
        cout << res->getString("candidate_name") << ", ";
      cout << endl;

      //get users vote                                                                              
      string for_candidate;
      cout << "Choose candidate to vote for: ";
      cin.ignore();
      getline(cin, for_candidate);

      pstmt.reset(con->prepareStatement("CALL cast_vote(?,?)"));
      pstmt->setString(1, username_);
      pstmt->setString(2, for_candidate);
      pstmt->executeQuery();
    }
    //catch sql errors
    catch (sql::SQLException &e){
      if(e.getErrorCode() == 1062)
        cout << "duplicate vote" << endl;
    }
  }
  
  //decleration
  sql::Driver                     *driver;
  sql::Connection                 *con;
  auto_ptr<sql::Statement>         stmt;
  auto_ptr<sql::PreparedStatement> pstmt;
  auto_ptr<sql::ResultSet>         res;
};
