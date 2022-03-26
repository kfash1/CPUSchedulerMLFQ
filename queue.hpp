//
//  queue.hpp
//  CPUScheduler
//
//  Created by Kevin Fash on 3/7/22.
//

#ifndef queue_hpp
#define queue_hpp

#include "process.hpp"

class Queue
{
public:
    Queue();
    Queue(string name);
    Queue(vector<Process> processes, string name);
    void displayReadyQueue();
    void displayIOQueue(int time);
    int runProcess(int cpuTime, Queue& ioQueue, Queue& completeQueue);
    void printData();
    void addProcess(Process process);
    void popFirstItem();
    void checkIOStatus(int time, Queue& readyQueue);
    void sortQueue();
    int getIdle();
    int getArrivalOfFirstItem();
    
private:
    vector<Process> process;
    string name;
    int idle = 0;
};


#endif /* queue_hpp */
