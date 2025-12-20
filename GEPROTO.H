/*****************************************************************************
 *                                                                           *
 *   GEPROTO.H                                                               *
 *                                                                           *
 *   Galactic Empire Function Prototypes                                     *
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

/* GEMAIN.C ****************************************************************/

VOID EXPORT init__galemp(VOID);
VOID FUNC dummy(VOID);
VOID FUNC iniwara(VOID);
SHORT FUNC gelogon(VOID);
VOID FUNC gedelete(CHAR *uid);
VOID FUNC gedeletea(CHAR *uid);
VOID FUNC gemidnight(VOID);
VOID FUNC calc_networth(VOID);
ULONG FUNC value_pl(VOID);
SHORT FUNC warlof(VOID);
VOID FUNC warhup(VOID);
VOID FUNC clswar(VOID);
SHORT FUNC galemp(VOID);
VOID FUNC outwar(SHORT filter , INT exclude_usrn , USHORT freq);
SHORT FUNC gepdb(SHORT func , CHAR *usrname , SHORT shipnum , WARSHP *geptr);
SHORT FUNC geudb(SHORT func , CHAR *usrname , WARUSR *geptr);
SHORT FUNC gesdb(SHORT func , PKEY *sect , GALSECT *geptr);
SHORT FUNC getplanetdat(INT usrn);
VOID FUNC load_team_tab(VOID);
VOID FUNC update_team_tab(VOID);
VOID FUNC plarti(VOID);
VOID FUNC plabld(VOID);
VOID FUNC warrti(VOID);
VOID FUNC autorti(VOID);
VOID FUNC warrti2(VOID);
VOID FUNC warrti3(VOID);
VOID FUNC outprfge(SHORT class , INT shpno); // RH
VOID FUNC outsect(SHORT filter , COORD *coordptr , INT exclude_usrn , USHORT freq);
VOID FUNC outrange(SHORT filter , COORD *coordptr);
SHORT FUNC ingegame(SHORT shpno);
VOID FUNC geshocst(SHORT opt , CHAR *str);
SHORT FUNC mnu_main(VOID);
SHORT FUNC mnu_main_ans(VOID);
SHORT FUNC mnu_fightsub(VOID);
SHORT FUNC mnu_admenu1(VOID);
SHORT mnu_admenu1a(VOID);
SHORT FUNC mnu_admenu2(VOID);
SHORT FUNC mnu_admenu2b(VOID);
SHORT FUNC mnu_admenu2e(VOID);
SHORT FUNC mnu_admenu2f1(VOID);
SHORT FUNC mnu_admenu2f2(VOID);
SHORT FUNC mnu_admenu2f3(VOID);
SHORT FUNC mnu_admenu2f4(VOID);
SHORT FUNC mnu_admenu2h(VOID);
SHORT FUNC mnu_admenu2i(VOID);
SHORT FUNC mnu_admenu2j(VOID);
SHORT FUNC mnu_choosesh(VOID);
SHORT mnu_menug(VOID);
SHORT FUNC mnu_menug1(VOID);
SHORT FUNC mnu_menug2(VOID);
VOID FUNC disp_main_menu(VOID);
VOID FUNC disp_menu_d(VOID);
VOID FUNC update_items(VOID);
VOID FUNC optdisp(VOID);
VOID FUNC stshdlr(VOID);

/* GEFUNCS.C ***************************************************************/

