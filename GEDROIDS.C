/*****************************************************************************
 *                                                                           *
 *   GEDROIDS.C                                                              *
 *                                                                           *
 *   Droid-related functions                                                 *
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

#include "gemain.h"

#define  GEDROIDS   1

 /* LOCAL GLOBAL DEFS *****************************************************/

CHAR droidname[UIDSIZ];

/**************************************************************************/

/**************************************************************************
 ** Droid Init Function                                                  **
 **************************************************************************/
VOID FUNC droid_init(WARSHP *ptr, INT usrn, SHORT class)
{
    if (usrn < 0 || usrn >= nships) {
        logthis(spr("DROID_INIT:bad usrn [%d]", usrn));
        return;
    }

    strncpy(droidname, "@Droid-", UIDSIZ); /* Bj Added name here */
    sprintf(&droidname[7], "%d", usrn);

    waruptr = warusroff(usrn);
    warsptr = warshpoff(usrn);

    logthis(spr("GE:INF:Adding %s user", droidname));

    initusr(droidname);

    memcpy(waruptr, &tmpusr, sizeof(WARUSR)); /* make it the current user */

    /* make me a Ship */
    logthis(spr("GE:INF:Adding %s ship - %d", ptr->userid, class));

    initshp(droidname, class);

    memcpy(ptr, &tmpshp, sizeof(WARSHP)); /* make is the current ship */
    sprintf(ptr->shipname, "%s%u\0", shipclass[class].shipname, usrn * usrn + gernd() % 100);

    if (univmax < 20) {
        ptr->coord.xcoord = rndm((DOUBLE)univmax * 2.0) - (DOUBLE)univmax;
        ptr->coord.ycoord = rndm((DOUBLE)univmax * 2.0) - (DOUBLE)univmax;
    } else {
        ptr->coord.xcoord = rndm(39.9) - 19.8;
        ptr->coord.ycoord = rndm(39.9) - 19.8;
    }

    ptr->phasrtype  = shipclass[class].max_phasr;
    ptr->shieldtype = shipclass[class].max_shlds;

    /* if murdonian transport - put tons on */
    if (sameas(shipclass[ptr->shpclass].typename, "Murdonian Transport")) {
        ptr->items[I_FLUXPOD]   = gernd() % 50;
        ptr->items[I_DECOYS]    = gernd() % 250;
        ptr->items[I_TORPEDO]   = gernd() % 250;
        ptr->items[I_MINE]      = gernd() % 100;
        ptr->items[I_JAMMERS]   = gernd() % 100;
        ptr->items[I_MISSILE]   = gernd() % 100;
        ptr->items[I_IONCANNON] = gernd() % 25;
        ptr->items[I_GOLD]      = gernd() % 250;
    } else {
        ptr->items[I_FLUXPOD] = gernd() % 50;
        ptr->items[I_DECOYS]  = gernd() % 25;
        ptr->items[I_MINE]    = gernd() % 10;
        ptr->items[I_JAMMERS] = gernd() % 10;
    }

    ptr->speed2b    = rndm((DOUBLE)(ptr->topspeed * 1000.0));
    ptr->holdcourse = 0;

    ptr->status = GESTAT_AUTO;
    ptr->tick   = CYBTICKTIME + gernd() % CYBTICKTIME;

    /*gepdb(GEUPDATE,ptr->userid,ptr->shipno,ptr); DONT NEED TO MAKE PERM*/
    prfmsg(DROIDNEW, gernd() % 359);
    outwar(FILTER, (USHORT)usrn, 0);
}

/**************************************************************************
 ** Droid Lives Function                                                 **
 **************************************************************************/
VOID FUNC droid_lives(WARSHP *ptr,INT usrn)
{

    if (usrn < 0 || usrn >= nships) {
        logthis(spr("DROID_LIVES:bad usrn [%d]", usrn));
        return;
    }

    sprintf(&droidname[7], "%d", usrn + 1);

    /* DEBUG
    logthis(spr("GE:%s Lives",droidname)); */

    /* reset the ticker to 255 to cause it to recalc */
    ptr->tick = 255;

    warusroff(usrn)->cash += CYB_ALLOW;

    if (sameas(shipclass[ptr->shpclass].typename, "Lydorian Garbage Scow"))
        droid_act_class_10(ptr, usrn); /* Lydorian Garbage Scow */
    else if (sameas(shipclass[ptr->shpclass].typename, "Murdonian Transport"))
        droid_act_class_11(ptr, usrn); /* Murdonian Transport */
    else if (sameas(shipclass[ptr->shpclass].typename, "Vakory Survey Drone"))
        droid_act_class_12(ptr, usrn); /* Vakory Survey Droid */

    ptr->energy = 50000L;

    if (ptr->tick == 255) {
        /* if just cruising around don't get back to me for some time */
        if (ptr->cantexit == 0) {
            ptr->tick = (CYBTICKTIME + gernd() % CYBTICKTIME) * 3;
        } else {
            ptr->tick = CYBTICKTIME + gernd() % CYBTICKTIME;
        }
    }
}

