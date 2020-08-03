#include "esmacat_shared_memory_comm.h"

//int esmacat_shared_memory_comm::number_of_process_attached_in_shared_memory= 0;

esmacat_shared_memory_comm::esmacat_shared_memory_comm()
{
    key = ftok("shmfile",DEFAULT_ROS_KEY_ID);
}

esmacat_shared_memory_comm::~esmacat_shared_memory_comm(){
    detach_shared_memory();
}

void esmacat_shared_memory_comm::init(){
    // shmget returns an identifier in shmid
    shared_memory_packet temp;
    shmid = shmget(key, sizeof(temp),0666|IPC_CREAT);
    // shmat to attach to shared memory
    data = (shared_memory_packet*) shmat(shmid,(void*)0,0);
}

void esmacat_shared_memory_comm::detach_shared_memory(){
    shmdt(data);
    shmctl(shmid,IPC_RMID,NULL);
}
