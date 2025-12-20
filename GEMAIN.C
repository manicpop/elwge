/*****************************************************************************
 *                                                                           *
 *   GEMAIN.C                                                                *
 *                                                                           *
 *   Main source file: module, initialization, data, and utility routines    *
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

/*
 * if FASTPLANET is defined, updates happen every 5 seconds.
 * This overrides the MSG file setting PLANTOCK, which provides
 * control via calculation with number of planets.
 * /
// #define FASTPLANET 

/*
 * This forces a regular build of sectors and planets.
 * Best kept for debugging or building a fast map
 */
//#define BLDPLNTS 1 

#include "gemain.h"

#define GEMAIN 1

// LOCAL GLOBAL DEFS ****************************************************

#define VERSION "3.4" // WG32/V10 conversion from 3.2e with some bug fixes -RH
#define GEHELP  "ELWGEHLP.MCV"
#define GEMSG   "ELWGE.MCV"

INT           gestt; // module state
struct module elwge = {
                    // module interface block
        "",         /*    description for main menu         */
        gelogon,    /*    user logon supplemental routine   */
        galemp,     /*    input routine if selected         */
        stshdlr,    /*    status-input routine if selected  */
        NULL,       /*    "injoth" routine for this module  */
        warlof,     /*    user logoff supplemental routine  */
        warhup,     /*    hangup (lost carrier) routine     */
        gemidnight, /*    midnight cleanup routine          */
        gedelete,   /*    delete-account routine            */
        clswar      /*    finish-up (sys shutdown) routine  */
};

DFAFILE *gebb1,   /* elwgeshp.dat */
        *gebb2,   /* elwgeplt.dat */
        *gebb4,   /* elwgemal.dat */
        *gebb5;   /* elwgeusr.dat */

HMCVFILE gemb,    /* elwge.msg    */
         gehlpmb, /* elwgehlp.msg */
         geshmb;  /* elwgeshp.msg */

static CHAR *geuser, *geship, *geplnt, *gemail, *geshipcl;
static CHAR *endmark;

SHORT      numwar = 0; // number of users in game
WARSHP     tmpshp;     // used to temporarly set up a new ship
WARSHP    *warshp, *warsptr;
WARUSR    *warusr, *waruptr;
SHORT      warusr_ecl, warshp_ecl;
WARUSR     tmpusr;
GALSECT    sector, *sptr1;
GALPLNT    planet;
GALWORM    worm;
PLANETAB  *ptab;
CHAR       map[MAXY][MAXX + 1];  // global scan map array
CHAR       mapc[MAXY][MAXX + 1]; // global scan map array color map
MINE      *mines;                // place to stuff mines
MAIL       mail;
MAILSTAT   tmpstat;
BEACONTAB *beacon;
GEMESSAGE *gemsg; // struct message *gemsg;

// *********************************************************************
// Global variables
// *********************************************************************

SHORT  nships;
SHORT  heading;
USHORT speed;
SHORT  lockwarn;
INT    msgsiz;

SHORT  xsect, ysect;
USHORT xcord, ycord;

PKEY     pkey;
USHORT   distance;
DOUBLE   ddistance;
SHORT    bearing;
USHORT   energy;
USHORT   damage;
CHAR    *gechrbuf, *gechrbuf2, *gechrbuf3;
CHAR    *warpbuf;
SHORT    plnum;
GALPLNT *plptr;
ITEM    *titems;
TEAM    *teamtab;

SHORT su;
SHORT tmp_usrnum;

LONG max_plrec, teambonus, pltvcash, pltvdiv, startcash;

USHORT plantime = PLANTIME;

SHORT gemaxplrs, gemaxlist, maxships, se100dam, profon, showopt, syscmds,
        sysonly, max_plnts, trans_opt, numships, univmax, plodds, wormodds,
        hpfirdst, hpdammax, pfirdist, pdammax, jamtime, maildays, torpsped,
        mislsped, decodds, nummines, usermines, maxdroids, cyb_class, clenguse,
        logflag, optmenu, cyb_gold, tot_classes, team_max, usegemsg, fse_state,
        phatowrp, misengfc, score_bonus, score_f1, score_f2, chgloser, univwrap,
        maxplanets;

CHAR *opttxt, optchr;

LONG *opttbl;

DOUBLE tor_fact, tdammax, mdammax, mis_fact, idammax, minedammax, repairrate,
        tooclose, hyperdist1, hyperdist2, plattrf1, plattrf2, plattrf3,
        plattrt1, plattrt2, plattrt3;

LONG plantock;

SHPKEY  shpkey;
MAILKEY mailkey;

SHIP *shipclass;

S00  *s00;
SHORT s00plnum;

SCANTAB *scantab;

LONG   shieldprice[TOPSHIELD];
LONG   phaserprice[TOPPHASER];
USHORT baseprice[NUMITEMS];

DOUBLE maxpl[NUMITEMS];    // Max items a planet can hold
LONG   weight[NUMITEMS];   // Net weight of each item
LONG   value[NUMITEMS];    // Net score value of each item on the planet
LONG   manhours[NUMITEMS]; // Number of items produced by 1000 men per day

typedef SHORT (*MenuFunc)(VOID); // Avoids Borland compiler warnings
#define MENU struct _menu
MENU
{
    SHORT    substt;
    MenuFunc func;
};

#define MENUNUM (sizeof(menu) / sizeof(MENU))

MENU menu[] = {{0,mnu_main},
               {1, mnu_main_ans},
               {FIGHTSUB, mnu_fightsub},
               {ADMENU1, mnu_admenu1},
               {ADMENU1A, mnu_admenu1a},
               {ADMENU2, mnu_admenu2},
               {ADMENU2B, mnu_admenu2b},
               {ADMENU2E, mnu_admenu2e},
               {ADMEN2F1, mnu_admenu2f1},
               {ADMEN2F2, mnu_admenu2f2},
               {ADMEN2F3, mnu_admenu2f3},
               {ADMEN2F4, mnu_admenu2f4},
               {ADMENU2H, mnu_admenu2h},
               {ADMENU2I, mnu_admenu2i},
               {ADMENU2J, mnu_admenu2j},
               {CHOOSESH, mnu_choosesh},
               {MENUG, mnu_menug},
               {MENUG1, mnu_menug1},
               {MENUG2, mnu_menug2}};

SHORT playgame; // RH added to avoid catastro when something goes funky 
                // with the game

/**
 ** Module API Routines
 **/

// System initialization routine (module hook)
VOID EXPORT init__elwge(VOID)
{
    stzcpy(elwge.descrp, gmdnam("ELWGE.MDF"), MNMSIZ);
    gestt = register_module(&elwge);
    iniwara(); // full initialization routine

    return;
}

VOID FUNC dummy(VOID)
{
}

