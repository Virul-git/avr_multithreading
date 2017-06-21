#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int ToRunTask=0;

void Task0(void);
void Task1(void);

int TaskReady;
int TaskRunning;

struct Task0_dump
{
	int count;
}Task0_var;

struct Task1_dump
{
	int count;
}Task1_var;



void Task_dump_var(int count, int taskid)
{
	switch(taskid)
	{
	case 0:
		Task0_var.count = count;
		break;
	case 1:
		Task1_var.count = count;
		break;
	};
}

struct Task
{
	char* TaskName;
	int TaskPriority;
	int TaskId;
	int TaskStatus;
	
}TaskL[2];

void init_task(char *taskname, int taskpriority, int taskid, int taskstatus)
{
	TaskL[taskid].TaskName = taskname;
	TaskL[taskid].TaskPriority = taskpriority;
	TaskL[taskid].TaskId = taskid;
	TaskL[taskid].TaskStatus = taskstatus;
}

void run_task(void)
{
	switch(ToRunTask)
	{
	case 0:
		Task0();
		break;
	case 1:
		Task1();
	};

}



void scheduler(void)
{	
	if(TaskL[0].TaskStatus ==1)
	{
		ToRunTask = 1;
	}
	if(TaskL[1].TaskStatus ==1)
	{
		ToRunTask = 0;
	}
	if(TaskRunning == 0)
	{
	run_task();
	};
	
	/*
	if(TaskL[ToRunTask].TaskPriority == 1)
	{
		run_task();
	};
	*/

}



void led_blink(int count)
{
	PORTC = 0x00;
	_delay_ms(10);
	while(count >= 0)
	{
		PORTC ^= 0x01;
		_delay_ms(10);
		count--;
	}
	
}


int main(void)
{
	sei(); 			// global interrupt elabled
	
	//////////////////  set for timer1 overflow interrupt
	TCCR1B =0x01;
	TIMSK = 0x07;
	
	
	DDRC = 0x01;
	DDRB = 0xFF;
	
	
	Task0_var.count = 2;
	Task1_var.count = 1;
	
	init_task("task0",0,0,0);
	init_task("task1",0,1,0);
	
	TaskReady =1;
	TaskRunning=0;
	
	
	while(1)
	{
		
	};
	
	return 0;
};


ISR(TIMER1_OVF_vect)
{

	scheduler();


}


void Task0(void)
{	
	int count;
	TaskReady = 0;
	TaskRunning = 1;
	TaskL[0].TaskStatus=1;
	
	PORTB = 0x01;
	count=Task0_var.count;
	while(1)
	{
		led_blink(count);
		
		TaskReady = 1;
		_delay_ms(100);
		TaskReady = 0;
		
		if(ToRunTask != 0)
		{
			//Task_dump_var(count,0);
			break;
		};
	};
	TaskRunning = 0;
	PORTB = 0x00;
	TaskL[0].TaskStatus=0;
	TaskReady = 1;
}

void Task1(void)
{	
	int count;
	TaskReady = 0;
	TaskRunning = 1;
	TaskL[1].TaskStatus =1;
	
	PORTB = 0x02;
	count=Task1_var.count;
	while(1)
	{
		led_blink(count);
		
		TaskReady = 1;
		_delay_ms(100);
		TaskReady = 0;
		
		if(ToRunTask != 1)
		{
			//Task_dump_var(count,1);
			break;
		};
	};
	TaskRunning = 0;
	PORTB = 0x00;
	TaskL[1].TaskStatus =0;
	TaskReady = 1;
}