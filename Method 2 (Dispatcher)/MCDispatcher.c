#include "MCDispatcher.h"

// Task Struct

#define TOTALTASKS 10

struct task {
	int delay;
	char next;
	void(*tkpntr)(void);
}taskarray[TOTALTASKS + 1];

// Variables

char RunningTask, NewTask;
char rerunflag;
char RQHead, RQTail, DQHead, DQTail;
int NextDelay;


// Checks if there is a place in array
char GetNewTask(void(*ptr)(void))
{
	char foundflag = 0;
	int i;
	for (i = 1; i <= TOTALTASKS; i++)
	{
		if (taskarray[i].tkpntr == 0 && !foundflag)
		{
			NewTask = i;
			taskarray[NewTask].tkpntr = ptr;
			foundflag = 1;
		}
	}
	return foundflag;
}
// Add task to Ready Queue
void QueTask(void(*ptr)(void))
{
	if (GetNewTask(ptr))							// If there is space 
	{
		DoQueReady();								// Add to queue
	}
}

// Add task to Delay Queue
void QueDelay(void(*ptr)(void), int d)
{
	if (GetNewTask(ptr))							// If there is space 
	{
		if (d != 0)
			NextDelay = d, DoQueDelay();								// Add to queue
		else DoQueReady();
	}
}

// Adds the new task to ready queue
void DoQueReady()
{
	if (RQHead == 0)
		RQHead = NewTask;			// Add the new task	
	else taskarray[RQTail].next = NewTask;

	RQTail = NewTask;
	taskarray[RQTail].next = 0;
	NewTask = 0;
}

void DoQueDelay()
{
	int Remainder = NextDelay, OldRemainder;
	char current = DQHead, prev = 0;
	char endflag = 0;

	if (!DQHead)
		DQHead = NewTask, taskarray[DQHead].delay = NextDelay, taskarray[DQHead].next = 0;

	else while (!endflag)
	{
		Remainder = Remainder - taskarray[current].delay;
		if (Remainder <0)
		{
			if (prev == 0)
				DQHead = NewTask, OldRemainder = NextDelay;
			else taskarray[prev].next = NewTask;

			taskarray[NewTask].delay = OldRemainder;
			taskarray[NewTask].next = current;
			
			while (current != 0)
				taskarray[current].delay = taskarray[current].delay - OldRemainder, current = taskarray[current].next;

			endflag = 1;
		}
		else if (taskarray[current].next == 0)        // Reached the end of the 
		{
			taskarray[current].next = NewTask;
			taskarray[NewTask].delay = Remainder;
			taskarray[NewTask].next = 0;
			endflag = 1;
		}

		prev = current;
		current = taskarray[current].next;
		OldRemainder = Remainder;
	}
}

// Initializes the Tasks Array
void InitMulti()
{
	RQHead = RQTail = DQHead = DQTail = 0;
	rerunflag = 0;
	NextDelay = 0;
	int i;
	for (i = 0; i <= TOTALTASKS; i++)
		taskarray[i].delay = 0, taskarray[i].tkpntr = 0, taskarray[i].next = 0;
}

// ReAdd a task back to the queue
void ReRunMe(int d)
{
	NewTask = RunningTask;
    NextDelay = d;
	rerunflag = 1;
}

// Run the Queue
void Dispatch()
{	if (RQHead != 0)
	{
		RunningTask = RQHead;
		RQHead = taskarray[RunningTask].next;

		if (!RQHead)
			RQTail = 0;

		(*taskarray[RunningTask].tkpntr)();
		taskarray[RunningTask].next = 0;

        if(rerunflag)
        {
            if (NextDelay != 0)
                DoQueDelay();
            else
                DoQueReady();
        }
        else taskarray[RunningTask].tkpntr = 0;
        
        rerunflag = 0;
	}
}

void FinishDelay()
{
	if (DQHead != 0)
	{
		taskarray[DQHead].delay--;
		while (taskarray[DQHead].delay == 0 && DQHead!=0)
		{
			NewTask = DQHead;
			DQHead = taskarray[DQHead].next;
			DoQueReady();
		}
	}
	
}