// Actual initialization routine
VOID FUNC iniwara(VOID)
{
    SHORT type, classbase;
    INT   i, n;
    SHORT j;
    LONG  numrecs;
    SHORT class_tab[50] = {0};

    playgame = 1;

    gemb = opnmsg(GEMSG);

    shocst(spr("ELW Galactic Empire v%s", VERSION), "(C) Copyright 2025 Elwynor Technologies - www.elwynor.com");

    endmark = stgopt(ENDMARK);
    if (!sameas(endmark, "ENDMARK")) {
        shocst("ELW GE - ELWGE.MCV Corrupted", "Game halted");
        playgame = 0;
        return;
    }

    logflag = ynopt(LOGFLG);
    logthis("iniwara() called, loading options from ELWGEMSG");

    geuser   = stgopt(GEUSER);
    geship   = stgopt(GESHIP);
    geplnt   = stgopt(GEPLNT);
    gemail   = stgopt(GEMAIL);
    geshipcl = stgopt(GESHIPCL);

    gemaxplrs  = (SHORT)numopt(MAXPLRS, 1, 256);
    gemaxlist  = (SHORT)numopt(MAXLIST, 3, 50);
    maxships   = (SHORT)numopt(MAXSHIPS, 1, 50);
    se100dam   = (SHORT)numopt(SE100DAM, 1, 101);
    showopt    = (SHORT)numopt(SHOWOPT, 0, 5);
    trans_opt  = ynopt(TRANSOPT);
    syscmds    = ynopt(SYSCMDS);
    sysonly    = ynopt(SYSONLY);
    max_plnts  = (SHORT)numopt(MAXPLNTS, 1, 256);
    plantock   = lngopt(PLANTOCK, 1, 32760) * 60L;
    numships   = (SHORT)numopt(NUMSHIPS, 1, 500);
    maxdroids  = (SHORT)numopt(MAXDROID, 0, 500);
    plodds     = (SHORT)numopt(PLODDS, 1, 20);
    wormodds   = (SHORT)numopt(WORMODDS, 1, 100);
    univmax    = (SHORT)numopt(UNIVMAX, 10, 32767);
    univwrap   = ynopt(UNIVWRAP);
    s00plnum   = (SHORT)numopt(S00PLNUM, 3, 9);
    maxplanets = (SHORT)numopt(MAXPLSE, 1, 9);
    teambonus  = lngopt(TEAMBONU, 0, 32000) * 100L; // was numopt - check msg file - RH
    team_max = (SHORT)numopt(TEAMMAX, 0, 32000);
    usegemsg = ynopt(USEGEMSG);

    profon = ynopt(PROFON);

    if (logflag)
        geshocst(0, "ELWGE:Ext Trace Logging ON!");
#ifdef FASTPLANET
    geshocst(0, "ELWGE:FASTPLANET ON!");
#endif

    hpfirdst = (SHORT)numopt(HPFIRDST, 1, 20);
    hpdammax = (SHORT)numopt(HPDAMMAX, 1, 200);
    pfirdist = (SHORT)numopt(PFIRDST, 1, 20);
    pdammax  = (SHORT)numopt(PDAMMAX, 1, 200);

    jamtime  = (SHORT)numopt(JAMTIME, 1, 10);
    maildays = (SHORT)numopt(MAILDAYS, 1, 7);
    torpsped = (SHORT)numopt(TORPSPED, 1, 10000);
    mislsped = (SHORT)numopt(MISLSPED, 1, 10000);

    nummines  = (SHORT)numopt(NUMMINES, 1, 200);
    usermines = (SHORT)numopt(USRMINES, 1, 200);

    decodds    = (SHORT)numopt(DECODDS, 1, 20);

    tor_fact   = (DOUBLE)numopt(TORFACT, 1, 50);
    tor_fact   = tor_fact / 10.0;
    tdammax    = (DOUBLE)numopt(TDAMMAX, 1, 100);
    mis_fact   = (DOUBLE)numopt(MISFACT, 1, 50);
    mis_fact   = mis_fact / 10.0;
    mdammax    = (DOUBLE)numopt(MDAMMAX, 1, 100);
    idammax    = (DOUBLE)numopt(IDAMMAX, 1, 100);
    minedammax = (DOUBLE)numopt(MNDAMMAX, 1, 200);
    repairrate = (DOUBLE)numopt(REPAIRRT, 1, 50);
    repairrate = repairrate / 100.0;

    tooclose = (DOUBLE)numopt(TOOCLOSE, 1, 32000);

    clenguse = (SHORT)numopt(CLENGUSE, 1, 32000);

    startcash = (LONG)numopt(STRTCASH, 1, 32000);
    startcash *= 1000L;

    max_plrec = (LONG)numopt(MAXPLREC, 10, 32767);
    max_plrec *= 2;

    cyb_gold = (SHORT)numopt(CYBGOLD, 0, 32000);

    hyperdist1 = (DOUBLE)numopt(HYPDST1, 1, 32000);
    hyperdist2 = (DOUBLE)numopt(HYPDST2, 1, 32000);

    plattrf1 = (DOUBLE)(numopt(PLATTRF1, 5, 1000));
    plattrf1 = plattrf1 / 100.0;

    plattrf2 = (DOUBLE)(numopt(PLATTRF2, 5, 1000));
    plattrf2 = plattrf2 / 100.0;

    plattrf3 = (DOUBLE)(numopt(PLATTRF3, 5, 1000));
    plattrf3 = plattrf3 / 100.0;

    plattrt1 = (DOUBLE)(numopt(PLATTRT1, 5, 1000));
    plattrt1 = plattrt1 / 100.0;

    plattrt2 = (DOUBLE)(numopt(PLATTRT2, 5, 1000));
    plattrt2 = plattrt2 / 100.0;

    msgsiz = outbsz - 384;

    // load the planet maximum table
    for (i = 0; i < NUMITEMS; ++i) {
        logthis(spr("Item %s", item_name[i]));

        maxpl[i] = (DOUBLE)(lngopt(ITMPL01 + i, 0L, 201228378L));
        logthis(spr("Itm #%d maxpl=%ld", i, (LONG)maxpl[i]));

        weight[i] = lngopt(ITMWT01 + i, 0L, 201228378L);
        logthis(spr("Itm #%d weight=%ld", i, weight[i]));

        value[i] = lngopt(ITMVAL01 + i, 0L, 201228378L);
        logthis(spr("Itm #%d value=%ld", i, value[i]));

        manhours[i] = lngopt(ITMMH01 + i, 0L, 201228378L);
        logthis(spr("Itm #%d manhours=%ld", i, manhours[i]));

        baseprice[i] = (USHORT)numopt(ITMPR01 + i, 0, 32000);
        logthis(spr("Itm #%d baseprice=%d", i, baseprice[i]));
    }

    // load the shieldprice table
    for (i = 0; i < TOPSHIELD; ++i) {
        logthis(spr("shieldtype %d", i));
        shieldprice[i] = lngopt(SHLDPR01 + i, 0L, 201228378L);
        logthis(spr("Shld #%d Price=%ld", i, shieldprice[i]));
    }

    // load the phaserprice table
    for (i = 0; i < TOPPHASER; ++i) {
        logthis(spr("phasertype %d", i));
        phaserprice[i] = lngopt(PHSRPR01 + i, 0L, 201228378L);
        logthis(spr("Phaser #%d Price=%ld", i, phaserprice[i]));
    }

    pltvcash = lngopt(PLTVCASH, 0L, 201228378L);
    logthis(spr("pltvcash=%ld", pltvcash));

    pltvdiv = lngopt(PLTVDIV, 0L, 201228378L);

    phatowrp = (SHORT)numopt(PHATOWRP, 0, 100);

    misengfc = (SHORT)numopt(MISENGFC, 1, 2000);

    score_bonus = (SHORT)numopt(SCRBONUS, 0, 32700);
    score_f2    = (SHORT)numopt(SCRFACT, 0, 32700);

    chgloser = (SHORT)numopt(CHGLOSER, 0, 100);

    optmenu = ynopt(OPTMENU);
    optchr  = (CHAR)chropt(OPTCHR);
    optchr  = (CHAR)toupper(optchr);
    opttxt  = stgopt(OPTTXT);

    logthis("initializing opttbl");
    n      = nterms * sizeof(LONG); // RH 4/6/2021
    opttbl = (LONG *)alczer(n);     // RH 4/6/2021

    logthis("opening gebb1 (ELWGESHP)");
    gebb1 = dfaOpen(geship, sizeof(WARSHP), NULL);

    if (usegemsg) {
        logthis("opening gebb4 (ELWGEMAL)");
        gebb4 = dfaOpen(gemail, sizeof(GEMESSAGE), NULL);
    }

    logthis("opening gebb2 (ELWGEPLNT)");
    // why would this be sizeof GALSECT -- GALSECT doesn't have
    // the userid, and these could get out of sync size
    // wise in the future - RH 4/2021
    gebb2 = dfaOpen(geplnt, sizeof(GALPLNT), NULL);

    numrecs = dfaCountRec();
    numrecs += 25;

    geshocst(1, spr("ELWGE:Numrecs (raw) originally %ld", numrecs));
    numrecs = (numrecs / (LONG)plodds) * 4L;

    plantime = (SHORT)(plantock / numrecs);

    if (plantime < 4)
        plantime = 4;

#ifdef FASTPLANET
    plantime = 4;
#endif

    geshocst(1, spr("ELWGE:Numrecs calculated to be %ld", numrecs));
    geshocst(1, spr("ELWGE:Plantime set to %d", plantime));

    logthis("opening gebb5 (ELWGEUSR)");

    gebb5 = dfaOpen(geuser, sizeof(WARUSR), NULL);

    gehlpmb = opnmsg(GEHELP);

    // ships in game is at least number of terminal channels
    nships = (SHORT)(nterms + numships); // nships = nterms (channels) +
                                         // numships (droid ships, nonuser)

    // allocate memory for user data table
    logthis("initializing warusr");
    warusr = (WARUSR *)alcblok(nships, sizeof(WARUSR)); // RH 4/4/21

    geshocst(1, spr("ELWGE:INF:User Mem: %ld", nships * sizeof(WARUSR)));

    // allocate memory for ship data table
    logthis("initializing warshp");
    warshp = (WARSHP *)alcblok(nships, sizeof(WARSHP)); // RH 4/4/21

    geshocst(1, spr("ELWGE:INF:Ship Mem: %ld", nships * sizeof(WARSHP)));

    // allocate memory for planet table
    logthis("initializing ptab");
    n    = (nships * sizeof(PLANETAB)) + 1;               // RH 4/6/2021
    ptab = (PLANETAB *)alcblok(nships, sizeof(PLANETAB)); // RH 4/6/21

    geshocst(1, spr("ELWGE:INF:Planet Table Mem: %d", n));

    // allocate memory for a temporary item table
    logthis("initializing titems");
    n      = nships * sizeof(ITEM); // RH 4/6/2021
    titems = (ITEM *)alczer(n);     // RH 4/6/2021

    geshocst(1, spr("ELWGE:INF:Temp Items Mem: %d", n));

    // allocate memory for a team table
    logthis("initializing teamtab");
    n       = MAXTEAMS * sizeof(TEAM); // RH 4/6/2021
    teamtab = (TEAM *)alczer(n);       // RH 4/6/2021

    geshocst(1, spr("ELWGE:INF:Team Tab Mem: %d", n));

    // allocate memory for scan table
    logthis("initializing scantab");
    n       = nships * sizeof(SCANTAB);                    // RH 4/6/2021
    scantab = (SCANTAB *)alcblok(nships, sizeof(SCANTAB)); // RH 4/6/2021

    geshocst(1, spr("ELWGE:INF:Scantab Mem: %d", n));

    // allocate memory for S00 table
    logthis("initializing s00");
    n   = s00plnum * sizeof(S00); // RH 4/6/2021
    s00 = (S00 *)alczer(n);       // RH 4/6/2021

    geshocst(1, spr("ELWGE:INF:S00 Mem: %d", n));

    // allocate memory for beacon table
    logthis("initializing beacon");
    n      = nships * sizeof(BEACONTAB); // RH 4/6/2021
    beacon = (BEACONTAB *)alczer(n);     // RH 4/6/2021

    geshocst(1, spr("ELWGE:INF:Beacontab Mem: %d", n));

    // allocate memory for the mail message table
    logthis("initializing gemsg");
    gemsg = (GEMESSAGE *)alczer(sizeof(GEMESSAGE)); // RH 4/6/2021

    geshocst(1, spr("ELWGE:INF:GEmsg Mem: %d", sizeof(GEMESSAGE)));

    // allocate memory for mine table
    logthis("initializing mines");
    n     = nummines * sizeof(MINE); // RH 4/6/2021
    mines = (MINE *)alczer(n);       // RH 4/6/2021

    geshocst(1, spr("ELWGE:INF:Mines Mem: %d", n));

    // allocate memory for garbage bucket
    logthis("initializing garbage bucket - gechrbuf");
    gechrbuf = (CHAR *)alczer(255); // RH 4/6/2021
    logthis("initializing garbage bucket - gechrbuf2");
    gechrbuf2 = (CHAR *)alczer(20); // RH 4/6/2021
    logthis("initializing garbage bucket - gechrbuf3");
    gechrbuf3 = (CHAR *)alczer(20); // RH 4/6/2021
    logthis("initializing garbage bucket - warpbuf");
    warpbuf = (CHAR *)alczer(40); // RH 4/6/2021

    // init empty mine field
    for (n = 0; n < nummines; ++n)
        mines[n].channel = 255;

    // init sector, planet, and worm table to bad values
    sector.xsect = 32767;
    sector.ysect = 32767;
    sector.plnum = 32767;

    planet.xsect = 32767;
    planet.ysect = 32767;
    planet.plnum = 32767;

    worm.xsect = 32767;
    worm.ysect = 32767;
    worm.plnum = 32767;

    setmbk(gemb);

    cyb_class = 0;

    // load the ship class table
    geshmb = opnmsg(geshipcl);
    setmbk(geshmb);

#define NCL 28

    // first audit the table

    // does the table have all the elements?
    n = (SXXEND - S01TYPE);
    i = n / NCL;
    if ((i * NCL) != n) {
        shocst("ELWGE:ERR:Ship Class Tbl Corrupted", "Game halted.");
        playgame = 0;
        return;
    }
    // this is how many inactive and active classes we have
    // since we only load active classes we must go figure
    // out how many that really is.
    geshocst(1, spr("ELWGE:INF:Fnd %d class slots", i));
    tot_classes = (SHORT)i;

    n = 0;

    for (i = 0; i < tot_classes; ++i) {
        classbase = (SHORT)(S01TYPE + (i * NCL));
        type = (SHORT)tokopt(classbase, "USER", "CYBORG", "DROID", "<NONE>",
                             NULL);

        class_tab[i] = classbase;

        if (type != CLASSTYPE_NONE)
            ++n;
    }

    geshocst(1, spr("ELWGE:INF:Fnd %d defined classes", n));

    // allocate memory for ship class table
    logthis("initializing shipclass");
    n         = tot_classes * sizeof(SHIP); // RH 4/6/2021
    shipclass = (SHIP *)alczer(n);          // RH 4/6/2021

    geshocst(1, spr("ELWGE:INF:Ship Class Mem: %d", n));

    // read in the ship classes
    i = 0;
    for (n = 0; n < tot_classes; ++n) {
        classbase             = class_tab[i];
        shipclass[i].max_type = (USHORT)tokopt(classbase, "USER", "CYBORG",
                                               "DROID", "<NONE>", NULL);
        shipclass[i].typename = stgopt(++classbase);
        logthis(spr("Loaded class %d - %s", i, shipclass[i].typename));

        shipclass[i].shipname = stgopt(++classbase);
        logthis(spr("  Shipname %s", shipclass[i].shipname));

        shipclass[i].max_shlds      = (UCHAR)numopt(++classbase, 0, 19);
        shipclass[i].max_phasr      = (CHAR)numopt(++classbase, 0, 19);
        shipclass[i].max_torps      = (UCHAR)ynopt(++classbase);
        shipclass[i].max_missl      = (UCHAR)ynopt(++classbase);
        shipclass[i].has_decoy      = (CHAR)ynopt(++classbase);
        shipclass[i].has_jam        = (CHAR)ynopt(++classbase);
        shipclass[i].has_zip        = (CHAR)ynopt(++classbase);
        shipclass[i].has_mine       = (CHAR)ynopt(++classbase);
        shipclass[i].max_attk       = (CHAR)ynopt(++classbase);
        shipclass[i].max_cloak      = (CHAR)ynopt(++classbase);
        shipclass[i].max_accel      = (USHORT)numopt(++classbase, 0, 32767);
        shipclass[i].max_warp       = (UCHAR)numopt(++classbase, 0, 255);
        shipclass[i].max_tons       = lngopt(++classbase, 1, 2000000000L);
        shipclass[i].max_price      = lngopt(++classbase, 1, 2000000000L);
        shipclass[i].max_points     = (USHORT)numopt(++classbase, 1, 32767);
        shipclass[i].scanrange      = lngopt(++classbase, 1, 9999999L);
        shipclass[i].cybs_can_att   = (UCHAR)ynopt(++classbase);
        shipclass[i].noclaim        = (CHAR)numopt(++classbase, 0, 255);
        shipclass[i].lowest_to_attk = (CHAR)numopt(++classbase, 0, 255);
        shipclass[i].tot_to_create  = (USHORT)numopt(++classbase, 0, 255);
        shipclass[i].tough_factor   = (CHAR)numopt(++classbase, 0, 1);
        shipclass[i].damfact        = (USHORT)numopt(++classbase, 0, 32767);
        shipclass[i].res_flag_2     = (USHORT)numopt(++classbase, 0, 32767);
        shipclass[i].res_flag_3     = (USHORT)numopt(++classbase, 0, 32767);

        shipclass[i].hlpmsg = ++classbase;

        shipclass[i].init_func = NULL;
        shipclass[i].tick_func = NULL;
        shipclass[i].kill_func = NULL;
        shipclass[i].won_func  = NULL;

        if (shipclass[i].max_type == CLASSTYPE_CYBORG) { // CYBORG
            if (cyb_class == 0) // set up base cybertron class
                cyb_class = (SHORT)i;
            shipclass[i].init_func = cyb_init;
            shipclass[i].tick_func = cyb_lives;
            shipclass[i].kill_func = cyb_died;
            shipclass[i].won_func  = cyb_won;
        } else if (shipclass[i].max_type == CLASSTYPE_DROID) { // DROID
            shipclass[i].init_func = droid_init;
            shipclass[i].tick_func = droid_lives;
            shipclass[i].kill_func = droid_died;
            shipclass[i].won_func  = droid_won;
        }
        geshocst(1, spr("ELWGE:INF:Init Class %s", shipclass[i].typename));

        ++i; // index the next table entry
    }

    setmbk(gemb);

    // number of parameters per planet entry
#define NPL 8

    // read in the neutral planets
    for (i = 0; i < s00plnum; ++i) {
        if ((S00P1RES + (i * NPL)) - (S00P1DEF + (i * NPL)) != (NPL - 1)) {
            shocst(spr("ELWGE:ERR:Sect00 Table Error %d Msg # %d", i + 1, S00P1DEF + (i * NPL)),"Game halted");
            playgame = 0;
            return;
        }
        classbase = (SHORT)(S00P1DEF + (i * NPL));
        n         = (INT)ynopt(classbase);
        if (!n) {
            shocst(spr("ELWGE:ERR:Sect00 Table Error %d Msg # %d", i + 1, S00P1DEF + (i * NPL)), "Game halted");
            playgame = 0;
            return;
        }

        s00[i].name  = stgopt(++classbase);
        s00[i].owner = stgopt(++classbase);
        s00[i].type  = (SHORT)numopt(++classbase, 0, 3);

        s00[i].xcoord = (DOUBLE)(numopt(++classbase, 100, 9900));
        s00[i].xcoord = s00[i].xcoord / 10000.0;
        s00[i].ycoord = (DOUBLE)(numopt(++classbase, 100, 9900));
        s00[i].ycoord = s00[i].ycoord / 10000.0;

        s00[i].env = (SHORT)numopt(++classbase, 0, 3);
        s00[i].res = (SHORT)numopt(++classbase, 0, 3);

        geshocst(1, spr("ELWGE:INF:I/S00 %d %s", s00[i].type, s00[i].name));
    }

    // Load the team table from disk
    logthis("loading team tab");
    load_team_tab();

    // turn on lockon warning global flag
    lockwarn = TRUE;

    if (playgame == 1) {
        logthis("starting the rtkick() routines");
        logthis("rtkick() warrti");
        rtkick(TICKTIME, warrti);
        logthis("rtkick() warrti2");
        rtkick(TICKTIME2, warrti2);
        logthis("rtkick() warrti3");
        rtkick(60, warrti3);
        logthis("rtkick() plarti");
        rtkick(plantime, plarti);
#ifdef BLDPLNTS
        logthis("rtkick() plabld");
        rtkick(30, plabld);
#endif
        logthis("rtkick() autorti");
        rtkick(1, autorti);
    }

    logthis("setting warshpoff(j)->status for all nships");
    for (j = 0; j < nships; ++j)
        warshpoff(j)->status = GESTAT_AVAIL;

    // find the module number (state) of the FSE for later use
    logthis("finding the FSE in a wonky way"); // RH 4/6/2021 - there's a better
                                               // way to hook this
    fse_state = -1;
    for (i = 0; i < nmods; i++) {
        if ((sameas((CHAR *)(module[i]->descrp), "Editor")) == TRUE)
            fse_state = (SHORT)i;
    }

    logthis("exiting iniwara(), returning you to your normal broadcast schedule");

    // tell everyone that we are up
    // if (playgame == 1)
    //   geshocst(0,spr("ELW Galactic Empire v%s Up!",VERSION));

    // end of initialization
}

