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
    cout << "Process\t\tBurst\t\tQueue\t\tArrival\t\tLastTime" << endl;

    //Checking if the ready queue is empty
    if (process.size() == 0)
    {
        cout << "[Empty]\t\t[Empty]\t\t[Empty]\t\t[Empty]" << endl;
    }

    for (int i=0; i<process.size(); i++)
    {
        cout << process[i].getName() << "\t\t\t" << process[i].getData(0);
        cout << "\t\t\t" << process[i].getPriorityQueue();
        cout << "\t\t\t" << process[i].getArrivalTime();
        cout << "\t\t\t" << process[i].getLastTime() << endl;
    }

    cout << endl << "-------------------------------------------" << endl << endl;
}

//Displays processes in IO queue
void Queue::displayIOQueue(int time)
{
    cout << "List of processes in the IO queue:" << endl << endl;
    cout << "Process\t\tRemaining IO time\tQueue" << endl;

      //Checking if the ioQueue is empty
    if (process.size() == 0)
    {
        cout << "[Empty]\t\t[Empty]\t\t\t\t[Empty]" << endl;
    }

    for (int i=0; i<process.size(); i++)
    {
        cout << process[i].getName() << "\t\t\t" << process[i].getArrivalTime()-time;
        cout << "\t\t\t\t\t" << process[i].getPriorityQueue() << endl;
    }
    cout << endl << "###########################################" << endl << endl;
}
//sorts the Queue
void Queue::sortQueue()
{
    //If this is empty, we don't want it to try to do a sort becasue it will crash
    if (process.size() == 0)
    {
        return;
    }
    
    //bubblesort for last time
    for (int i = 0; i < process.size()-1; i++)
    {
        for (int j = 0; j < process.size()-i-1; j++)
        {
            if (process[j].getLastTime() > process[j+1].getLastTime() && process[j].getArrivalTime() != 0)
            {
                swap(process[j], process[j+1]);
            }
        }
    }
    
    //bubblesort for arrival time
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
    
    //bubblesort for priority queue
    for (int i = 0; i < process.size()-1; i++)
    {
        for (int j = 0; j < process.size()-i-1; j++)
        {
            if (process[j].getPriorityQueue() > process[j+1].getPriorityQueue())
            {
                swap(process[j], process[j+1]);
            }
        }
    }
}