VOID FUNC lookupshp(VOID);
VOID FUNC tossingegame(VOID);
SHORT FUNC initshp(CHAR *userid , SHORT type);
SHORT FUNC initusr(CHAR *userid);
SHORT FUNC findships(VOID);
VOID FUNC selectship(VOID);
VOID FUNC repairship(WARSHP *ptr , INT usrn); // RH
VOID FUNC rotateship(WARSHP *ptr , INT usrn); // RH
VOID FUNC accel(WARSHP *ptr , INT usrn);
VOID FUNC hyperspace(WARSHP *ptr , INT usrn , SHORT flag);
VOID FUNC moveship(WARSHP *ptr , INT usrn);
VOID FUNC telezip(WARSHP *ptr , INT usrn);
VOID FUNC gravity(WARSHP *ptr , INT usrn);
VOID FUNC checkdist(WARSHP *ptr , INT usrn);
VOID FUNC refresh(WARSHP *ptr , INT usrn);
VOID FUNC checkdam(WARSHP *ptr , INT usrn);
VOID FUNC killem(WARSHP *ptr , INT usrn);
VOID FUNC recharge(WARSHP *ptr , INT usrn);
VOID FUNC fluxstat(WARSHP *ptr , INT usrn);
VOID FUNC shieldstat(WARSHP *ptr , INT usrn);
VOID FUNC cloakstat(WARSHP *ptr , INT usrn);
VOID FUNC checkmines(VOID);
SHORT FUNC useenergy(WARSHP *ptr , INT usrn , SHORT amount);
VOID FUNC checktm(WARSHP *ptr , INT usrn);
VOID FUNC acctm(WARSHP* ptr, INT usrn, SHORT mt, INT l_channel);
VOID FUNC cleartm(INT l_channel);
VOID FUNC fireion(WARSHP *ptr , INT usrn);
VOID FUNC destruct(WARSHP *ptr , INT usrn);
SHORT FUNC valpcnt(CHAR *ptr , USHORT minnum , USHORT maxnum);
SHORT FUNC valdegree(CHAR *ptr);
VOID FUNC randamage(WARSHP *ptr , INT usrn);
USHORT FUNC pdamage(WARSHP *wptr , DOUBLE dist , SHORT foc);
VOID FUNC setsect(WARSHP *ptr);
VOID FUNC movecoord(COORD *pointb , COORD *pointa);
SHORT FUNC samesect(COORD *pointb , COORD *pointa);
SHORT FUNC genearas(CHAR *str1 , CHAR *str2);
SHORT FUNC mailscan(CHAR *userid , SHORT class);
SHORT mailread(CHAR *userid , SHORT class);
VOID FUNC mailit(SHORT flag);
SHORT FUNC sendit(VOID);
VOID FUNC prf2tx(VOID);
SHORT FUNC sendgemsg(GEMESSAGE *msgptr);
VOID FUNC shieldup(WARSHP *wptr , INT usrn);
VOID FUNC shielddn(WARSHP *wptr , INT usrn);
SHORT FUNC shieldhit(WARSHP *wptr , INT usrn , SHORT dam);
VOID FUNC shieldrep(WARSHP *wptr , INT usrn);
VOID FUNC shieldchg(WARSHP *wptr , INT usrn);
VOID FUNC charge(WARSHP *wptr , SHORT *max , SHORT *pct);
SHORT FUNC chkweight(WARSHP *wptr , SHORT itm , LONG amt);
LONG FUNC calcweight(WARSHP *wptr);
CHAR FUNC shpltr(INT usrn , SHORT ship);
CHAR *FUNC username(WARSHP *ptr);
VOID FUNC logthis(CHAR *str);
WARUSR *FUNC warusroff(INT usrn);
WARSHP *FUNC warshpoff(INT usrn);
DOUBLE FUNC ton_fact(WARSHP *ptr , DOUBLE damfact);
CHAR *FUNC showarp(DOUBLE l_speed);

/* GECMDS.C ****************************************************************/

