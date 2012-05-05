/*
 * OpenClonk, http://www.openclonk.org
 *
 * Copyright (c) 1998-2000, 2003-2004, 2007  Matthes Bender
 * Copyright (c) 2001-2007, 2009-2010  Sven Eberhardt
 * Copyright (c) 2003-2008  Peter Wortmann
 * Copyright (c) 2004-2006, 2008-2011  Günther Brammer
 * Copyright (c) 2005, 2009-2010  Armin Burgmeier
 * Copyright (c) 2009-2010  Nicolas Hake
 * Copyright (c) 2010  Richard Gerum
 * Copyright (c) 2010  Benjamin Herr
 * Copyright (c) 2001-2009, RedWolf Design GmbH, http://www.clonk.de
 *
 * Portions might be copyrighted by other authors who have contributed
 * to OpenClonk.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * See isc_license.txt for full license and disclaimer.
 *
 * "Clonk" is a registered trademark of Matthes Bender.
 * See clonk_trademark_license.txt for full license.
 */

/* Object definition */

#include <C4Include.h>
#include <C4Def.h>

#include <C4Components.h>
#include <C4Config.h>
#include <C4FileMonitor.h>
#include <C4Language.h>
#include <C4Object.h>
#include <C4RankSystem.h>
#include <C4SoundSystem.h>

void C4Def::DefaultDefCore()
{
	rC4XVer[0]=rC4XVer[1]=rC4XVer[2]=rC4XVer[3]=0;
	RequireDef.Clear();
	Shape.Default();
	Entrance.Default();
	Collection.Default();
	PictureRect.Default();
	SolidMask.Default();
	TopFace.Default();
	Component.Default();
	BurnTurnTo=C4ID::None;
	BuildTurnTo=C4ID::None;
	GrowthType=0;
	CrewMember=0;
	NativeCrew=0;
	Mass=0;
	Value=0;
	Exclusive=0;
	Category=0;
	Constructable=0;
	Rotateable=0;
	RotatedEntrance=0;
	Float=0;
	ColorByOwner=0;
	NoHorizontalMove=0;
	BorderBound=0;
	LiftTop=0;
	GrabPutGet=0;
	ContainBlast=0;
	UprightAttach=0;
	ContactFunctionCalls=0;
	Line=0;
	LineIntersect=0;
	IncompleteActivity=0;
	AttractLightning=0;
	Oversize=0;
	Fragile=0;
	NoPushEnter=0;
	Explosive=0;
	Projectile=0;
	DragImagePicture=0;
	VehicleControl=0;
	Pathfinder=0;
	NoComponentMass=0;
	MoveToRange=0;
	NoStabilize=0;
	ClosedContainer=0;
	SilentCommands=0;
	TemporaryCrew=0;
	BlitMode=C4D_Blit_Normal;
	NoBreath=0;
	ConSizeOff=0;
	NoGet=0;
	NeededGfxMode=0;
	NoTransferZones=0;
}

bool C4Def::LoadDefCore(C4Group &hGroup)
{
	StdStrBuf Source;
	if (hGroup.LoadEntryString(C4CFN_DefCore,&Source))
	{
		StdStrBuf Name = hGroup.GetFullName() + (const StdStrBuf &)FormatString("%cDefCore.txt", DirectorySeparator);
		if (!Compile(Source.getData(), Name.getData()))
			return false;
		Source.Clear();

		// Let's be bold: Rewrite, with current version
		/*rC4XVer[0] = C4XVER1; rC4XVer[1] = C4XVER2; rC4XVer[2] = C4XVER3; rC4XVer[3] = C4XVER4;
		hGroup.Rename(C4CFN_DefCore, C4CFN_DefCore ".old");
		Save(hGroup);*/

		// Check mass
		if (Mass < 0)
		{
			DebugLogF("WARNING: Def %s (%s) at %s has invalid mass!", GetName(), id.ToString(), hGroup.GetFullName().getData());
			Mass = 0;
		}

		return true;
	}
	return false;
}

