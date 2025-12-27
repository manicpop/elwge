/*****************************************************************************
 *                                                                           *
 *   GECYBS.C                                                                *
 *                                                                           *
 *   Cybertron-related functions                                             *
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

#define  GECYBS   1

/* LOCAL GLOBAL DEFS ****************************************************/

CHAR   cybname[UIDSIZ]; 
SHORT  cybhaltflg = 0;
DOUBLE d_topspeed;

/**************************************************************************
 ** Cyborg functions                                                     **
 **************************************************************************/

VOID FUNC cyb_init(WARSHP *ptr, INT usrn, SHORT class)
{
    logthis(spr("@Cyb_init usrn=%d,class=%d", usrn, class));

    if (usrn < 0 || usrn >= nships) {
        logthis(spr("CYB_INIT:bad usrn [%d]", usrn));
        return;
    }

    strncpy(cybname, "@Cybrg-", UIDSIZ); /* Bj Added name here */
    sprintf(&cybname[7], "%d", usrn);

    if (!(geudb(GELOOKUP, cybname, &tmpusr))) {
        initusr(cybname);
        geudb(GEADD, tmpusr.userid, &tmpusr);
        logthis(spr("GE:INF:Adding %s user", tmpusr.userid));
    }

    waruptr = warusroff(usrn);
    warsptr = warshpoff(usrn);

    if (geudb(GELOOKUP, cybname, waruptr)) {
        geudb(GEGET, cybname, waruptr);

        if (waruptr->cash > CYB_MAXCASH)
            waruptr->cash = CYB_MAXCASH;

        logthis(spr("GE:INF:Load %s user", waruptr->userid));

        if (gepdb(GELOOKUPNAME, cybname, 0, ptr)) {
            dfaAbsRec(ptr, 0);
            logthis(spr("GE:INF:Load %s ship", ptr->userid));

            ptr->status     = GESTAT_AUTO;
            ptr->phasr      = 100;
            ptr->cybmine    = (byte)255;
            ptr->speed2b    = (DOUBLE)(ptr->topspeed) * 500.0;
            ptr->cybupdate  = 100 + gernd() % 20;
            ptr->holdcourse = 0;
            ptr->tick       = CYBTICKTIME + gernd() % (CYBTICKTIME * 5);

            /* SANITY CHECK */
            if (shipclass[ptr->shpclass].max_type != CLASSTYPE_CYBORG) {
                geshocst(0, spr("ELWGE:ERR:NOTCYBCLS %d", ptr->shpclass));
            }
        } else {
            /* make me a Cybertron or a Cyberquad */
            logthis(spr("GE:INF:Adding %s ship - %d", ptr->userid, class));

            initshp(cybname, class);
            gepdb(GEADD, tmpshp.userid, tmpshp.shipno, &tmpshp);
            memcpy(ptr, &tmpshp, sizeof(WARSHP)); /* make is the current ship */

            logthis(spr("GE:INF:Add shp,cls=%d/%d", class, ptr->shpclass));
            sprintf(ptr->shipname, "%s%u\0", shipclass[class].shipname, usrn * usrn + gernd() % 100);
            logthis(spr("  Named: %s", ptr->shipname));

            ptr->coord.xcoord = rndm((DOUBLE)univmax * 2.0) - (DOUBLE)univmax;
            ptr->coord.ycoord = rndm((DOUBLE)univmax * 2.0) - (DOUBLE)univmax;
            ptr->phasrtype    = shipclass[class].max_phasr;
            ptr->shieldtype   = shipclass[class].max_shlds;
            ptr->cybmine      = (byte)255;

            ptr->items[I_FLUXPOD] = gernd() % 5;
            ptr->items[I_DECOYS]  = gernd() % 25;
            ptr->items[I_TORPEDO] = gernd() % 25;
            ptr->items[I_MINE]    = gernd() % 100;
            ptr->items[I_JAMMERS] = gernd() % 100;
            ptr->items[I_GOLD]    = gernd() % cyb_gold;

            ptr->holdcourse = 0;

            /* cyborg skill level is his propensity to make an error, the higher the #
              the less chance he will make an error                                   */
            ptr->cybskill = (byte)gernd() % 15 + 3;
            /*		ptr->cybskill = 100;*/

            ptr->status = GESTAT_AUTO;
            ptr->tick   = CYBTICKTIME + gernd() % CYBTICKTIME;
            /* DEBUG */

            gepdb(GEUPDATE, ptr->userid, ptr->shipno, ptr);
            prfmsg(CYBNEW, gernd() % 359);
            outwar(FILTER, (USHORT)usrn, 0);
        }
    } else {
        /* DEBUG */
        geshocst(0, spr("ELWGE:ERR:NO FIND %s user", cybname));
    }
}