/* ptr to sender , usrn = reciever */
VOID FUNC droid_annoy(WARSHP *ptr,INT usrn,SHORT rnd,SHORT first,SHORT last)
{
    if ((gernd() % rnd) == 1) {
        prfmsg(first + gernd() % (last - first + 1), ptr->shipname);
        outprfge(FILTER, usrn);
    }
}

/**************************************************************************
 ** Class 10 Action                                                      **
 ** Lydorian Garbage Scow                                                **
 **************************************************************************/
VOID FUNC droid_act_class_10(WARSHP *ptr,INT usrn)
{
    WARSHP *wptr;
    INT     zothusn;
    DOUBLE  ddist;

    /* am I being jammed ? */
    if (ptr->jammer == 0) {
        /* look at all the other ships */
        for (zothusn = 0; zothusn < nterms; zothusn++) {
            wptr = warshpoff(zothusn);
            /* if not me, and playing, and not cyborg, go getem */
            if (ingegame((SHORT)zothusn) && wptr->status == GESTAT_USER) {
                ddist = cdistance(&ptr->coord, &wptr->coord);
                ddist *= 10000;
                if (ddist < (DOUBLE)shipclass[ptr->shpclass].scanrange) {
                    ptr->tick = CYBTICKTIME + gernd() % CYBTICKTIME;
                    droid_annoy(ptr, zothusn, 4, DRDMSG6, DRDMSG6);
                }
            }
        }
    } else {
        ptr->speed2b    = 999.9; /* has no warp capability */
        ptr->holdcourse = gernd() % 50 + 10;
    }
    /* Runs with shields up all the time */
    if (ptr->speed < 1000.0)
        shieldup(ptr, usrn);
    else
        shielddn(ptr, usrn);
}

/**************************************************************************
 ** Class 11 Action                                                      **
 ** Murdonian Transport                                                  **
 **************************************************************************/
VOID FUNC droid_act_class_11(WARSHP *ptr,INT usrn)
{
    WARSHP *wptr;
    INT     zothusn;
    DOUBLE  ddist;

    ddist = 999999.9;
    /* am I being jammed ? */
    if (ptr->jammer == 0) {
        /* look at all the other ships */
        for (zothusn = 0; zothusn < nterms; zothusn++) {
            wptr = warshpoff(zothusn);
            /* if not me, and playing, and not cyborg, go getem */
            if (ingegame((SHORT)zothusn) && wptr->status == GESTAT_USER) {
                ddist = cdistance(&ptr->coord, &wptr->coord);
                ddist *= 10000;
                if (ddist < (DOUBLE)shipclass[ptr->shpclass].scanrange) {
                    if (ptr->holdcourse == 0)
                        ptr->speed2b = rndm(999.9);
                    if (ptr->speed < 1000.0)
                        shieldup(ptr, usrn);
                    else
                        shielddn(ptr, usrn);

                    ptr->tick = CYBTICKTIME + gernd() % CYBTICKTIME;
                    droid_annoy(ptr, zothusn, 4, DRDMSG11, DRDMSG15);
                }
            }
        }
        if (ptr->cantexit > 0 && ptr->lastfired >= 0) {
            logthis(spr("Droid (murtran) cantexit - lastfired = %d", ptr->lastfired));
            wptr    = warshpoff(ptr->lastfired);
            zothusn = ptr->lastfired;

            droid_annoy(ptr, zothusn, 4, DRDHLP11, DRDHLP15);

            /* fire phasers at the fool */
            if (ptr->where == 1 && wptr->where == 1) {
                if (ddist < 30000) {
                    ptr->degrees = (SHORT)(cbearing(&ptr->coord, &wptr->coord, ptr->heading) + .5);
                    firehp(ptr, usrn);
                }
            } else if ((ptr->where == 0 && wptr->where == 0) || (ptr->where == 0 && wptr->where >= 2)) {
                ptr->degrees = 0;
                ptr->percent = 2;
                if (ptr->phasr >= PMINFIRE && wptr->cloak != 10) {
                    /* logthis(spr("GE:phaser fired %d",ptr->degrees));*/
                    firep(ptr, usrn);
                }

                /* just to confuse them sometimes alter attack vector */
                if (ptr->holdcourse == 0 && gernd() % 10 == 0) {
                    ptr->speed2b    = rndm(10000.0);
                    ptr->head2b     = rndm(359.9);
                    ptr->holdcourse = gernd() % 10 + 3;
                }
            }

            /* if we are in hyperspace and fighting and missiles detected
              get out of hyperspace */
            if (ptr->where == 1) {
                if (missl_attached(ptr, usrn)) {
                    ptr->speed2b    = rndm(999.0);
                    ptr->holdcourse = gernd() % 15 + 5;
                }
            } else {
                shieldup(ptr, usrn);
            }
        }
    } else {
        ptr->speed2b    = (DOUBLE)(ptr->topspeed * 1000);
        ptr->holdcourse = gernd() % 50 + 10;
    }
}

