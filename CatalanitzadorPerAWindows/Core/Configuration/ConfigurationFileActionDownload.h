/* 
 * Copyright (C) 2012 Jordi Mas i Hern�ndez <jmas@softcatala.org>
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

#include "ActionID.h"
#include "ApplicationVersion.h"
#include <string>

using namespace std;


class ConfigurationFileActionDownload
{
	public:
			ConfigurationFileActionDownload() {}

			bool IsEmpty()
			{
				return m_urls.size() == 0 &&
					m_maxVersion.GetString().empty() &&
					m_minVersion.GetString().empty();
			}

			ApplicationVersion& GetMaxVersion() {return m_maxVersion;}
			void SetMaxVersion(ApplicationVersion version) {m_maxVersion = version;}

			ApplicationVersion& GetMinVersion() {return m_minVersion;}
			void SetMinVersion(ApplicationVersion version) {m_minVersion = version;}
	
			vector <wstring>& GetUrls() {return m_urls;}
			void AddUrl(wstring url) {m_urls.push_back(url);}

	private:

			ApplicationVersion m_maxVersion;
			ApplicationVersion m_minVersion;
			vector <wstring> m_urls;
};

