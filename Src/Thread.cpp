/****************************************************************
BeebEm - BBC Micro and Master 128 Emulator
Copyright (C) 2023  Chris Needham

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program; if not, write to the Free
Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA  02110-1301, USA.
****************************************************************/

#include "Windows.h"

#ifdef WIN32
#include <process.h>
#endif

#include "Thread.h"

/****************************************************************************/

#ifdef WIN32

Thread::Thread() :
	m_hThread(nullptr),
	m_hStartEvent(nullptr),
	m_bQuit(false)
{
}

#else

Thread::Thread()
{
}

#endif

/****************************************************************************/

Thread::~Thread()
{
}

/****************************************************************************/

bool Thread::Start()
{
	#ifdef WIN32

	m_bQuit = false;
	m_hStartEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);

	if (m_hStartEvent == nullptr)
	{
		return false;
	}

	m_hThread = reinterpret_cast<HANDLE>(_beginthreadex(
		nullptr,        // security
		0,              // stack_size
		s_ThreadFunc,   // start_address
		this,           // arglist
		0,              // initflag
		nullptr         // thrdaddr
	));

	bool bSuccess = m_hThread != nullptr;

	if (bSuccess)
	{
		WaitForSingleObject(m_hStartEvent, INFINITE);
	}

	CloseHandle(m_hStartEvent);
	m_hStartEvent = nullptr;

	return bSuccess;

	#else

	return true;

	#endif
}

/****************************************************************************/

bool Thread::IsStarted() const
{
	#ifdef WIN32

	return m_hThread != nullptr;

	#else

	return true;

	#endif
}

/****************************************************************************/

void Thread::Join()
{
	#ifdef WIN32

	m_bQuit = true;

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);
	m_hThread = nullptr;

	#endif
}

/****************************************************************************/

bool Thread::ShouldQuit() const
{
	return m_bQuit;
}

/****************************************************************************/

unsigned int __stdcall Thread::s_ThreadFunc(void *parameter)
{
	#ifdef WIN32

	Thread* pThread = reinterpret_cast<Thread*>(parameter);

	// Notify the parent thread that this thread has started running.
	SetEvent(pThread->m_hStartEvent);

	return pThread->ThreadFunc();

    #else

    return 0;

    #endif

}

/****************************************************************************/
