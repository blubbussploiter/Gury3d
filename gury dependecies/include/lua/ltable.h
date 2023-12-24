/*
** $Id: ltable.h,v 2.10.1.1 2007/12/27 13:02:25 roberto Exp $
** Lua tables (hash)
** See Copyright Notice in lua.h
*/

#ifndef ltable_h
#define ltable_h

#include "lobject.h"


#define gnode(t,i)	(&(t)->node[i])
#define gkey(n)		(&(n)->i_key.nk)
#define gval(n)		(&(n)->i_val)
#define gnext(n)	((n)->i_key.nk.next)

#define key2tval(n)	(&(n)->i_key.tvk)


LUAI_FUNC const TValue *luaH_getnum (LTable *t, int key);
LUAI_FUNC TValue *luaH_setnum (lua_State *L, LTable *t, int key);
LUAI_FUNC const TValue *luaH_getstr (LTable *t, TString *key);
LUAI_FUNC TValue *luaH_setstr (lua_State *L, LTable *t, TString *key);
LUAI_FUNC const TValue *luaH_get (LTable *t, const TValue *key);
LUAI_FUNC TValue *luaH_set (lua_State *L, LTable *t, const TValue *key);
LUAI_FUNC LTable *luaH_new (lua_State *L, int narray, int lnhash);
LUAI_FUNC void luaH_resizearray (lua_State *L, LTable *t, int nasize);
LUAI_FUNC void luaH_free (lua_State *L, LTable *t);
LUAI_FUNC int luaH_next (lua_State *L, LTable *t, StkId key);
LUAI_FUNC int luaH_getn (LTable *t);


#if defined(LUA_DEBUG)
LUAI_FUNC Node *luaH_mainposition (const LTable *t, const TValue *key);
LUAI_FUNC int luaH_isdummy (Node *n);
#endif


#endif