VOID FUNC cyb_lives(WARSHP *ptr,INT usrn)
{
    WARSHP *wptr;
    INT     zothusn;
    INT     i;
    DOUBLE  ddist;

    if (!sameas(ptr->userid, warusroff(usrn)->userid))
        geshocst(0, "GE:ERR:Cyb Names !=");

    i = usrn;
    ++i;

    sprintf(&cybname[7], "%d", i);

    logthis(spr("@cyb_lives %s", cybname));

    /* reset the ticker to 255 to cause it to recalc */
    ptr->tick = 255;

    /* save off the topspeed in 1000's */
    d_topspeed = (DOUBLE)ptr->topspeed * 1000.0;

    /* allowance money for cybertrons */
    warusroff(usrn)->cash += CYB_ALLOW;

    /* countdown to database update */
    db_update(ptr, usrn);

    /* am I being jammed ? */
    if (ptr->jammer == 0) {
        /* look at all the other ships */
        for (zothusn = 0; zothusn < nterms; zothusn++) {
            wptr = warshpoff(zothusn);
            /* if not me, and playing, and not cyborg, go getem */
            if (usroff(zothusn)->state == gestt && usroff(zothusn)->substt >= FIGHTSUB && wptr->cloak != 10) {
                ddist = cdistance(&ptr->coord, &wptr->coord);
                ddist *= 10000;

                if (!neutral(&ptr->coord) && ddist < (DOUBLE)shipclass[ptr->shpclass].scanrange) {
                    /* if ordinary cyb see if this is his lucky day */
                    if (isquad(ptr) && gernd() % CYB_BREAKOFF == 0) {
                        prfmsg(CYBLUCK, ptr->shipname);
                        outprfge(FILTER, zothusn);
                        ptr->cybmine = (byte)255; /* take a break */

                        ptr->speed2b = d_topspeed;
                    }

                    /* fire phasers at the fool */
                    if (ptr->where == 1 && wptr->where == 1 && gebemean(ptr, zothusn)) {
                        if (ddist < (tooclose + rndm(tooclose)) || shipclass[wptr->shpclass].cybs_can_att || wptr->cantexit > 0 || ptr->cantexit > 0) {
                            if (ddist < 30000.0) {
                                ptr->degrees = (SHORT)(cbearing(&ptr->coord, &wptr->coord, ptr->heading) + .5);
                                firehp(ptr, usrn);
                            }
                        }
                    } else if (ptr->where == 0 && wptr->where != 1) {
                        ptr->degrees = (SHORT)(cbearing(&ptr->coord, &wptr->coord, ptr->heading) + .5);
                        ptr->percent = 2;

                        /* if the guy gets closer then 2000 or is not little guy or has fired */
                        if (ddist < (tooclose + rndm(tooclose)) || shipclass[wptr->shpclass].cybs_can_att || wptr->cantexit > 0) {
                            cyb_attack(ptr, usrn, wptr, zothusn);
                            cyb_annoy(ptr, zothusn, 20, 13, 16);
                            cyb_lay_decoys(ptr, zothusn);
                        } else {
                            cyb_annoy(ptr, zothusn, 20, 9, 12);
                        }
                    }
                }
            }
        }
    } else {
        /* as long as they can't see ... the other player must be trying to get
          away.... might as well mine the area */

        if (shipclass[ptr->shpclass].has_mine && ptr->items[I_MINE] > 0 && gernd() % 5 == 0)
            laymine(ptr, usrn, 10);

        ptr->speed2b    = d_topspeed + rndm(3000.0);
        ptr->holdcourse = gernd() % 7 + 2;
    }

    /* cyberbases should not do these */
    if (shipclass[ptr->shpclass].max_accel > 0) {
        cyb_check_damage(ptr, usrn);
        cyb_check_lockon(ptr, usrn);
    }

    ptr->energy = 50000L;

    /*DEBUG
    geshocst(0,spr("ELWGE:cm %d",(SHORT)ptr->cybmine)); */

    /* check for jammer */
    if (ptr->jammer > 0) {
        ptr->speed2b    = d_topspeed + rndm(3000.0);
        ptr->holdcourse = gernd() % 10 + 5;
    }

    if (ptr->tick == 255) {
        /* if just cruising around don't get back to me for some time */
        if (ptr->cantexit == 0) {
            ptr->tick = (CYBTICKTIME + gernd() % CYBTICKTIME) * 5;
        } else {
            if (!isquad(ptr))
                ptr->tick = CYBTICKTIME + gernd() % CYBTICKTIME;
            else
                ptr->tick = 2 + gernd() % CYBTICKTIME;
        }
    }
}

