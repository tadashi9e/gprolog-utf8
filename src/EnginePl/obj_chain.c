/*-------------------------------------------------------------------------*
 * GNU Prolog                                                              *
 *                                                                         *
 * Part  : Prolog engine                                                   *
 * File  : obj_chain.c                                                     *
 * Descr.: object chaining management                                      *
 * Author: Daniel Diaz                                                     *
 *                                                                         *
 * Copyright (C) 1999,2000 Daniel Diaz                                     *
 *                                                                         *
 * GNU Prolog is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation; either version 2, or any later version.       *
 *                                                                         *
 * GNU Prolog is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU        *
 * General Public License for more details.                                *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc.  *
 * 59 Temple Place - Suite 330, Boston, MA 02111, USA.                     *
 *-------------------------------------------------------------------------*/

/* $Id$ */

#include <stdio.h>

#include "obj_chain.h"
#include "pl_params.h"
#include "gp_config.h"

#define DBGPRINTF printf

#if 0
#define DEBUG
#endif


void Fatal_Error(char *format, ...);




/*---------------------------------*
 * Constants                       *
 *---------------------------------*/

/*---------------------------------*
 * Type Definitions                *
 *---------------------------------*/

typedef struct
{
  void (*fct_exec_system) ();
  void (*fct_exec_user) ();
}
ObjInf;




/*---------------------------------*
 * Global Variables                *
 *---------------------------------*/

#ifndef _MSC_VER

extern ObjChain obj_chain_begin;
extern ObjChain *obj_chain_end;

#else

extern long obj_chain_begin;
extern long obj_chain_end;

#endif

static ObjInf obj_tbl[MAX_OBJECT];
static int nb_obj;




/*---------------------------------*
 * Function Prototypes             *
 *---------------------------------*/




/*-------------------------------------------------------------------------*
 * Under WIN32 with MSVC++ 6.0                                             *
 *                                                                         *
 * We use the possibility to define user sections in objects. We group the *
 * address of each initialization function in a same section (named .INIT).*
 * We use 2 markers: obj_chain_begin and obj_chain_end whose address (&)   *
 * delimit the start and the end of the initializer function address table.*
 * To know the start and end address in this section we cannot assume the  *
 * (MS) linker fits the section in the order it finds the objects. However,*
 * if a section name contains a $ in it, the $ and all everything that     *
 * follows will be stripped off in the executable file. Before the linker  *
 * strips down the name, it combines the sections with names that match up *
 * to the $. The name portion after the $ is used in arranging the OBJ     *
 * sections in the executable. These sections are sorted alphabetically,   *
 * based on the portion of the name after the $. For example, 4 sections   *
 * called .INIT$m, .INIT$a, .INIT$z and .INIT$b will be combined into a    *
 * single section called .INIT in the executable. The data in this section *
 * will start with .INIT$a�s data, continue with .INIT$b�s, with .INIT$m   *
 * and end with .INIT$z�s. Inside a same (sub)section the order is unknown.*
 * We use $a for obj_chain_begin, $z for obj_chain_end, $b for initializers*
 * defined in C code and $m for initializers issued from Prolog code.      *
 * This ensures C initializers are executed before Prolog initializers.    *
 * The order of Prolog initializations is unknown, butall systems inits are*
 * before user inits (generated by :- initialization).                     *
 * WARNING: when linking do not use any superflous flag (e.g. debugging),  *
 * I have spent a lot of time to find that /ZI causes troubles (the .INIT  *
 * section contains much more information and then it is not correct to use*
 * its whole content between &obj_chain_begin and &obj_begin_stop.         *
 *-------------------------------------------------------------------------*/

#if 0
/*-------------------------------------------------------------------------*
 * The following code is used by gcc itself. It should be interesting to   *
 * see it it is possible to reuse it (on any/most architectures) to do a   *
 * similar job to what we do under Win32 (cf. above).                      *
 *-------------------------------------------------------------------------*/

/*
 * The following macros are used to declare global sets of objects, which
 * are collected by the linker into a `struct linker_set' as defined below.
 *
 * NB: the constants defined below must match those defined in
 * ld/ld.h.  Since their calculation requires arithmetic, we
 * can't name them symbolically (e.g., 23 is N_SETT | N_EXT).
 */
