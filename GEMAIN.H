/*****************************************************************************
 *                                                                           *
 *   GEMAIN.H                                                                *
 *                                                                           *
 *   Galactic Empire Structs and Defines                                     *
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

#include "gcomm.h"
#include "majorbbs.h"
#include "gme.h"
#include "elwge.h"
#include "elwgehlp.h"
#include "elwgeshp.h"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define NEUTRAL_X 0
#define NEUTRAL_Y 0

#define ROTENGUSE 30 /* Energy used to rotate                     */

#define ACCENGAMT 120 /* Acceleration energy usage                 */
#define MOVENGMIN 3000
#define MOVENGUSE 10

#define PRELOAD  10 /* Phaser reload rate / tick                 */
#define PMINFIRE 60 /* Minimum amount to fire phasor             */
#define PENGUSE  57 /* Amount of energy used by phasers charging */
#define PHABIAS   2 /* Degrees to bias "spread" to better aim    */

#define HPMINFIR 6000 /* Minimum energy in flux to fire Hyper phasr*/
#define HPFIRAMT 5000
#define HPBEAMW  5

#define ENGYMAX  65000U /* Maximum energy one can have               */
#define ENGRECHG 1      /* Energy recharge amount / tick             */
#define ENGYMIN  5000   /* Minimum Energy in FLUX to autoload        */

#define SHMINPWR 200  /* Minimum power fore shields                */
#define SHENGUSE 100  /* Energy used every tick for shields        */
#define SHMINCHG 5    /* Minimum charge to be effective            */

#define TONFACT 15000.0 /* divisor for ton factor */

#define SSMAX 10000

#define PI 3.1415927

#define GELOOKUP 1
#define GEADD 2
#define GEDELETE 3
#define GEUPDATE 4
#define GEGET 5
#define GENEXT 6
#define GELOOKUPNAME 7
#define GEGETNOW 8

#define MAXPLANETS 9

#define MAXX 30
#define MAXY 15

#define MAXTORPS 3  /* max # of Torpedoes */
#define MAXMISSL 3  /* max # of Missiles  */
#define MAXDECOY 10 /* max # of Decoys    */

#define DECOYTIME 15  /* times TICKTIME how long a decoy lives */
#define TICKTIME 6    /* how often does RTKICK fire    6   */
#define TICKTIME2 1   /* how often does RTKICK fire       */
#define CYBTICKTIME 6 /* Cyborgs do stuff this fast       */
#define PLANTIME 55   /* how often do planets get updated */

#define FIRETICKS 10  /* how many ticks to hold a player when involved in a battle */

#define NUMITEMS 14

#define I_MEN 0
#define I_MISSILE 1
#define I_TORPEDO 2
#define I_IONCANNON 3
#define I_FLUXPOD 4
#define I_FOOD 5
#define I_FIGHTER 6
#define I_DECOYS 7
#define I_TROOPS 8
#define I_ZIPPERS 9
#define I_JAMMERS 10
#define I_MINE 11
#define I_GOLD 12
#define I_SPY 13

#define SHIELDUP 1
#define SHIELDDN 2
#define SHIELDDM 3

#define SHIELD_FACTOR 4

#define COUNTDOWN 20 /* self destruct countdown */

#define CYBSLO       3        /* slows down the cyborgs reaction time       */
#define CYB_ALLOW    35       /* how much allowance per tick                */
#define CYB_MAXCASH  2000000L /* maximum cash cybertrons can hold           */
#define CYB_BE_NICE  30       /* 30 kills grace before we get tough         */
#define CYB_BE_EASY  60       /* 60 kills till we get really mean           */
#define CYB_BREAKOFF 500      /* 1 in x chance that cyb will break off attk */
#define CYB_MINDAM   75       /* % of damage when cyb jams and mines        */

#define TELEDAM 17 /* amount of damage done when edge reached */

#define SHOWDOC 1  /* Show (0 for not) captured planet map to victor */
#define RNDDOC  6  /* Odds of planet map being captured */

#define MINERANGE 10000 /* maximum Mine range                     */

#define DESTRUCTRANGE 10000