/* look if another cyborg (or cyborgs) has this ship claimed */
SHORT FUNC notclaimed(WARSHP *ptr,INT usrn)
{
    WARSHP *wptr;
    INT     zothusn;
    SHORT   nc;

    nc = 0;
    for (zothusn = nterms; zothusn < nships; zothusn++) {
        wptr = warshpoff(zothusn);
        if (wptr->status == GESTAT_AUTO && shipclass[wptr->shpclass].max_type == CLASSTYPE_CYBORG && wptr->cybmine == (byte)usrn)
            ++nc;
    }
    /*DEBUG
    geshocst(0,spr("nc=%d",nc));*/

    logthis(spr("notclaimed: nc = %d, class = %d, class.noclaim = %d", nc, ptr->shpclass, shipclass[ptr->shpclass].noclaim));
    return (nc < shipclass[ptr->shpclass].noclaim);
}

/* ptr to sender , usrn = reciever */
VOID FUNC cyb_annoy(WARSHP *ptr,INT usrn,SHORT rnd,SHORT first,SHORT last)
{
    SHORT base;
    SHORT sel;

    if ((gernd() % rnd) == 1) {
        base = CYBBASEM;
        base += (ptr->shpclass - cyb_class) * 16;

        first = first + base;
        last  = last + base;
        sel   = first + gernd() % (last - first + 1);

        if (sel < CYBLASTM) {
            prfmsg(sel, ptr->shipname);
            outprfge(FILTER, usrn);
        }
        sprintf(gechrbuf, "cyb_ann shnm=<%s> usrn=%d base=%d frst=%d lst=%d sel=%d", ptr->shipname, usrn, base, first, last, sel);
        logthis(gechrbuf);
    }
}

SHORT FUNC cybwhoops(WARSHP *ptr,INT zothusn)
{
    USHORT err, i;

    err = 0;

    for (i = 0; i < zothusn; ++i)
        gernd();

    if ((gernd() % (SHORT)(ptr->cybskill)) == 1) {
        err = 1;
        if (gernd() % 5 == 1) {
            /*		cyb_annoy(ptr,zothusn,4,CYBWOOP1,CYBWOOP4);*/
        }
    }

    return (err);
}

SHORT FUNC gebemean(WARSHP *ptr,INT usrn)
{

    /* cyberquads are ALWAYS mean */
    if (isquad(ptr))
        return (1);

    /* if this player has accumulated 50 kills play harder */
    if (warusroff(usrn)->kills > CYB_BE_NICE)
        return (1);

    if (gernd() % CYBSLO == 0)
        return (1);

    return (0);
}

