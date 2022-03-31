#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>

using namespace std;

#define MAX_FLY_TIME 20000
#define MIN_FLY_TIME 6000

int main(int argc, char** argv)
{
    mutex m;
    unsigned seed = 147; // just random seed for rand function
    int honey = 0;
    int honey_portion = 5;
    int bees_num = atoi(argv[1]);
    int bear_eats = atoi(argv[2]);
    int bear_sleep_time = atoi(argv[3]) * 1000;
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
                honey += honey_portion;
                srand(seed++);
                cout << "Bee number " << this_thread::get_id() << " carried honey\n";
                m.unlock();
            }    
        }); 
        bees[i].detach();
    }

    thread bear([&]()
    {
        while (true)
        {
            this_thread::sleep_for(chrono::milliseconds(bear_sleep_time));
            m.lock();
            honey -= bear_eats;
            if (honey < 0)
                bear.detach();
            else
                cout << "Bear ate " << bear_eats << " honey\n";
            m.unlock();
        }
    });
    

    
    while (bear.joinable()) {
        this_thread::sleep_for(chrono::milliseconds(1000));
        cout << honey << "\n";
    }

    cout << "Bear had nothing to eat and he died\n";
    return 0;   
}
