#include "atom.h"
#include "atomqueue.h"

uint8_t atomQueueCreate(ATOM_QUEUE* qptr, uint8_t* buff_ptr, uint32_t unit_size, uint32_t max_num_msgs)
{
	return ATOM_OK;
}

uint8_t atomQueueDelete(ATOM_QUEUE* qptr)
{
	return ATOM_OK;
}

uint8_t atomQueueGet(ATOM_QUEUE* qptr, int32_t timeout, uint8_t* msgptr)
{
	return ATOM_OK;
}

uint8_t atomQueuePut(ATOM_QUEUE* qptr, int32_t timeout, uint8_t* msgptr)
{
	return ATOM_OK;
}