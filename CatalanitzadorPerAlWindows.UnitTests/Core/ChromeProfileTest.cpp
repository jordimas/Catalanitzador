﻿/* 
 * Copyright (C) 2012-2013 Jordi Mas i Hernàndez <jmas@softcatala.org>
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
#include "Defines.h"
#include "ChromeProfile.h"
#include "Application.h"

using ::testing::Return;
using ::testing::_;
using ::testing::StrCaseEq;

class ChomeProfileTest : public ChromeProfile
{
public:	

	ChomeProfileTest() : ChromeProfile() {};
	ChomeProfileTest(wstring str) : ChromeProfile(str) {};

	void SetUIRelPathAndFile(wstring pathfile) { m_ui_file = pathfile; }
	void SetPreferencesRelPathAndFile(wstring pathfile) { m_preferences_file = pathfile; }

	virtual wstring GetUIRelPathAndFile() 
	{
		if (m_ui_file.empty())
			return ChromeProfile::GetUIRelPathAndFile();
		else
			return m_ui_file;
	}

	virtual wstring GetPreferencesRelPathAndFile() 
	{
		if (m_preferences_file.empty())
			return ChromeProfile::GetPreferencesRelPathAndFile();
		else
			return m_preferences_file;		
	}

private:

	wstring m_ui_file;
	wstring m_preferences_file;
};

wstring GetFileNameFromFullFileName(wstring path)
{
	wchar_t drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	_wsplitpath_s(path.c_str(), drive, dir, fname, ext);
	wstring file = fname;
	file += ext;
	return file;
}

wstring GetPathFromFullFileName(wstring path)
{
	wchar_t drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	_wsplitpath_s(path.c_str(), drive, dir, fname, ext);
	wstring file = drive;
	file += dir;
	return file;
}

TEST(ChomeProfileTest, SpanishUI_IsUiLocaleOk)
{
	wstring location;
	ChomeProfileTest chromeProfile;

	Application::GetExecutionLocation(location);
	location += L"Chrome\\SpanishUI_NoAcceptLanguage\\User Data\\";
	chromeProfile.SetPath(location);

	EXPECT_FALSE(chromeProfile.IsUiLocaleOk());
}

TEST(ChomeProfileTest, CatatanUI_IsUiLocaleOk)
{
	wstring location;
	ChomeProfileTest chromeProfile;

	Application::GetExecutionLocation(location);
	location += L"Chrome\\CatalanUI_NoAcceptLanguage\\User Data\\";
	chromeProfile.SetPath(location);

	EXPECT_TRUE(chromeProfile.IsUiLocaleOk());
}

TEST(ChomeProfileTest, SpanishUI_AcceptLanguage_es_de_br_ReadLanguageCode)
{
	wstring location, langcode;
	ChomeProfileTest chromeProfile;

	Application::GetExecutionLocation(location);
	location += L"Chrome\\SpanishUI_AcceptLanguage_es_de_br\\User Data\\";
	chromeProfile.SetPath(location);

	bool bRslt = chromeProfile.ReadLanguageCode(langcode);
	EXPECT_TRUE(bRslt);
	EXPECT_THAT(langcode, StrCaseEq(L"es,de,br"));
	EXPECT_FALSE(chromeProfile.IsAcceptLanguagesOk());
}

TEST(ChomeProfileTest,NoAcceptLanguage_ReadLanguageCode)
{
	wstring location, langcode;
	ChomeProfileTest chromeProfile;

	Application::GetExecutionLocation(location);
	location += L"Chrome\\SpanishUI_NoAcceptLanguage\\User Data\\";
	chromeProfile.SetPath(location);

	bool bRslt = chromeProfile.ReadLanguageCode(langcode);
	EXPECT_FALSE(bRslt);
	EXPECT_THAT(langcode, StrCaseEq(L""));
	EXPECT_FALSE(chromeProfile.IsAcceptLanguagesOk());
}

TEST(ChomeProfileTest, NoFile_ReadLanguageCode)
{
	wstring location, langcode;
	ChomeProfileTest chromeProfile;

	Application::GetExecutionLocation(location);	
	chromeProfile.SetPath(location);

	bool bRslt = chromeProfile.ReadLanguageCode(langcode);
	EXPECT_FALSE(bRslt);
	EXPECT_THAT(langcode, StrCaseEq(L""));
	EXPECT_FALSE(chromeProfile.IsAcceptLanguagesOk());
}

TEST(ChomeProfileTest, WriteUILocale)
{
	wstring location, langcode;
	ChomeProfileTest chromeProfile;
	TempFile tempFile;

	Application::GetExecutionLocation(location);
	location += L"Chrome\\SpanishUI_NoAcceptLanguage\\User Data\\";
	location += L"/../User Data/Local State";
	CopyFile(location.c_str(), tempFile.GetFileName().c_str(), false);
	
	chromeProfile.SetPath(GetPathFromFullFileName(tempFile.GetFileName()));
	chromeProfile.SetUIRelPathAndFile(GetFileNameFromFullFileName(tempFile.GetFileName()));

	chromeProfile.WriteUILocale();
	EXPECT_TRUE(chromeProfile.IsUiLocaleOk());	
}

TEST(ChomeProfileTest, UpdateAcceptLanguages_NoPrevLanguage)
{
	wstring location, langcode;
	ChomeProfileTest chromeProfile;
	TempFile tempFile;

	Application::GetExecutionLocation(location);
	location += L"Chrome\\SpanishUI_NoAcceptLanguage\\User Data\\";
	location += L"/../User Data/Default/Preferences";
	CopyFile(location.c_str(), tempFile.GetFileName().c_str(), false);
	
	chromeProfile.SetPath(GetPathFromFullFileName(tempFile.GetFileName()));
	chromeProfile.SetPreferencesRelPathAndFile(GetFileNameFromFullFileName(tempFile.GetFileName()));

	chromeProfile.UpdateAcceptLanguages();
	EXPECT_TRUE(chromeProfile.IsAcceptLanguagesOk());
}

TEST(ChomeProfileTest, UpdateAcceptLanguages_PrevLanguage)
{
	wstring location, langcode;
	ChomeProfileTest chromeProfile;
	TempFile tempFile;

	Application::GetExecutionLocation(location);
	location += L"Chrome\\SpanishUI_AcceptLanguage_es_de_br\\User Data\\";
	location += L"/../User Data/Default/Preferences";
	CopyFile(location.c_str(), tempFile.GetFileName().c_str(), false);
	
	chromeProfile.SetPath(GetPathFromFullFileName(tempFile.GetFileName()));
	chromeProfile.SetPreferencesRelPathAndFile(GetFileNameFromFullFileName(tempFile.GetFileName()));

	chromeProfile.UpdateAcceptLanguages();
	EXPECT_TRUE(chromeProfile.IsAcceptLanguagesOk());
}