// User logon routine
GBOOL FUNC gelogon(VOID)
{
    setmbk(gemb);

    if (!hasmkey(PLAYKEY))
        return (0);

    if (!geudb(GELOOKUP, usaptr->userid, warusroff(usrnum))) {
        prfmsg(GECALLS);
        outprf(usrnum);
    } else if (gernd() % 10 == 1) {
        prfmsg(GECALLS2);
        outprf(usrnum);
    }

    return (0);
}

// User deleted routine
VOID FUNC gedelete(CHAR *uid)
{

    if (geudb(GELOOKUP, uid, &tmpusr)) {
        geudb(GEGET, uid, &tmpusr);
        while (gepdb(GELOOKUPNAME, uid, 0, &tmpshp)) {
            dfaAbsRec(&tmpshp, 0);
            gepdb(GEDELETE, tmpshp.userid, tmpshp.shipno, &tmpshp);
            logthis(spr("GE:Deleted %s ship %d", tmpshp.userid, tmpshp.shipno));
        }
        geudb(GEDELETE, tmpusr.userid, &tmpusr);
        logthis(spr("GE:Deleted %s user", tmpusr.userid));
        return;
    }

    geshocst(1, spr("ELWGE:User %s not in DB", uid));
    return;
}

// System cleanup/maintenance routine
VOID FUNC gemidnight(VOID)
{
    SHORT i, prodReports = 0;
    SHORT foundit;
    LONG  scr;
    CHAR  tmpbuf2[2];

    setmbk(gemb);

    geshocst(0, spr("ELWGE:INF:Begin Cleanup"));

    // clear out planet counter
    geshocst(1, spr("ELWGE:INF:Cleanup Phase-1"));
    dfaSetBlk(gebb5);
    if (dfaQueryLO(0)) {
        do {
            dfaAbsRec(&tmpusr, 0);
            if (!sameto(tmpusr.userid, KEY)) {
                tmpusr.planets    = 0;
                tmpusr.score      = 0;
                tmpusr.plscore    = 0;
                tmpusr.population = 0;
                dfaUpdate(&tmpusr);
                dfaAbsRec(&tmpusr, 0);
            }
        } while (dfaQueryNX());
    }

    geshocst(1, spr("ELWGE:INF:Cleanup Phase-2"));

    dfaSetBlk(gebb2);

    if (dfaQueryLO(0)) {
        do {
            dfaAbsRec(&planet, 0);
            dfaSetBlk(gebb5);
            if (planet.type == PLTYPE_PLNT) {
                if (planet.userid[0] != 0) {
                    if (dfaQueryEQ(planet.userid, 0)) {
                        dfaAbsRec(&tmpusr, 0);
                        plptr = &planet;
                        calc_networth();
                        ++tmpusr.planets;
                        tmpusr.population += (plptr->items[I_MEN].qty / 10000L);
                        dfaUpdate(&tmpusr);
                        dfaAbsRec(&tmpusr, 0);

                        // now go create the Status Record

                        strncpy(tmpstat.userid, tmpusr.userid, UIDSIZ);
                        tmpstat.class = MAIL_CLASS_PRODRPT;
                        tmpstat.type  = MESG20;
                        tmpstat.stamp = cofdat(today());
                        sprintf(tmpstat.dtime, "%s - %.5s", ncedat(today()), nctime(now()));
                        strcpy(tmpstat.name1, planet.name);
                        tmpstat.int1 = planet.xsect;
                        tmpstat.int2 = planet.ysect;
                        tmpstat.cash = planet.cash;
                        tmpstat.debt = planet.debt;
                        tmpstat.tax  = planet.tax;
                        for (i = 0; i < NUMITEMS; ++i)
                            tmpstat.itemqty[i] = planet.items[i].qty;

                        // HERE

                        memcpy(&mail, &tmpstat, sizeof(MAILSTAT));
                        mailit(0);
                        prodReports++;
                    }
                }
            }
            dfaSetBlk(gebb2);
        } while (dfaQueryNX());
    }
    logthis(spr("Sent %d production reports", prodReports));

    sprintf(gechrbuf, "%ld", (dfaCountRec() / 2L));
    geshocst(0, spr("ELWGE:INF:Plnt DB Size %sk", gechrbuf));

    if (dfaCountRec() >= (ULONG)max_plrec)
        geshocst(0, "ELWGE:INF:Max Sect Reached");

    // purge mail older than 7 days
    if (usegemsg) {
        geshocst(1, spr("ELWGE:INF:Cleanup Phase-3"));
        dfaSetBlk(gebb4);

        i = cofdat(today());
        i -= maildays; // back up 1 week
        if (dfaQueryLO(0)) {
            do {
                dfaAbsRec(gemsg, 0);
                if (gemsg->m.nrpl < i) // we robbed nreply for the stamp
                    dfaDelete();
                if (gemsg->m.to[0] == '*') // non-live player
                    dfaDelete();

            } while (dfaQueryNX());
        }
    } else
        geshocst(1, spr("ELWGE:INF:Skip Cleanup Phase-3"));

    geshocst(1, spr("ELWGE:INF:Cleanup Phase-4"));
    dfaSetBlk(gebb5);

    // zero out the team count and score
    for (i = 0; i < MAXTEAMS; ++i) {
        teamtab[i].teamcount = 0;
        teamtab[i].teamscore = 0;
    }

    // first count up the members of a team
    if (dfaQueryLO(0)) {
        do {
            dfaAbsRec(&tmpusr, 0);
            if (!sameto(tmpusr.userid, KEY)) { /* ignore the secret key record */
                foundit = FALSE;
                if (tmpusr.teamcode > 0) {
                    for (i = 0; i < MAXTEAMS; ++i) {
                        if (teamtab[i].teamcode == (LONG)tmpusr.teamcode) {
                            foundit = TRUE;
                            break;
                        }
                    }
                    if (foundit == FALSE) {
                        tmpusr.teamcode = 0;
                        logthis(spr("Reset Teamcode to 0 [%s]", tmpusr.userid));
                    } else {
                        ++teamtab[i].teamcount;
                        sprintf(gechrbuf, "++Teamcnt %ld %s", tmpusr.teamcode,
                                tmpusr.userid);
                        logthis(gechrbuf);
                    }
                }
                if (foundit == FALSE)
                    dfaUpdate(&tmpusr);
            }
            dfaAbsRec(&tmpusr, 0);
        } while (dfaQueryNX());
    }

    // update player and team scores
    if (dfaQueryLO(0)) {
        do {
            dfaAbsRec(&tmpusr, 0);
            if (!sameto(tmpusr.userid, KEY)) { /* ignore the secret key record */
                tmpusr.score = tmpusr.plscore + tmpusr.klscore;

                if (tmpusr.teamcode > 0) {
                    for (i = 0; i < MAXTEAMS; ++i) {
                        if (teamtab[i].teamcode == (LONG)tmpusr.teamcode) {
                            break;
                        }
                    }

                    // just incase things get messed up
                    if (teamtab[i].teamcount == 0)
                        teamtab[i].teamcount = 1;

                    teamtab[i].teamscore += teambonus;

                    sprintf(gechrbuf, "++Teamscr=%ld+(%ld/%d) %s",
                            teamtab[i].teamscore, tmpusr.score,
                            teamtab[i].teamcount, tmpusr.userid);
                    logthis(gechrbuf);

                    scr = tmpusr.score / (LONG)teamtab[i].teamcount;
                    teamtab[i].teamscore += scr;
                }
                dfaUpdate(&tmpusr);
            }
            dfaAbsRec(&tmpusr, 0);
        } while (dfaQueryNX());
    }

    // remove any teams with no players
    for (i = 0; i < MAXTEAMS; ++i) {
        if (teamtab[i].teamcode > 0 && teamtab[i].teamcount == 0) {
            teamtab[i].teamcode = -1;
            geshocst(0, spr("ELWGE:INF:Removed Team %s", teamtab[i].teamname));
        }
    }

    // update the team scores on disk
    update_team_tab();

    /* 12/19/91 added to create a roster position data field in the user record
      which is later used to calculate bonus points. */

    i = 0;

    dfaSetBlk(gebb5);

    strncpy(tmpbuf2, KEY, 1);

    if (dfaQueryHI(1)) {
        do {
            dfaAbsRec(&tmpusr, 1);
            if (tmpusr.score > 0 && tmpusr.userid[0] != tmpbuf2[0] &&
                tmpusr.userid[0] != '@') {
                ++i;
                tmpusr.rospos = i;
                dfaUpdate(&tmpusr);
                dfaAbsRec(&tmpusr, 1);
            }
        } while (dfaQueryPR());
    }

    geshocst(0, spr("ELWGE:INF:End Cleanup"));
    return;
}