struct cmd *FUNC gesearch(CHAR *ptr , struct cmd tab [], SHORT len);
VOID FUNC gwar(VOID);
VOID FUNC warnop(VOID);
VOID FUNC cmd_mailtest(VOID);
VOID FUNC cmd_gehelp(VOID);
VOID FUNC cmd_impulse(VOID);
VOID FUNC cmd_warp(VOID);
VOID FUNC cmd_rotate(VOID);
VOID FUNC cmd_flux(VOID);
VOID FUNC cmd_orbit(VOID);
VOID FUNC cmd_phas(VOID);
VOID FUNC firep(WARSHP *ptr , INT usrn);
VOID FUNC firehp(WARSHP *ptr , INT usrn);
VOID FUNC cmd_torp(VOID);
VOID FUNC torp(WARSHP *ptr , INT usrn , SHORT shpnum);
VOID FUNC cmd_missl(VOID);
SHORT FUNC lockon(WARSHP *ptr , SHORT type , SHORT ship , INT usrn);
SHORT FUNC findshp(CHAR *ptr , SHORT type);
VOID FUNC zaphim(WARSHP *ptr , INT usrn);
VOID FUNC cmd_decoy(VOID);
VOID FUNC cmd_jammer(VOID);
VOID FUNC jam(WARSHP *ptr , INT usrn);
VOID FUNC cmd_zipper(VOID);
VOID FUNC zip(WARSHP *ptr , INT usrn);
VOID FUNC cmd_mine(VOID);
SHORT FUNC laymine(WARSHP *ptr , INT usrn , SHORT timer);
VOID FUNC cmd_send(VOID);
VOID FUNC cmd_freq(VOID);
VOID FUNC cmd_report(VOID);
VOID FUNC damstr(SHORT damage);
VOID FUNC cmd_scan(VOID);
VOID FUNC scan_sh(VOID);
VOID FUNC scan_pl(VOID);
VOID FUNC scan_ra(VOID);
VOID FUNC scan_se(VOID);
VOID FUNC scan_lo(VOID);
VOID scan_hy(VOID);
VOID FUNC update_scantab(WARSHP *ptr , INT usrn);
VOID FUNC pick_letter(SCANTAB *ptr);
VOID FUNC map_planets(VOID);
VOID FUNC clearmap(VOID);
VOID FUNC printmap(VOID);
VOID FUNC printmapfull(VOID);
USHORT FUNC coord2(DOUBLE dcoord);
SHORT FUNC coord1(DOUBLE dcoord);
VOID FUNC cmd_shields(VOID);
VOID FUNC cmd_cloak(VOID);
VOID FUNC cmd_transfer(VOID);
VOID trans_down(SHORT item);
VOID trans_up(SHORT item);
VOID FUNC cmd_abandon(VOID);
VOID FUNC cmd_admin(VOID);
VOID FUNC cmd_attack(VOID);
SHORT FUNC attack_men(ULONG num);
SHORT FUNC attack_fig(ULONG num);
VOID FUNC call_4_help(SHORT send_spy_mail , SHORT won);
VOID FUNC wonplnt(VOID);
VOID FUNC cmd_geroster(VOID);
VOID FUNC cmd_planet(VOID);
VOID FUNC cmd_sell(VOID);
VOID FUNC sell(SHORT item);
VOID FUNC cmd_buy(VOID);
VOID FUNC cmd_price(VOID);
VOID FUNC buy(SHORT item);
LONG FUNC amt4sale(SHORT item);
LONG FUNC price(USHORT item , ULONG amt);
VOID FUNC cmd_maint(VOID);
VOID FUNC cmd_new(VOID);
VOID FUNC cmd_sysop(VOID);
VOID done(VOID);
VOID FUNC cmd_rename(VOID);
VOID FUNC cmd_destruct(VOID);
VOID FUNC cmd_abort(VOID);
VOID FUNC cmd_lock(VOID);
VOID FUNC cmd_navigate(VOID);
VOID FUNC cmd_who(VOID);
VOID FUNC cmd_displ(VOID);
VOID FUNC cmd_set(VOID);
VOID FUNC cmd_team(VOID);
VOID FUNC badfmt(SHORT msg);
CHAR *FUNC teamname(WARUSR *ptr);
VOID FUNC cmd_clear(VOID);
VOID FUNC ansifunc(SHORT func);
VOID FUNC cmd_data(VOID);
VOID FUNC scan_data1(VOID);
VOID FUNC scan_data2(VOID);
CHAR *FUNC gedots(SHORT numdots);
VOID FUNC cmd_spy(VOID);
VOID FUNC cmd_jettison(VOID);
VOID FUNC jettison(SHORT item);

