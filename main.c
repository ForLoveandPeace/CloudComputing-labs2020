#include "threadpool.h"
#include "sudoku.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <queue>
//#include <sys/sysinfo.h>
#include <pthread.h>
#include <sys/time.h>
using namespace std;
void* work(void* arg)
{
    char *p = (char*) arg;
    printf("threadpool callback fuction : %s.\n", p);
    sleep(1);
}
#define N 81
#define NUM 9
struct _board_{
    int board[N];
    int num;
    _board_(int *B,int n){
        for(int i = 0; i < N; ++i){
            board[i]=B[i];
        }
        num = n;
    }
};

queue<_board_> dispatch_q;
pthread_cond_t full;
pthread_cond_t empty;
pthread_mutex_t lock;

void init_cond()
{
    pthread_cond_init(&full, NULL);
    pthread_cond_init(&empty, NULL);
    pthread_mutex_init(&lock, NULL);
}

int64_t now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void print_board(int *B)
{
    for (int cell = 0; cell < N; ++cell) {
        printf("%d",B[cell]);
    }
    printf("\n");
}


void get_board(const char in[N],int *Board)
{
    for (int cell = 0; cell < N; ++cell) {
        Board[cell] = in[cell] - '0';
        assert(0 <= Board[cell] && Board[cell] <= NUM);
    }
}
int total = 0;
void* input(void *arg)
{
    char s[100];
    scanf("%s",s);
    FILE* fp = fopen(s, "r");
    assert(fp != NULL);
    char puzzle[128];
    int* B = new int[N];
    
    while (fgets(puzzle, sizeof puzzle, fp) != NULL) {
        if (strlen(puzzle) >= N) {
            //need locker
            pthread_mutex_lock(&lock);
/*
            while(full){
                pthread_cond_wait(&full, &lock);
            }
*/
            ++total;
            get_board(puzzle,B);
	    print_board(B);
            _board_ d_b(B,total);
            dispatch_q.push(d_b);
            pthread_cond_signal(&empty);
            pthread_mutex_unlock(&lock);
        }
    }
    delete B;
}

int main(int argc, char* argv[])
{
//    struct threadpool *pool = threadpool_init(10, 20);
//    int thread_number = get_nprocs_conf(); 
    
    int total_solved = 0;
    
//    bool (*solve)(int) = solve_sudoku_dancing_links;
    int64_t start = now();

    pthread_t pthd_in;
    int pth_in = pthread_create(&pthd_in,0,input,NULL);


    pthread_join(pthd_in, 0);
    int64_t end = now();
    double sec = (end-start)/1000000.0;
    printf("%f sec %f ms each %d\n", sec, 1000*sec/total, total_solved);

    return 0;
}
/*
int main(void)
{
    
    threadpool_add_job(pool, work, (void *)"1");
    threadpool_add_job(pool, work, (void *)"2");
    threadpool_add_job(pool, work, (void *)"3");
    threadpool_add_job(pool, work, (void *)"4");
    threadpool_add_job(pool, work, (void *)"5");
    threadpool_add_job(pool, work, (void *)"6");
    threadpool_add_job(pool, work, (void *)"7");
    threadpool_add_job(pool, work, (void *)"8");
    threadpool_add_job(pool, work, (void *)"9");
    threadpool_add_job(pool, work, (void *)"10");
    threadpool_add_job(pool, work, (void *)"11");
    threadpool_add_job(pool, work, (void *)"12");
    threadpool_add_job(pool, work, (void *)"13");
    threadpool_add_job(pool, work, (void *)"14");
    threadpool_add_job(pool, work, (void *)"15");
    threadpool_add_job(pool, work, (void *)"16");
    threadpool_add_job(pool, work, (void *)"17");
    threadpool_add_job(pool, work, (void *)"18");
    threadpool_add_job(pool, work, (void *)"19");
    threadpool_add_job(pool, work, (void *)"20");


    sleep(5);
    threadpool_destroy(pool);
    return 0;
}
*/