bool C4Def::Save(C4Group &hGroup)
{
	StdStrBuf Out;
	if (! Decompile(&Out, FormatString("%s::DefCore.txt", id.ToString()).getData()) )
		return false;
	return hGroup.Add(C4CFN_DefCore,Out,false,true);
}

bool C4Def::Compile(const char *szSource, const char *szName)
{
	return CompileFromBuf_LogWarn<StdCompilerINIRead>(mkNamingAdapt(*this, "DefCore"), StdStrBuf(szSource), szName);
}

bool C4Def::Decompile(StdStrBuf *pOut, const char *szName)
{
	return DecompileToBuf_Log<StdCompilerINIWrite>(mkNamingAdapt(*this, "DefCore"), pOut, szName);
}

void C4Def::CompileFunc(StdCompiler *pComp)
{

	pComp->Value(mkNamingAdapt(id,                "id",                 C4ID::None          ));
	pComp->Value(mkNamingAdapt(toC4CArr(rC4XVer),             "Version"                               ));
	pComp->Value(mkNamingAdapt(mkParAdapt(RequireDef, false), "RequireDef",         C4IDList()        ));

	const StdBitfieldEntry<int32_t> Categories[] =
	{

		{ "C4D_StaticBack",               C4D_StaticBack          },
		{ "C4D_Structure",                C4D_Structure           },
		{ "C4D_Vehicle",                  C4D_Vehicle             },
		{ "C4D_Living",                   C4D_Living              },
		{ "C4D_Object",                   C4D_Object              },

		{ "C4D_Goal",                     C4D_Goal                },
		{ "C4D_Rule",                     C4D_Rule                },
		{ "C4D_Environment",              C4D_Environment         },
		
		{ "C4D_Background",               C4D_Background          },
		{ "C4D_Parallax",                 C4D_Parallax            },
		{ "C4D_MouseSelect",              C4D_MouseSelect         },
		{ "C4D_Foreground",               C4D_Foreground          },
		{ "C4D_MouseIgnore",              C4D_MouseIgnore         },
		{ "C4D_IgnoreFoW",                C4D_IgnoreFoW           },

		{ NULL,                           0                       }
	};

	pComp->Value(mkNamingAdapt(mkBitfieldAdapt<int32_t>(Category, Categories),
	                           "Category",           0             ));

	pComp->Value(mkNamingAdapt(ContactFunctionCalls,          "ContactCalls",       0                 ));
	pComp->Value(mkParAdapt(Shape, false));
	pComp->Value(mkNamingAdapt(Value,                         "Value",              0                 ));
	pComp->Value(mkNamingAdapt(Mass,                          "Mass",               0                 ));
	pComp->Value(mkNamingAdapt(Component,                     "Components",         C4IDList()        ));
	pComp->Value(mkNamingAdapt(SolidMask,                     "SolidMask",          TargetRect0       ));
	pComp->Value(mkNamingAdapt(TopFace,                       "TopFace",            TargetRect0       ));
	pComp->Value(mkNamingAdapt(PictureRect,                   "Picture",            Rect0             ));
	pComp->Value(mkNamingAdapt(StdNullAdapt(),                "PictureFE"                             ));
	pComp->Value(mkNamingAdapt(Entrance,                      "Entrance",           Rect0             ));
	pComp->Value(mkNamingAdapt(Collection,                    "Collection",         Rect0             ));
	pComp->Value(mkNamingAdapt(Exclusive,                     "Exclusive",          0                 ));
	pComp->Value(mkNamingAdapt(Line,                          "Line",               0                 ));
	pComp->Value(mkNamingAdapt(LineIntersect,                 "LineIntersect",      0                 ));
	pComp->Value(mkNamingAdapt(CrewMember,                    "CrewMember",         0                 ));
	pComp->Value(mkNamingAdapt(NativeCrew,                    "NoStandardCrew",     0                 ));
	pComp->Value(mkNamingAdapt(Constructable,                 "Construction",       0                 ));
	pComp->Value(mkNamingAdapt(BuildTurnTo,                   "ConstructTo",        C4ID::None        ));

	const StdBitfieldEntry<int32_t> GrabPutGetTypes[] =
	{

		{ "C4D_GrabGet"            ,C4D_Grab_Get},
		{ "C4D_GrabPut"            ,C4D_Grab_Put},

		{ NULL,                     0}
	};

	pComp->Value(mkNamingAdapt(mkBitfieldAdapt(GrabPutGet, GrabPutGetTypes),
	                           "GrabPutGet",         0                 ));

	pComp->Value(mkNamingAdapt(Rotateable,                    "Rotate",             0                 ));
	pComp->Value(mkNamingAdapt(RotatedEntrance,               "RotatedEntrance",    0                 ));
	pComp->Value(mkNamingAdapt(Float,                         "Float",              0                 ));
	pComp->Value(mkNamingAdapt(ContainBlast,                  "ContainBlast",       0                 ));
	pComp->Value(mkNamingAdapt(ColorByOwner,                  "ColorByOwner",       0                 ));
	pComp->Value(mkNamingAdapt(NoHorizontalMove,              "HorizontalFix",      0                 ));
	pComp->Value(mkNamingAdapt(BorderBound,                   "BorderBound",        0                 ));
	pComp->Value(mkNamingAdapt(LiftTop,                       "LiftTop",            0                 ));
	pComp->Value(mkNamingAdapt(UprightAttach,                 "UprightAttach",      0                 ));
	pComp->Value(mkNamingAdapt(GrowthType,                    "StretchGrowth",      0                 ));
	pComp->Value(mkNamingAdapt(IncompleteActivity,            "IncompleteActivity", 0                 ));
	pComp->Value(mkNamingAdapt(AttractLightning,              "AttractLightning",   0                 ));
	pComp->Value(mkNamingAdapt(Oversize,                      "Oversize",           0                 ));
	pComp->Value(mkNamingAdapt(Fragile,                       "Fragile",            0                 ));
	pComp->Value(mkNamingAdapt(Explosive,                     "Explosive",          0                 ));
	pComp->Value(mkNamingAdapt(Projectile,                    "Projectile",         0                 ));
	pComp->Value(mkNamingAdapt(NoPushEnter,                   "NoPushEnter",        0                 ));
	pComp->Value(mkNamingAdapt(DragImagePicture,              "DragImagePicture",   0                 ));
	pComp->Value(mkNamingAdapt(VehicleControl,                "VehicleControl",     0                 ));
	pComp->Value(mkNamingAdapt(Pathfinder,                    "Pathfinder",         0                 ));
	pComp->Value(mkNamingAdapt(MoveToRange,                   "MoveToRange",        0                 ));
	pComp->Value(mkNamingAdapt(NoComponentMass,               "NoComponentMass",    0                 ));
	pComp->Value(mkNamingAdapt(NoStabilize,                   "NoStabilize",        0                 ));
	pComp->Value(mkNamingAdapt(ClosedContainer,               "ClosedContainer",    0                 ));
	pComp->Value(mkNamingAdapt(SilentCommands,                "SilentCommands",     0                 ));
	pComp->Value(mkNamingAdapt(TemporaryCrew,                 "TemporaryCrew",      0                 ));
	pComp->Value(mkNamingAdapt(BlitMode,                      "BlitMode",           C4D_Blit_Normal   ));
	pComp->Value(mkNamingAdapt(NoBreath,                      "NoBreath",           0                 ));
	pComp->Value(mkNamingAdapt(ConSizeOff,                    "ConSizeOff",         0                 ));
	pComp->Value(mkNamingAdapt(NoGet,                         "NoGet",              0                 ));
	pComp->Value(mkNamingAdapt(NoTransferZones,               "NoTransferZones",    0                 ));
	pComp->Value(mkNamingAdapt(NeededGfxMode,                 "NeededGfxMode",      0                 ));

	const StdBitfieldEntry<int32_t> AllowPictureStackModes[] =
	{

		{ "APS_Color",      APS_Color    },
		{ "APS_Graphics",   APS_Graphics },
		{ "APS_Name",       APS_Name     },
		{ "APS_Overlay",    APS_Overlay  },
		{ NULL,             0            }
	};

	pComp->Value(mkNamingAdapt(mkBitfieldAdapt<int32_t>(AllowPictureStack, AllowPictureStackModes),
	                           "AllowPictureStack",   0                ));
}