// User logged off routine
SHORT FUNC warlof(VOID)
{
    warsptr = warshpoff(usrnum);
    waruptr = warusroff(usrnum);

    logthis(spr("WARLOF called 4 %s", waruptr->userid));
    return (0);
}

// User hangup / lost connection routine
VOID FUNC warhup(VOID)
{
    dfaSetBlk(gebb1);
    setmbk(gemb);

    warsptr = warshpoff(usrnum);
    waruptr = warusroff(usrnum);

    logthis(spr("WARHUP called 4 %s", waruptr->userid));

    if (warsptr->status == GESTAT_USER) {
        if (ingegame((SHORT)usrnum)) {
            // if modem hangup
            logthis(spr("User Hungup Status = %d", status));
            // if (warsptr->cantexit > 0 && status == 11)
            if (warsptr->cantexit > 0) {
                killem(warsptr, usrnum);
                warsptr->where = -1;
            } else {
                prfmsg(WARHUP, username(warsptr));
                outsect(ALWAYS, &warsptr->coord, (USHORT)usrnum, 0);
                cleartm(usrnum);
                gepdb(GEUPDATE, warsptr->userid, warsptr->shipno, warsptr);

                geudb(GEUPDATE, waruptr->userid, waruptr);
            }
            --numwar;
        }
    }

    warsptr->status = GESTAT_AVAIL;
    return;
}

// System Shutdown Routine
VOID FUNC clswar(VOID)
{
    if (gemb != NULL) {
        clsmsg(gemb);
        gemb = NULL;
    }

    dfaClose(gebb1);
    dfaClose(gebb4);

    dfaClose(gebb2);
    dfaClose(gebb5);

    logthis("***GALACTIC EMPIRE SHUTDOWN***");
    return;
}

// Status-Input Handler
VOID FUNC stshdlr(VOID)
{

    if (status == CYCLE) {
        switch (usrptr->substt) {
        case OPTDISP:
            setmbk(gemb);
            if (btuoba(usrnum) > (OUTSIZ / 2)) {
                optdisp();
            } else {
                btuinj(usrnum, CYCLE);
            }

            break;

        case OPTDISP2:
            setmbk(gemb);
            usrptr->substt = 1;
            prf(".");
            disp_main_menu();
            outprfge(ALWAYS, usrnum);
            return;

        default:
            dfsthn();
            return;
        }
    } else {
        dfsthn();
    }
}

// Module main input routine
GBOOL FUNC galemp(VOID)
{
    SHORT i, rtn;

    if (playgame == 0) {
        prf("\nThe Galactic Empire universe has experienced excessive ion radiation\n");
        prf("and it's not safe to travel in. The SysOp is heading a task force to\n");
        prf("clear the way for re-entry.\n\n");
        return (0);
    }

    dfaSetBlk(gebb1);
    setmbk(gemb);
    warsptr = warshpoff(usrnum);
    waruptr = warusroff(usrnum);

    for (i = 0; i < MENUNUM; ++i) {
        if (menu[i].substt == usrptr->substt) {
            rtn = menu[i].func(); // (*(menu[i].func))();
            clrprf();
            return (rtn);
        }
    }

    return (1);
}

/**
 ** End Module API Routines
 **/

// determine the net worth of a planet
VOID FUNC calc_networth(VOID)
{
    ULONG v;

    v = value_pl();
    tmpusr.plscore += v;
}

ULONG FUNC value_pl(VOID)
{
    ULONG v;
    SHORT i;

    v = (plptr->cash + plptr->tax) / (1000000L / pltvcash);

    for (i = 0; i < NUMITEMS; ++i) {
        v += (value[i] * ((LONG)plptr->items[i].qty / pltvdiv));
    }

    return (v);
}

// Broadcast a message to all ships (except exclude_usrn)
VOID FUNC outwar(SHORT filter, INT exclude_usrn, USHORT freq)
{
    SHORT i;
    INT   zothusn;

    for (zothusn = 0; zothusn < nships; zothusn++) {
        if (zothusn != exclude_usrn && ingegame((SHORT)zothusn)) {
            if (freq == 0) {
                outprfge(filter, zothusn);
            } else {
                for (i = 0; i < 3; ++i) {
                    if (freq == warshpoff(zothusn)->freq[i]) {
                        outprfge(filter, zothusn);
                    }
                }
            }
        }
    }

    clrprf();
}

// Player/ship database functionality
SHORT FUNC gepdb(SHORT func, CHAR *usrname, SHORT shipnum, WARSHP *geptr)
{
    SHORT rtn;

    dfaSetBlk(gebb1);
    rtn = 0;

    strncpy(shpkey.userid, usrname, UIDSIZ);
    shpkey.shipno = shipnum;
    logthis(spr("GEPDB called: F=%d,%s,%d,%s", func, usrname, shipnum, geptr->userid));
    switch (func) {

    case GELOOKUP:
        if (dfaQueryEQ(&shpkey, 1))
            rtn = 1;
        break;

    case GEADD:
        if (!dfaInsertDup(geptr))
            geshocst(0, spr("ELWGE:ERR:Ship ins Fail %s", usrname));

        break;

    case GEDELETE:
        if (dfaAcqEQ(NULL, &shpkey, 1)) {
            dfaDelete();
            rtn = 1;
        } else {
            geshocst(0, spr("ELWGE:ERR:Ship Del Fail %s", usrname));
        }
        break;

    case GEUPDATE:
        if (dfaAcqEQ(NULL, &shpkey, 1)) {
            dfaUpdate(geptr);
            rtn = 1;
        } else {
            geshocst(0, spr("ELWGE:ERR:Ship Upd Fail %s", usrname));
        }
        break;

    case GEGET:
        if (dfaAcqEQ(geptr, &shpkey, 1))
            rtn = 1;
        break;

    case GENEXT:
        if (dfaQueryNX())
            rtn = 1;
        break;

    case GELOOKUPNAME:
        if (dfaQueryEQ(usrname, 0))
            rtn = 1;
        break;

    default:
        rtn = 0;
    }
    return (rtn);
}

// User database functionality
SHORT FUNC geudb(SHORT func, CHAR *usrname, WARUSR *geptr)
{
    SHORT rtn;

    dfaSetBlk(gebb5);
    rtn = 0;

    logthis(spr("GEUDB called: F=%d,%s,%s", func, usrname, geptr->userid));
    switch (func) {

    case GELOOKUP:
        if (dfaQueryEQ(usrname, 0))
            rtn = 1;
        logthis(spr("GE: lookup *%s* f:%d", usrname, rtn));
        break;

    case GEADD:

        if (!dfaInsertDup(geptr))
            geshocst(0, spr("ELWGE:ERR:User ins Fail %s", usrname));

        break;

    case GEDELETE:
        if (dfaAcqEQ(NULL, usrname, 0)) {
            dfaDelete();
            rtn = 1;
        } else {
            geshocst(0, spr("ELWGE:ERR:User Del Fail %s", usrname));
        }
        break;

    case GEUPDATE:
        logthis(spr("DEBUG <%s> <%s> update", usrname, geptr->userid));
        if (dfaAcqEQ(NULL, usrname, 0)) {
            dfaUpdate(geptr);
            rtn = 1;
        } else {
            geshocst(0, spr("ELWGE:ERR:User Upd Fail %s", usrname));
        }
        break;

    case GEGET:
        if (dfaAcqEQ(geptr, usrname, 0)) {
            rtn = 1;
        } else {
            geshocst(0, spr("ELWGE:ERR:User Get Fail %s", usrname));
        }
        break;

    default:
        rtn = 0;
    }
    return (rtn);
}

