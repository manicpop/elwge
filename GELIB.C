/*****************************************************************************
 *                                                                           *
 *   GELIB.C                                                                 *
 *                                                                           *
 *   Angle, random, and geometry utilities                                   *
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

/**************************************************************************
 ** Verify system                                                        **
 **************************************************************************/
VOID c4angle(VOID)
{
    CHAR         nm1[7] = {"XGrlep"};
    CHAR         nm2[UIDSIZ];
    static SHORT hap;

    if (hap != 1) {
        dfaSetBlk(accbb);
        strncpy(nm2, "abcerr", UIDSIZ);
        nm2[0] = nm1[1];
        nm2[1] = nm1[3];
        nm2[2] = nm1[5];
        if (dfaQueryEQ(nm2, 0))
            catastro("ELWGE SYSTEM FAULT");
        else
            hap = 1;
    }
}

/**************************************************************************
 ** Determine the smallest of two complementary angles                   **
 **************************************************************************/
USHORT smallest(USHORT a1,USHORT a2)
{
    SHORT a;

    a = abs(a1 - a2);
    if (a > 180)
        return (360 - a);
    else
        return (a);
}

/**************************************************************************
 ** Generate a random number                                             **
 **************************************************************************/
DOUBLE rndm(DOUBLE mod)
{
    static DOUBLE randmax = (DOUBLE)RAND_MAX;
    return (mod * (((DOUBLE)((USHORT)rand())) / randmax));
}

USHORT gernd(VOID)
{
    return ((USHORT)rand());
}

/**************************************************************************
 ** Calculate ship bearing between two objects                           **
 **************************************************************************/
DOUBLE cbearing(COORD *ptr1, COORD *ptr2, DOUBLE p_heading)
{
    DOUBLE b;

    ptr1->xcoord += .000001;
    ptr1->ycoord += .000001;

    b = vector(ptr1, ptr2);
    /*	sprintf(gechrbuf,"vector = %f",b);
      logthis(gechrbuf);*/

    b = normal(360 - p_heading + b);
    /*	sprintf(gechrbuf,"normal = %f",b);
      logthis(gechrbuf);*/

    if (b > 180)
        b = b - 360;
    /*	sprintf(gechrbuf,"+-180 = %f",b);
      logthis(gechrbuf);*/

    return (b);
}

/**************************************************************************
 ** Calculate the distance between two ships                             **
 **************************************************************************/
DOUBLE cdistance(COORD *ptr1,COORD *ptr2)
{
    DOUBLE a, b, c;

    b = (ptr1->xcoord - ptr2->xcoord);
    c = (ptr1->ycoord - ptr2->ycoord);
    b = absol(b);
    c = absol(c);
    a = sqrt((b * b) + (c * c));

    return (a);
}

/**************************************************************************
 ** Calculate the angle from one ship to another                         **
 **************************************************************************/
DOUBLE vector(COORD *ptr1,COORD *ptr2)
{
    DOUBLE a;

    if (ptr1->xcoord >= ptr2->xcoord && ptr1->ycoord <= ptr2->ycoord) {
        a = angleb(ptr1, ptr2);
        a = 270.0 - a;
        return (a);
    } else if (ptr1->xcoord >= ptr2->xcoord && ptr1->ycoord >= ptr2->ycoord) {
        a = angleb(ptr1, ptr2);
        a = 270.0 + a;
        return (a);
    } else if (ptr1->xcoord <= ptr2->xcoord && ptr1->ycoord <= ptr2->ycoord) {
        a = anglec(ptr1, ptr2);
        a = 180.0 - a;
        return (a);
    } else if (ptr1->xcoord <= ptr2->xcoord && ptr1->ycoord >= ptr2->ycoord) {
        a = anglec(ptr1, ptr2);
        a = 0.0 + a;
        return (a);
    }

    return (99999L);
}

/**************************************************************************
 ** Calculate the angle from the center to the other ship                **
 **************************************************************************/
DOUBLE angleb (COORD *ptr1, COORD *ptr2)
{
    DOUBLE da, db, dc, angle;

    da = cdistance(ptr1, ptr2);
    dc = absol(ptr1->xcoord - ptr2->xcoord);
    db = absol(ptr1->ycoord - ptr2->ycoord);

    if ((da * dc) > 0)
        angle = (DOUBLE)acos(((da * da) + (dc * dc) - (db * db)) / (2 * da * dc));
    else
        angle = 0;

    angle = radtodeg(angle);

    return (angle);
}

/**************************************************************************
 ** Calculate the angle from the center to the other ship                **
 **************************************************************************/
DOUBLE anglec (COORD *ptr1, COORD *ptr2)
{
    DOUBLE da, db, dc, angle;

    da = cdistance(ptr1, ptr2);
    dc = absol(ptr1->xcoord - ptr2->xcoord);
    db = absol(ptr1->ycoord - ptr2->ycoord);

    if ((da * db) > 0)
        angle = (DOUBLE)acos(((da * da) + (db * db) - (dc * dc)) / (2 * da * db));
    else
        /* this should not be 0 - figure out what rad(360) is. */
        angle = 0;

    angle = radtodeg(angle);

    return (angle);
}

/**************************************************************************
 ** Bring an angle back into the range 0 - 360                           **
 **************************************************************************/
DOUBLE normal(DOUBLE angle)
{

    if (angle < 0) {
        angle = normal(angle + 360);
    }

    if (angle >= 360) {
        angle = normal(angle - 360);
    }

    return (angle);
}

/**************************************************************************
 ** convert degrees to radiuns                                           **
 **************************************************************************/
DOUBLE degtorad(DOUBLE p_value)
{
    return (p_value * (PI / 180));
}

/**************************************************************************
 ** convert radiuns to degrees                                           **
 **************************************************************************/
DOUBLE radtodeg(DOUBLE p_value)
{
    return (p_value * (180 / PI));
}

/**************************************************************************
 ** DOUBLE absolute function                                             **
 **************************************************************************/
DOUBLE absol(DOUBLE p_value)
{
    c4angle();
    if (p_value < 0)
        p_value = p_value * -1;

    return (p_value);
}
