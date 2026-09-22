# mod-paladin-forever

An [AzerothCore](https://www.azerothcore.org/) module (WotLK 3.3.5a) that gives Paladins
two combat abilities early while leveling, without talents or trainer trips.

## What it does

On login and on level-up, a Paladin automatically learns:

- **Crusader Strike** from an early level (configurable, default level 10) — an instant
  weapon strike that deals Holy damage.
- **Consecration** as a baseline ability — the highest rank the character's level allows.

Spells are granted only when not already known. Uses stock WotLK spells only — no custom
spells, no DBC edits, no client patch.

## Configuration

`conf/mod_paladin_forever.conf.dist`:

| Key                                   | Default | Description                              |
|---------------------------------------|---------|------------------------------------------|
| `PaladinForever.Enable`               | `1`     | Master on/off switch                     |
| `PaladinForever.CrusaderStrikeLevel`  | `10`    | Level at which Crusader Strike is granted |

## Installation

Clone into your AzerothCore `modules/` directory and rebuild the worldserver.

## License

Released under the GNU GPL v2 (or later).
