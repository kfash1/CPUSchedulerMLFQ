//SJF Source Code
//Kevin Fash
//Laura Casals

#ifndef process_hpp
#define process_hpp

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

#define UNINIT_ARRIVAL_TIME - 1

class Process
{
public:
    Process();
    Process(string name, vector<int> values);
    int getArrivalTime();
    void setArrivalTime(int time);
    int getProcessState();
    void setProcessState(int state);
    void calculateTotalBurst();
    void setFirstArrivalTime(int time);
    void setLastTime(int time);
    string getName();
    int getData(int i);
    void set(int i, int value);
    void print();
    void popFirstItem();
    int processSize();
    int getTotalBurst();
    int getFirstArrivalTime();
    int getLastTime();
    int getPriorityQueue();
    void setPriorityQueue(int queue);

private:
    string name;
    int arrivalTime = 0; //time when process arrived to ready state
    vector<int> data; //contains data
    int processState = 0; //Determines whether process is ready, in I/O
    int totalBurst;
    int firstArrivalTime;
    int lastTime;
    int priorityQueue = 1;
};



#endif /* process_hpp */
