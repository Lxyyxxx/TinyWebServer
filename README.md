TinyWebServer
===============

# Preparation
1. 安装并启动 MySQL
```bash
# Environment: Ubuntu 22.04.2 (WSL 2)
sudo apt install mysql-server
sudo /etc/init.d/mysql restart
sudo mysql
alter user 'root'@'localhost' identified by 'mypasswd';
sudo /etc/init.d/mysql restart
sudo mysql -u root -p
```
2. 安装 MySQL Connector/C++
```bash
sudo apt install libmysqlclient-dev
```
3. 建立数据库
```sql
--- 建立数据库
create database tinywebserverdb;

--- 创建用户表
USE tinywebserverdb;
CREATE TABLE user(
    username varchar(50) primary key,
    passwd varchar(50) not null
)ENGINE=InnoDB;

--- 添加用户的账号密码
INSERT INTO user(username, passwd) VALUES('myname', 'mypasswd');
```
```sql
--- 验证
show databases;
use tinywebserverdb;
show tables;
select * from user;
```
4. 在 `main.cpp` 中设置数据库和用户信息
```cpp
std::string user = "root";  // 登录 mysql 的用户名
std::string passwd = "mypasswd";
std::string db_name = "tinywebserverdb";
```

# Start
```bash
make clean && make server
sudo ./server
```

# Update
1. `Config`，`WebServer` 使用构造函数进行初始化
2. 删去 `using namespace std;`
3. MySQL 数据库增加主键