#ifndef ESMACAT_SHARED_MEMORY_COMM_H
#define ESMACAT_SHARED_MEMORY_COMM_H
#define DEFAULT_ROS_KEY_ID  2108        // street number of Harmonic Bionics

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <iostream>



class esmacat_shared_memory_comm
{
    struct shared_memory_packet {
        float analog_input[16];
        bool stop = false;
        double elapsed_time = 0;
        uint64_t loop_cnt = 0;
        uint64_t state = 1;
    };

private:
    key_t key;
    bool is_the_shared_memory_detached= 0;
    int shmid = 0;
public:
//    static int number_of_process_attached_in_shared_memory;
    shared_memory_packet* data;
    esmacat_shared_memory_comm();
    ~esmacat_shared_memory_comm();
    void init();
    void change_shared_memory_key(key_t k){key= k;} // only use this function before init
    void detach_shared_memory();
};

#endif // ESMACAT_SHARED_MEMORY_COMM_H

