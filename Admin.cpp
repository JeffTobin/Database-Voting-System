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

class Admin : public Person {
public:
  Admin(string username = "", string password = "") : Person(username, password) {
    //connect to database                
    driver = get_driver_instance();
    con = driver->connect("dbdev.cs.kent.edu", "jtobin6", "4RkbbfT5");
    con->setSchema("jtobin6");
  }

  virtual void menu() {
    //display & loop menu                                                              
    string menu_choice = "";
    while(menu_choice != "l") {
      cout << "CHOOSE AN OPTION: [r]egister user, [e]rase user, [s]how users, [a]dd candidate, [d]elete candidate, [p]olls, [l]ogout: ";
      cin >> menu_choice;

      try{
        if(menu_choice == "p")  //show current polls                                                 
          show_polls();

        else if(menu_choice == "d")  //delete candidate                                              
          delete_candidate();

        else if(menu_choice == "a")  //add candidate                                                 
          add_candidate();

        else if(menu_choice == "s")  //show users list                                               
          show_users();

        else if(menu_choice == "e")  //erase user                                                    
          erase_user();

        else if(menu_choice == "r")  //register user                                                 
          register_user();

        else if(menu_choice != "l")
          cout << "Command not recognized" << endl;
      }
      catch (sql::SQLException &e){
        cout << "====REQUEST ERROR====" << endl;
      }
    }
  }

private:
  void register_user (){
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

    pstmt.reset(con->prepareStatement("CALL register_user(?,?,?,?)"));
    pstmt->setString(1, username);
    pstmt->setString(2, full_name);
    pstmt->setString(3, password);
    pstmt->setString(4, email);
    pstmt->executeQuery();
  }

  void erase_user (){
    string username;
    cout << "Enter username: ";
    cin >> username;

    pstmt.reset(con->prepareStatement("CALL delete_user(?)"));
    pstmt->setString(1, username);
    pstmt->executeQuery();
  }

  void show_users (){
    //display header                                                                          
    cout << setw(5)  << left << "CNT"   << setw(20) << left << "USERNAME"
         << setw(20) << left << "NAME"  << setw(20) << left << "PASSWORD"
         << setw(30) << left << "EMAIL" << setw(3)  << left << "ADMIN STATUS"
         << endl << endl;

    //display users                                                                           
    stmt.reset(con->createStatement());
    res.reset(stmt->executeQuery("SELECT * FROM users"));

    int cnt = 0;
    while (res->next())
      cout << setw(5)  << left << ++cnt
           << setw(20) << left << res->getString("username")
           << setw(20) << left << res->getString("full_name")
           << setw(20) << left << res->getString("password")
           << setw(30) << left << res->getString("email")
           << setw(3)  << left << res->getString("admin_status") << endl;
  }
  
  void add_candidate (){
    string candidate;
    cout << "Enter candidates full name: ";
    cin.ignore();
    getline(cin, candidate);

    pstmt.reset(con->prepareStatement("CALL add_candidate(?)"));
    pstmt->setString(1, candidate);
    pstmt->executeQuery();
  }

  void delete_candidate (){
    string candidate;
    cout << "Enter candidates full name: ";
    cin.ignore();
    getline(cin, candidate);

    pstmt.reset(con->prepareStatement("CALL delete_candidate(?)"));
    pstmt->setString(1, candidate);
    pstmt->executeQuery();
  }

  void show_polls () {
    //display header                                                                          
    cout << setw(5) << left << "CNT" << setw(20) << left << "CANDIDATE" << setw(20)
         << left << "NUM OF VOTE" << endl << endl;

    //display poll                                                                            
    stmt.reset(con->createStatement());
    res.reset(stmt->executeQuery("SELECT * FROM candidates"));

    int cnt = 0;
    while (res->next())
      cout << setw(5)  << left << ++cnt
           << setw(20) << left << res->getString("candidate_name")
           << setw(20) << left << res->getString("num_of_votes") << endl;
  }

  sql::Driver                     *driver;
  sql::Connection                 *con;
  auto_ptr<sql::Statement>         stmt;
  auto_ptr<sql::PreparedStatement> pstmt;
  auto_ptr<sql::ResultSet>         res;
};