/**************************************************************************
 ** Class 12 Action                                                      **
 ** Vakory Survey Droid                                                  **
 **************************************************************************/
VOID FUNC droid_act_class_12(WARSHP *ptr,INT usrn)
{
    SHORT   i, j;
    WARSHP *wptr;
    INT     zothusn;
    DOUBLE  ddist;

    ddist = 999999.9;
    /* am I being jammed ? */
    if (ptr->jammer == 0) {
        /* look at all the other ships */
        for (zothusn = 0; zothusn < nterms; zothusn++) {
            wptr = warshpoff(zothusn);
            /* if not me, and playing, and not cyborg, go getem */
            if (ingegame((SHORT)zothusn) && wptr->status == GESTAT_USER) {
                ddist = cdistance(&ptr->coord, &wptr->coord);
                ddist *= 10000;
                if (ddist < (DOUBLE)shipclass[ptr->shpclass].scanrange) {
                    if (ptr->speed < 1000.0)
                        shieldup(ptr, usrn);
                    else
                        shielddn(ptr, usrn);

                    droid_annoy(ptr, zothusn, 4, DRDMSG1, DRDMSG5);
                    ptr->tick = CYBTICKTIME + gernd() % CYBTICKTIME;
                }
            }
        }
        if (ptr->cantexit > 0 && ptr->lastfired > 0) {
            logthis(spr("Droid (vakory) cantexit - lastfired = %d", ptr->lastfired));
            wptr    = warshpoff(ptr->lastfired);
            zothusn = ptr->lastfired;

            droid_annoy(ptr, zothusn, 4, DRDHLP1, DRDHLP5);

            /* fire phasers at the fool */
            if (ptr->where == 1 && wptr->where == 1) {
                if (ddist < 30000) {
                    ptr->degrees = (SHORT)(cbearing(&ptr->coord, &wptr->coord, ptr->heading) + .5);
                    firehp(ptr, usrn);
                }
            } else if ((ptr->where == 0 && wptr->where == 0) || (ptr->where == 0 && wptr->where >= 2)) {
                ptr->degrees = 0;
                ptr->percent = 2;
                if (ptr->phasr >= PMINFIRE && wptr->cloak != 10) {
                    /* logthis(spr("GE:phaser fired %d",ptr->degrees));*/
                    firep(ptr, usrn);
                }

                /* fire torpedoes at the fool */
                j = gernd() % 2;
                for (i = 0; i < j; ++i) {
                    ptr->items[I_TORPEDO] = (gernd() % 5) + 1;
                    if (i > 0)
                        lockwarn = FALSE;
                    torp(ptr, usrn, (SHORT)zothusn);
                }

                /* just to confuse them sometimes alter attack vector */
                if (ptr->holdcourse == 0 && gernd() % 20 == 1) {
                    ptr->speed2b    = rndm(5000.0);
                    ptr->head2b     = rndm(359.9);
                    ptr->holdcourse = gernd() % 10 + 3;
                }
            }

            /* if we are in hyperspace and fighting and missiles detected
                speed up and loose them */
            if (missl_attached(ptr, usrn)) {
                ptr->speed2b    = rndm(5900.0) + 5000.0;
                ptr->holdcourse = gernd() % 5 + 5;
            }

            if (ptr->speed < 1000.0)
                shieldup(ptr, usrn);
            else
                shielddn(ptr, usrn);

            if (ptr->damage > 75) {
                if (ptr->items[I_MINE] > 0)
                    laymine(ptr, usrn, 10);

                if (ptr->items[I_JAMMERS] > 0)
                    jam(ptr, usrn);

                ptr->speed2b    = (DOUBLE)(ptr->topspeed * 1000);
                ptr->head2b     = rndm(359.9);
                ptr->holdcourse = gernd() % 30 + 20;
            }
        }
    } else {
        ptr->speed2b    = (DOUBLE)(ptr->topspeed * 1000);
        ptr->holdcourse = gernd() % 50 + 10;
    }
}


VOID FUNC droid_won(WARSHP *ptr, INT usrn, WARSHP *wptr)
{
    (VOID) usrn;
    (VOID) wptr;
    ptr->speed2b = rndm(5000.0);
}

VOID FUNC droid_died(WARSHP *ptr, INT usrn, WARSHP *wptr)
{
    (VOID) usrn;
    (VOID) wptr;
    ptr->status = GESTAT_AVAIL;
    logthis(spr("GE:INF:%s Died!", ptr->userid));
}

SHORT FUNC missl_attached(WARSHP *ptr,INT usrn)
{
    SHORT    i;
    MISSILE *mptr;

    (VOID) usrn; // usrn=usrn; // avoid warning

    for (i = 0, mptr = ptr->lmissl; i < MAXMISSL; ++i, ++mptr) {
        if (mptr->distance > 0) {
            return (TRUE);
        }
    }

    return (FALSE);
}
