/*
 * mod-paladin-forever
 *
 * A module for the mod-playerbots AzerothCore fork (WotLK 3.3.5a). Gives paladins
 * baseline combat abilities early in their career:
 *   1. Crusader Strike at level 10 (spell 35395) - instant weapon strike with holy damage
 *   2. Consecration ranks based on player level (grant highest rank whose min level <= player level)
 *
 * No server-side changes (no custom spells, no DBC patches), client-safe.
 *
 * Released under GNU GPL v2; redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"

#include <vector>
#include <utility>

namespace PaladinForever
{
    struct Config
    {
        bool Enable;
        uint32 CrusaderStrikeLevel;
    };

    Config& GetConfig()
    {
        static Config cfg;
        return cfg;
    }

    // Consecration ranks in ascending order by minimum level requirement.
    // Format: {minLevel, spellId}
    // These spell IDs should be verified against wow_world.spell_dbc:
    //   SELECT ID, Name_Lang_enUS FROM spell_dbc WHERE ID IN
    //   (26573, 20116, 20922, 20923, 20924, 27779, 48818, 48819);
    static const std::vector<std::pair<uint8, uint32>> ConsecrationRanks = {
        {20, 26573},  // Level 20+
        {30, 20116},  // Level 30+
        {40, 20922},  // Level 40+
        {50, 20923},  // Level 50+
        {60, 20924},  // Level 60+
        {70, 27779},  // Level 70+
        {75, 48818},  // Level 75+
        {80, 48819},  // Level 80
    };

    // Crusader Strike spell ID (instantly available baseline ability in vanilla WotLK).
    static constexpr uint32 CrusaderStrikeSpellId = 35395;

    // Grant Crusader Strike if the paladin meets the level requirement and does not
    // already know it (avoid duplicate learn messages).
    void GrantCrusaderStrike(Player* player, uint32 minLevel)
    {
        if (player->GetLevel() < minLevel)
            return;

        if (player->HasSpell(CrusaderStrikeSpellId))
            return;

        player->learnSpell(CrusaderStrikeSpellId, false);
    }

    // Grant the highest-rank Consecration the paladin qualifies for.
    void GrantConsecration(Player* player)
    {
        uint8 playerLevel = player->GetLevel();
        uint32 spellToLearn = 0;

        // Find the highest rank we can teach (iterate in reverse to find the max).
        for (auto it = ConsecrationRanks.rbegin(); it != ConsecrationRanks.rend(); ++it)
        {
            if (playerLevel >= it->first)
            {
                spellToLearn = it->second;
                break;
            }
        }

        // No rank is available for this level yet.
        if (spellToLearn == 0)
            return;

        // Do not teach if the player already knows this spell (e.g., from trainer).
        if (player->HasSpell(spellToLearn))
            return;

        player->learnSpell(spellToLearn, false);
    }

    // Grant both Crusader Strike and Consecration on login or level-up.
    void GrantPaladinAbilities(Player* player)
    {
        if (player->getClass() != CLASS_PALADIN)
            return;

        if (!GetConfig().Enable)
            return;

        GrantCrusaderStrike(player, GetConfig().CrusaderStrikeLevel);
        GrantConsecration(player);
    }
}

using PaladinForever::GetConfig;

// =====================================================================
//  PlayerScript: grant abilities on login and level-up.
// =====================================================================
class PaladinForeverPlayerScript : public PlayerScript
{
public:
    PaladinForeverPlayerScript() : PlayerScript("PaladinForever_PlayerScript") { }

    void OnPlayerLogin(Player* player) override
    {
        if (!GetConfig().Enable)
            return;

        if (player->getClass() != CLASS_PALADIN)
            return;

        PaladinForever::GrantPaladinAbilities(player);
    }

    void OnPlayerLevelChanged(Player* player, uint8 /*oldlevel*/) override
    {
        if (!GetConfig().Enable)
            return;

        if (player->getClass() != CLASS_PALADIN)
            return;

        PaladinForever::GrantPaladinAbilities(player);
    }
};

// =====================================================================
//  WorldScript: config load.
// =====================================================================
class PaladinForeverWorldScript : public WorldScript
{
public:
    PaladinForeverWorldScript() : WorldScript("PaladinForever_WorldScript") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        PaladinForever::Config& cfg = GetConfig();
        cfg.Enable               = sConfigMgr->GetOption<bool>("PaladinForever.Enable", true);
        cfg.CrusaderStrikeLevel  = sConfigMgr->GetOption<uint32>("PaladinForever.CrusaderStrikeLevel", 10);
    }
};

// =====================================================================
//  Registration
// =====================================================================
void AddPaladinForeverScripts()
{
    new PaladinForeverPlayerScript();
    new PaladinForeverWorldScript();
}