/* countdown to database update */
VOID FUNC db_update(WARSHP *ptr,INT usrn)
{
    WARUSR *wuptr;

    if (ptr->cybupdate > 1) {
        --ptr->cybupdate;
        return;
    } else if (ptr->cybupdate == 1) {
        if (ptr->cybmine == 255) {
            ptr->speed2b = rndm(d_topspeed); /* change the direction */
            ptr->head2b  = rndm(359.9);
        }
        ptr->cybupdate = 100 + gernd() % 100;
        return;
    } else if (ptr->cybupdate == 0) { /* am I crazy or does this code never ever get executed???????????*/
        wuptr = warusroff(usrn);
        logthis(spr("GE:DBG:Cyb UUpd %s", wuptr->userid));
        geudb(GEUPDATE, wuptr->userid, wuptr);
        logthis(spr("GE:DBG:Cyb PUpd %s", ptr->userid));
        gepdb(GEUPDATE, ptr->userid, ptr->shipno, ptr);
        ptr->cybupdate = 100 + gernd() % 100;
        return;
    }

    ptr->cybupdate = 20;
}

/**************************************************************************
** Attack the other player                                               **
**************************************************************************/
VOID FUNC cyb_attack(
WARSHP   *ptr,    /* ptr to cyb ship   */
INT      usrn,    /* cybs ship number  */
WARSHP   *wptr,   /* ptr to users ship */
INT      zothusn) /* users ship number */
{
    MISSILE *mptr;
    SHORT    i, j;

    (VOID) wptr; // wptr = wptr; /* eliminates warning*/ /* no it doesn't - RH 4/4/2021 :) */

    if (ptr->phasr >= PMINFIRE && gebemean(ptr, zothusn)) {
        /* geshocst(0,spr("GE:phaser fired %d",ptr->degrees));*/
        if (!cybwhoops(ptr, zothusn))
            firep(ptr, usrn);
    }

    /* fire torpedoes at the fool */
    j = gernd() % 6;
    if (!isquad(ptr) && warusroff(zothusn)->kills < CYB_BE_EASY)
        j = gernd() % 2;

    if (!gebemean(ptr, zothusn))
        j = 0;

    /* if this class has no torps don't fire any */
    if (shipclass[ptr->shpclass].max_torps == 0)
        j = 0;

    for (i = 0; i < j; ++i) {
        ptr->items[I_TORPEDO] = (gernd() % 5) + 1;
        if (i > 0)
            lockwarn = FALSE;
        torp(ptr, usrn, (SHORT)zothusn);
    }

    /* launch Zippers if needed */
    if (gernd() % 10 == 1 && shipclass[ptr->shpclass].has_zip) {
        if (ptr->minesnear == TRUE) {
            if (gernd() % 3 == 1) {
                ptr->items[I_ZIPPERS] = 1;
                zip(ptr, usrn);
                ptr->minesnear = FALSE;
            }
            /* get the hell out of here ...then come back */
            ptr->speed2b    = d_topspeed;
            ptr->head2b     = rndm(359.9);
            ptr->holdcourse = gernd() % 20 + 3;
        }
    }

    /* just to confuse them sometimes alter attack vector */
    if (gernd() % 20 == 1) {
        ptr->speed2b    = d_topspeed;
        ptr->head2b     = rndm(359.9);
        ptr->holdcourse = gernd() % 10 + 3;
    }

    /* if we are in hyperspace and fighting and missiles detected
      get out of hyperspace and get shields up */
    if (ptr->where == 1) {
        for (i = 0, mptr = ptr->lmissl; i < MAXMISSL; ++i, ++mptr) {
            if (mptr->distance > 0) {
                ptr->speed2b    = rndm(5000.00) + 4500.00;
                ptr->holdcourse = gernd() % 5 + 5;
                break;
            }
        }
    } else {
        shieldup(ptr, usrn);
    }

    /*
    if (!cybwhoops(ptr,zothusn) && gebemean(ptr,zothusn)) {
        prfmsg(SCAN1,ptr->shipname);
        outprfge(FILTER,zothusn);
      }*/
    /*EXTRA MESSAGES WE JUST DONT NEED */
}

