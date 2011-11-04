﻿/* 
 * Copyright (C) 2011 Jordi Mas i Hernàndez <jmas@softcatala.org>
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
#include "PropertyPageUI.h"
#include "Serializer.h"

#include <vector>
using namespace std;

class FinishPropertyPageUI: public PropertyPageUI
{
public:
		FinishPropertyPageUI();
		~FinishPropertyPageUI();
		void SetActions(vector <Action *> * value) { m_actions = value;}
		void SetSerializer(Serializer* serializer) { m_serializer = serializer; }
		void SetSendStats(BOOL *pbSendStats) { m_pbSendStats = pbSendStats;}

private:
		virtual void _onInitDialog();
		virtual	void _onFinish();
		bool _isRebootNeed();
		static DWORD WINAPI _uploadXmlThead(LPVOID lpParam);
		
		HANDLE m_hThread;
		vector <Action *> * m_actions;
		Serializer* m_serializer;
		BOOL* m_pbSendStats;
};
