//MLFQ Source Code
//Kevin Fash
//Laura Casals

#include "process.hpp"
#include "queue.hpp"


int main()
{
    int cpuTime = 0;
    
    cout << "CPU Scheduler Project - MLFQ" << endl;
    cout << "Kevin Fash" << endl;
    cout << "Laura Casals" << endl << endl;
    
    //process goes {CPU burst, I/O time, CPU burst, I/O time, CPU burst, I/O time,........, last CPU burst}
    vector<int> data1 = {7, 22, 6, 19, 12, 44, 8, 21, 10, 37, 5, 24, 6, 44, 7 , 43, 8};
    vector<int> data2 = {14, 48, 15, 44, 17, 42, 22, 37, 19, 76, 14, 41, 16, 31, 17, 43, 18};
    vector<int> data3 = {8, 43, 7, 41, 6, 45, 8, 21, 9, 35, 14, 18, 5, 26, 3, 31, 6};
    vector<int> data4 = {13, 37, 4, 41, 5, 35, 12, 41, 8, 55, 15, 34, 6, 73, 5, 77, 3 };
    vector<int> data5 = {6, 34, 7, 21, 5, 44, 6, 32, 7, 28, 3, 48, 11, 44, 6, 33, 3, 28, 4};
    vector<int> data6 = {9, 32, 4, 28, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8};
    vector<int> data7 = {14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10};
    vector<int> data8 = {4, 64, 5, 53, 6, 44, 4, 73, 6, 87, 5 ,66, 8, 25, 6, 33, 9, 41, 7};
    vector<int> data9 = {13, 37, 8, 41, 7, 27, 12, 29, 5, 27, 6, 18, 3, 33, 4, 62, 6};

    //Initializing processes
    Process p1("P1", data1);
    Process p2("P2", data2);
    Process p3("P3", data3);
    Process p4("P4", data4);
    Process p5("P5", data5);
    Process p6("P6", data6);
    Process p7("P7", data7);
    Process p8("P8", data8);
    Process p9("P9", data9);
    
    //Creating vector of Process to build readyQueue
    vector<Process> transferQueue1 {p1, p2, p3, p4, p5, p6, p7, p8, p9};
    
    Queue readyQueue(transferQueue1, "readyQueue");
    Queue ioQueue("ioQueue");
    Queue completeQueue("completeQueue");
    
    
    //Start of SJF algorithm
    
    readyQueue.sortQueue();
    
    //not actual value just testing
    for (int i=0;i<25;i++)
    //while (cpuTime != 896)
    {
        cout << "Current Time: " << cpuTime << endl;
    
        cpuTime = readyQueue.runProcess(cpuTime, ioQueue, completeQueue);
    
        ioQueue.checkIOStatus(cpuTime, readyQueue);
    
        readyQueue.sortQueue();

        ioQueue.sortQueue();
        
        readyQueue.displayReadyQueue();

        ioQueue.displayIOQueue(cpuTime);
    }

    /* Turned off for testing

    //Displaying final output for SJF algorithm
    cout << "Finished" << endl;
    cout << endl;
    cout << "Total time: " << cpuTime << endl;
    cout << endl << "CPU Utlization: " << 100-((float(readyQueue.getIdle()))/(float(cpuTime))*100) << "%" << endl;


    //printing the RT, WT, TT, and Avgs
    completeQueue.printData();
    
    */
    cout << endl << endl;
    return 0;
}