/**************************************************************************
 ** Lay down some decoys                                                 **
 **************************************************************************/
VOID FUNC cyb_lay_decoys(WARSHP *ptr,INT zothusn)
{
    SHORT i;

    /* send out a decoy */
    if (!cybwhoops(ptr, zothusn)) {
        for (i = 0; i < 5; ++i)
            if (ptr->decout[i] == 0)
                ptr->decout[i] = DECOYTIME;
    }
}

/**************************************************************************
 ** if hunting, and badly damaged dump mines, jam, and boogie            **
 **************************************************************************/
VOID FUNC cyb_check_damage(WARSHP *ptr,INT usrn)
{
    if (ptr->cybmine < 255 && ptr->damage > CYB_MINDAM && (gernd() % 10 == 0)) {
        if (shipclass[ptr->shpclass].has_mine && ptr->items[I_MINE] > 0 && gernd() % 5 == 0)
            laymine(ptr, usrn, 10);

        if (shipclass[ptr->shpclass].has_jam && ptr->items[I_JAMMERS] > 0 && gernd() % 100 == 0)
            jam(ptr, usrn);

        ptr->speed2b    = d_topspeed;
        ptr->head2b     = rndm(359.9);
        ptr->holdcourse = gernd() % 10 + 5;
    }
}

/**************************************************************************
 ** Check lockon status                                                  **
 **************************************************************************/