// Sector database functionality
SHORT FUNC gesdb(SHORT func, PKEY *sect, GALSECT *geptr)
{
    SHORT rtn;
    LONG  numrecs; // = 0;

    logthis(spr("Func GESDB, func = %d, sect*= %ld,geptr*=%ld", func, (LONG)sect, (LONG)geptr));
    logthis(spr("            xsect %d, ysect %d, plnum %d", sect->xsect, sect->ysect, sect->plnum));

    dfaSetBlk(gebb2);
    rtn = 0;

    switch (func) {

    case GELOOKUP:
        if (!dfaQueryEQ(sect, 0))
            rtn = 1;
        break;

    case GEUPDATE:
        if (dfaAcqEQ(NULL, sect, 0)) {
            dfaUpdate(geptr);
            rtn = 1;
        } else {
            geshocst(0, spr("ELWGE:ERR:Plt Upd Fail x%d,y%d,p%d", sect->xsect,
                            sect->ysect, sect->plnum));
        }
        break;

    case GEADD:
        numrecs = dfaCountRec();
        if (numrecs < max_plrec) {
            logthis(spr("GE:DBG:Ins Sect %d %d %d", geptr->xsect, geptr->ysect, geptr->plnum));

            if (!dfaInsertDup(geptr))
                geshocst(0, "ELWGE:ERR:Sect/plt ins Fail");

            logthis("GE:DBG:Ins Sect suceeded");
        } else {
            geshocst(1, "ELWGE:INF:Max Sect Reached");
        }
        break;

    case GEGET:
        if ((geptr->xsect != sect->xsect) || (geptr->ysect != sect->ysect) ||
            (geptr->plnum != sect->plnum)) {
            if (dfaAcqEQ(geptr, sect, 0)) {
                logthis("gesdb GEGET dfaAcqEQ found record");
                rtn = 1;
            }
        } else {
            logthis("gesdb GEGET record already in memory");
            rtn = 1;
        }
        break;

    case GEGETNOW:
        if (dfaAcqEQ(geptr, sect, 0))
            rtn = 1;
        break;

    default:
        rtn = 0;
    }
    return (rtn);
}

SHORT FUNC getplanetdat(INT usrn) // NOTE plnum MUST be set before this is called
{
    SHORT i, bbad;

    if (plnum > 0 && plnum <= MAXPLANETS) {
        getsector(&(warshpoff(usrn)->coord));

        // DEBUG Is numplan  set right or is it 9
        if (plnum <= sector.numplan) {
            logthis(spr("Getsectdat:plnum = %d, numplan = %d", plnum, sector.numplan));
            getplanet(&(warshpoff(usrn)->coord), plnum);
            // If this fails what happens
            plptr = &planet;
            if (plptr->type == PLTYPE_WORM) {
                memcpy(&worm, &planet, sizeof(GALWORM)); /* make it the current user */
            } else {
                if (plptr->beacon[0] != 0) {
                    bbad = FALSE;

                    for (i = 0; i < BEACONMSGSZ; ++i) {
                        if (plptr->beacon[i] < ' ' || plptr->beacon[i] > '~') {
                            plptr->beacon[0] = 0;
                            bbad             = TRUE;
                            break;
                        }
                    }
                    if (!bbad) {
                        movecoord(&beacon[usrn].coord, &plptr->coord);
                        beacon[usrn].plnum = plnum;
                        strncpy(beacon[usrn].beacon, plptr->beacon, BEACONMSGSZ);
                    }
                }
            }
        } else {
            return (FALSE);
        }
    }
    return (TRUE);
}

// Team Table database functionality
VOID FUNC load_team_tab(VOID)
{
    CHAR  buffer[256];
    FILE *mzfp;
    SHORT i;

    // clear out the memory team table
    for (i = 0; i < MAXTEAMS; ++i) {
        teamtab[i].teamcode    = 0;
        teamtab[i].teamname[0] = 0;
        teamtab[i].teamcount   = 0;
        teamtab[i].teamscore   = 0;
        teamtab[i].password[0] = 0;
        teamtab[i].secret[0]   = 0;
    }

    logthis("Loading Team Table");

    if ((mzfp = fopen("ELWGETEA.DAT", "r")) != NULL) {
        i = 0;
        while (fgets(buffer, sizeof(buffer), mzfp) != NULL) {
            if (sameto("TEAM|", buffer) && buffer[80] == '|') {
                // logthis(spr("Loading TT Elmt # %d",i));
                // logthis(spr("Buffer = [%s]",buffer));
                strncpy(gechrbuf, &buffer[5], 5);
                gechrbuf[5]         = 0;
                teamtab[i].teamcode = atol(gechrbuf);
                logthis(spr(" Team Code [%s]", gechrbuf));

                strncpy(teamtab[i].teamname, &buffer[11], 30);
                stripb(teamtab[i].teamname);
                logthis(spr(" Team Name [%s]", teamtab[i].teamname));

                strncpy(gechrbuf, &buffer[42], 5);
                gechrbuf[5]          = 0;
                teamtab[i].teamcount = (USHORT)atoi(gechrbuf);
                logthis(spr(" Team Cnt [%s]", gechrbuf));

                strncpy(gechrbuf, &buffer[48], 10);
                gechrbuf[10]         = 0;
                teamtab[i].teamscore = atol(gechrbuf);
                logthis(spr(" Team Score [%s]", gechrbuf));

                strncpy(teamtab[i].password, &buffer[59], 10);
                stripb(teamtab[i].password);
                strncpy(teamtab[i].secret, &buffer[70], 10);
                stripb(teamtab[i].secret);

                i++;
                if (i >= MAXTEAMS)
                    break;
            } else {
                geshocst(0, "ELWGE:ERR Bad Team Rcd - Ignored");
            }
        }
        fclose(mzfp);
    }
}

VOID FUNC update_team_tab(VOID)
{
    FILE *hdl;
    SHORT i;

    hdl = fopen("ELWGETEA.DAT", "wt");

    if (hdl != (FILE *)0) {
        for (i = 0; i < MAXTEAMS; ++i) {
            if (teamtab[i].teamcode != -1) {
                fprintf(hdl, "TEAM|%5ld|%-30s|%5d|%10ld|%-10s|%-10s|\n",
                        teamtab[i].teamcode, teamtab[i].teamname,
                        teamtab[i].teamcount, teamtab[i].teamscore,
                        teamtab[i].password, teamtab[i].secret);
            }
        }
        fclose(hdl);
    }

    return;
}

// Planet economic functions
VOID FUNC plarti(VOID)
{
    static LONG   fpos    = 0;
    static USHORT plntcnt = 0;
    static USHORT plntpop = 0;
    static USHORT sectcnt = 0;
    static USHORT wormcnt = 0;
    SHORT         flag, tic, plnt_type, not_done;
    SHORT         firstime = FALSE;
    SHORT         i;
#define MAXTIC 20
    static LONG tocks = 0;
    DOUBLE      ftocktime, ftockfact;
    USHORT      minutes;
    SHORT       intkey;

    logthis("TICK:plarti entered");
    dfaSetBlk(gebb2);

    ++tocks;

    // if first time through get the first record in the file
    if (fpos == 0) {
        logthis("plarti:fpos == 0 reset stuff");
        intkey = 1;
        if (dfaAcqGT(&planet, &intkey, 2)) {
            logthis("plarti:querried first planet get fpos");
            fpos = dfaAbs();
        }
        tocks    = 0;
        sectcnt  = 0;
        wormcnt  = 0;
        plntcnt  = 0;
        plntpop  = 0;
        firstime = TRUE;
    }

    // if we still do not have any records go no further
    if (fpos == 0) {
        rtkick(plantime, plarti);
        return;
    }

    tic      = 0;
    not_done = TRUE;

    logthis("plarti:get absolute planet record");
    dfaGetAbs(&planet, fpos, 2);

    do {
        tic++;
        if (tic > MAXTIC) {
            // must get the current record to mark the spot
            dfaAbsRec(&planet, 2);
            fpos = dfaAbs();
            logthis("plarti:hit max tic - break do loop");
            break;
        }

        if (firstime == TRUE || dfaQueryNX()) {
            if (firstime == TRUE)
                logthis("First time through plarti do loop");

            firstime = FALSE;
            logthis("plarti: got next record");

            plnt_type = (SHORT)(gebb2->key[0]);
            if (plnt_type == SECTYPE_NORMAL) {
                logthis("plarti:found sector record");
                ++sectcnt;
            } else if (plnt_type == PLTYPE_PLNT) {
                dfaAbsRec(&planet, 2);
                fpos     = dfaAbs();
                not_done = FALSE;
                logthis("plarti:found planet record");
                ++plntcnt;
            } else if (plnt_type == PLTYPE_WORM) {
                logthis("plarti:found wormhole record");
                ++wormcnt;
            } else {
                logthis("GE:ERR:Bad Plt Type in Db");
            }

        } else {
            // hit end of file - recalibrate tock

            logthis("plarti:EOF hit - recalibrate");
            ftocktime = ((DOUBLE)(tocks * plantime)) + 1.0;
            minutes   = (USHORT)(ftocktime / 60);
            ftockfact = ((DOUBLE)plantock) / ftocktime;
            ftocktime = ((DOUBLE)plantime * ftockfact);

            // no smaller than every 3 seconds
            if (ftocktime < 3.0) {
                geshocst(1, "ELWGE:INF:plarti:recalb tic forced to 3");
                plantime = 3;
            } else
                plantime = (SHORT)ftocktime;

            tocks = 0;
            fpos  = 0;

            geshocst(1, spr("ELWGE:INF:plarti:recalib t=%d", plantime));
            geshocst(1, spr("ELWGE:INF:plarti:pass took %d", minutes));
            geshocst(1, spr("ELWGE:INF:plarti:# sectors %d", sectcnt));
            geshocst(1, spr("ELWGE:INF:plarti:# wormholes %d", wormcnt));
            geshocst(1, spr("ELWGE:INF:plarti:# plnts tot %d", plntcnt));
            geshocst(1, spr("ELWGE:INF:plarti:# plnts pop %d", plntpop));

#ifdef FASTPLANET
            geshocst(1, "ELWGE:INF:FASTPLANET Override set to 5");
            plantime = 5;
#endif

            break;
        }
    } while (not_done);

    flag = 0;
    if (fpos != 0 && tic <= MAXTIC) {
        logthis("got a planet record");
        plptr = &planet;
        if (plptr->items[0].qty > 0 &&
            plptr->userid[0] != 0) { // any men on planet?
            logthis("and it has a population");
            ++plntpop;
            logthis("calling multiply");
            multiply();
            logthis("calling checkspy");
            check_spy();
            logthis("back from checkspy");
            dfaSetBlk(gebb2);
            setmbk(gemb);
            flag = 1;
        }
    }

    if (planet.xsect == 0 && planet.ysect == 0 && planet.plnum == 1) {
        logthis("Updating Zygor");

        for (i = 0; i < NUMITEMS; ++i) {
            planet.items[i].qty  = 1032000L;
            planet.items[i].sell = 'Y';
            planet.items[i].markup2a = (baseprice[i] * 2) + (gernd() % baseprice[i]);
        }

        flag = 1;
    }

    if (planet.xsect == 0 && planet.ysect == 0 && planet.plnum == 2) {
        logthis("Updating T-station");
        planet.items[I_TROOPS].qty  = 1032000L; 
        planet.items[I_TROOPS].sell = 'Y';
        planet.items[I_TROOPS].markup2a = (baseprice[I_TROOPS] * 2) + (gernd() % baseprice[I_TROOPS]);

        planet.items[I_MEN].qty  = 1032000L;
        planet.items[I_MEN].sell = 'Y';
        planet.items[I_MEN].markup2a = (baseprice[I_MEN] * 2) + (gernd() % baseprice[I_MEN]);

        planet.items[I_FOOD].qty  = 1032000L;
        planet.items[I_FOOD].sell = 'Y';
        planet.items[I_FOOD].markup2a = (baseprice[I_FOOD] * 2) + (gernd() % baseprice[I_FOOD]);
        flag = 1;
    }

    if (flag == 1) {
        logthis("plarti:changes made to planet - update it");
        gesdb(GEUPDATE, (PKEY *)&planet, (GALSECT *)&planet);
    }

    rtkick(plantime, plarti);

    return;
}