//-------------------------------- C4Def -------------------------------------------------------

C4Def::C4Def(): Script(this), C4PropList(ScriptEngine.GetPropList())
{
	assert(ScriptEngine.GetPropList());
	Graphics.pDef = this;
	Default();
}

void C4Def::Default()
{
	DefaultDefCore();
	Next=NULL;
	Temporary=false;
	Filename[0]=0;
	Creation=0;
	Count=0;
	MainFace.Set(NULL,0,0,0,0);
	Script.Clear();
	StringTable.Clear();
	pClonkNames=NULL;
	pRankNames=NULL;
	pRankSymbols=NULL;
	fClonkNamesOwned = fRankNamesOwned = fRankSymbolsOwned = false;
	iNumRankSymbols=1;
}

C4Def::~C4Def()
{
	Clear();
}

void C4Def::Clear()
{
	C4PropList::Clear();

	Graphics.Clear();

	Script.Clear();
	StringTable.Clear();
	if (pClonkNames && fClonkNamesOwned) delete pClonkNames; pClonkNames=NULL;
	if (pRankNames && fRankNamesOwned) delete pRankNames; pRankNames=NULL;
	if (pRankSymbols && fRankSymbolsOwned) delete pRankSymbols; pRankSymbols=NULL;
	fClonkNamesOwned = fRankNamesOwned = fRankSymbolsOwned = false;
}