#define TOPSHIELD 19
#define TOPPHASER 19

#define SECTYPE_NORMAL 1
#define PLTYPE_PLNT 2
#define PLTYPE_WORM 3

#define GESTAT_AVAIL 0
#define GESTAT_USER 1
#define GESTAT_AUTO 2

#define CLASSTYPE_USER 1
#define CLASSTYPE_CYBORG 2
#define CLASSTYPE_DROID 3
#define CLASSTYPE_NONE 4

#define MAIL_CLASS_DISTRESS 1
#define MAIL_CLASS_PRODRPT 3
#define MAIL_CLASS_GAMESTATS 4

/* ansi display function codes*/

#define HOMEY 1
#define CLEAR 2

/* user options */

#define SCANNAMES 0
#define SCANHOME 1
#define SCANFULL 2
#define MSG_FILTER 3

#define MAXTEAMS 50

#define BEACONMSGSZ 75
#define GEMSGSIZ 784
#define FIXEDMSGSIZ sizeof(struct _gemessage)

#define ALWAYS 1
#define FILTER 2

/*****************************************************************************/
/* End of Defines                                                            */
/*****************************************************************************/

#ifndef byte
#define byte CHAR
#endif

#define COORD struct _coord

COORD
{
    DOUBLE xcoord; // 000-007
    DOUBLE ycoord; // 008-015
}; // size: 16

#define MINE struct _mine
MINE
{
    byte   channel; // 000
#ifndef __BUILDV10MODULE  
    CHAR pack1;
#endif
    // pack 1
    USHORT timer;   // 002-003
#ifndef __BUILDV10MODULE
    CHAR pack2[4];
#endif
    // pack 4
    COORD  coord;   // 008-023
}; // size: 24 (5 packed bytes)

#define MISSILE struct _missile
MISSILE
{
    CHAR   channel;  // 000
#ifndef __BUILDV10MODULE  
    CHAR pack1;
#endif
    // pack 1
    USHORT distance; // 002-003
    USHORT energy;   // 004-005
}; // size: 6 (1 packed byte)

#define TORPEDO struct _torpedo
TORPEDO
{
    CHAR   channel;  // 000
#ifndef __BUILDV10MODULE
    CHAR pack1;
#endif
    // pack 1
    USHORT distance; // 002-003
}; // size: 4 (1 packed byte)

// ELWGEUSR.DAT
#define WARUSR struct warusr
WARUSR
{                            /* user data blocks                          */
    CHAR userid[UIDSIZ];     /* userid of player                  KEY0    */ // 000-029
    byte options[30];        /*                                           */ // 030-059
    ULONG score;             /*   Your score               31     KEY1    */ // 060-063
    ULONG cash;              /*   cash on hand                            */ // 064-067
    ULONG debt;              /*   I owe, I owe, its off to work I go      */ // 068-071
    ULONG plscore;           /*                                           */ // 072-075
    ULONG klscore;           /*                                           */ // 076-079
    ULONG population;        /*                                           */ // 080-083
    ULONG teamcode;          /*                                   KEY2    */ // 084-087
    USHORT noships;          /*   Number of ships                         */ // 088-089
    SHORT  topshipno;        /*   Top ship number used                    */ // 090-091
    SHORT  kills;            /*   No kills                                */ // 092-093
    USHORT rospos;           /*                                           */ // 094-095
    SHORT  planets;          /*   Planets owned                           */ // 096-097
    CHAR   filler[256 - 98];                                                 // 098-255
}; // size 256, 98 before filler