#ifdef BLDPLNTS
VOID FUNC plabld(VOID)
{
    COORD temp;

    logthis("plabld entered");
    temp.xcoord = rndm((DOUBLE)univmax * 2) - (DOUBLE)univmax;
    temp.ycoord = rndm((DOUBLE)univmax * 2) - (DOUBLE)univmax;
    getsector(&temp);
    rtkick(10, plabld);
}
#endif

// Realtime kicker (rtkick) routines
VOID FUNC warrti(VOID)
{
    SHORT   zothusn; // general purpose other-user channel number
    WARSHP *wptr;
    SHORT   cntr;

    logthis("TICK:Warrtia entered");

    cntr = 0;

#ifdef LOG
    printf("warrti kick\r");
#endif

    checkmines(); // check for mines

    for (zothusn = 0; zothusn < nships; zothusn++) {
        wptr = warshpoff(zothusn);
        if (ingegame(zothusn)) {
            logthis(spr("Chk Shp Stat %s", wptr->userid));
            dfaSetBlk(gebb1);
            setmbk(gemb);
            if (wptr->status == GESTAT_USER)
                ++cntr;
            fluxstat(wptr, zothusn);
            repairship(wptr, zothusn);
            shieldstat(wptr, zothusn);
            cloakstat(wptr, zothusn);

            /*DEBUG
                geshocst(0,spr("ELWGE:Chn %d checktm",zothusn));
                */

            checktm(wptr, zothusn); // check torps, missl, and decoys
            fireion(wptr, zothusn);
            recharge(wptr, zothusn);
            checkdam(wptr, zothusn);
        }
    }
    numwar = cntr;

    rtkick(TICKTIME, warrti);
}

// Realtime kicker (rtkick) routine for all automatons
VOID FUNC autorti(VOID)
{
    SHORT        zothusn; // general purpose other-user channel number
    WARSHP      *wptr;
    static SHORT ticktock1 = 0;
    static SHORT ticktock2 = 0;
    SHORT        count, class;
    SHORT        i, j, clscnt;

    logthis("TICK:autorti entered");
    setmbk(gemb);
    dfaSetBlk(gebb1);

    // 12/19/91 spread out disk I/O over more time

    if (ticktock1 == 0)
        ticktock1 = (SHORT)nterms;

    ++ticktock2;

    logthis(spr("ticktock1 = %d -- ticktock2 = %d", ticktock1, ticktock2));
    if (ticktock2 >= 30 && ticktock1 < nships) {
        logthis("ticktock2 >=30 and ticktock1 < nships");
        logthis("ticktock2 >=30 and ticktock1 < nships");
        logthis("ticktock2 >=30 and ticktock1 < nships");
        logthis("ticktock2 >=30 and ticktock1 < nships");
        logthis("ticktock2 >=30 and ticktock1 < nships");
        wptr    = warshpoff(ticktock1);
        zothusn = ticktock1;

        if (wptr->status == GESTAT_AVAIL) {
            class = -1;
            logthis("Chan Stat = GESTAT_AVAIL");
            // look through the classes for artificial classes
            for (i = 0; i < tot_classes; ++i) {
                if (shipclass[i].max_type == CLASSTYPE_CYBORG ||
                    shipclass[i].max_type == CLASSTYPE_DROID) {
                    // is this class filled
                    clscnt = 0;
                    for (j = 0; j < nships; ++j) {
                        // is this a automatron and of this class
                        if ((warshpoff(j))->status == GESTAT_AUTO &&
                            (warshpoff(j))->shpclass == i) {
                            clscnt++;
                        }
                    }
                    logthis(spr("Class %d -- Count %d", i, clscnt));
                    // is this class full?
                    if (clscnt < shipclass[i].tot_to_create) {
                        // NO - (i) is now set to the class to create
                        class = i;
                        break;
                    }
                }
            }

            /* should also add a random factor to choose a auto class even if
             * the class is full */

            if (gernd() % 100 == 0 || class == -1) {
                logthis("pick random class");
                // look through the classes for artificial classes
                class = -1;
                for (j = 0; j < 200; ++j) {
                    i = gernd() % tot_classes;
                    if (shipclass[i].max_type == CLASSTYPE_CYBORG ||
                        shipclass[i].max_type == CLASSTYPE_DROID) {
                        class = i;
                        break;
                    }
                }
            }

            logthis(spr("picked class - %d", class));

            // initialize the non-user ship areas
            if (class > -1) {
                logthis(spr("Calling init_func 4 cls %d", class));
                logthis(spr("   Name: %s", shipclass[i].typename));

                if (shipclass[class].init_func != NULL)
                    (*(shipclass[class].init_func))(wptr, zothusn, class);
            }
        }

        if (++ticktock1 >= nships)
            ticktock1 = (SHORT)nterms;
        ticktock2 = 0;
    }

    if (cybhaltflg <= 0) {
        cybhaltflg = 0;

        // cybertron loop
        for (count = (SHORT)nterms; count < nships; count++) {
            wptr    = warshpoff(count);
            zothusn = count;

            if (wptr->status == GESTAT_AUTO) {
                if (wptr->tick == 0) {
                    logthis(spr("Calling tick_func 4 usn %d", zothusn));
                    logthis(spr("  Class %d", wptr->shpclass));

                    if (shipclass[wptr->shpclass].tick_func != NULL)
                        (*(shipclass[wptr->shpclass].tick_func))(wptr, zothusn);
                } else {
                    --wptr->tick;
                }
            }
        }
    } else {
        --cybhaltflg;
    }

    logthis("Exiting AUTORTI");

    rtkick(1, autorti);
    return;
}

// Realtime Kicker (rtkick) routine
VOID FUNC warrti2(VOID)
{
    SHORT        zothusn; // general purpose other-user channel number
    WARSHP      *wptr;
    static SHORT clicker = 0;

    logthis("TICK:PWarrti2 entered");
    zothusn = clicker;

    while (zothusn < nships) {
        if (ingegame(zothusn)) {
            wptr = warshpoff(zothusn);
            dfaSetBlk(gebb1);
            setmbk(gemb);
            rotateship(wptr, zothusn);
            accel(wptr, zothusn);
            moveship(wptr, zothusn);
            destruct(wptr, zothusn);
        }
        zothusn += 3;
    }

    clicker = (clicker + 1) % 3;

    rtkick(TICKTIME2, warrti2);
    return;
}

// Realtime Kicker (rtkick) routine
VOID FUNC warrti3(VOID)
{
    COORD tmpcoord = {0};

    logthis("TICK:Warrti3a entered");

    setmbk(gemb);
    prfmsg(ZAPHIM2);

    tmpcoord.xcoord = 0.0;
    tmpcoord.ycoord = 0.0;
    outsect(FILTER, &tmpcoord, 99, 0);
    rtkick(120, warrti3);

    return;
}

/**
 ** Begin Utility Functions
 **/

// outprf except to an automaton
VOID FUNC outprfge(SHORT class, INT shpno)
{
    if (shpno >= 0 && shpno < nterms) {
        if (usroff(shpno)->state == gestt) {
            if (class == ALWAYS) {
                outprf(shpno);
                return;
            } else if (class == FILTER &&
                       (warusroff(shpno)->options[MSG_FILTER] == TRUE)) {
                clrprf();
                return;
            } else {
                outprf(shpno);
                return;
            }
        }
    }

    clrprf();
}

// Outprf to all ships in the sector (except exclude_usrn)
VOID FUNC outsect(SHORT filter, COORD *coordptr, INT exclude_usrn, USHORT freq)
{
    SHORT i;
    INT   zothusn;

    for (zothusn = 0; zothusn < nterms; zothusn++) {
        if (ingegame((SHORT)zothusn) && zothusn != exclude_usrn) {
            if (samesect(&(warshpoff(zothusn)->coord), coordptr)) {
                if (freq == 0) {
                    outprfge(filter, zothusn);
                } else {
                    for (i = 0; i < 3; ++i) {
                        if (freq == warshpoff(zothusn)->freq[i]) {
                            outprfge(filter, zothusn);
                        }
                    }
                }
            }
        }
    }

    clrprf();
}

// Outprf to all ships in scanning range of the sending ship
VOID FUNC outrange(SHORT filter, COORD *coordptr)
{
    DOUBLE  ddist;
    SHORT   zothusn;
    WARSHP *wptr;

    for (zothusn = 0; zothusn < nships; zothusn++) {
        wptr = warshpoff(zothusn);
        if (ingegame(zothusn) &&
            shipclass[wptr->shpclass].max_type == CLASSTYPE_USER) {
            ddist = cdistance(coordptr, &wptr->coord);
            ddist *= 10000;
            if (ddist > 1 &&
                ddist < (DOUBLE)shipclass[wptr->shpclass].scanrange)
                outprfge(filter, zothusn);
        }
    }

    clrprf();
}

// Check if a ship/user is in the game
SHORT FUNC ingegame(SHORT shpno)
{
    if (shpno >= nships || shpno < 0)
        return (FALSE);

    if (shpno < nterms)
        if (usroff(shpno)->state == gestt && usroff(shpno)->substt >= FIGHTSUB)
            return (TRUE);

    if (shpno >= nterms && warshpoff(shpno)->status == GESTAT_AUTO)
        return (TRUE); // automatons are always here!

    return (FALSE);
}

// System console audit trail output
VOID FUNC geshocst(SHORT opt, CHAR *str)
{
    CHAR tmpbuf[40];

    // kill warning
    str = str;

    // Always display
    if (opt == 0) {
        tmp_usrnum = (SHORT)usrnum;
        usrnum     = -1;
        strncpy(tmpbuf, str, 32);
        tmpbuf[31] = '\0'; // NULL;
        shocst(tmpbuf, str);
        usrnum = tmp_usrnum;
    } else if (opt <= showopt) {
        tmp_usrnum = (SHORT)usrnum;
        usrnum     = -1;
        strncpy(tmpbuf, str, 32);
        tmpbuf[31] = '\0'; // NULL;
        shocst(tmpbuf, str);
        usrnum = tmp_usrnum;
    }
    if (logflag)
        logthis(spr("CON: %s", str));
}

