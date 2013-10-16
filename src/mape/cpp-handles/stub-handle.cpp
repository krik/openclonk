/*
 * mape - C4 Landscape.txt editor
 *
 * Copyright (c) 2005-2009 Armin Burgmeier
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

#include "C4Include.h"

#include "C4Aul.h"
#include "C4AulDebug.h"
#include "C4GameControl.h"
#include "C4Def.h"
#include "C4DefList.h"
#include "C4Facet.h"
#include "C4GameObjects.h"
#include "C4GameParameters.h"
#include "C4GraphicsResource.h"
#include "C4Landscape.h"
#include "C4PXS.h"
#include "C4Record.h"
#include "C4RoundResults.h"
#include "CSurface8.h"
#include "C4MapScript.h"

/* This file implements stubs for the parts of the engine that are not used
 * by mape. It also instantiates global variables required by mape that are
 * not instantiated elsewhere. In particular, we avoid C4Globals.cpp. */

/* These are actually used by mape: */
#ifdef _DEBUG
C4Set<C4PropList *> C4PropList::PropLists;
#endif
C4Set<C4PropListNumbered *> C4PropListNumbered::PropLists;
int32_t C4PropListNumbered::EnumerationIndex = 0;
C4StringTable Strings;
C4AulScriptEngine ScriptEngine;
C4DefList Definitions;

/* These are just stubs used by dead code: */
C4Landscape Landscape;
C4PXSSystem PXS;
C4Config Config;
C4GameObjects Objects;
C4Reloc Reloc;
class C4Draw *pDraw = NULL;

bool EraseItemSafe(const char *szFilename) {return false;}
void Smoke(int32_t tx, int32_t ty, int32_t level, DWORD dwClr) {}
class C4SoundInstance *StartSoundEffectAt(const char *, int32_t, int32_t, bool, int32_t) { return NULL; }

C4Config::C4Config() {}
C4Config::~C4Config() {}
const char* C4Config::AtTempPath(const char *) { return NULL; }
const char* C4Config::AtRelativePath(char const* s) {return s;}
bool C4Reloc::Open(C4Group&, char const*) const {return false;}

bool C4Draw::TextOut(const char *, CStdFont &, float, C4Surface *, float, float, DWORD, BYTE, bool) { return false; }

CSurface8::CSurface8(int, int) {}

C4Facet::C4Facet() {}
void C4Facet::Set(C4Surface*, float, float, float, float) {}
int32_t C4Facet::GetSectionCount() { return 0; }
C4Facet C4Facet::TruncateSection(int32_t) { return *this; }

C4Surface::C4Surface() {}
C4Surface::~C4Surface() {}
bool C4Surface::Read(CStdStream &, const char *) { return false; }
bool C4Surface::Lock() { return false; }
bool C4Surface::Unlock() { return false; }
DWORD C4Surface::GetPixDw(int iX, int iY, bool fApplyModulation) { return 0; }

C4Pattern::C4Pattern() {}
void C4Pattern::Clear() {}
bool C4Pattern::Set(C4Surface *, int) { return false; }
DWORD C4Pattern::PatternClr(unsigned int, unsigned int) const { return 0; }
C4Pattern& C4Pattern::operator=(C4Pattern const&) { return *this; }

C4IDList::C4IDList() {}
C4IDList::~C4IDList() {}
void C4IDList::Default() {}
void C4IDList::Clear() {}
C4IDList& C4IDList::operator=(C4IDList const&) { return *this; }
bool C4IDList::operator==(const C4IDList&) const { return false; }
int32_t C4IDList::GetIDCount(C4ID, int32_t) const { return 0; }
bool C4IDList::SetIDCount(C4ID, int32_t, bool) { return false; }
void C4IDList::CompileFunc(StdCompiler *, bool) {}
C4IDListChunk::C4IDListChunk() {}
C4IDListChunk::~C4IDListChunk() {}

void C4Def::IncludeDefinition(C4Def*) {}

C4DefList::C4DefList() {}
C4DefList::~C4DefList() {}
C4Def* C4DefList::ID2Def(C4ID) {return NULL;}
void C4DefList::Draw(C4ID, C4Facet &, bool, int32_t) {}
C4Def * C4DefList::GetDef(int) {return 0;}
int C4DefList::GetDefCount() {return 0;}
void C4DefList::CallEveryDefinition() {}
void C4DefList::ResetIncludeDependencies() {}
bool C4DefList::DrawFontImage(const char* szImageTag, C4Facet& rTarget, C4DrawTransform* pTransform) { return false; }
float C4DefList::GetFontImageAspect(const char* szImageTag) { return -1.0f; }

C4Landscape::C4Landscape() {}
C4Landscape::~C4Landscape() {}
bool C4Landscape::FindMatSlide(int&, int&, int, int, int) { return false; }
int32_t C4Landscape::ExtractMaterial(int32_t, int32_t) { return 0; }
bool C4Landscape::InsertMaterial(int32_t, int32_t *, int32_t *, int32_t, int32_t, bool) { return false; }
bool C4Landscape::Incinerate(int32_t, int32_t) { return false; }
bool C4Landscape::ClearPix(int32_t, int32_t) { return false; }
void C4Landscape::CheckInstabilityRange(int32_t, int32_t) {}
void C4Landscape::HandleTexMapUpdate() {}

