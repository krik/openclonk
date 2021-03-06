/*
 * OpenClonk, http://www.openclonk.org
 *
 * Copyright (c) 1998-2000, Matthes Bender
 * Copyright (c) 2001-2009, RedWolf Design GmbH, http://www.clonk.de/
 * Copyright (c) 2009-2013, The OpenClonk Team and contributors
 *
 * Distributed under the terms of the ISC license; see accompanying file
 * "COPYING" for details.
 *
 * "Clonk" is a registered trademark of Matthes Bender, used with permission.
 * See accompanying file "TRADEMARK" for details.
 *
 * To redistribute this file separately, substitute the full license texts
 * for the above references.
 */

/* Value to identify object definitions */

#include "C4Include.h"
#include "C4Id.h"
#include "StdCompiler.h"

#include <utility>

C4ID::NamesList C4ID::names;
C4ID::LookupTable C4ID::lookup;

const C4ID C4ID::None(std::string("None"));
const C4ID C4ID::Clonk(std::string("Clonk"));
const C4ID C4ID::Bubble(std::string("Fx_Bubble"));

// TODO: Remove these eventually, since they are deprecated.
const C4ID C4ID::CnMaterial(std::string("CNMT"));
const C4ID C4ID::Flag(std::string("FLAG"));
const C4ID C4ID::Conkit(std::string("CNKT"));
const C4ID C4ID::Flame(std::string("FLAM"));
const C4ID C4ID::Melee(std::string("MELE"));

C4ID::C4ID(const std::string &s) { assign(s); }

void C4ID::assign(const std::string &s)
{
	LookupTable::const_iterator it = lookup.find(s);
	if (it != lookup.end())
		v = it->second;
	else
	{
		v = names.size();
		names.push_back(s);
		lookup.insert(std::make_pair(s, v));
	}
}

void C4ID::CompileFunc(StdCompiler *pComp)
{
	if (pComp->isDecompiler())
	{
		assert(v < names.size());
		pComp->String(&names[v][0], names[v].size(), StdCompiler::RCT_ID);
	}
	else
	{
		char *data;
		pComp->String(&data, StdCompiler::RCT_ID);
		v = C4ID(data).v;
		StdBuf::DeletePointer(data);
	}
}