VOID FUNC cyb_check_lockon(WARSHP *ptr,INT usrn)
{
    WARSHP *wptr;
    SHORT   zothusn;
    DOUBLE  ddist;
    DOUBLE  low_dist; // = 999999999.0;
    SHORT   low_ship;
    SHORT   lta; /* lowest to attack */

    low_dist = 999999999.0;
    low_ship = -1;

    /* if cyborg not seeking - countdown */
    zothusn = ptr->cybmine;

    if (ptr->holdcourse > 0) {
        --(ptr->holdcourse);
        return;
    }

    if (zothusn >= nterms) {
        ptr->cybmine = (byte)255;
    } else {
        if (!ingegame(zothusn)) {
            ptr->cybmine = (byte)255;
            ptr->speed2b = rndm(d_topspeed); /* let them cruise */
            return;
        }

        wptr = warshpoff(zothusn);

        if (wptr->cloak == 10) {
            ptr->holdcourse = gernd() % 5 + 5;
            ptr->speed2b    = rndm(d_topspeed); /* let them cruise */

            /* if the guy is cloaked then give up after awhile */
            if (gernd() % 10 == 0)
                ptr->cybmine = 255;

            return;
        }

        low_ship = zothusn;
        low_dist = cdistance(&ptr->coord, &(wptr->coord));
    }

    if (ptr->cybmine == (byte)255) {
        lta = shipclass[ptr->shpclass].lowest_to_attk - 1;

        for (zothusn = 0; zothusn < nterms; zothusn++) {
            wptr = warshpoff(zothusn);
            /* if playing, and not cloaked, go getem */
            if (ingegame(zothusn) && wptr->cloak != 10) {
                if (lta <= wptr->shpclass && notclaimed(wptr, zothusn)) {
                    /* figure out who is closest */
                    ddist = cdistance(&ptr->coord, &wptr->coord);
                    if (ddist < low_dist) {
                        low_dist = ddist;
                        low_ship = zothusn;
                    }
                }
            }
        }
    }

    if (low_ship == -1 || low_ship >= nterms) {
        ptr->tick    = 255; /* no one in game so cool it for awhile */
        ptr->cybmine = 255;
    } else {
        ptr->cybmine = (byte)low_ship;
        wptr         = warshpoff(low_ship);
        if (low_dist >= hyperdist1) {
            /* if far away invoke hyper-warp 20 X normal speed */
            ptr->speed2b    = (DOUBLE)low_dist * 2000.0;
            ptr->speed      = ptr->speed2b;
            ptr->head2b     = vector(&ptr->coord, &(wptr->coord));
            ptr->where      = 1;
            ptr->shieldstat = SHIELDDN;
            /* DEBUG
            logthis(spr("CybSpeed LONG Top=%d S2B=%ld Dist=%ld",
              ptr->topspeed,(LONG)ptr->speed2b,(LONG)low_dist));
              prf("***\r<%s>W gonna get you LONG<%d-%d> S2B=%s \r",cybname,(SHORT)ptr->coord.xcoord,(SHORT)ptr->coord.ycoord,spr("%ld",(LONG)ptr->speed2b));
            outwar(ALWAYS,usrn,0);*/
        } else if (low_dist >= hyperdist2) {
            /* BRAKE!!!!!! */
            if (ptr->speed > 20000.0)
                ptr->speed = 20000.0;
            ptr->speed2b = d_topspeed;
            ptr->head2b  = vector(&ptr->coord, &(wptr->coord));
            logthis(spr("CybSpeed MID Top=%d S2B=%ld Dist=%ld", ptr->topspeed, (LONG)ptr->speed2b, (LONG)low_dist));
            /* DEBUG
            prf("***\r<%s>W gonna get you MID <%d-%d> S2B=%s \r",cybname,(SHORT)ptr->coord.xcoord,(SHORT)ptr->coord.ycoord,spr("%ld",(LONG)ptr->speed2b));
            outwar(ALWAYS,usrn,0);*/
            cyb_annoy(ptr, low_ship, 60, 1, 4);
        } else if (low_dist > 3.0) {
            if (ptr->speed > d_topspeed)
                ptr->speed = d_topspeed;

            ptr->head2b  = vector(&ptr->coord, &(wptr->coord));
            ptr->speed2b = d_topspeed;
            /* DEBUG
            prf("***\r<%s>I gonna get you SHORT <%d-%d> S2B=%s \r",cybname,(SHORT)ptr->coord.xcoord,(SHORT)ptr->coord.ycoord,spr("%ld",(LONG)ptr->speed2b));
            outwar(ALWAYS,usrn,0); */
            cyb_annoy(ptr, low_ship, 30, 5, 8);
            if (ptr->where == 0)
                shieldup(ptr, usrn);
        } else if (low_dist <= 3.0) {
            if (ptr->speed > d_topspeed)
                ptr->speed = d_topspeed;

            ptr->head2b = vector(&ptr->coord, &(wptr->coord));
            if (wptr->where == 1)
                ptr->speed2b = ((wptr->speed2b > d_topspeed) ? d_topspeed : (wptr->speed2b * 1.25));
            else
                ptr->speed2b = ((low_dist > .5) ? 990.0 : rndm(500.0));

            /* DEBUG
            prf("***\r<%s>I gonna get you SHORT <%d-%d> S2B=%s \r",cybname,(SHORT)ptr->coord.xcoord,(SHORT)ptr->coord.ycoord,spr("%ld",(LONG)ptr->speed2b));
            outwar(ALWAYS,usrn,0); */
            cyb_annoy(ptr, low_ship, 30, 5, 8);
            if (ptr->where == 0)
                shieldup(ptr, usrn);
        }
    }
}

VOID FUNC cyb_won(
WARSHP	*ptr,   /* ptr to Cyber who won     */
INT		usrn,   /* usernum of cyber who won */
WARSHP  *wptr)  /* ptr to ship cyber killed */
{
    (VOID) usrn; // usrn = usrn;
    (VOID) wptr; // wptr = wptr;
    ptr->cybmine   = (byte)255;
    ptr->speed2b   = 2000.0;
    ptr->cybupdate = 0;
}

VOID FUNC cyb_died(
WARSHP  *ptr,   /* ptr to Cyber who died        */
INT     usrn,   /* usernum of cyber who died    */
WARSHP  *wptr)  /* ptr to ship who killed cyber */
{
    (VOID) usrn; // usrn = usrn;
    (VOID) wptr; // wptr = wptr;
    ptr->status = GESTAT_AVAIL;
}

SHORT FUNC isquad(WARSHP *ptr) /* ptr to Cyber */
{
    return (shipclass[ptr->shpclass].tough_factor == CYB_TOUGH_1);
}
