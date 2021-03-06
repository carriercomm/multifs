/*
 * MultiFS Distributing Multicast Filesystem
 * Copyright (c) 2011 Wouter Coene <wouter@irdc.nl>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MULTIFS_H
#define MULTIFS_H

#include "compat.h"
#include "error.h"
#include "hash.h"
#include "pack.h"

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/param.h>
#include <sys/types.h>

#define nitems(arr)		(sizeof(arr) / sizeof((arr)[0]))
#define min(a, b)		((a) < (b)? (a) : (b))
#define max(a, b)		((a) < (b)? (b) : (a))

/* protocol version */
#define NET_VERSION	1

/* port number */
#define NET_PORT	14655	/* from random.org, so guaranteed to be random */

/* filesystem state */
struct multifs {
	bool		 foreground;
	bool		 debug;
	const char	*fsname;
	size_t		 fsnamelen;
	const char	*fsroot;
	size_t		 fsrootlen;
	pid_t		 netpid;
	int		 netfd;
	int		 maxmsglen;
};

/* network messages */
enum msg {
	/*
	 * Basics
	 */
	MSG_RESEND		= 0x00,	/* request that a packet be resent */

	/*
	 * Token negotiation
	 */	
	MSG_TOKEN_WHERE		= 0x20,	/* look for the token */
	MSG_TOKEN_HERE,			/* report where the token is */
	MSG_TOKEN_ASK,			/* ask for the token */
	MSG_TOKEN_GIVE		= 0x2f,	/* give the token away */

	/*
	 * Object operations
	 */
	MSG_OBJ_SYMLINK		= 0x30,
	MSG_OBJ_RENAME,
	MSG_OBJ_SETATTR,

	/*
	 * Directory operations
	 */
	MSG_DIR_CREATE		= 0x40,
	MSG_DIR_REMOVE,

	/*
	 * File operations
	 */
	MSG_FILE_CREATE		= 0x50,
	MSG_FILE_TRUNCATE,
	MSG_FILE_LINK,
	MSG_FILE_UNLINK,
	MSG_FILE_SETLOCK,
	MSG_FILE_WRITE,

	/*
	 * When to send sequence
	 */
	MSG_WITH_SEQUENCE	= MSG_TOKEN_GIVE
};

/* fuse.c */
int		 multifs_main(int, char *[], struct multifs *);
int		 multifs_process(struct multifs *, enum msg, const char *, size_t);

/* net.c */
void		 net_init(struct multifs *);
int		 net_send(int, enum msg, const char *, ...);

#endif /* MULTIFS_H */