bool C4Def::Load(C4Group &hGroup,
                 DWORD dwLoadWhat,
                 const char *szLanguage,
                 C4SoundSystem *pSoundSystem)
{
	bool fSuccess=true;

	bool AddFileMonitoring = false;
	if (Game.pFileMonitor && !SEqual(hGroup.GetFullName().getData(),Filename) && !hGroup.IsPacked())
		AddFileMonitoring = true;

	// Store filename
	SCopy(hGroup.GetFullName().getData(),Filename);

	// Verbose log filename
	if (Config.Graphics.VerboseObjectLoading>=3)
		Log(hGroup.GetFullName().getData());

	if (AddFileMonitoring) Game.pFileMonitor->AddDirectory(Filename);

	// particle def?
	if (hGroup.AccessEntry(C4CFN_ParticleCore))
	{
		// def loading not successful; abort after reading sounds
		fSuccess=false;
		// create new particle def
		C4ParticleDef *pParticleDef = new C4ParticleDef();
		// load it
		if (!pParticleDef->Load(hGroup))
		{
			// not successful :( - destroy it again
			delete pParticleDef;
		}
		// done
	}


	// Read DefCore
	if (fSuccess) fSuccess=LoadDefCore(hGroup);

	// skip def: don't even read sounds!
	if (fSuccess && Game.C4S.Definitions.SkipDefs.GetIDCount(id, 1)) return false;

	if (!fSuccess)
	{

		// Read sounds even if not a valid def (for pure ocd sound folders)
		if (dwLoadWhat & C4D_Load_Sounds)
			if (pSoundSystem)
				pSoundSystem->LoadEffects(hGroup);

		return false;
	}

	// Read surface bitmap
	if (dwLoadWhat & C4D_Load_Bitmap)
		if (!Graphics.Load(hGroup, !!ColorByOwner))
		{
			DebugLogF("  Error loading graphics of %s (%s)", hGroup.GetFullName().getData(), id.ToString());
			return false;
		}

	// Read string table
	StringTable.LoadEx(hGroup, C4CFN_ScriptStringTbl, szLanguage);

	// Register ID with script engine
	::ScriptEngine.RegisterGlobalConstant(id.ToString(), C4VPropList(this));

	// Read script
	if (dwLoadWhat & C4D_Load_Script)
	{
		// reg script to engine
		Script.Reg2List(&::ScriptEngine);
		// Load script
		Script.Load(hGroup, C4CFN_Script, szLanguage, &StringTable);
	}

	// read clonknames
	if (dwLoadWhat & C4D_Load_ClonkNames)
	{
		// clear any previous
		if (pClonkNames) delete pClonkNames; pClonkNames=NULL;
		if (hGroup.FindEntry(C4CFN_ClonkNameFiles))
		{
			// create new
			pClonkNames = new C4ComponentHost();
			if (!pClonkNames->LoadEx(hGroup, C4CFN_ClonkNames, szLanguage))
			{
				delete pClonkNames; pClonkNames = NULL;
			}
			else
				fClonkNamesOwned = true;
		}
	}

	// read clonkranks
	if (dwLoadWhat & C4D_Load_RankNames)
	{
		// clear any previous
		if (pRankNames) delete pRankNames; pRankNames=NULL;
		if (hGroup.FindEntry(C4CFN_RankNameFiles))
		{
			// create new
			pRankNames = new C4RankSystem();
			// load from group
			if (!pRankNames->Load(hGroup, C4CFN_RankNames, 1000, szLanguage))
			{
				delete pRankNames; pRankNames=NULL;
			}
			else
				fRankNamesOwned = true;
		}
	}

	// read rankfaces
	if (dwLoadWhat & C4D_Load_RankFaces)
	{
		// clear any previous
		if (pRankSymbols) delete pRankSymbols; pRankSymbols=NULL;
		// load new
		if (hGroup.AccessEntry(C4CFN_RankFacesPNG))
		{
			pRankSymbols = new C4FacetSurface();
			if (!pRankSymbols->GetFace().ReadPNG(hGroup)) { delete pRankSymbols; pRankSymbols=NULL; }
		}
		// set size
		if (pRankSymbols)
		{
			pRankSymbols->Set(&pRankSymbols->GetFace(), 0,0, pRankSymbols->GetFace().Hgt,pRankSymbols->GetFace().Hgt);
			int32_t Q; pRankSymbols->GetPhaseNum(iNumRankSymbols, Q);
			if (!iNumRankSymbols) { delete pRankSymbols; pRankSymbols=NULL; }
			else
			{
				if (pRankNames)
				{
					// if extended rank names are defined, subtract those from the symbol count. The last symbols are used as overlay
					iNumRankSymbols = Max<int32_t>(1, iNumRankSymbols - pRankNames->GetExtendedRankNum());
				}
				fRankSymbolsOwned = true;
			}
		}
	}

	// Read sounds
	if (dwLoadWhat & C4D_Load_Sounds)
		if (pSoundSystem)
			pSoundSystem->LoadEffects(hGroup);

	if(Graphics.Type == C4DefGraphics::TYPE_Bitmap)
	{
		// Bitmap post-load settings
		if (Graphics.GetBitmap())
		{
			// check SolidMask
			if (SolidMask.x<0 || SolidMask.y<0 || SolidMask.x+SolidMask.Wdt>Graphics.Bmp.Bitmap->Wdt || SolidMask.y+SolidMask.Hgt>Graphics.Bmp.Bitmap->Hgt) SolidMask.Default();
			// Set MainFace (unassigned bitmap: will be set by GetMainFace())
			MainFace.Set(NULL,0,0,Shape.Wdt,Shape.Hgt);
		}

		// Adjust picture rect
		if ((PictureRect.Wdt==0) || (PictureRect.Hgt==0))
			PictureRect.Set(0,0,Shape.Wdt*Graphics.Bmp.Bitmap->Scale, Shape.Hgt*Graphics.Bmp.Bitmap->Scale);

		// validate TopFace
		if (TopFace.x<0 || TopFace.y<0 || TopFace.x+TopFace.Wdt>Graphics.Bmp.Bitmap->Wdt || TopFace.y+TopFace.Hgt>Graphics.Bmp.Bitmap->Hgt)
		{
			TopFace.Default();
			// warn in debug mode
			DebugLogF("invalid TopFace in %s (%s)", GetName(), id.ToString());
		}
	}
	else
	{
		TopFace.Default();
		PictureRect.Default();
		SolidMask.Default();
	}

	// Temporary flag
	if (dwLoadWhat & C4D_Load_Temporary) Temporary=true;

	return true;
}

