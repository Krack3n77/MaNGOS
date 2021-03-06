/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* ScriptData
SDName: Spell_Scripts
SD%Complete: 100
SDComment: Spell scripts for dummy effects (if script need access/interaction with parts of other AI or instance, add it in related source files)
SDCategory: Spell
EndScriptData */

/* ContentData
spell 19512
spell 8913
EndContentData */

#include "precompiled.h"

/* When you make a spell effect:
- always check spell id and effect index
- always return true when the spell is handled by script
*/

enum
{
	// quest 6124/6129
	SPELL_APPLY_SALVE                   = 19512,

	NPC_SICKLY_DEER                     = 12298,
	NPC_SICKLY_GAZELLE                  = 12296,

	NPC_CURED_DEER                      = 12299,
	NPC_CURED_GAZELLE                   = 12297,

	// target morbent fel
	SPELL_SACRED_CLEANSING              = 8913,
	NPC_MORBENT                         = 1200,
	NPC_WEAKENED_MORBENT                = 24782,
};

bool EffectDummyCreature_spell_dummy_npc(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
	switch(uiSpellId)
	{
	case SPELL_SACRED_CLEANSING:
		{
			if (uiEffIndex == EFFECT_INDEX_1)
			{
				if (pCreatureTarget->GetEntry() != NPC_MORBENT)
					return true;

				pCreatureTarget->UpdateEntry(NPC_WEAKENED_MORBENT);
				return true;
			}
			return true;
		}
	case SPELL_APPLY_SALVE:
		{
			if (uiEffIndex == EFFECT_INDEX_0)
			{
				if (pCaster->GetTypeId() != TYPEID_PLAYER)
					return true;

				if (pCreatureTarget->GetEntry() == NPC_SICKLY_DEER && ((Player*)pCaster)->GetTeam() == ALLIANCE)
					pCreatureTarget->UpdateEntry(NPC_CURED_DEER);

				if (pCreatureTarget->GetEntry() == NPC_SICKLY_GAZELLE && ((Player*)pCaster)->GetTeam() == HORDE)
					pCreatureTarget->UpdateEntry(NPC_CURED_GAZELLE);

				return true;
			}
			return true;
		}
	}

	return false;
}

struct MANGOS_DLL_DECL npc_eye_of_kilroggAI : public ScriptedAI
{
	npc_eye_of_kilroggAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{ 
		SetCombatMovement(false);
		Reset(); 
	}

	void Reset()
	{
	}

	void UpdateAI(const uint32 /*uiDiff*/)
	{
	}

	void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/)
	{
		Unit* owner = m_creature->GetOwner();
		if (owner)
		{
			owner->RemoveAura(126, SpellEffectIndex::EFFECT_INDEX_1);
		}
	}
};

CreatureAI* GetAI_npc_eye_of_kilrogg(Creature* pCreature)
{
	return new npc_eye_of_kilroggAI(pCreature);
}

struct MANGOS_DLL_DECL npc_dream_visionAI : public ScriptedAI
{
    npc_dream_visionAI(Creature* pCreature) : ScriptedAI(pCreature) 
    { 
        SetCombatMovement(false);
        Reset(); 
    }

    void Reset()
    {
    }

    void UpdateAI(const uint32 /*uiDiff*/)
    {
    }

    void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/)
    {
        Unit* owner = m_creature->GetOwner();
        if (owner)
        {
            owner->RemoveAura(11403, SpellEffectIndex::EFFECT_INDEX_1);
        }
    }
};

CreatureAI* GetAI_npc_dream_vision(Creature* pCreature)
{
    return new npc_dream_visionAI(pCreature);
}

void AddSC_spell_scripts()
{
	Script* pNewScript;

	pNewScript = new Script;
	pNewScript->Name = "spell_dummy_npc";
	pNewScript->pEffectDummyNPC = &EffectDummyCreature_spell_dummy_npc;
	pNewScript->RegisterSelf();

	pNewScript = new Script;
	pNewScript->Name = "npc_eye_of_kilrogg";
	pNewScript->GetAI = &GetAI_npc_eye_of_kilrogg;
	pNewScript->RegisterSelf();
    
    pNewScript = new Script;
    pNewScript->Name = "npc_dream_vision";
    pNewScript->GetAI = &GetAI_npc_dream_vision;
    pNewScript->RegisterSelf();
}
