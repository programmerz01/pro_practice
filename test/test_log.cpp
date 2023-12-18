#include "../include/log.h"

void TestLog()
{
    // 阻塞队列长度为0，同步写日志
    int cnt = 0, level = 0;
    Log::Instance()->init(level, "../log/test", ".log", 0);
    for (level = 3; level >= 0; level--)
    {
        Log::Instance()->SetLevel(level);
        for (int j = 0; j < 10000; j++)
        {
            for (int i = 0; i < 4; i++)
            {
                LOG_BASE(i, "%s 111111111 %d level = %d, j = %d, i = %d ============= ", "Test", cnt++, level, j, i);
            }
        }
    }

    // 阻塞队列长度为5000，异步写日志
    cnt = 0;
    Log::Instance()->init(level, "../log/test", ".log", 5000);
    for (level = 0; level < 4; level++)
    {
        Log::Instance()->SetLevel(level);
        for (int j = 0; j < 10000; j++)
        {
            for (int i = 0; i < 4; i++)
            {
                LOG_BASE(i, "%s 222222222 %d level = %d, j = %d, i = %d ============= ", "Test", cnt++, level, j, i);
            }
        }
    }
}

int main()
{
    TestLog();
}