// ELWGESHP.DAT
#define WARSHP struct warshp
WARSHP
{                             /* ship data blocks                          */
    CHAR userid[UIDSIZ];      /* userid of player                    KEY0  */ // 000-029
    SHORT shipno;             /* Ship id number                      KEY1  */ // 030-031
    CHAR  shipname[35];       /* Ship name                                 */ // 032-066
    byte  phasrtype;          /* Type of Phaser system                     */ // 067
    ULONG items[NUMITEMS];    /* all the items                             */ // 068-123 // 14, total 56
    MISSILE lmissl[MAXMISSL]; /* Locked on missiles                        */ // 124-141 // 3, total 18 
    byte  shieldtype;         /* shield type                               */ // 142
    byte  shieldstat;         /* shield status (supposed to be up/dn)      */ // 143
    COORD coord;              /* Coordinates                               */ // 144-159 
    TORPEDO ltorps[MAXTORPS]; /* Locked on torpedoes                       */ // 160-171 // 3, total 12
    byte   status;            /*   Status of this record                   */ // 172
    byte   warncntr;          /*   warning counter                         */ // 173
    byte   tick;              /*   Ship Ticker                             */ // 174
    byte   holdcourse;        /*   Hold course ticker                      */ // 175
    DOUBLE heading;           /*   Heading of this ship                    */ // 176-183
    DOUBLE head2b;            /*   direction rotating to                   */ // 184-191
    DOUBLE speed;             /*   Speed                                   */ // 192-199
    DOUBLE speed2b;           /*   Speed to be                             */ // 200-207
    DOUBLE damage;            /*   Damage on this ship (0 - 100%)          */ // 208-215
    DOUBLE energy;            /*   Amount of energy available              */ // 216-223
    DOUBLE phasr;             /*   Phaser strength (0 - 100%)              */ // 224-231
    SHORT  shpclass;          /*   Ship Class   display                    */ // 232-233
    SHORT  kills;             /*   No kills                                */ // 234-235
    SHORT  lastfired;         /*   usernumber of last user to fire on you  */ // 236-237
    SHORT  shield;            /*   shield charge                           */ // 238-239
    SHORT  cloak;             /*   cloak flag  -1 = up 0 = down + = time   */ // 240-241
    SHORT  degrees;           /*   work fields for calculating degrees     */ // 242-243
    SHORT  percent;           /*   Work field for calculating percentages  */ // 244-245
    SHORT  tactical;          /*   Tactical Display damage indicator       */ // 246-247
    SHORT  helm;              /*   Helm Control damage indicator           */ // 248-249
    SHORT  train;             /*   Training screen counter                 */ // 250-251
    SHORT  where;             /*   Hyperspace flag                         */ // 252-253
    USHORT decout[MAXDECOY];  /*   decoys out                              */ // 254-273 // 10, total 20
    USHORT jammer;            /*   supper time?                            */ // 274-275
    USHORT freq[3];           /*   sub space, hyper space, planetary       */ // 276-281 
    USHORT titem;             /*   establishing a colony mode temp item    */ // 282-283
    USHORT hostile;           /*   being hostile flag                      */ // 284-285
    USHORT cantexit;          /*   cannot exit counter                     */ // 286-287
    USHORT repair;            /*   repairs underway flag                   */ // 288-289
    USHORT hypha;             /*   hyper-phasers cooling flag              */ // 290-291
    USHORT firecntl;          /*   fire-control damage flag                */ // 292-293
    SHORT  lock;              /*   set to the ship we are locked onto      */ // 294-295
    SHORT  topspeed;          /*   top speed                               */ // 296-297
    byte   destruct;          /*   self destruct countdown                 */ // 298
    byte   cybmine;           /*   I have found me a ship to seek          */ // 299
    byte   cybskill;          /*   Cyborg skill level                      */ // 300
    byte   cybupdate;         /*   update cyborg counter                   */ // 301
    byte   emulate;           /*   permits cybertron emulation             */ // 302
    byte   minesnear;         /*   set when mines are near                 */ // 303
    CHAR   filler[512 - 304]; /*   filler to get to 512 for some reason    */ // 304-511
}; // size 512, before filler 304

#define ITEM struct item
ITEM
{                    /* items                                     */
    ULONG  qty;      /* quantity on hand                          */ // 000-003
    ULONG  sold2a;   /* # sold to allies                          */ // 004-007
    USHORT rate;     /* rate of manufacture                       */ // 008-009
    USHORT reserve;  /* qty to reserve                            */ // 010-011
    USHORT markup2a; /* mark up to allies                         */ // 012-013
    CHAR   sell;     /* sell to allies?                           */ // 014
#ifndef __BUILDV10MODULE
    CHAR pack1;
#endif
    // pack 1
}; // size: 16