void C4Def::Draw(C4Facet &cgo, bool fSelected, DWORD iColor, C4Object *pObj, int32_t iPhaseX, int32_t iPhaseY, C4DrawTransform* trans)
{
	if(fSelected)
		pDraw->DrawBoxDw(cgo.Surface, cgo.X, cgo.Y, cgo.X + cgo.Wdt - 1, cgo.Y + cgo.Hgt - 1, C4RGB(0xca, 0, 0));

	C4DefGraphics* graphics = pObj ? pObj->GetGraphics() : &Graphics;
	graphics->Draw(cgo, iColor, pObj, iPhaseX, iPhaseY, trans);
}

int32_t C4Def::GetValue(C4Object *pInBase, int32_t iBuyPlayer)
{
	C4Value r = Call(PSF_CalcDefValue, &C4AulParSet(C4VObj(pInBase), C4VInt(iBuyPlayer)));
	int32_t iValue = Value;
	if (r != C4VNull)
		iValue = r.getInt();
	// do any adjustments based on where the item is bought
	if (pInBase)
	{
		r = pInBase->Call(PSF_CalcBuyValue, &C4AulParSet(C4VPropList(this), C4VInt(iValue)));
		if (r != C4VNull)
			iValue = r.getInt();
	}
	return iValue;
}

