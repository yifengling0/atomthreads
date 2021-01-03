#include "atom.h"
#include "windows.h"
#include "win32port.h"
#include "_threadmanager.h"



static int _enable_int_cpu = 1;
static	CRITICAL_SECTION 	_critical_section;

int get_cc(void)
{
	if (_enable_int_cpu == 1) {
		EnterCriticalSection(&_critical_section);
		_enable_int_cpu = 0;
		return 0;
	}else {
		return 1;
	}
}

void set_cc(int ccr)
{
	if (ccr == 0) {
		LeaveCriticalSection(&_critical_section);
		_enable_int_cpu = 1;
	}
}


/** Set to TRUE when OS is started and running threads */
uint8_t atomOSStarted = FALSE;

/* Number of nested interrupts */
static int atomIntCnt = 0;

extern uint8_t atomOSInit(void* idle_thread_stack_bottom, uint32_t idle_thread_stack_size, uint8_t idle_thread_stack_check)
{
    InitializeCriticalSection(&_critical_section);
	return 0;
}

void atomOSStart(void)
{
	atomOSStarted = TRUE;
}

/**
 * \b atomIntEnter
 *
 * Interrupt handler entry routine.
 *
 * Must be called at the start of any interrupt handlers that may
 * call an OS primitive and make a thread ready.
 *
 * @return None
 */
void atomIntEnter(void)
{
	/* Increment the interrupt count */
	atomIntCnt++;
}

/**
 * \b atomSched
 *
 * This is an internal function not for use by application code.
 *
 * This is the main scheduler routine. It is called by the various OS
 * library routines to check if any threads should be scheduled in now.
 * If so, the context will be switched from the current thread to the
 * new one.
 *
 * The scheduler is priority-based with round-robin performed on threads
 * with the same priority. Round-robin is only performed on timer ticks
 * however. During reschedules caused by an OS operation (e.g. after
 * giving or taking a semaphore) we only allow the scheduling in of
 * threads with higher priority than current priority. On timer ticks we
 * also allow the scheduling of same-priority threads - in that case we
 * schedule in the head of the ready list for that priority and put the
 * current thread at the tail.
 *
 * @param[in] timer_tick Should be TRUE when called from the system tick
 *
 * @return None
 */
void atomSched(uint8_t timer_tick)
{
    //do nothing.
}

/**
 * \b atomIntExit
 *
 * Interrupt handler exit routine.
 *
 * Must be called at the end of any interrupt handlers that may
 * call an OS primitive and make a thread ready.
 *
 * This is responsible for calling the scheduler at the end of
 * interrupt handlers to determine whether a new thread has now
 * been made ready and should be scheduled in.
 *
 * @param timer_tick TRUE if this is a timer tick
 *
 * @return None
 */
void atomIntExit(uint8_t timer_tick)
{
    /* Decrement the interrupt count */
    atomIntCnt--;

    /* Call the scheduler */
    atomSched(timer_tick);
}


//
//extern uint8_t tcbEnqueuePriority(ATOM_TCB** tcb_queue_ptr, ATOM_TCB* tcb_ptr);
//extern ATOM_TCB* tcbDequeueHead(ATOM_TCB** tcb_queue_ptr);
//extern ATOM_TCB* tcbDequeueEntry(ATOM_TCB** tcb_queue_ptr, ATOM_TCB* tcb_ptr);
//extern ATOM_TCB* tcbDequeuePriority(ATOM_TCB** tcb_queue_ptr, uint8_t priority);
//

// TODO: 切换任务的时候调用线程函数获取当前线程的tcb
/** This is a pointer to the TCB for the currently-running thread */
static ATOM_TCB* curr_tcb = NULL;

ATOM_TCB* atomCurrentContext(void)
{
    /* Return the current thread's TCB or NULL if in interrupt context */
    if (atomIntCnt == 0) {
        curr_tcb = tcbManagerFind(GetCurrentThreadId());
        return (curr_tcb);
    }
    else {
        return (NULL);
    }
}

//
uint8_t atomThreadCreate(ATOM_TCB* tcb_ptr, uint8_t priority, void (*entry_point)(uint32_t), uint32_t entry_param, void* stack_bottom, uint32_t stack_size, uint8_t stack_check)
{
    HANDLE	handle;
    int threadId;
    uint8_t ret = ATOM_OK;

    if ((tcb_ptr == NULL) || (entry_point == NULL) || (stack_bottom == NULL)
        || (stack_size == 0))
    {
        /* Bad parameters */
        ret = ATOM_ERR_PARAM;
    }
    else {

        handle = CreateThread(NULL, stack_size, (LPTHREAD_START_ROUTINE)entry_point, 0, 0, &threadId);

        if (handle) {
            tcb_ptr->priority = priority;
            tcb_ptr->entry_point = entry_point;
            tcb_ptr->entry_param = entry_param;
        }
        else {
            ret = ATOM_ERR_PARAM;
        }
    }

    return ret;
}

#ifdef ATOM_STACK_CHECKING
uint8_t atomThreadStackCheck(ATOM_TCB* tcb_ptr, uint32_t* used_bytes, uint32_t* free_bytes);
{
    //TODO : Windows 上检查堆栈的方法实现。
    return 0;
}
#endif

void archContextSwitch(ATOM_TCB* old_tcb_ptr, ATOM_TCB* new_tcb_ptr)
{

}


void archFirstThreadRestore(ATOM_TCB* new_tcb_ptr)
{

}

/**
 * Initialise a threads stack so it can be scheduled in by
 * archFirstThreadRestore or the pend_sv_handler.
 */
void archThreadContextInit(ATOM_TCB* tcb_ptr, void* stack_top,
    void (*entry_point)(uint32_t), uint32_t entry_param)
{
    //TODO: 初始化一些堆栈信息等
}