#define PLNTCOORD struct _plntcoord
PLNTCOORD
{
    SHORT type;  /* planet type                               */ // 000-001
#ifndef __BUILDV10MODULE
    CHAR pack1[6];
#endif
    // pack 6
    COORD coord; /* location                                  */ // 007-023
}; // size is 24

#define PLANETAB struct planetab
PLANETAB
{
    PLNTCOORD planets[MAXPLANETS]; // 000-215
}; // size 216

#define GALSECT struct galsect
GALSECT
{                               /* star system data record                   */
    SHORT xsect;                /* sector x coord                     KEY0   */ // 000-001
    SHORT ysect;                /* sector y coord                     KEY0   */ // 002-003
    SHORT plnum;                /* always 0                           KEY0   */ // 004-005
    SHORT type;                 /* type of sector                     KEY2   */ // 006-007
    SHORT numplan;              /* number of planetary objects               */ // 008-009
#ifndef __BUILDV10MODULE
    CHAR pack1[6];
#endif
    // pack 6, probably unavoidable unless ptab came first
    PLNTCOORD ptab[MAXPLANETS]; /* planet table - plntcoord=24 (pack6); 216  */ // 016-231
    CHAR filler[512 - 232];     /* fill to 512 for some reason               */ // 232-511
}; // size 512, 232 before filler

// ELWGEPLT.DAT
#define GALPLNT struct _galplnt
// even though v3.2 opened the planet data file with galsect's struct, this is
// the right file. the first 4 fields are the same for key0/2, key1 at 25 is
// right for userid -RH 4/2021
GALPLNT
{                             /* star system data record                   */
    SHORT xsect;              /* sector x coord                     KEY0   */ // 000-001
    SHORT ysect;              /* sector y coord                     KEY0   */ // 002-003
    SHORT plnum;              /* planet number                      KEY0   */ // 004-005
    SHORT type;               /* type of sector                     KEY2   */ // 006-007
    COORD coord;              /* coordinates                               */ // 008-023
    CHAR  userid[UIDSIZ];     /* userid                             KEY1   */ // 024-053
    CHAR  name[20];           /* planet name                               */ // 054-073
    CHAR  enviorn;            /* enviornment factor                        */ // 074
    CHAR  resource;           /* resources                                 */ // 075
    ULONG cash;               /* cash on hand                              */ // 076-079
    ULONG debt;               /* amount you owe                            */ // 080-083
    ULONG tax;                /* amount of tax collected                   */ // 084-087
    SHORT taxrate;            /* rate to tax population                    */ // 088-089
    CHAR  warnings;           /* warnings to intruders                     */ // 090
    CHAR  password[10];       /* password for alliance                     */ // 091-100
    CHAR lastattack[UIDSIZ];  /* userid of last attacker                   */ // 101-130
#ifndef __BUILDV10MODULE
    CHAR pack1;
#endif
    // pack 1
    ITEM items[NUMITEMS];     /* items 195 14 items (item size 16)         */ // 132-355
    CHAR beacon[BEACONMSGSZ]; /* beacon message (size 75)                  */ // 356-430
    CHAR spyowner[UIDSIZ];    /* spy owner ?                               */ // 431-460
#ifndef __BUILDV10MODULE
    CHAR pack2;
#endif
    // pack 1
    SHORT technology;         /* technology ?                              */ // 462-463
    ULONG teamcode;           /* teamcode                                  */ // 464-467
    CHAR  filler[512 - 468];  /* fill to 512 for some reason               */ // 468-511
}; // size 512, 468 before filler

#define GALWORM struct galworm
GALWORM
{                   /* star system data record                   */
    SHORT xsect;    /* sector x coord                            */ // 000-001
    SHORT ysect;    /* sector y coord                            */ // 002-003
    SHORT plnum;    /* planet number                             */ // 004-005
    SHORT type;     /* type of sector                            */ // 006-007
    COORD coord;                                                    // 008-023
    COORD destination;                                              // 024-039 
    SHORT visible;  /* is the wormhole visible flag              */ // 040-041
    CHAR  name[20];                                                 // 042-061
    CHAR  filler[512 - 30 - (sizeof(COORD) * 2)];                   // 062-511
}; // size 512, 62 before filler

