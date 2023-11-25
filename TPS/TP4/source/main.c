#include "hardware.h"
#include "Drivers/board.h"

#include  <os.h>
#include <stddef.h>
#include <stdio.h>

/* Task Start */
#define TASKSTART_STK_SIZE 		512u
#define TASKSTART_PRIO 			2u
static OS_TCB TaskStartTCB;
static CPU_STK TaskStartStk[TASKSTART_STK_SIZE];

/* Encoder Task */


/* Card Task */



void App_Init (void);
void App_Run (void);


static void encoderTask(void *p_arg)
{
	(void)p_arg;
	OS_ERR os_err;

    OS_MSG_SIZE size;
    while(1)
    {
		//Encoder things...
    }

}

static void CardTask(void *p_arg)
{
	(void)p_arg;
	OS_ERR os_err;

    OS_MSG_SIZE size;

    while(1)
    {
		//CaardReader things...
	}

}


static void TaskStart(void *p_arg)
{
    (void)p_arg;
    OS_ERR os_err;

    /* Initialize the uC/CPU Services. */
    CPU_Init();

#if OS_CFG_STAT_TASK_EN > 0u
    /* (optional) Compute CPU capacity with no task running */
    OSStatTaskCPUUsageInit(&os_err);
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    App_Init();

    /* Create Tasks */


    while (1) {
        App_Run();
    }
}

int main(void)
{
    OS_ERR err;

#if (CPU_CFG_NAME_EN == DEF_ENABLED)
    CPU_ERR  cpu_err;
#endif

    hw_Init();

    OSInit(&err);
 #if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u
	 /* Enable task round robin. */
	 OSSchedRoundRobinCfg((CPU_BOOLEAN)1, 0, &err);
 #endif
    OS_CPU_SysTickInit(SystemCoreClock / (uint32_t)OSCfg_TickRate_Hz);

   OSTaskCreate(&TaskStartTCB,
                "App Task Start",
                 TaskStart,
                 0u,
                TASKSTART_PRIO,
                &TaskStartStk[0u],
                (TASKSTART_STK_SIZE / 10u),
                 TASKSTART_STK_SIZE,
                 0u,
                 0u,
                 0u,
                (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                &err);

    OSStart(&err);

	/* Should Never Get Here */
    while (1) {

    }
}
