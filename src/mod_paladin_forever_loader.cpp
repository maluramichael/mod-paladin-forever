/*
 * mod-paladin-forever loader.
 *
 * The playerbots fork auto-globs every module's sources into one lib and looks
 * up a loader symbol derived from the folder name: for folder "mod-paladin-forever"
 * that symbol is exactly "Addmod_paladin_foreverScripts". It must exist and call our
 * real registration function.
 *
 * Released under GNU GPL v2 or (at your option) any later version.
 */

void AddPaladinForeverScripts();

void Addmod_paladin_foreverScripts()
{
    AddPaladinForeverScripts();
}