#define PKEY struct _pkey
PKEY
{                /* star system data record key               */
    SHORT xsect; /* sector x coord                            */ // 000-001
    SHORT ysect; /* sector y coord                            */ // 002-003
    SHORT plnum; /* always 0                                  */ // 004-005
}; // size 6

#define WORMTAB struct _wormtab
WORMTAB
{
    COORD coord; // 000-015
    COORD dest;  // 016-031
}; // size 32

#define MAILKEY struct _mailkey
MAILKEY
{
    CHAR userid[UIDSIZ]; /* UserID UIDSIZ=30                          */ // 000-029
#ifndef __BUILDV10MODULE
    CHAR pack1[2];
#endif
    // pack 2
    LONG class;                                                          // 032-035
}; // size: 36

#define MAIL struct _mail
MAIL
{
    CHAR userid[UIDSIZ]; /* UserID UIDSIZ=30             */ // 000-029
#ifndef __BUILDV10MODULE
    CHAR pack1[2];
#endif
    // pack 2
    LONG class;                                             // 032-035
    SHORT type;                                             // 036-037
    SHORT stamp;                                            // 038-039
    CHAR  dtime[20];                                        // 040-059
    CHAR  topic[30];                                        // 060-089
    CHAR  string1[80];                                      // 090-169 
    CHAR  name1[25];                                        // 170-194
    CHAR  name2[25];                                        // 195-219 
    SHORT int1;                                             // 220-221
    SHORT int2;                                             // 222-223
    SHORT int3;                                             // 224-225
#ifndef __BUILDV10MODULE
    CHAR pack2[2];
#endif
    // pack 2
    LONG  long1;                                            // 228-231 
    LONG  long2;                                            // 232-235
    LONG  long3;                                            // 236-239 
    CHAR  filler[253 - 240];
#ifndef __BUILDV10MODULE
    CHAR pack3[3];
#endif
    // pack 3
}; // size: 256

#define MAILSTAT struct _mailstat
MAILSTAT
{
    CHAR userid[UIDSIZ]; /* UserID UIDSIZ=30             */ // 000-029
#ifndef __BUILDV10MODULE
    CHAR pack1[2];
#endif
    // pack 2
    LONG class;                                             // 032-035
    SHORT type;                                             // 036-037
    SHORT stamp;                                            // 038-039
    CHAR  dtime[20];                                        // 040-059
    CHAR  topic[30];                                        // 060-089
    CHAR  name1[25];                                        // 090-114
#ifndef __BUILDV10MODULE
    CHAR pack2;
#endif
    // pack 1 
    SHORT int1;                                             // 116-117 
    SHORT int2;                                             // 118-119
    ULONG cash;           /* cash on hand                */ // 120-123
    ULONG debt;           /* amount you owe              */ // 124-127
    ULONG tax;            /* amount of tax collected     */ // 128-131
    ULONG itemqty[NUMITEMS]; /* NUMITEMS=14, 14x4 = 56   */ // 132-187
    CHAR filler[253 - 188];                                 /* 188-252 */
#ifndef __BUILDV10MODULE
    CHAR pack3[3];
#endif
    // pack 3
}; // size: 256

#define SHPKEY struct _shpkey
SHPKEY
{
    CHAR  userid[UIDSIZ]; // 000-029
    SHORT shipno;         // 030-032
}; // size 32

