#include "StdAfx.h"
#include "Setup.h"

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr *Mgr)
{
	RegisterRaventScript(Mgr);
	RegisterVillager(Mgr);
	RegisterItemScript(Mgr);
	RegisterRaventHostScript(Mgr);
}