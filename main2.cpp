#include<stdio.h>
#include<vector>
#include<map>
#include<iterator>
#include<functional>
#include<iostream>


int TIME = 0;
int NUM,QUANTUM;
struct process
{
    int process_no;
    int Burst_Time;
    int Turnaround_Time;
    int  Waiting_Time;
    int Remaining_Time;
    int arrivalTime;
    int priority;
    bool InProcessQueue = false;
    bool IsCompleted = false;
};


void ToprocessQueue( std::multimap<int,process, std::greater <int> > &process_queue, std::vector<process> &ready_queue)
{
    std::vector<process> :: iterator ptr;
    for(ptr = ready_queue.begin(); ptr != ready_queue.end(); ptr++)
    {
        if(ptr->arrivalTime <= TIME && ptr->InProcessQueue == false)
        {
            process_queue.insert(std::pair<int,process>(ptr->priority,*ptr));
            ptr->InProcessQueue = true;
        }
    }
}
void ToFinalQueue( std::multimap<int,process, std::greater <int> > &process_queue, std::vector<process> &final_queue)
{
    std::multimap<int,process> :: iterator ptr;
    for(ptr = process_queue.begin(); ptr != process_queue.end(); ptr++)
    {
        if(ptr->second.Remaining_Time == 0 && ptr->second.IsCompleted == false)
        {
            final_queue.push_back(ptr->second);
            ptr->second.IsCompleted = true;
            ptr->second.priority = 0;
            process_queue.erase(ptr);
            break;
        }
    }
}


void RoundRobin_Schedular(std::multimap<int,process, std::greater <int> > &process_queue,std::vector<process> &ready_queue,std::vector<process> &final_queue)
{
    if(process_queue.empty())
    {
        TIME++;
        ToprocessQueue(process_queue,ready_queue);
    }
    else
    {
        std::multimap<int,process> :: iterator ptr;
        ptr = process_queue.begin();
        {
            if(ptr->second.Remaining_Time > QUANTUM)
            {
                std::multimap<int,process> :: iterator ptr1;
                for(int i=0;i<QUANTUM;i++)
                {
                    for(ptr1 = process_queue.begin(); ptr1 != process_queue.end(); ++ptr1)
                    {
                        if(ptr1 != ptr && ptr1->second.IsCompleted == false)
                        {
                            ptr1->second.Waiting_Time += 1;                      
                        }
                        else if(ptr == ptr1)
                        {
                            ptr->second.Remaining_Time -= 1;
                           
                        }
                    }
                    TIME++;
                    
                    ToprocessQueue(process_queue,ready_queue);
                }
                
                process_queue.erase(ptr);
            	process_queue.insert(std::pair<int ,process>(ptr->first,ptr->second));
            }
            else if(ptr->second.Remaining_Time == 0)
            {
                process_queue.erase(ptr);
                process_queue.insert(std::pair<int ,process>(ptr->first,ptr->second)); 
            }
            else
            {
                std::multimap<int ,process> ::iterator ptr1;
                for(int i=0;i<ptr->second.Remaining_Time;i++)
                {
                    for(ptr1 = process_queue.begin(); (ptr1 != process_queue.end()); ptr1++)
                    {
                        if(ptr1 != ptr && ptr1->second.IsCompleted == false)
                        {
                            ptr1->second.Waiting_Time += 1;                      
                        }
                        else if(ptr == ptr1)
                        {
                    		continue;
                        }
                    }
                    TIME++;
                    ToprocessQueue(process_queue,ready_queue);
                    
                }               
                ptr->second.Remaining_Time = 0;
                process_queue.erase(ptr);
                process_queue.insert(std::pair<int ,process>(ptr->first,ptr->second));
            }
        
        }
    
    }

}

void print(std::multimap<int,process, std::greater <int> > &tempQ)
{
    std::cout<<" process queue  "<<std::endl;
    std::multimap<int,process> :: iterator ptr;
    std::cout<<"process no\t"<<"Remaining Time\t"<<" Waiting Time\t"<<"in queue\t"<<"priror\t"<<std::endl;
	for(ptr = tempQ.begin(); ptr != tempQ.end(); ptr++)
    {
    	
        std::cout<<" \t"<<ptr->second.process_no<<"\t"<<ptr->second.Remaining_Time<<" \t" <<ptr->second.Waiting_Time<<" \t" <<ptr->second.InProcessQueue<<"\t"<<ptr->second.priority<<std::endl;
    }
}

void print(std::vector<process> &tempQ)
{
    std::cout<<" ready queue or final "<<std::endl;
    std::vector<process> :: iterator ptr;
    std::cout<<"Remaining Time\t"<<" Waiting Time"<<std::endl;
	for(ptr = tempQ.begin(); ptr != tempQ.end(); ptr++)
    {
    	
        std::cout<<" \t"<<ptr->process_no<<"\t"<<ptr->Remaining_Time<<" \t" <<ptr->Waiting_Time<<std::endl;
    }
}

int main()
{
    printf("Enter the number of process \n");
    scanf(" %d",&NUM);
    printf("Enter time QUANTUM\n");
    scanf("%d",&QUANTUM);
    struct process p[1000];
    std::vector<process> ready_queue;
    std::vector<process> final_queue;
    std::multimap<int,process, std::greater <int> > process_queue;

    for(int i=0;i<NUM;i++)
    {
        printf("\n Enter the burst time and arrival time and priority of process[%d] ",i+1);
        scanf("%d %d %d",&p[i].Burst_Time,&p[i].arrivalTime,&p[i].priority);
        p[i].Remaining_Time = p[i].Burst_Time;
        p[i].Waiting_Time = 0;
        p[i].process_no = i+1;
        ready_queue.push_back(p[i]);
    }
    print(ready_queue);
    ToprocessQueue(process_queue,ready_queue);
    print(process_queue);
    do
    {
        RoundRobin_Schedular(process_queue,ready_queue,final_queue);
        ToFinalQueue(process_queue,final_queue);
        print(process_queue);
        std::cout<<final_queue.size()<<std::endl;
        std::cout<<process_queue.size();
        std::cout<<std::endl<<TIME<<std::endl;
		/*char c;
        std::cin>>c;*/
    } while (final_queue.size() != NUM);
    print(final_queue);
}
