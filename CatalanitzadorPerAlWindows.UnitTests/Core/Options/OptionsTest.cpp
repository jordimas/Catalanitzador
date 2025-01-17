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

#include "stdafx.h"
#include "Defines.h"
#include "Options.h"
#include <sstream>
#include <string>

using ::testing::HasSubstr;
using ::testing::StrCaseEq;

TEST(OptionsTest, Option_SetNew)
{
	const wchar_t* VALUE = L"123";

	Option option(OptionSystemRestore, wstring(VALUE));
	Options options;
	options.Set(option);
	
	EXPECT_THAT(1, options.GetOptions().size());
}

TEST(OptionsTest, Option_SetAlreadyExisting)
{
	const wchar_t* VALUE1 = L"123";
	const wchar_t* VALUE2 = L"321";

	Options options;

	Option option(OptionSystemRestore, wstring(VALUE1));	
	options.Set(option);

	Option option2(OptionSystemRestore, wstring(VALUE2));
	options.Set(option2);
	
	EXPECT_THAT(1, options.GetOptions().size());
	wstring value = options.GetOptions().at(0).GetValue();
	EXPECT_THAT(VALUE2, StrCaseEq(value.c_str()));
}