/****************************************************************************
 * The following mnu functions are response handlers for input from the
 * player while in a particular state. Each of these then typically results
 * in additional menus/messages being displayed to the player and the
 * players state (substt) modified.
 ****************************************************************************/

// Display main menu of game - first menu of the game
SHORT FUNC mnu_main(VOID)
{
    prfmsg(INTRO, VERSION);
    disp_main_menu();
    outprfge(ALWAYS, usrnum);
    usrptr->substt = 1;
    return (1);
}

// Player selected an option from the GE main menu
SHORT FUNC mnu_main_ans(VOID)
{
    if (margc == 0) {
        prfmsg(REPRMT);
        outprfge(ALWAYS, usrnum);
        return (1);
    } else if (margc == 1) {
        if (sameas(input, "P")) {
            if (!hasmkey(PLAYKEY)) {
                prfmsg(FORPLAY);
                outprfge(ALWAYS, usrnum);
                prfmsg(REPRMT);
                outprfge(ALWAYS, usrnum);
            } else {
                if (numwar < gemaxplrs) {
                    lookupshp();
                    clrprf();
                    return (1);
                } else {
                    prfmsg(NOSHPS);
                    outprfge(ALWAYS, usrnum);
                    prfmsg(REPRMT);
                    outprfge(ALWAYS, usrnum);
                    return (1);
                }
            }
        } else if (sameas(input, "G")) {
            prfmsg(EXPLAIN);
            outprfge(ALWAYS, usrnum);
            prfmsg(REPRMT);
            outprfge(ALWAYS, usrnum);
            return (1);
        } else if (sameas(input, "R")) {
            cmd_geroster();
            prfmsg(REPRMT);
            outprfge(ALWAYS, usrnum);
            return (1);
        } else if (sameas(input, "M")) {
            disp_menu_d();
            outprfge(ALWAYS, usrnum);
            return (1);
        } else if (sameas(input, "5")) {
            mailread("@@sysstat1", MAIL_CLASS_GAMESTATS);
            prfmsg(REPRMT);
            outprfge(ALWAYS, usrnum);
            return (1);
        } else if (sameas(input, "I")) {
            prfmsg(COINFO);
            outprfge(ALWAYS, usrnum);
            return (1);
        } else if (sameas(input, "?")) {
            disp_main_menu();
            outprfge(ALWAYS, usrnum);
            return (1);
        } else if (sameas(input, "x")) {
            prfmsg(EXIWAR);
            outprfge(ALWAYS, usrnum);
            btupmt(usrnum, 0);
            return (0);
        } else if (optmenu) {
            input[0] = (CHAR)toupper(input[0]);
            if (input[0] == optchr) {
                optdisp();
                return (1);
            }
        }
        disp_main_menu();
        outprfge(ALWAYS, usrnum);
        return (1);
    }

    return (1);
}

// Player is in game and entered a command
SHORT FUNC mnu_fightsub(VOID)
{
    if (sameas(input, "x")) {
        if (warsptr->cantexit == 0) {
            cleartm(usrnum);
            gepdb(GEUPDATE, warsptr->userid, warsptr->shipno, warsptr);
            geudb(GEUPDATE, waruptr->userid, waruptr);
            disp_main_menu();
            outprfge(ALWAYS, usrnum);
            prfmsg(EXIWAR2, warsptr->shipname);
            outsect(ALWAYS, &warsptr->coord, (USHORT)usrnum, 0);
            numwar         = 0;
            usrptr->substt = 1;
            btupmt(usrnum, 0);
            warsptr->status = GESTAT_AVAIL;
        } else {
            prfmsg(CANTEXT);
            outprfge(ALWAYS, usrnum);
        }
    } else {
        if (margc > 0)
            gwar();
        else {
            prfmsg(HUH);
            outprfge(ALWAYS, usrnum);
        }
    }

    return (1);
}

/**
 ** player has asked to admin a planet they do not own, and has been prompted
 ** to respond with yes or no to the question "do you wish to claim this
 *planet".
 **/
SHORT FUNC mnu_admenu1(VOID)
{
    SHORT i;

    if (margc > 0) {
        if (genearas("y", margv[0])) {
            plnum = warsptr->where - 10;

            getplanetdat(usrnum);

            strncpy(plptr->userid, warsptr->userid, UIDSIZ);
            ++waruptr->planets;

            for (i = 0; i < NUMITEMS; ++i)
                plptr->items[i].rate = 0;

            plptr->items[I_MEN].rate  = 50;
            plptr->items[I_FOOD].rate = 50;

            setsect(warsptr); // build PKEY
            gesdb(GEUPDATE, &pkey, &sector);

            pkey.plnum = plnum;
            gesdb(GEUPDATE, (PKEY *)&pkey, (GALSECT *)&planet);

            prfmsg(ADMENU1A);
            outprfge(ALWAYS, usrnum);
            usrptr->substt = ADMENU1A;
        } else if (genearas("n", margv[0])) {
            prfmsg(ADMIN3);
            outprfge(ALWAYS, usrnum);
            usrptr->substt = FIGHTSUB;
        }
    } else {
        prfmsg(ADMENU1);
        outprfge(ALWAYS, usrnum);
    }

    return (1);
}

// Player asked to name a new planet and should respond with string
SHORT mnu_admenu1a(VOID)
{

    if (margc > 0) {
        plnum = warsptr->where - 10;

        getplanetdat(usrnum);
        rstrin();

        *margv[0] = (CHAR)toupper(*margv[0]);
        strncpy(plptr->name, margv[0], 19);
        plptr->name[19] = 0;

        setsect(warsptr); // build PKEY
        pkey.plnum = plnum;
        gesdb(GEUPDATE, (PKEY *)&pkey, (GALSECT *)&planet);

        prfmsg(ADMENU1B, plnum, plptr->name, warsptr->userid,
               warsptr->shipname);

        prfmsg(ADMENU2);
        outprfge(ALWAYS, usrnum);
        usrptr->substt = ADMENU2;
    } else {
        prfmsg(ADMENU1A);
        outprfge(ALWAYS, usrnum);
    }

    return (1);
}

// Player was displayed the admin menu and should've selected an option
SHORT FUNC mnu_admenu2(VOID)
{
    SHORT i;

    if (margc > 0) {
        if (*margv[0] >= '1' && *margv[0] <= '7') {
            plnum = warsptr->where - 10;

            getplanetdat(usrnum);

            switch (*margv[0]) {
            case '1':
                prfmsg(ADMIN01, plptr->name);
                prfmsg(DASHES);
                prfmsg(ADMIN02);
                for (i = 0; i < NUMITEMS; ++i) {
                    sprintf(gechrbuf, "%-11s %5u %5ld %5u %5u %1c %5ld",
                            item_name[i], plptr->items[i].rate,
                            plptr->items[i].qty, plptr->items[i].markup2a,
                            plptr->items[i].reserve, plptr->items[i].sell,
                            plptr->items[i].sold2a);
                    prf("%s\r", gechrbuf);
                }
                prfmsg(DASHES);
                sprintf(gechrbuf, "%ld", plptr->cash);
                prfmsg(ADMIN04, gechrbuf);
                sprintf(gechrbuf, "%ld", plptr->tax);
                prfmsg(ADMIN04A, gechrbuf);
                prfmsg(ADMIN04B, plptr->taxrate);
                // prfmsg(ADMIN05,plptr->warnings);
                prfmsg(ADMIN06, plptr->password);
                prfmsg(DASHES);
                prfmsg(PRESSKEY);
                usrptr->substt = ADMENU2;
                break;

            case '2':
                prfmsg(ADMENU2B);
                usrptr->substt = ADMENU2B;
                break;

            case '3':
                prfmsg(ADMENU2E);
                usrptr->substt = ADMENU2E;
                break;

            case '4':
                prfmsg(ADMENU2G);
                usrptr->substt = ADMENU1A;
                break;

            case '5':
                prfmsg(ADMENU2H);
                usrptr->substt = ADMENU2H;
                break;

            case '6':
                prfmsg(ADMENU2I);
                usrptr->substt = ADMENU2I;
                break;

            case '7':
                prfmsg(ADMENU2J);
                usrptr->substt = ADMENU2J;
                break;
            }

            outprfge(ALWAYS, usrnum);
        } else if (sameas(input, "x")) {
            prfmsg(ADMIN3);
            outprfge(ALWAYS, usrnum);
            usrptr->substt = FIGHTSUB;
        } else {
            prfmsg(ADMENU2);
            outprfge(ALWAYS, usrnum);
        }
    } else {
        prfmsg(ADMENU2);
        outprfge(ALWAYS, usrnum);
    }

    return (1);
}

// Player asked to transfer cash from the planet and was prompted to enter the
// amount
SHORT FUNC mnu_admenu2b(VOID)
{
    ULONG amt;

    plnum = warsptr->where - 10;

    getplanetdat(usrnum);
    amt = atol(margv[0]);

    if (amt <= plptr->tax) {
        sprintf(gechrbuf, "%ld", amt);
        prfmsg(ADMENU2C, gechrbuf);
        outprfge(ALWAYS, usrnum);
        waruptr->cash += amt;
        plptr->tax -= amt;
        setsect(warsptr); // build PKEY
        pkey.plnum = plnum;
        gesdb(GEUPDATE, (PKEY *)&pkey, (GALSECT *)&planet);
    } else {
        prfmsg(ADMENU2D);
        outprfge(ALWAYS, usrnum);
    }
    prfmsg(ADMENU2);
    usrptr->substt = ADMENU2;

    return (1);
}

// Player asked to modify parameters on an item, asked to input name of item
SHORT FUNC mnu_admenu2e(VOID)
{
    SHORT i;

    for (i = 0; i < NUMITEMS; ++i) { // skip notused
        if (genearas(kwrd[i], margv[0])) {
            warsptr->titem = i;
            prfmsg(ADMEN2F1, item_name[warsptr->titem]);
            outprfge(ALWAYS, usrnum);
            usrptr->substt = ADMEN2F1;
            return (1);
        }
    }
    usrptr->substt = ADMENU2;
    prfmsg(ADMENU2);
    outprfge(ALWAYS, usrnum);

    return (1);
}

/**
 ** player selected an item, was prompted to select the percent of effort
 ** on his selected item and should have responded with a percent.
 **/
SHORT FUNC mnu_admenu2f1(VOID)
{
    USHORT amt;

    amt = (USHORT)atoi(margv[0]);

    if (margc == 1 && amt <= 100) {
        titems[usrnum].rate = amt;
        prfmsg(ADMEN2F2, item_name[warsptr->titem]);
        outprfge(ALWAYS, usrnum);
        usrptr->substt = ADMEN2F2;
        return (1);
    }
    prfmsg(ADMEN2F1, item_name[warsptr->titem]);
    outprfge(ALWAYS, usrnum);

    return (1);
}

// Player was asked how much to charge for his selected item
SHORT FUNC mnu_admenu2f2(VOID)
{
    USHORT amt;

    amt = (USHORT)atoi(margv[0]);

    if (margc == 1 && amt <= 32000) {
        titems[usrnum].markup2a = amt;
        prfmsg(ADMEN2F3, item_name[warsptr->titem]);
        outprfge(ALWAYS, usrnum);
        usrptr->substt = ADMEN2F3;

        return (1);
    }

    prfmsg(ADMEN2F2, item_name[warsptr->titem]);
    outprfge(ALWAYS, usrnum);

    return (1);
}

