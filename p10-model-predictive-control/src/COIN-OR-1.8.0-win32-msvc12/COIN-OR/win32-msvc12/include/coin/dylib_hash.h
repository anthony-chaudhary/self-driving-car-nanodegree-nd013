#ifndef _DYLIB_HASH_H
#define _DYLIB_HASH_H
/*
  This file is part of the support library for the Dylp LP distribution.

        Copyright (C) 2005 -- 2007 Lou Hafer

        School of Computing Science
        Simon Fraser University
        Burnaby, B.C., V5A 1S6, Canada
        lou@cs.sfu.ca

  This code is licensed under the terms of the Eclipse Public License (EPL).
*/

/*                Hash Table Structures                 */

/*
  In order that the hashing routines can be used for a number of different
  tables, they do not have any knowledge of the thing being hashed. All that
  is maintained is the association between a key and some generic object.

  @(#)hash.h	1.3 06/22/04
  svn/cvs: $Id: dylib_hash.h 407 2010-12-31 20:48:48Z lou $
*/

/* 
  The basic hash table entry structure
  
  field		description
  -----		-----------
  next		next entry at this bucket
  key		hash key (character string)
  ent		structure associated with this entry
*/

typedef struct hel_tag { struct hel_tag *next ;
			 const char *key ;
			 void *ent ; } hel ;

/* Hash table interface routines */

extern void *dyhash_lookup(const char *key, hel *hashtab[], int size),
            *dyhash_search(const char *key, hel *hashtab[],
	    		   int size, bool init),
	    *dyhash_enter(const char *key, hel *hashtab[],
	    		  int size, void *entry),
	    *dyhash_erase(const char *key, hel *hashtab[], int size) ;

#endif /* _DYLIB_HASH_H */
