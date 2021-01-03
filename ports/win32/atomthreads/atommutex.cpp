#include "atom.h"
#include "atommutex.h"
#include "windows.h"
#include <map>

using namespace std;

static map<ATOM_MUTEX*, HANDLE> _mutexMaps;
static	CRITICAL_SECTION 	_critical_section;

static HANDLE FindHanle(ATOM_MUTEX* mutex) 
{
	HANDLE handle = INVALID_HANDLE_VALUE;

	EnterCriticalSection(&_critical_section);
	auto it = _mutexMaps.find(mutex);

	if (it != _mutexMaps.end()) {
		handle = it->second;
	}
	LeaveCriticalSection(&_critical_section);

	return (handle);
}

uint8_t atomMutexCreate(ATOM_MUTEX* mutex)
{
	
	uint8_t ret = ATOM_OK;

	/* Parameter check */
	if (mutex == NULL)
	{
		/* Bad mutex pointer */
		ret = ATOM_ERR_PARAM;
	}
	else
	{
		HANDLE handle = CreateMutex(NULL, FALSE, NULL);

		if (handle != INVALID_HANDLE_VALUE) {
			mutex->count = 0;
			EnterCriticalSection(&_critical_section);
			_mutexMaps.insert(make_pair(mutex, handle));
			LeaveCriticalSection(&_critical_section);
		}
	}

	return ATOM_OK;
}

uint8_t atomMutexDelete(ATOM_MUTEX* mutex)
{
	uint8_t ret = ATOM_OK;

	/* Parameter check */
	if (mutex == NULL)
	{
		/* Bad mutex pointer */
		ret = ATOM_ERR_PARAM;
	}
	else
	{
		auto handle = FindHanle(mutex);

		if (handle != INVALID_HANDLE_VALUE) {
			CloseHandle(handle);
			_mutexMaps.erase(mutex);
		}
		else {
			ret = ATOM_ERR_DELETED;
		}
	}

	return ret;
}

uint8_t atomMutexGet(ATOM_MUTEX* mutex, int32_t timeout)
{
	uint8_t ret = ATOM_OK;

	/* Parameter check */
	if (mutex == NULL)
	{
		/* Bad mutex pointer */
		ret = ATOM_ERR_PARAM;
	}
	else
	{
		auto handle = FindHanle(mutex);

		if (handle != INVALID_HANDLE_VALUE) {
			if (timeout == -1) ret = ATOM_ERR_PARAM;
			else {
				DWORD dw = timeout == 0 ? INFINITE : timeout;
				WaitForSingleObject(handle, dw);
			}
		}
	}

	return ret;
}

uint8_t atomMutexPut(ATOM_MUTEX* mutex)
{
	uint8_t ret = ATOM_OK;

	/* Parameter check */
	if (mutex == NULL)
	{
		/* Bad mutex pointer */
		ret = ATOM_ERR_PARAM;
	}
	else
	{
		auto handle = FindHanle(mutex);

		if (handle != INVALID_HANDLE_VALUE) {
			ReleaseMutex(handle);
		}
	}

	return ret;
}