void C4Sky::Default() {}
C4Sky::~C4Sky() {}

void C4LSector::Clear() {}

C4ObjectList::C4ObjectList() {}
C4ObjectList::~C4ObjectList() {}
void C4ObjectList::Default() {}
void C4ObjectList::Clear() {}
void C4ObjectList::InsertLinkBefore(C4ObjectLink*, C4ObjectLink*) {}
void C4ObjectList::InsertLink(C4ObjectLink*, C4ObjectLink*) {}
void C4ObjectList::RemoveLink(C4ObjectLink*) {}
bool C4ObjectList::Add(C4Object*, C4ObjectList::SortType, C4ObjectList*) {return 0;}
bool C4ObjectList::Remove(C4Object*) {return 0;}
bool C4ObjectList::AssignInfo() {return 0;}
bool C4ObjectList::ValidateOwners() {return 0;}

void C4NotifyingObjectList::InsertLinkBefore(C4ObjectLink *pLink, C4ObjectLink *pBefore) {}
void C4NotifyingObjectList::InsertLink(C4ObjectLink*, C4ObjectLink*) {}
void C4NotifyingObjectList::RemoveLink(C4ObjectLink*) {}

C4GameObjects::C4GameObjects() {}
C4GameObjects::~C4GameObjects() {}
void C4GameObjects::Clear(bool) {}
void C4GameObjects::Default() {}
bool C4GameObjects::Remove(C4Object*) {return 0;}
bool C4GameObjects::AssignInfo() {return 0;}
bool C4GameObjects::ValidateOwners() {return 0;}
C4Object * C4GameObjects::ObjectPointer(int) {return 0;}
void C4GameObjects::UpdateScriptPointers() {}
C4Value C4GameObjects::GRBroadcast(char const*, C4AulParSet*, bool, bool) {return C4Value();}

C4PXSSystem::C4PXSSystem() {}
C4PXSSystem::~C4PXSSystem() {}
bool C4PXSSystem::Create(int, C4Real, C4Real, C4Real, C4Real) { return false; }

void AddDbgRec(C4RecordChunkType, const void *, int) {}

#if 0
/* Pulled in by C4Game... */
CStdFont::CStdFont() {}
C4PathFinder::C4PathFinder() {}
C4PathFinder::~C4PathFinder() {}
C4TransferZones::C4TransferZones() {}
C4TransferZones::~C4TransferZones() {}
C4PacketBase::C4PacketBase() {}
C4PacketList::C4PacketList() {}
C4PacketBase::~C4PacketBase() {}
C4PacketList::~C4PacketList() {}
C4Control::C4Control() {}
C4Control::~C4Control() {}
C4GameControl::C4GameControl(): Network(this) {}
C4GameControl::~C4GameControl() {}
C4GameControlNetwork::C4GameControlNetwork(C4GameControl*): pParent(NULL) {}
C4GameControlNetwork::~C4GameControlNetwork() {}
C4GraphicsResource::C4GraphicsResource(): CaptionFont(FontCaption), TitleFont(FontTitle), TextFont(FontRegular), MiniFont(FontTiny), TooltipFont(FontTooltip) {}
C4GraphicsResource::~C4GraphicsResource() {}
C4GameParameters::C4GameParameters() {}
C4GameParameters::~C4GameParameters() {}

//C4Extra::C4Extra() {}
//C4Extra::~C4Extra() {}
void C4Extra::Clear() {}

static C4KeyboardInput KeyboardInput;
void C4KeyboardInput::Clear() {}
bool C4KeyboardInput::IsValid = false;
C4KeyboardInput &C4KeyboardInput_Init() { return KeyboardInput; }

static C4GameParameters GameParameters;
static C4RoundResults GameRoundResults;
C4Game::C4Game(): Parameters(GameParameters), Clients(Parameters.Clients), Teams(Parameters.Teams), PlayerInfos(Parameters.PlayerInfos), RestorePlayerInfos(Parameters.RestorePlayerInfos), RoundResults(GameRoundResults), Input(Control.Input), KeyboardInput(C4KeyboardInput_Init()) {}
C4Game::~C4Game() {}
#endif

C4AulDebug *C4AulDebug::pDebug;
void C4AulDebug::DebugStep(C4AulBCC*, C4Value*) {}

C4MapScriptHost MapScript;
C4MapScriptHost::C4MapScriptHost() {}
C4MapScriptHost::~C4MapScriptHost() {}
void C4MapScriptHost::Clear() {}
C4PropListStatic *C4MapScriptHost::GetPropList() {return NULL;}
bool C4MapScriptHost::Load(C4Group &, const char *, const char *, C4LangStringTable *) { return false; }
void C4MapScriptHost::AddEngineFunctions() {}