#define SHIP struct _ship
SHIP
{
    CHAR *typename;                             // 000-003
    CHAR *shipname;                             // 004-007
    LONG  max_tons;                             // 008-011
    LONG  max_price;                            // 012-015
    LONG  scanrange;                            // 016-019
    VOID (*init_func)(WARSHP *, INT, SHORT);    // 020-023
    VOID (*tick_func)(WARSHP *, INT);           // 024-027
    VOID (*kill_func)(WARSHP *, INT, WARSHP *); // 028-031
    VOID (*won_func)(WARSHP *, INT, WARSHP *);  // 032-035
    USHORT max_accel;                           // 036-037
    USHORT max_points;                          // 038-039
    USHORT max_type;                            // 040-041
    USHORT tot_to_create;                       // 042-043
    USHORT damfact;                             // 044-045
    USHORT res_flag_2;                          // 046-047
    USHORT res_flag_3;                          // 048-049
    SHORT  hlpmsg;                              // 050-051
    UCHAR  max_shlds;                           // 052
    CHAR   max_phasr;                           // 053
    UCHAR  max_torps;                           // 054
    UCHAR  max_missl;                           // 055
    CHAR   has_decoy;                           // 056
    CHAR   has_jam;                             // 057
    CHAR   has_zip;                             // 058
    CHAR   has_mine;                            // 059
    CHAR   max_attk;                            // 060
    CHAR   max_cloak;                           // 061
    UCHAR  max_warp;                            // 062
    UCHAR  cybs_can_att;                        // 063
    CHAR   lowest_to_attk;                      // 064
    CHAR   noclaim;                             // 065
    CHAR   tough_factor;                        // 066
#ifndef __BUILDV10MODULE
    CHAR pack1;
#endif
    // pack 2
}; // size: 68, 2 packed bytes

#define CYB_TOUGH_0 0
#define CYB_TOUGH_1 1

typedef VOID (*CmdFunc)(VOID);

struct cmd {
    CHAR   *command; // 000-003
    CmdFunc func;    // VOID(*func)(); // 004-007
    byte    cando;   // 008
#ifndef __BUILDV10MODULE
    CHAR pack1[3];
#endif
    // pack 3
}; // size 12

#define SHIPTAB struct _shiptab
SHIPTAB
{
    SHORT  shipno;  // 000-001
    CHAR   letter;  // 002
    CHAR   flag;    // 003
    SHORT  bearing; // 004-005
    SHORT  heading; // 006-007
    DOUBLE dist;    // 008-015
    DOUBLE speed;   // 016-023
}; // size 24, 0 packed bytes

#define NOSCANTAB 15

#define SCANTAB struct _scantab
SCANTAB
{
    SHIPTAB ship[NOSCANTAB];
}; // size 360, reduced from 480 before packing SHIPTAB

#define S00 struct _s00
S00
{
    CHAR  *name;   // 000-003
    CHAR  *owner;  // 004-007
    DOUBLE xcoord; // 008-015
    DOUBLE ycoord; // 016-023
    SHORT  type;   // 024-025
    SHORT  env;    // 026-027
    SHORT  res;    // 028-029
#ifndef __BUILDV10MODULE
    CHAR pack1[2];
#endif
    // pack 2
}; // size 32 - 2 packed bytes

#define TEAM struct _team
TEAM
{
    LONG   teamcode;     // 000-003
    ULONG  teamscore;    // 004-007
    USHORT teamcount;    // 008-009
    SHORT  flag;         // 010-011
    CHAR   teamname[31]; // 012-042
    CHAR   password[11]; // 043-053
    CHAR   secret[11];   // 054-064
#ifndef __BUILDV10MODULE
    CHAR pack1[3];
#endif
    // pack 3
}; // size 68, 3 packed bytes

#define BEACONTAB struct _beacontab
BEACONTAB
{
    COORD coord;               // 000-015
    SHORT plnum;               // 016-017
    CHAR  beacon[BEACONMSGSZ]; // 018-092
#ifndef __BUILDV10MODULE
    CHAR pack1[3];
#endif
    // pack 3
}; // 96, 3 packed bytes

// ELWGEMAL.DAT
#define GEMESSAGE struct _gemessage
GEMESSAGE
{
    struct message m;              // 000-0671
#ifndef __BUILDV10MODULE
    CHAR pack1[3];
#endif
    CHAR text[GEMSGSIZ]; // 672-1455
}; // size 1456, 0 packed

#define KEY                                                                    \
    "\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255" \
    "\255\255\255\255\255\255\255\255\255\255\255\255\255"

#define FUNC EXPORT

#include "geglobal.h"
