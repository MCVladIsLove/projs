#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <locale>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main(int argc, char** argv)
{
    int mem = shm_open("test", O_CREAT | O_RDWR, S_IRWXU);
    if (mem == -1)
    {
        cout << "cant open shared memory err\n";
        return 1;
    }
    int res = ftruncate(mem, sizeof(int*));
    if (res == -1)
    {
        cout << "cant truncate memory\n";
        return 1;
    }
    
    int* honey = (int*)mmap(NULL, sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED, mem, 0);   
    if (honey == MAP_FAILED)
    {
        cout << "sharing memory failed\n";
        return 1;
    }
    *honey = 0;
    int honey_portion = 5;
    int bees_num = atoi(argv[1]);
    char *args[] = {" ", argv[1], NULL};
    int id = fork();
    if (id == 0)
        execv("./bee.exe", args);
    else
    {
        mutex m;
        int bear_eats = atoi(argv[2]);
        int bear_sleep_time = atoi(argv[3]) * 1000;
        
        thread bear([&]()
        {
            while (true)
            {
                this_thread::sleep_for(chrono::milliseconds(bear_sleep_time));
                m.lock();
                *honey -= bear_eats;
                if (*honey < 0)
                    bear.detach();
                else
                    cout << "Bear ate " << bear_eats << " honey\n";
                m.unlock();
            }
        });
        

        
        while (bear.joinable()) {
            this_thread::sleep_for(chrono::milliseconds(1000));
            cout << *honey << "\n";
        }
    }
    cout << "Bear had nothing to eat and he died\n";
    return 0;   
}