// Player was asked if s/he wished to sell this item to other ships
SHORT FUNC mnu_admenu2f3(VOID)
{

    if (margc == 1 && (genearas("y", margv[0]) || genearas("n", margv[0]))) {
        titems[usrnum].sell = (CHAR)toupper(*margv[0]);
        prfmsg(ADMEN2F4, item_name[warsptr->titem]);
        outprfge(ALWAYS, usrnum);
        usrptr->substt = ADMEN2F4;
        return (1);
    }
    prfmsg(ADMEN2F3, item_name[warsptr->titem]);
    outprfge(ALWAYS, usrnum);
    return (1);
}

// Player was asked hom much to reserve for stockpiling
SHORT FUNC mnu_admenu2f4(VOID)
{
    USHORT amt;

    amt = (USHORT)atoi(margv[0]);

    if (margc == 1 && amt <= 32000) {
        titems[usrnum].reserve = amt;
        update_items();
        prfmsg(ADMENU2);
        outprfge(ALWAYS, usrnum);
        usrptr->substt = ADMENU2;
        return (1);
    }
    prfmsg(ADMEN2F4, item_name[warsptr->titem]);
    outprfge(ALWAYS, usrnum);
    return (1);
}

// Player was asked how much to set taxes at
SHORT FUNC mnu_admenu2h(VOID)
{
    USHORT amt;

    amt = (USHORT)atoi(margv[0]);

    if (margc == 1 && amt <= 100) {
        plnum = warsptr->where - 10;
        getplanetdat(usrnum);

        plptr->taxrate = amt;

        setsect(warsptr); // build PKEY
        pkey.plnum = plnum;
        gesdb(GEUPDATE, (PKEY *)&pkey, (GALSECT *)&planet);

        prfmsg(ADMENU2);
        outprfge(ALWAYS, usrnum);
        usrptr->substt = ADMENU2;
        return (1);
    }
    prfmsg(ADMENU2H);
    outprfge(ALWAYS, usrnum);
    return (1);
}

// Player was asked to specify the trade password for his/her planet
SHORT FUNC mnu_admenu2i(VOID)
{

    if (margc == 1) {
        plnum = warsptr->where - 10;
        getplanetdat(usrnum);

        strncpy(plptr->password, margv[0], 9);
        plptr->password[9] = '\0'; // RH was plptr->password[10]=0 but that's an overrun

        setsect(warsptr); // build PKEY
        pkey.plnum = plnum;
        gesdb(GEUPDATE, (PKEY *)&pkey, (GALSECT *)&planet);

        if (sameas(plptr->password, "none")) {
            prfmsg(ADMEN2I2);
            plptr->teamcode = 0;
        } else if (sameas(plptr->password, "team")) {
            if (waruptr->teamcode > 0) {
                plptr->teamcode = waruptr->teamcode;
                prfmsg(ADMEN2I3);
            } else {
                plptr->teamcode    = 0;
                plptr->password[0] = 0;
                prfmsg(ADMEN2I4);
            }
        } else {
            prfmsg(ADMEN2I1, plptr->password);
            plptr->teamcode = 0;
        }

        prfmsg(ADMENU2);
        outprfge(ALWAYS, usrnum);
        usrptr->substt = ADMENU2;
        return (1);
    }
    prfmsg(ADMENU2I);
    outprfge(ALWAYS, usrnum);
    return (1);
}

// Player asked to input beacon message and should've responded with a string
SHORT FUNC mnu_admenu2j(VOID)
{

    plnum = warsptr->where - 10;

    getplanetdat(usrnum);

    if (margc == 0) {
        plptr->beacon[0] = 0;
        prfmsg(ADMEN2J1);
    } else {
        rstrin();
        strncpy(plptr->beacon, margv[0], BEACONMSGSZ);
        plptr->beacon[BEACONMSGSZ - 1] = 0;
        prfmsg(ADMEN2J2);
    }

    setsect(warsptr); // build PKEY
    pkey.plnum = plnum;
    gesdb(GEUPDATE, (PKEY *)&pkey, (GALSECT *)&planet);

    prfmsg(ADMENU2);
    outprfge(ALWAYS, usrnum);
    usrptr->substt = ADMENU2;
    return (1);
}

// Player selected item 1 from the main menu
SHORT FUNC mnu_choosesh(VOID)
{
    selectship();
    return (1);
}

/**
 ** Player selected read messages from main menu, was displayed the mail
 ** sub-menu, and was asked to select an option
 **/
SHORT mnu_menug(VOID)
{
    if (margc > 0) {
        switch (tolower(*margv[0])) {

        case '1':

            if (mailread(usaptr->userid, MAIL_CLASS_DISTRESS)) {
                prfmsg(usrptr->substt = MENUG1);
                outprfge(ALWAYS, usrnum);
            } else {
                disp_menu_d();
                outprfge(ALWAYS, usrnum);
            }
            break;

        case '2':

            if (mailread(usaptr->userid, MAIL_CLASS_PRODRPT)) {
                prfmsg(usrptr->substt = MENUG2);
                outprfge(ALWAYS, usrnum);
            } else {
                disp_menu_d();
                outprfge(ALWAYS, usrnum);
            }
            break;

        case 'x':

            disp_main_menu();
            outprfge(ALWAYS, usrnum);
            usrptr->substt = 1;
            break;

        default:

            disp_menu_d();
            outprfge(ALWAYS, usrnum);
            break;
        }
    } else {
        disp_menu_d();
        outprfge(ALWAYS, usrnum);
    }

    return (1);
}

/**
 ** Player selected option 1 on the mail sub-menu and
 ** player was displayed the first message in the mail file and has been asked
 ** to press N for next or X to exit.
 **/
SHORT FUNC mnu_menug1(VOID)
{

    if (margc > 0) {
        switch (tolower(*margv[0])) {

        case 'n':

            if (mailread(usaptr->userid, MAIL_CLASS_DISTRESS)) {
                prfmsg(usrptr->substt = MENUG1);
                outprfge(ALWAYS, usrnum);
            } else {
                disp_menu_d();
                outprfge(ALWAYS, usrnum);
            }
            break;

        case 'x':

            disp_menu_d();
            outprfge(ALWAYS, usrnum);
            break;

        default:

            prfmsg(MENUG1);
            outprfge(ALWAYS, usrnum);
            break;
        }
    } else {
        prfmsg(MENUG1);
        outprfge(ALWAYS, usrnum);
    }

    return (1);
}

/**
 ** Player selected option 2 on the mail sub-menu and
 ** player was displayed the first message in the mail file and has been asked
 ** to press N for next or X to exit.
 **/
SHORT FUNC mnu_menug2(VOID)
{
    if (margc > 0) {
        switch (tolower(*margv[0])) {

        case 'n':

            if (mailread(usaptr->userid, MAIL_CLASS_PRODRPT)) {
                prfmsg(usrptr->substt = MENUG2);
                outprfge(ALWAYS, usrnum);
            } else {
                disp_menu_d();
                outprfge(ALWAYS, usrnum);
            }
            break;

        case 'x':

            disp_menu_d();
            outprfge(ALWAYS, usrnum);
            break;

        default:

            prfmsg(MENUG2);
            outprfge(ALWAYS, usrnum);
            break;
        }
    } else {
        prfmsg(MENUG2);
        outprfge(ALWAYS, usrnum);
    }

    return (1);
}

// (Re-)Displays the main menu
VOID FUNC disp_main_menu(VOID)
{
    prfmsg(MENUA);

    if (usegemsg) {
        if (mailscan(usaptr->userid, 0))
            prfmsg(MENUB2);
        else
            prfmsg(MENUB1);
    }

    if (optmenu)
        prf("\r   %c ... %s", optchr, opttxt);

    prfmsg(MENUC);
}

// (Re-)Displays the mail sub-menu
VOID FUNC disp_menu_d(VOID)
{

    prfmsg(MENUD);
    if (mailscan(usaptr->userid, MAIL_CLASS_DISTRESS))
        prfmsg(MENUE2);
    else
        prfmsg(MENUE1);

    if (mailscan(usaptr->userid, MAIL_CLASS_PRODRPT))
        prfmsg(MENUF2);
    else
        prfmsg(MENUF1);

    prfmsg(usrptr->substt = MENUG);
}

VOID FUNC update_items(VOID)
{
    SHORT i, pcnt = 0;

    plnum = warsptr->where - 10;

    getplanetdat(usrnum);

    for (i = 0; i < NUMITEMS; ++i)
        if (i != warsptr->titem)
            pcnt += plptr->items[i].rate;

    if ((titems[usrnum].rate + pcnt) > 100) {
        titems[usrnum].rate = 100 - pcnt;
        if (titems[usrnum].rate > 100)
            titems[usrnum].rate = 0;

        prfmsg(ADMEN2FA, item_name[warsptr->titem], titems[usrnum].rate);
        outprfge(ALWAYS, usrnum);
    }

    i = (titems[usrnum].rate + pcnt);
    if (i < 100) {
        i = 100 - i;
        prfmsg(ADMEN2FB, i);
        outprfge(ALWAYS, usrnum);
    }
    plptr->items[warsptr->titem].rate     = titems[usrnum].rate;
    plptr->items[warsptr->titem].sell     = titems[usrnum].sell;
    plptr->items[warsptr->titem].reserve  = titems[usrnum].reserve;
    plptr->items[warsptr->titem].markup2a = titems[usrnum].markup2a;
    setsect(warsptr); // build PKEY
    pkey.plnum = plnum;
    gesdb(GEUPDATE, (PKEY *)&pkey, (GALSECT *)&planet);
}

VOID FUNC optdisp(VOID)
{
    static FILE *hdl = (FILE *)0;

    if (hdl == (FILE *)0) {
        hdl = fopen("elwgemnu.txt", "rt");
        if (hdl == (FILE *)0)
            geshocst(0, "ELWGE:ERR ELWGEMNU.TXT Open Failed");
        else
            logthis("optdisp: elwgemnu.txt opened");
    }

    if (hdl != (FILE *)0) {
        if (fseek(hdl, opttbl[usrnum], 0) == 0) {
            if (fgets(gechrbuf, 85, hdl) != NULL) {
                logthis(gechrbuf);
                prf(gechrbuf);
                outprfge(ALWAYS, usrnum);
            } else {
                opttbl[usrnum] = 0;
                logthis("optdisp: hit eof - fpos set back to 0");
                usrptr->substt = OPTDISP2;
                btuinj(usrnum, CYCLE);
                return;
            }
            opttbl[usrnum] = ftell(hdl);
            usrptr->substt = OPTDISP;
            btuinj(usrnum, CYCLE);
        } else {
            logthis(spr("optdisp: seek error fpos = %ld", opttbl[usrnum]));
            prf("\r*** Seek Err in text file ***\r");
            outprfge(ALWAYS, usrnum);
        }
    } else {
        prf("*** FILE MISSING - Notify Sysop!! ***");
        outprfge(ALWAYS, usrnum);
    }

    return;
}
