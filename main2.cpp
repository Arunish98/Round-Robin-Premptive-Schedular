#include<stdio.h>
#include<map>
#include<iterator>
#include<vector>
#include<functional>
#include<iostream>

int time = 0;
int num,quantum;
struct process
{
    int Burst_Time;
    int Turnaround_Time;
    int  Waiting_Time;
    int Remaining_Time;
    int arrivalTime;
    int priority;
    bool inProcessQueue = false;
};

void checkProcess(std::multimap<int,process, std::greater <int> > &tempQ)
{
    std::multimap<int,process> :: iterator ptr;
    for(ptr = tempQ.begin(); ptr != tempQ.end(); ptr++)
    {
        if((ptr->second.arrivalTime <= time) && (ptr->second.inProcessQueue == false) && (ptr->second.Remaining_Time != 0))
        {
            ptr->second.inProcessQueue = true;
        }
        else if(ptr->second.Remaining_Time == 0)
        {
            ptr->second.inProcessQueue = false;
        }
    }
}


void print(std::multimap<int,process, std::greater <int> > &tempQ)
{
    std::multimap<int,process> :: iterator ptr;
    std::cout<<"Remaining Time\t"<<" Waiting Time"<<std::endl;
	for(ptr = tempQ.begin(); ptr != tempQ.end(); ptr++)
    {
    	
        std::cout<<ptr->second.Remaining_Time<<" \t" <<ptr->second.Waiting_Time<<std::endl;
    }
}


void RoundRobin_Schedular(std::multimap<int,process, std::greater <int> > &tempQ)
{
    //std::multimap<int,process, std::greater <int> > tempQ1;
    //tempQ.swap(tempQ1);
    std::multimap<int,process> :: iterator ptr;
    int count = 0;
    for(ptr = tempQ.begin();ptr != tempQ.end(); ptr++)
    {
    	
        if(ptr->second.inProcessQueue == true)
        {        
            if(ptr->second.Remaining_Time >= quantum)
            {
                ptr->second.Remaining_Time -= quantum;
                std::multimap<int,process> :: iterator ptr1;
                for(ptr1 = tempQ.begin(); (ptr1 != tempQ.end()); ptr1++)
                {
                    if(ptr1 != ptr)
                    {
                        if(ptr1->second.inProcessQueue == true)
                        {
                            ptr1->second.Waiting_Time += quantum;
                            time += quantum;
                        }
                    
                        checkProcess(tempQ);
                    }
                }
                count = 1;
            }
            else if(ptr->second.Remaining_Time > 0)
            {
                
                std::multimap<int,process> :: iterator ptr1;
                for(ptr1 = tempQ.begin(); (ptr1 != tempQ.end()); ptr1++)
                {
                    if(ptr1 != ptr)
                    {
                        if(ptr1->second.inProcessQueue == true)
                        {
                            ptr1->second.Waiting_Time += ptr->second.Remaining_Time;
                            time += ptr->second.Remaining_Time;
                        }
                        checkProcess(tempQ);
                    }
                }
                ptr->second.Remaining_Time = 0;
                count = 1;
            }
            else
            {
                continue;
            }

        }
        else
        {
            continue;
        }
        print(tempQ);
    } 
    if(count == 0)
    time++;
}

int sum(std::multimap<int,process, std::greater <int> > &tempQ)
{
    int sum = 0;
    std::multimap<int,process> :: iterator ptr;
    for(ptr = tempQ.begin(); ptr != tempQ.end(); ptr++)
    {
        sum += ptr->second.Remaining_Time;
    }
    return sum;

}





int main()
{
    printf("Enter the number of process \n");
    scanf(" %d",&num);
    printf("Enter time quantum\n");
    scanf("%d",&quantum);
    struct process p[1000];
    //std::vector<process> ready_queue;
    std::multimap<int,process, std::greater <int> > process_queue;
    for(int i=0;i<num;i++)
    {
        printf("\n Enter the burst time and arrival time of process[%d] ",i+1);
        scanf("%d %d",&p[i].Burst_Time,&p[i].arrivalTime);
        p[i].Remaining_Time = p[i].Burst_Time;
        p[i].Waiting_Time = 0;
        p[i].priority = 0;
        process_queue.insert(std::pair<int,process>(p[i].priority,p[i]));
    }

    checkProcess(process_queue);

    print(process_queue);
    do
    {
        RoundRobin_Schedular(process_queue);
        checkProcess(process_queue);
        print(process_queue);
    } while (sum(process_queue));


}
