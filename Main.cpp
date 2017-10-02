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

#include "loginFactory.cpp"

using namespace std;

int main(){
  string menu_choice = "";
  Person *user;

  while(menu_choice != "e") {
    cout << "CHOOSE AN OPTION: [l]ogin, [r]egister, [e]xit: ";
    cin >> menu_choice;

    if(menu_choice == "l"){  //login                                                          
      string username, password;
      cout << "username: ";
      cin  >>  username;
      cout << "password: ";
      cin.ignore();
      getline(cin, password);

      user = loginFactory::instance().login(username, password);
      if(user)
	user->menu();
    }

    else if (menu_choice == "r"){
      user = loginFactory::instance().register_user();
      if(user)
	user->menu();
    }

    else if(menu_choice != "e")
      cout <<"Command not recognized" << endl;
  }

  return EXIT_SUCCESS;
}
