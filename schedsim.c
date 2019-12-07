#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"
void findWaitingTimeRR(ProcessType plist[], int n,int quantum) 
{ 
    int rem_bt[n];
    int i, t = 0; 
    for (i = 0; i < n; i++)
    {
      rem_bt[i] = plist[i].bt;
      plist[i].wt = 0; 
    }
    int is_done = 0; 
    while(!is_done)
    {
      is_done = 1; 
      for(i = 0; i < n; i++)
      {
        if (rem_bt[i] > 0)
        {
          is_done = 0;
          if (rem_bt[i] > quantum)
          {
            t = t + quantum;
            rem_bt[i] -= quantum;
          }
          else
          {
            t = t + rem_bt[i];
            plist[i].wt = t - plist[i].bt;
            rem_bt[i] = 0;  
          }
        }

      }
    }
} 
void findWaitingTimeSJF(ProcessType plist[], int n)
{       
      int i, process_counter = 0, current_time = 0, completion_time = 0; 
      int rem_bt[n];
      for(i = 0; i < n; i++)
      {
        rem_bt[i] = plist[i].bt;
      }
      while(process_counter < n)
      {
        int minimum_index = 0;
        for(i = 1; i < n; i++)
        {
          if (rem_bt[i] < rem_bt[minimum_index])
          {
            minimum_index = i;
          }
        }
        rem_bt[minimum_index] -= 1;
        if (rem_bt[minimum_index] == 0)
        {
          process_counter += 1;
          completion_time = current_time + 1;          
          plist[minimum_index].wt = completion_time - plist[minimum_index].bt;
          rem_bt[minimum_index] = 9999;
        }
      current_time += 1;
      }
} 
void findWaitingTime(ProcessType plist[], int n)
{ 
    plist[0].wt = 0 +  plist[0].art; 
    for (int  i = 1; i < n ; i++ ) 
        plist[i].wt =  plist[i-1].bt + plist[i-1].wt; 
} 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
int my_comparer(const void *this, const void *that)
{ 
    if (((ProcessType*)this)->pri < ((ProcessType*)that)->pri)
    {
        return 1;
    }
    return 0;
} 
void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
    findWaitingTime(plist, n);  
    findTurnAroundTime(plist, n); 
    printf("\n*********\nFCFS\n");
}
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    findWaitingTimeSJF(plist, n); 
    findTurnAroundTime(plist, n); 
    printf("\n*********\nSJF\n");
}
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    findWaitingTimeRR(plist, n, quantum); 
    findTurnAroundTime(plist, n); 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}
void findavgTimePriority( ProcessType plist[], int n) 
{ 
    qsort(plist, n, sizeof(ProcessType), my_comparer);	
    findWaitingTime(plist, n); 
    findTurnAroundTime(plist, n); 
    printf("\n*********\nPriority\n");
}
void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 
ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) 
    {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }
    ProcessType *plist = parse_file(input_file, n);  
    fclose(input_file);  
    return plist;
}
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;
    ProcessType *process_list;  
    if (argc < 2) 
    {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	  }
    n = 0;
    process_list = initProc(argv[1], &n);  
    findavgTimeFCFS(process_list, n);    
    printMetrics(process_list, n);
    n = 0;
    process_list = initProc(argv[1], &n);   
    findavgTimeSJF(process_list, n);    
    printMetrics(process_list, n);
    n = 0; 
    process_list = initProc(argv[1], &n);    
    findavgTimePriority(process_list, n);     
    printMetrics(process_list, n);
    n = 0;
    process_list = initProc(argv[1], &n);    
    findavgTimeRR(process_list, n, quantum); 
    printMetrics(process_list, n);
    return 0; 
} 