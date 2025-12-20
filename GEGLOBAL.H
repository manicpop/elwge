/*****************************************************************************
 *                                                                           *
 *   GEGLOBAL.H                                                              *
 *                                                                           *
 *   Galactic Empire Global Definitions and Prototypes                       *
 *                                                                           *
 *   All development through v3.2e         M. Murdock     03/17/1992         *
 *   Worldgroup 3.2 Conversion v3.3        R. Hadsall     04/03/2021         *
 *   Major BBS v10  Conversion v3.4        R. Hadsall     12/05/2025         *
 *                                                                           *
 * Copyright (C) 2006-2025 Rick Hadsall.  All Rights Reserved.               *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU Affero General Public License as published  *
 * by the Free Software Foundation, either version 3 of the License, or      *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU Affero General Public License for more details.                       *
 *                                                                           *
 * You should have received a copy of the GNU Affero General Public License  *
 * along with this program. If not, see <https://www.gnu.org/licenses/>.     *
 *                                                                           *
 * Additional Terms for Contributors:                                        *
 * 1. By contributing to this project, you agree to assign all right, title, *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies.                              *
 * 2. You grant Rick Hadsall and Elwynor Technologies a non-exclusive,       *
 *    royalty-free, worldwide license to use, reproduce, prepare derivative  *
 *    works of, publicly display, publicly perform, sublicense, and          *
 *    distribute your contributions                                          *
 * 3. You represent that you have the legal right to make your contributions *
 *    and that the contributions do not infringe any third-party rights.     *
 * 4. Rick Hadsall and Elwynor Technologies are not obligated to incorporate *
 *    any contributions into the project.                                    *
 * 5. This project is licensed under the AGPL v3, and any derivative works   *
 *    must also be licensed under the AGPL v3.                               *
 * 6. If you create an entirely new project (a fork) based on this work, it  *
 *    must also be licensed under the AGPL v3, you assign all right, title,  *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies, and you must include these   *
 *    additional terms in your project's LICENSE file(s).                    *
 *                                                                           *
 * By contributing to this project, you agree to these terms.                *
 *                                                                           *
 *****************************************************************************/

/* GEMAIN Global Defs ******************************************************/

#ifndef GEMAIN

extern INT gestt;

extern DFAFILE *gebb1, /* GEship.dat   */
               *gebb2, /* GEplanet.dat */
               *gebb4, /* GEmail.dat   */
               *gebb5; /* GEuser.dat   */

extern HMCVFILE gemb, gehlpmb, geshmb;

extern SHORT numwar; /* number of users in game  */

extern WARSHP  tmpshp; /* used to temporarly set up a new ship */
extern WARSHP *warshp, *warsptr;
extern WARUSR *warusr, *waruptr;
extern SHORT   warusr_ecl, warshp_ecl;
extern WARUSR  tmpusr;

extern GALSECT sector, *sptr1;

extern GALPLNT planet;

extern GALWORM worm;

extern PLANETAB *ptab;

extern CHAR  map[MAXY][MAXX + 1]; /* global scan map array */
extern CHAR mapc[MAXY][MAXX + 1]; /* global scan map array color map */

extern MINE *mines; /* place to stuff mines */

extern MAIL     mail;
extern MAILSTAT tmpstat;

extern BEACONTAB *beacon;

extern GEMESSAGE *gemsg;

extern SHORT    nships;
extern SHORT    heading;
extern USHORT   speed;
extern SHORT    lockwarn;
extern SHORT    xsect, ysect;
extern USHORT   xcord, ycord;
extern PKEY     pkey;
extern USHORT   distance;
extern DOUBLE   ddistance;
extern SHORT    bearing;
extern USHORT   energy;
extern USHORT   damage;
extern CHAR    *gechrbuf, *gechrbuf2, *gechrbuf3;
extern CHAR    *warpbuf;
extern SHORT    sectnum, plnum;
extern GALPLNT *plptr;
extern ITEM    *titems;
extern TEAM    *teamtab;

extern SHORT su;
extern SHORT tmp_usrnum;

extern USHORT plantime;

extern LONG max_plrec, teambonus, pltvcash, pltvdiv, startcash;

extern SHORT gemaxplrs, gefreebies, gemaxlist, maxships, se100dam, profon,
             showopt, syscmds, sysonly, max_plnts, trans_opt, numships, univmax,
             plodds, wormodds, hpfirdst, hpdammax, pfirdist, pdammax, jamtime,
             maildays, torpsped, mislsped, decodds, nummines, usermines, maxdroids,
             cyb_class, clenguse, logflag, optmenu, cyb_gold, tot_classes, team_max,
             usegemsg, fse_state, phatowrp, misengfc, score_bonus, score_f1,
             score_f2, chgloser, univwrap, maxplanets;

extern CHAR *opttxt, optchr;

extern LONG *opttbl;

extern DOUBLE tor_fact, tdammax, mis_fact, mdammax, idammax, minedammax,
              repairrate, tooclose, hyperdist1, hyperdist2, plattrf1, plattrf2,
              plattrf3, plattrt1, plattrt2, plattrt3;

extern INT msgsiz;

extern LONG plantock;

#ifdef TESTDRIVE
extern SHORT endtd, nowtd, lefttd;
#endif

extern SHPKEY  shpkey;
extern MAILKEY mailkey;

extern SHIP *shipclass;

extern S00  *s00;
extern SHORT s00plnum;

extern SCANTAB *scantab;

extern LONG   shieldprice[TOPSHIELD];
extern LONG   phaserprice[TOPPHASER];
extern USHORT baseprice[NUMITEMS];
extern DOUBLE maxpl[NUMITEMS];
extern LONG   weight[NUMITEMS];
extern LONG   value[NUMITEMS];
extern LONG   manhours[NUMITEMS];

#endif

/* GECMDS Global Defs ******************************************************/

#ifndef GECMDS

extern CHAR *kwrd[NUMITEMS];
extern CHAR *item_name[NUMITEMS];

#endif

/* GEFUNCS Global Defs *****************************************************/

#ifndef GEFUNCS
#endif

/* GEPLANET Global Defs ****************************************************/

#ifndef GEPLANET
#endif

/* GECYBS Global Defs ******************************************************/

#ifndef GECYBS
extern CHAR  cybname[UIDSIZ];
extern SHORT cybhaltflg;
#endif

/* Function Prototypes *****************************************************/

#include "geproto.h"
