#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

long g_data = 0;
pthread_rwlock_t lock;  //读写锁

void* wr_cb(void* arg) {
    while (1) {
        //加写入锁
        pthread_rwlock_wrlock(&lock);
        g_data++;
        usleep(10);
        printf("write clock, thread id: %lu, data: %d\n", pthread_self(),
               g_data);
        //释放锁
        pthread_rwlock_unlock(&lock);
        usleep(1000);
    }
    return NULL;
}

void* rd_cb(void* arg) {
    while (1) {
        //加读出锁
        pthread_rwlock_rdlock(&lock);
        printf("read clock, thread id: %lu, data: %d\n", pthread_self(),
               g_data);
        //释放锁
        pthread_rwlock_unlock(&lock);
        usleep(1000);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t thd[4];  // 4个线程

    pthread_create(&thd[0], NULL, wr_cb, NULL);
    pthread_create(&thd[1], NULL, wr_cb, NULL);
    pthread_create(&thd[2], NULL, rd_cb, NULL);
    pthread_create(&thd[3], NULL, rd_cb, NULL);

    int i;
    for (i = 0; i < 4; i++) {
        pthread_join(thd[i], NULL);
    }

    //销毁
    pthread_rwlock_destroy(&lock);
    return 0;
}