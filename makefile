CXX ?= g++

# MYSQL_CONCPP_DIR = "/usr/lib/x86_64-linux-gnu"
# CPPFLAGS = -I $(MYSQL_CONCPP_DIR)/include -L $(MYSQL_CONCPP_DIR)/lib64
LDLIBS = -lpthread -lmysqlclient
DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXXFLAGS += -g -std=c++23
else
    CXXFLAGS += -O2 -std=c++23
endif

server: main.cpp  ./timer/lst_timer.cpp ./http/http_conn.cpp ./log/log.cpp ./CGImysql/sql_connection_pool.cpp  webserver.cpp config.cpp
	$(CXX) -o server  $^ $(CPPFLAGS) $(LDLIBS) $(CXXFLAGS)
	
clean:
	rm -r server
