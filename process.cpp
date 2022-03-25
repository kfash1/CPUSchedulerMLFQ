//SJF Source Code
//Kevin Fash
//Laura Casals

#include "process.hpp"

//Object constructor
Process::Process() :
    totalBurst(0),
    firstArrivalTime(UNINIT_ARRIVAL_TIME)
{
    
}

Process::Process(string name, vector<int> values):
    totalBurst(0),
    firstArrivalTime(UNINIT_ARRIVAL_TIME)
{
    this->name = name;
    this->data = values;
    calculateTotalBurst();
}

//Returns the arrival time when the process will get to ready state
int Process::getArrivalTime()
{
    return arrivalTime;
}

//Sets the arrival time
void Process::setArrivalTime(int time)
{
    arrivalTime = time;
}

//Returns the process state
int Process::getProcessState()
{
    return processState;
}

//Changes the process state
void Process::setProcessState(int state)
{
    processState = state;
}

//calculating all the CPU and I/0 bursts
void Process::calculateTotalBurst()
{
    totalBurst = 0;
    for(int i = 0; i < data.size(); i ++)
    {
        totalBurst += data[i];
    }
}

//first time its on the CPU
void Process::setFirstArrivalTime(int time)
{
    firstArrivalTime = time;
}

//Last time its on the CPU
void Process::setLastTime(int time)
{
    lastTime = time;
}

//Returns name
string Process::getName()
{
    return name;
}

//Returns value of data from vector at index i
int Process::getData(int i)
{
    return data[i];
}

void Process::set(int i, int value)
{
    data[i] = value;
}

void Process::print()
{
    for (int i = 0; i < data.size(); i++)
    {
        cout << data[i] << ", ";
    }
    cout << endl;
}

void Process::popFirstItem()
{
    data.erase(data.begin());
}

int Process::processSize()
{
    return int(data.size());
}

int Process::getTotalBurst()
{
    return totalBurst;
}

int Process::getFirstArrivalTime()
{
    return firstArrivalTime;
}

int Process::getLastTime()
{
    return lastTime;
}

int Process::getPriorityQueue()
{
    return priorityQueue;
}

void Process::setPriorityQueue(int queue)
{
    priorityQueue = queue;
}