/* GECYBS.C ****************************************************************/

VOID FUNC cyb_init(WARSHP *ptr , INT usrn , SHORT class);
VOID FUNC cyb_lives(WARSHP *ptr , INT usrn);
SHORT FUNC notclaimed(WARSHP *ptr , INT usrn);
VOID FUNC cyb_annoy(WARSHP *ptr , INT usrn , SHORT rnd , SHORT first , SHORT last);
SHORT FUNC cybwhoops(WARSHP *ptr , INT zothusn);
SHORT FUNC gebemean(WARSHP *ptr , INT usrn);
VOID FUNC db_update(WARSHP *ptr , INT usrn);
VOID FUNC cyb_attack(WARSHP *ptr , INT usrn , WARSHP *wptr , INT zothusn);
VOID FUNC cyb_lay_decoys(WARSHP *ptr , INT zothusn);
VOID FUNC cyb_check_damage(WARSHP *ptr , INT usrn);
VOID FUNC cyb_check_lockon(WARSHP *ptr , INT usrn);
VOID FUNC cyb_won(WARSHP *ptr , INT usrn , WARSHP *wptr);
VOID FUNC cyb_died(WARSHP *ptr , INT usrn , WARSHP *wptr);
SHORT FUNC isquad(WARSHP *ptr);

/* GEDROIDS.C **************************************************************/

VOID FUNC  droid_init(WARSHP *ptr, INT usrn, SHORT class);
VOID FUNC droid_lives(WARSHP *ptr , INT usrn);
VOID FUNC droid_annoy(WARSHP *ptr , INT usrn , SHORT rnd , SHORT first , SHORT last);
VOID FUNC droid_act_class_10(WARSHP *ptr, INT usrn);
VOID FUNC droid_act_class_11(WARSHP *ptr, INT usrn);
VOID FUNC droid_act_class_12(WARSHP *ptr, INT usrn);
VOID FUNC droid_won(WARSHP *ptr, INT usrn, WARSHP *wptr);
VOID FUNC droid_died(WARSHP *ptr, INT usrn, WARSHP *wptr);
SHORT FUNC missl_attached(WARSHP *ptr , INT usrn);

/* GEPLANET.C **************************************************************/

VOID FUNC  check_spy(VOID);
VOID FUNC multiply(VOID);
SHORT FUNC getsector(COORD *sect);
SHORT FUNC xgetsector(COORD *sect , SHORT wormy);
VOID FUNC build_plan_1(SHORT idx);
VOID FUNC build_plan_2(SHORT idx);
VOID FUNC build_other(SHORT idx);
VOID FUNC build_worm(SHORT idx);
SHORT FUNC getplanet(COORD *sect , SHORT plnt);
SHORT FUNC neutral(COORD *coord);

/* GELIB.C *****************************************************************/

VOID   c4angle(VOID);
USHORT smallest(USHORT a1 , USHORT a2);
DOUBLE rndm(DOUBLE mod);
USHORT gernd(VOID);
DOUBLE cbearing(COORD *ptr1 , COORD *ptr2 , DOUBLE heading);
DOUBLE cdistance(COORD *ptr1 , COORD *ptr2);
DOUBLE vector(COORD *ptr1 , COORD *ptr2);
DOUBLE angleb(COORD *ptr1 , COORD *ptr2);
DOUBLE anglec(COORD *ptr1 , COORD *ptr2);
DOUBLE normal(DOUBLE angle);
DOUBLE degtorad(DOUBLE value);
DOUBLE radtodeg(DOUBLE value);
DOUBLE absol(DOUBLE value);

#undef P
