#include "atom.h"
#include "windows.h"
#include "_threadmanager.h"
#include "atomport.h"

#include <vector>

typedef struct _win_task_t {
    HANDLE      thread;
    uint32_t    stack_size;
    void*       stack_bottom;
    ATOM_TCB* tcb_ptr;
}WinTask_T;

static std::vector<WinTask_T> _tasks;
static	CRITICAL_SECTION 	_critical_section;


void tcbManagerInit()
{
    _tasks.clear();
    InitializeCriticalSection(&_critical_section);
}

void tcbManagerAdd(ATOM_TCB* tcb_ptr, HANDLE handle, void* stack_bottom, uint32_t stack_size)
{
    WinTask_T taskInfo = { handle , stack_size, stack_bottom, tcb_ptr };

    EnterCriticalSection(&_critical_section);

    _tasks.push_back(taskInfo);

    LeaveCriticalSection(&_critical_section);
}

ATOM_TCB* tcbManagerFind(int threadId)
{
    std::vector<WinTask_T>::iterator it;
    ATOM_TCB* ret = NULL;

    EnterCriticalSection(&_critical_section);

    it = std::find_if(_tasks.begin(), _tasks.end(), [threadId](WinTask_T &t)->bool {
            auto id = GetThreadId(t.thread);
            return id == threadId;
        }
    );

    if (it != _tasks.end()) {
        ret = it->tcb_ptr;
    }
        
    LeaveCriticalSection(&_critical_section);

    return (ret);
}