void C4Def::Synchronize()
{
}

int32_t C4Def::GetComponentCount(C4ID idComponent)
{
	return Component.GetIDCount(idComponent);
}

C4ID C4Def::GetIndexedComponent(int32_t idx)
{
	return Component.GetID(idx);
}

void C4Def::GetComponents(C4IDList *pOutList, C4Object *pObjInstance)
{
	assert(pOutList);
	assert(!pOutList->GetNumberOfIDs());
	// no valid script overload: Assume object or definition components
	if (pObjInstance)
		*pOutList = pObjInstance->Component;
	else
		*pOutList = Component;
}

void C4Def::IncludeDefinition(C4Def *pIncludeDef)
{
	// inherited rank infos and clonk names, if this definition doesn't have its own
	if (!fClonkNamesOwned) pClonkNames = pIncludeDef->pClonkNames;
	if (!fRankNamesOwned) pRankNames = pIncludeDef->pRankNames;
	if (!fRankSymbolsOwned) { pRankSymbols = pIncludeDef->pRankSymbols; iNumRankSymbols = pIncludeDef->iNumRankSymbols; }
}

void C4Def::ResetIncludeDependencies()
{
	// clear all pointers into foreign defs
	if (!fClonkNamesOwned) pClonkNames = NULL;
	if (!fRankNamesOwned) pRankNames = NULL;
	if (!fRankSymbolsOwned) { pRankSymbols = NULL; iNumRankSymbols = 0; }
}

C4PropList *C4Def::GetActionByName(const char *actname)
{
	if (!actname) return NULL;
	C4String * actname_str = Strings.RegString(actname);
	actname_str->IncRef();
	C4PropList *r = GetActionByName(actname_str);
	actname_str->DecRef();
	return r;
}

C4PropList *C4Def::GetActionByName(C4String *actname)
{
	assert(actname);
	// If we get the null string or ActIdle by name, return NULL action
	if (!actname || actname == &Strings.P[P_Idle]) return NULL;
	// otherwise, query actmap
	C4Value ActMap; GetProperty(P_ActMap, &ActMap);
	if (!ActMap.getPropList()) return NULL;
	C4Value Action; ActMap.getPropList()->GetPropertyByS(actname, &Action);
	if (!Action.getPropList()) return NULL;
	return Action.getPropList();
}