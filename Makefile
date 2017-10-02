output: User.cpp Admin.cpp loginFactory.cpp Main.cpp
	g++ -Wall -I/usr/include/cppconn -o vlr User.cpp Admin.cpp loginFactory.cpp Main.cpp  -L/usr/lib -lmysqlcppconn -w
