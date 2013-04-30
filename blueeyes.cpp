#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <unistd.h>
using namespace std;

#define SIZE 1024*1024
#define MB 30000
#define THREAD_N 20

typedef struct {
    char ptr[SIZE];
} Node;

void *thread_func(void *arr);

int main(int argc, char** argv) {
    int forkn = 0;
    if (argc > 1)
        forkn = atoi(argv[1]);

    if (forkn == 0)
        forkn = 1;

    srand(time(0));
    for (int i = 0; i < forkn; i++) {
        int pid = fork();
        if (pid == 0) {
            vector<Node*> q;
            for (int j = 0; j < MB; j++) { 
                q.push_back(new Node);
                for (int k = 0; k < SIZE; k++)
                    q[j]->ptr[k] = rand()%256;
            }
            pthread_t thr[THREAD_N];
            int ret;
            while (1) {
                for (int j = 0; j < THREAD_N; j++) {
                    ret = pthread_create( &thr[j], NULL, thread_func, (void*) &q);
                }
                for (int j = 0; j < THREAD_N; j++)
                    pthread_join(thr[j], NULL);
            }
        }
    }

    return 0;    
}

void *thread_func(void *ptr) {
    vector<Node*>* v =  (vector<Node*>*)ptr;
    while (1) {
        for (int i = 0; i < MB; i++)
            for (int j = 0; j < SIZE; j++)
                (*v)[i]->ptr[j] = rand()%256;
    }
}
