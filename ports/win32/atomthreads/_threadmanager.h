#ifndef _THREADMANAGERH_
#define _THREADMANAGERH_

#ifdef __cplusplus
extern "C" {
#endif

	extern void tcbManagerInit();
	extern void tcbManagerAdd(ATOM_TCB* tcb_ptr, HANDLE handle, void* stack_bottom, uint32_t stack_size);
    extern ATOM_TCB* tcbManagerFind(int threadId);

#ifdef __cplusplus
};
#endif


#endif