#ifndef _DBCONNPOOL_H
#define _DBCONNPOOL_H

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <memory>

class DBconnpool{
public:
    /* 初始化连接池，其他线程从连接池中获取连接*/
    DBconnpool(std::string host, std::string user, std::string pwd, std::string db_name, int port, int conn_num = 10):
        host_(host), user_(user), pwd_(pwd), dbName_(db_name), port_(port){
            for(int i = 0; i < conn_num; i++){
                sql::Driver *driver = get_driver_instance();
                sql::Connection *conn = driver->connect(host_, user_, pwd_);
                conn->setSchema(dbName_);
                connQue_.push(conn);
            }
        }
    
    ~DBconnpool(){
        while(!connQue_.empty()){
            sql::Connection *conn = connQue_.front();
            connQue_.pop();
            delete conn;
        }
    }

    /* 获取一个sql连接，当unique_ptr被释放时，自动调用定义的func将连接重新入队列 */
    std::unique_ptr<sql::Connection, std::function<void(sql::Connection *)>> get_dbconn(){
        // 等待连接
        std::unique_lock<std::mutex> locker(mtx_);
        cond_.wait(locker, [this]{return !connQue_.empty();});

        // 获取第一个连接
        sql::Connection *conn = connQue_.front();
        connQue_.pop();

        return std::unique_ptr<sql::Connection, std::function<void(sql::Connection *)>>(conn, [this](sql::Connection *conn){
            std::lock_guard<std::mutex> locker(mtx_);
            connQue_.push(conn);
            cond_.notify_one();
        });
    };

private:
    std::string host_;
    std::string user_;
    std::string pwd_;
    std::string dbName_;
    int port_;

    std::queue<sql::Connection *> connQue_;
    std::mutex mtx_;
    std::condition_variable cond_;
};

#endif // _DBCONNPOOL_H