//sorts the Queue
void Queue::sortIOQueue()
{
    //If this is empty, we don't want it to try to do a sort becasue it will crash
    if (process.size() == 0)
    {
        return;
    }

    //bubblesort for arrival time
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

int Queue::runProcess(int cpuTime, Queue& ioQueue, Queue& completeQueue)
{
    //accounting for Idle time if the ready Queue is empty
    //but there are still process in the ioQueue
    if(process.size() == 0)
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

    int processBurst = process[0].getData(0);

    //Getting the value for when the process first gets on the CPU
    if(process[0].getFirstArrivalTime() == UNINIT_ARRIVAL_TIME)
    {
        process[0].setFirstArrivalTime(cpuTime);
    }

    
    //Priority 1 Process. Since its priority 1 it will never get
    //preempted
    if (process[0].getPriorityQueue()==1)
    {
        //normal process because CPU burst is 8 or less
        if (processBurst <= 8)
        {
            cout << "Process on the CPU: " << process[0].getName() << endl;
            cpuTime = cpuTime + processBurst;
            process[0].setLastTime(cpuTime);
            
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
                //process and delete
                Process transfer = process[0];
                completeQueue.addProcess(transfer);
                popFirstItem();
            }
            return cpuTime;
        }
        //Since burst is over 8, only 8 units will be done and priority downgraded
        else
        {
            cout << "Process on the CPU: " << process[0].getName() << endl;
            cpuTime += 8;
            process[0].setLastTime(cpuTime);
            //setting new CPU burst since it only got to do 8 units
            process[0].set(0,process[0].getData(0)-8);
            process[0].setPriorityQueue(2);
            
            //updating arrival time - the time left over from not completing its CPU burst
            //int newArrivalTime = cpuTime + process[0].getData(0);
            process[0].setArrivalTime(cpuTime);

            return cpuTime;
        }
    }
    
    //Queue 2, need to implement preemptive functionality
    else if (process[0].getPriorityQueue()==2)
    {
        //testing preemptive process
        /*
        int preemptTime = 0;
        int processNum = 0;
        int preemptStatus = 0;
        while (processNum < ioQueue.process.size() && preemptStatus == 0)
        {
            if (ioQueue.process[processNum].getPriorityQueue() == 1 && (ioQueue.process[processNum].getArrivalTime() - cpuTime) < process[0].getData(0))
            {
                cout << "Process on the CPU: " << process[0].getName() << endl;
                preemptStatus = 1;
                cout << "Test Process " << process[0].getName() << " is getting preempted by " << ioQueue.process[processNum].getName() << endl;
            }
            else
            {
                processNum++;
            }
        }
        if (preemptStatus == 1)
        {
            //comments to check the display output
            
            //cout << "The current cpuTime is " << cpuTime << " in Q2-1" << endl;
            //cout << "Process " << process[0].getName() << " is getting preempted becasue a Q1 became available" << endl;
            //cout << "Inside the preempted loop for Q2" << endl;
            //cout << "The cpuTime is currently " << cpuTime << endl;
            
            preemptTime = process[0].getData(0) - (ioQueue.process[processNum].getArrivalTime() - cpuTime);
            process[0].set(0, preemptTime);
            cpuTime += (ioQueue.process[processNum].getArrivalTime() - cpuTime);
            process[0].setArrivalTime(cpuTime);
            return cpuTime;
        }
        */
        //if the CPU burst is less than or 12, it doesn't get placed in a lower queue
        if (processBurst <= 12)
        {
            cout << "Process on the CPU: " << process[0].getName() << endl;
            
            //testing preemptive process
            int preemptTime = 0;
            int processNum = 0;
            int preemptStatus = 0;
            while (processNum < ioQueue.process.size() && preemptStatus == 0)
            {
                if (ioQueue.process[processNum].getPriorityQueue() == 1 && (ioQueue.process[processNum].getArrivalTime() - cpuTime) < process[0].getData(0))
                {
                    cout << "Process on the CPU: " << process[0].getName() << endl;
                    preemptStatus = 1;
                    cout << "Test Process " << process[0].getName() << " is getting preempted by " << ioQueue.process[processNum].getName() << endl;
                }
                else
                {
                    processNum++;
                }
            }
            if (preemptStatus == 1)
            {
                //comments to check the display output
                /*
                cout << "The current cpuTime is " << cpuTime << " in Q2-1" << endl;
                cout << "Process " << process[0].getName() << " is getting preempted becasue a Q1 became available" << endl;
                cout << "Inside the preempted loop for Q2" << endl;
                cout << "The cpuTime is currently " << cpuTime << endl;
                */
                preemptTime = process[0].getData(0) - (ioQueue.process[processNum].getArrivalTime() - cpuTime);
                process[0].set(0, preemptTime);
                cpuTime += (ioQueue.process[processNum].getArrivalTime() - cpuTime);
                process[0].setArrivalTime(cpuTime);
                
                process[0].setLastTime(cpuTime);
                return cpuTime;
            }
            
            
            
            
            cpuTime += processBurst;
            process[0].setLastTime(cpuTime);
            
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
            else
            {
                Process transfer = process[0];
                completeQueue.addProcess(transfer);
                popFirstItem();
            }
            cout << "Returning from Q2-2" << endl;
            return cpuTime;
        }
        //Since burst is over 12, only 12 units will be done and priority downgraded
        else
        {
            cout << "Process on the CPU: " << process[0].getName() << endl;
            
            //testing preemptive process
            int preemptTime = 0;
            int processNum = 0;
            int preemptStatus = 0;
            while (processNum < ioQueue.process.size() && preemptStatus == 0)
            {
                if (ioQueue.process[processNum].getPriorityQueue() == 1 && (ioQueue.process[processNum].getArrivalTime() - cpuTime) < process[0].getData(0))
                {
                    cout << "Process on the CPU: " << process[0].getName() << endl;
                    preemptStatus = 1;
                    cout << "Test Process " << process[0].getName() << " is getting preempted by " << ioQueue.process[processNum].getName() << endl;
                }
                else
                {
                    processNum++;
                }
            }
            if (preemptStatus == 1)
            {
                //comments to check the display output
                /*
                cout << "The current cpuTime is " << cpuTime << " in Q2-1" << endl;
                cout << "Process " << process[0].getName() << " is getting preempted becasue a Q1 became available" << endl;
                cout << "Inside the preempted loop for Q2" << endl;
                cout << "The cpuTime is currently " << cpuTime << endl;
                */
                preemptTime = process[0].getData(0) - (ioQueue.process[processNum].getArrivalTime() - cpuTime);
                process[0].set(0, preemptTime);
                cpuTime += (ioQueue.process[processNum].getArrivalTime() - cpuTime);
                process[0].setArrivalTime(cpuTime);
                process[0].setLastTime(cpuTime);
                return cpuTime;
            }
            
            
            
            cpuTime = cpuTime + 12;
            
            //setting new CPU burst since it only got to do 12 units
            process[0].set(0,process[0].getData(0)-12);
            process[0].setPriorityQueue(3);
            
            //updating arrival time
            //int newArrivalTime = cpuTime + process[0].getData(0);
            process[0].setArrivalTime(cpuTime);

            //cout << "After the process is preempted its new Arrival time is " << newArrivalTime << endl;
            //cout << "Which is the cputime " << cpuTime  << " + " << process[0].getData(0) << endl;
            
            process[0].setLastTime(cpuTime);
            return cpuTime;
        }
    }
    
    //Queue 3 need to implement preemptive functionality
    else
    {
        cout << "Process on the CPU: " << process[0].getName() << endl;
        
        
        //testing preemptive process
        int preemptTime = 0;
        int processNum = 0;
        int preemptStatus = 0;
        while (processNum < ioQueue.process.size() && preemptStatus == 0)
        {
            if (ioQueue.process[processNum].getPriorityQueue() < 3 && (ioQueue.process[processNum].getArrivalTime() - cpuTime) < process[0].getData(0))
            {
                cout << "Process on the CPU: " << process[0].getName() << endl;
                preemptStatus = 1;
                cout << "Test Process " << process[0].getName() << " is getting preempted by " << ioQueue.process[processNum].getName() << endl;
            }
            else
            {
                processNum++;
            }
        }
        if (preemptStatus == 1)
        {
            //comments to check the display output
            /*
            cout << "The current cpuTime is " << cpuTime << " in Q2-1" << endl;
            cout << "Process " << process[0].getName() << " is getting preempted becasue a Q1 became available" << endl;
            cout << "Inside the preempted loop for Q2" << endl;
            cout << "The cpuTime is currently " << cpuTime << endl;
            */
            preemptTime = process[0].getData(0) - (ioQueue.process[processNum].getArrivalTime() - cpuTime);
            process[0].set(0, preemptTime);
            cpuTime += (ioQueue.process[processNum].getArrivalTime() - cpuTime);
            process[0].setArrivalTime(cpuTime);
            process[0].setLastTime(cpuTime);
            return cpuTime;
        }
        
        
        //testing preemptive process
        //int startTime = cpuTime;
        //int endTime = cpuTime + processBurst;
        /*
        int preemptTime = 0;
        
        if(ioQueue.process[0].getPriorityQueue() < 3 && (ioQueue.process[0].getArrivalTime() - cpuTime) < process[0].getData(0))
        {
            //cout << "Inside Q3" << endl;
            //cout << "The cpuTime is curently " << cpuTime << endl;

            
            preemptTime = process[0].getData(0) - (ioQueue.process[0].getArrivalTime() - cpuTime);
            
            process[0].set(0, preemptTime);
            cpuTime = cpuTime + (ioQueue.process[0].getArrivalTime() - cpuTime);
            process[0].setArrivalTime(cpuTime);

            return cpuTime;

        }
        */
        //end of preempt testing
        cpuTime = cpuTime + processBurst;
        process[0].setLastTime(cpuTime);
    
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
        else
        {
            Process transfer = process[0];
            completeQueue.addProcess(transfer);
            popFirstItem();
        }
        return cpuTime;
    }
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

//If empty returns 1, else 0
int Queue::isEmpty()
{
    if (process.size() == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