#define MAKE_SET(set, sym, type) \
        asm(".stabs \"_" #set "\", " #type ", 0, 0, _" #sym)
#define TEXT_SET(set, sym) MAKE_SET(set, sym, 23)
#define DATA_SET(set, sym) MAKE_SET(set, sym, 25)
#define BSS_SET(set, sym)  MAKE_SET(set, sym, 27)
#define ABS_SET(set, sym)  MAKE_SET(set, sym, 21)

#endif




/*-------------------------------------------------------------------------*
 * FIND_LINKED_OBJECTS                                                     *
 *                                                                         *
 *-------------------------------------------------------------------------*/
void
Find_Linked_Objects(void)
{
#ifndef _MSC_VER
  ObjChain *p, **q;
#else
  long *p;
  void (*q) ();
#endif
  int i;

  nb_obj = 0;

#ifdef DEBUG
  DBGPRINTF("ObjChain: chain begin: %#x\n", (unsigned) &obj_chain_begin);
  DBGPRINTF("ObjChain: chain end  : %#x\n", (unsigned) &obj_chain_end);
#endif

#ifndef _MSC_VER
  if (obj_chain_begin.next != &obj_chain_end
      || obj_chain_end != &obj_chain_begin)
    Fatal_Error("obj_chain: executable file corrupted");
#endif


#ifndef _MSC_VER
  p = &obj_chain_begin + 1;
  while (p < (ObjChain *) &obj_chain_end)
    {
      if (p->magic1 != OBJ_CHAIN_MAGIC_1 || p->magic2 != OBJ_CHAIN_MAGIC_2
	  || (q = p->next) < (ObjChain **) &obj_chain_begin
	  || q > &obj_chain_end || *q != p)
	{
#ifdef DEBUG
#if 0
	  DBGPRINTF("failing at %#x\n", p);
#else
	  DBGPRINTF(".");
#endif
#endif


#if MSDOS
	  p = (ObjChain *) ((char *) p + 1);
#else
	  p = (ObjChain *) ((long *) p + 1);
#endif

	  continue;
	}

#ifdef DEBUG
      DBGPRINTF("\n*** Obj Found  start: %#x  end: %#x  Initializer: %#x\n",
		(unsigned) p, (unsigned) q, (unsigned) (p->fct_init));
#endif
      (*p->fct_init) ();

#if defined(M_powerpc_linux) || defined(M_alpha_linux) || \
    defined(M_mips_irix)
      p = (ObjChain *) ((long *) p + 1);
#elif 0
      p = (ObjChain *) ((char *) p + 1);
#else
      p = (ObjChain *) (q + 1);
#endif
    }

#else /* _MSC_VER */
  p = &obj_chain_begin;
  while (++p < &obj_chain_end)
    {
      q = (void (*)()) *p;
#ifdef DEBUG
      DBGPRINTF("\n*** Obj Found  addr: %#x  Initializer: %#x\n",
		(unsigned) p, (unsigned) q);
#endif
      (*q) ();
    }
#endif /* !MSC_VER */



#if 1
  i = nb_obj;			/* call Exec System functions from last to first */
  while (--i >= 0)
    {
#ifdef DEBUG
      DBGPRINTF("\n+++ Executing Exec System Function at: %#lx\n",
		(long) (obj_tbl[i].fct_exec_system));
#endif
      if (obj_tbl[i].fct_exec_system != NULL)
	(*(obj_tbl[i].fct_exec_system)) ();
    }

  i = nb_obj;			/* call Exec User functions from last to first */
  while (--i >= 0)
    {
#ifdef DEBUG
      DBGPRINTF("\n+++ Executing Exec User Function at: %#lx\n",
		(long) (obj_tbl[i].fct_exec_user));
#endif
      if (obj_tbl[i].fct_exec_user != NULL)
	(*(obj_tbl[i].fct_exec_user)) ();
    }
#endif
}




/*-------------------------------------------------------------------------*
 * NEW_OBJECT                                                              *
 *                                                                         *
 * Called by compiled prolog code.                                         *
 *-------------------------------------------------------------------------*/
void
New_Object(void (*fct_exec_system) (), void (*fct_exec_user) ())
{
  obj_tbl[nb_obj].fct_exec_system = fct_exec_system;
  obj_tbl[nb_obj].fct_exec_user = fct_exec_user;
  nb_obj++;
}
