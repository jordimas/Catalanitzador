/* 
 * Copyright (C) 2011 Jordi Mas i Hern�ndez <jmas@softcatala.org>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
 * 02111-1307, USA.
 */

#include "stdafx.h"
#include "Runner.h"
#include "Wow64.h"
#include "psapi.h"

bool Runner::Execute(wchar_t* program, wchar_t* params, bool b64bits)
{
	STARTUPINFO si;
	PVOID OldValue = NULL;
	Wow64 wow64;

	ZeroMemory (&si, sizeof (si));
	si.cb = sizeof(si);
	ZeroMemory (&pi, sizeof (pi));
	
	if (b64bits)
	{
		wow64.DisableFsRedirection(&OldValue);
	}

	// Start the child process
	if (!CreateProcess(program, params,         
		NULL,  NULL,  FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		int error = GetLastError();
		g_log.Log (L"Runner::Execute. Error %u running '%s' with params '%s'", (wchar_t *)error,
			program, params);

		return false;
	}

	if (b64bits)
	{
		wow64.RevertRedirection(OldValue);		
	}
	
	return true;
}

bool Runner::IsRunning()
{
	DWORD dwStatus;

	GetExitCodeProcess(pi.hProcess, &dwStatus);
	return dwStatus == STILL_ACTIVE;
}

void Runner::WaitUntilFinished()
{
	WaitForSingleObject(pi.hProcess, INFINITE);
}

DWORD Runner::GetProcessID(wstring name)
{	
	DWORD aProcesses[4096], cbNeeded, processID;
    
	processID = 0;
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
		g_log.Log (L"Runner::IsProcessRunning. Error EnumProcesses");
        return processID;
    }
    
    for (unsigned int i = 0; i < cbNeeded / sizeof(DWORD); i++)
    {
		wchar_t szProcessName[MAX_PATH];	

        if (aProcesses[i] != 0)
        {
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

			if (hProcess)
			{
				HMODULE hMod;
				DWORD cbNeeded;

				if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
				{
					GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(wchar_t));

					if (lstrcmpi(szProcessName, name.c_str()) ==0)
					{
						processID = aProcesses[i];
						break;
					}
				}
			}
			CloseHandle(hProcess);
        }
    }
	g_log.Log (L"Runner::IsProcessRunning. Process '%s' is running %u", (wchar_t *)name.c_str(), (wchar_t *)processID);
    return processID;
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{	
	DWORD processID;

	GetWindowThreadProcessId(hWnd, &processID);

	if (processID == (DWORD) lParam)
	{
		PostMessage(hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	}
	return TRUE;
}

bool Runner::RequestQuitToProcessID(DWORD processID)
{
	EnumWindows(EnumWindowsProc, processID);
	return true;
}