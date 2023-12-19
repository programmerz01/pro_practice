#include <iostream>
#include <cassert>
#include <mutex>
#include <map>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include "../include/log.h"
#include "../include/threadpool.h"
#include "../include/dbconnpool.h"

std::mutex mtx;
int sum = 0;

void thread_demo(int i)
{
    std::lock_guard<std::mutex> locker(mtx);
    for (int i = 0; i < 10; i++)
    {
        sum++;
    }
}

void thread_test_db(DBconnpool *pool)
{
    auto conn = pool->get_dbconn();

    std::unique_ptr<sql::Statement> stmt(conn->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("select * from user"));
    
    std::map<std::string, std::string> user_map;

    while(res->next()){
        user_map[res->getString("username")] = res->getString("passwd");
    }
    

    assert(user_map["admin"] == "admin");
    assert(user_map.size() == 1);
}

void test_threadpool()
{
    ThreadPool pool(10);
    for(int i = 0; i < 20; i++){
        pool.AddTask(std::bind(thread_demo, i));
    }
    usleep(100);
    assert(sum == 200);
}

void test_dbpool()
{
    DBconnpool db_pool("localhost", "root", "bupt0954", "users", 3306, 10);

    auto conn = db_pool.get_dbconn();
    std::unique_ptr<sql::Statement> stmt(conn->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("select * from user"));
    
    // while(res->next()){
    //     std::cout<<res->getString("username")<< res->getString("passwd") <<std::endl;
    // }
}

void test_multithread_dbpool()
{
    DBconnpool db_pool("localhost", "root", "bupt0954", "users", 3306, 10);
    ThreadPool thread_pool(10);

    for (int i = 0; i < 20; i++)
    {
        thread_pool.AddTask(std::bind(thread_test_db, &db_pool)); 
    }

    // 如果主线程结束，db_pool被释放，子线程无法返回链接从而死锁
    usleep(100);
}


int main()
{
    test_threadpool();
    test_dbpool();
    test_multithread_dbpool();
     
    return 0;
}
