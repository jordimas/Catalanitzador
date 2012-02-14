﻿/* 
 * Copyright (C) 2012 Jordi Mas i Hernàndez <jmas@softcatala.org>
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
 
#pragma once

#include "Action.h"

#include <vector>
#include <map>
using namespace std;

class ShowLicensesDlgUI
{
public:	
		ShowLicensesDlgUI();
		~ShowLicensesDlgUI();
		void Run(HWND hWnd);
		void SetActions(vector <Action *> * value) { m_actions =  value;}

private:
		static void _setLicenseTextForItem(HWND hWndDlg, int index);
		static LRESULT CALLBACK _dlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
		HMODULE m_handle;
		vector <Action *> * m_actions;
};
