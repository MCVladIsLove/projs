#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

#define MAX_FLY_TIME 20000
#define MIN_FLY_TIME 6000

int main(int argc, char** argv)
{
    int mem = shm_open("test", O_CREAT | O_RDWR, S_IRWXU);
    if (mem == -1)
    {
        cout << "cant open shared memory err\n";
        return 1;
    }
    int* honey = (int*)mmap(NULL, sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED, mem, 0);   
    if (honey == MAP_FAILED)
    {
        cout << "sharing memory failed\n";
        return 1;
    }
    mutex m;
    int bees_num = atoi(argv[1]);
    int honey_portion = 5;
    unsigned seed = 147; // just random seed for rand function
    thread* bees = (thread*)malloc(sizeof(thread) * bees_num);
    for (int i = 0; i < bees_num; i++)
    {
        bees[i] = thread([&]()
        {
            srand(seed++);
            while (true)
            {
                this_thread::sleep_for(chrono::milliseconds(rand() % MAX_FLY_TIME + MIN_FLY_TIME));
                m.lock();
                *honey += honey_portion;
                srand(seed++);
                cout << "Bee number " << this_thread::get_id() << " carried honey\n";
                m.unlock();
            }    
        }); 
        
    }
    
    while(*honey >= 0) 
    {
       
    }
    
    return 0;
}