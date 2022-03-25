//
//  queue.cpp
//  CPUScheduler
//
//  Created by Kevin Fash on 3/7/22.
//

#include "queue.hpp"
#include <iomanip>


Queue::Queue()
{
    
}

Queue::Queue(string name)
{
    this->name = name;
}

Queue::Queue(vector<Process> process, string name)
{
    this->process = process;
    this->name = name;
}

//Displays processes in ready queue
void Queue::displayReadyQueue()
{
    cout << endl << "*******************************************" << endl << endl;
    cout << "List of processes in the ready queue:" << endl << endl;
    cout << "Process      Burst" << endl;

    //Checking if the ready queue is empty
    if (process.size() == 0)
    {
        cout << "[Empty]" << "      " << "[Empty]" << endl;
    }

    for (int i=0; i<process.size(); i++)
    {
        cout << process[i].getName() << "           " << process[i].getData(0) << endl;
    }

    cout << endl << "-------------------------------------------" << endl << endl;
}

//Displays processes in IO queue
void Queue::displayIOQueue(int time)
{
    cout << "List of processes in the IO queue:" << endl << endl;
    cout << "Process      Remaining IO time" << endl;

      //Checking if the ioQueue is empty
    if (process.size() == 0)
    {
        cout << "[Empty]" << "      " << "[Empty]" << endl;
    }

    for (int i=0; i<process.size(); i++)
    {
        cout << process[i].getName() << "           " << process[i].getArrivalTime()-time << endl;
    }
    cout << endl << "###########################################" << endl << endl;
}

//Sort the processes by CPU burst
void Queue::sortReadyQueue()
{
    //If this is empty, we don't want it to try to do a sort becasue it will crash
    if (process.size() == 0)
    {
        return;
    }

    //bubblesort
    for (int i = 0; i < process.size()-1; i++)
    {
        for (int j = 0; j < process.size()-i-1; j++)
        {
            if (process[j].getData(0) > process[j+1].getData(0))
            {
                swap(process[j], process[j+1]);
            }
        }
    }
}

void Queue::sortIOQueue()
{
    //If this is empty, we don't want it to try to do a sort becasue it will crash
    if (process.size() == 0)
    {
        return;
    }

    //bubblesort
    for (int i = 0; i < process.size()-1; i++)
    {
        for (int j = 0; j < process.size()-i-1; j++)
        {
            if (process[j].getArrivalTime() > process[j+1].getArrivalTime())
            {
                swap(process[j], process[j+1]);
            }
        }
    }
}

//Takes process with shortest job and adds
//its burst to the cpuTime.  It then moves the process
//to the ioQueue and updates the arrivalTime for
//when it will be able to go back to the readyQueue.
//Passes the ioQueue by reference so both queues can
//be updated
int Queue::runProcess(int cpuTime, Queue& ioQueue, Queue& completeQueue)
{
     //accounting for Idle time if the ready Queue is empty
     //but there are still process in the ioQueue
    if( process.size() == 0)
    {
        int nextArrival = ioQueue.getArrivalOfFirstItem();

        if (nextArrival != UNINIT_ARRIVAL_TIME)
        {
            idle += (nextArrival - cpuTime);
            return nextArrival;
        }
        else{
            idle += 1;
            return cpuTime + 1;
        }
    }

    //Getting the value for when the process first gets on the CPU
    if(process[0].getFirstArrivalTime() == UNINIT_ARRIVAL_TIME)
    {
        process[0].setFirstArrivalTime(cpuTime);
    }


    cout << "Process on the CPU: " << process[0].getName() << endl;
    cpuTime = cpuTime + process[0].getData(0);
    
    //Checking to see if process is down to its last item, if so
    //deletes the process
    if (process[0].processSize() > 1)
    {
        //Setting arrival time on process
        int newArrivalTime = cpuTime + process[0].getData(1);
        process[0].setArrivalTime(newArrivalTime);
    
        process[0].popFirstItem();
        process[0].popFirstItem();
    
        Process transfer = process[0];
        ioQueue.addProcess(transfer);
    
        popFirstItem();
    }
    else{
        process[0].setLastTime(cpuTime);
        Process transfer = process[0];
        completeQueue.addProcess(transfer);
        popFirstItem();
    }
    
    return cpuTime;
}

void Queue::printData()
{
    cout << left << setw(2) << "Process"
         << right << setw(7) << "RT"
         << right << setw(12) << "WT"
         << right << setw(12) << "TT" << endl;
    cout << endl;

    //Sorting the finished processes in order
    for (int i = 0; i < process.size() - 1; i++)
    {
        for (int j = 0; j < process.size() - i - 1; j++)
        {
            if (process[j].getName() > process[j + 1].getName())
            {
                swap(process[j], process[j + 1]);
            }
        }
    }

    float responseTimeAvg = 0;
    float waitTimeAvg = 0;
    float turnAroundAvg = 0;

    int k = 0;
    while(k < process.size())
    {
        int responseTime = process[k].getFirstArrivalTime() - 0;
        responseTimeAvg = responseTimeAvg + responseTime;

        int turnAround = process[k].getLastTime();
        turnAroundAvg = turnAroundAvg + turnAround;

        int waitTime = turnAround - process[k].getTotalBurst();
        waitTimeAvg = waitTimeAvg + waitTime;

        cout << left << setw(2) << process[k].getName()
             << right << setw(12) << responseTime
             << right << setw(12) << waitTime
             << right << setw(12) << turnAround << endl;

        k++;

    }

    cout << left << setw(2) << "Avg"
         << setprecision(4) 
         << right << setw(13) << responseTimeAvg/9
         << right << setw(11) << waitTimeAvg/9
         << right << setw(11) << turnAroundAvg/9 << endl;
    

}

void Queue::addProcess(Process p)
{
    process.push_back(p);
}

void Queue::popFirstItem()
{
    process.erase(process.begin());
}

//Checks the processes in the IO queue to see if they're ready
//to return to the ready queue
void Queue::checkIOStatus(int time, Queue& readyQueue)
{
    int i = 0;
    while(i < process.size())
    {
        //do your stuff
        if(process[i].getArrivalTime()<=time)
        {
            Process transfer = process[i];
            readyQueue.addProcess(transfer);
            
            popFirstItem();

            i = 0;//setting the conditional variable to the starting value
            continue;//and going to the next iteration to "restart" the loop
        }
        else
        {
            i++;
        }
    }
}

int Queue::getIdle()
{
    return idle;
}

int Queue::getArrivalOfFirstItem()
{
    if (process.size() > 0)
    {
        return process[0].getArrivalTime();
    }

    return UNINIT_ARRIVAL_TIME;
}