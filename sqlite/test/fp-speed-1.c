/*
** Performance testing of floating-point binary-to-decimal conversion for
** SQLite versus the standard library.
**
** To compile:
**
**    make sqlite3.c
**    gcc -Os -c sqlite3.c
**    gcc -I. -Os test/fp-speed-1.c sqlite3.o -ldl -lm -lpthread
**
** To run the test:
**
**    time ./a.out 0 10000000     <-- standard library
**    time ./a.out 1 10000000     <-- SQLite
**    time ./a.out 2 10000000     <-- SQLite with long-double disabled
*/
static double aVal[] = {
  -1.0163830486285643089e+063,
  +0.0049243807391586981e-019,
  +7.3818732407343994867e-095,
  +7.0678595219225717143e+000,
  +9.2807266319850025655e+120,
  +5.8871050861933394663e+135,
  -2.2998023621259662805e-168,
  -1.5903581924910847482e+181,
  +2.4313441816844978216e-001,
  -3.8290987328945399091e-147,
  +1.8787914062744001349e-009,
  +0.7270653560487146653e-014,
  +0.0639577697913183665e+107,
  +5.2292338879315861930e-103,
  +6.3747682672872231319e-021,
  +8.6972339538329106984e-129,
  -9.5074486051597691937e-026,
  -8.6480257845368753018e-005,
  -3.5657595775797470332e+017,
  -7.8323106179731761351e+161,
  +7.7813875274120800372e+077,
  -1.8739718928360196596e-050,
  +8.6898145915593357218e-051,
  +6.0566766359877837871e+002,
  +4.1703379217148160678e+199,
  +2.1283871288746651560e+150,
  -6.8395236083891810637e+128,
  -6.2114299763939529155e-147,
  -2.0753525742614637350e-149,
  +5.8727459803944290257e-007,
  +8.5888991062002101817e+010,
  +6.8624461031355917632e+026,
  -3.3053986756670905167e-075,
  -4.3596843152763444945e-108,
  +0.0834139520104099673e+098,
  -8.8581986548904222440e-192,
  -3.6622095428727698202e+038,
  -6.6965852297025063260e+005,
  +1.8204169347406488441e-054,
  +6.5234508038649000384e-065,
  +1.5923006018219011453e+083,
  +1.7362555291656389480e+018,
  +7.2875431976854785882e+160,
  +1.2835880105958926748e-146,
  +8.0516253320320819420e-113,
  +6.6324633399381145440e-030,
  -1.7126500070280062058e-149,
  +1.6995738341583712335e+042,
  +7.6048766923738663725e-112,
  +0.6159117235449455043e-004,
  +5.7544894355415943289e-135,
  +8.2970228592690581890e-023,
  -6.5531925360163067447e+020,
  +5.8321334606187030050e+120,
  +5.6557166509571891727e+095,
  +0.3322789708438408704e-114,
  -7.1210648776698686623e-050,
  -9.6721262526706343301e+179,
  -3.4583916571377395084e-106,
  +4.7896094323214750793e-172,
  -9.6926028040004137875e-056,
  +7.0683848275381385483e-198,
  -5.2970114182162961964e-007,
  -4.4287021200905393271e-170,
  +0.0728891155732808799e-189,
  -9.1855462025879447465e+175,
  +3.7294126234131007796e+015,
  +2.6857421882792719241e+003,
  -4.7070607333624685339e+039,
  +7.2175820768279334274e+136,
  -8.3678412534261163481e-115,
  +2.2174844304241822163e+019,
  +0.1949824588606861016e+112,
  -9.7334052955672071912e+151,
  -9.7793887766936999879e-142,
  -5.1561164587416931561e+139,
  -7.5048993577765174789e-022,
  +7.3556076568687784568e+107,
  -5.0681628575533599865e-144,
  +1.5209705642027747811e+165,
  -7.5989782535048296040e-101,
  +1.3654137203389775871e-016,
  -1.6441720554651372066e+087,
  -4.9042433196141125923e+000,
  -7.7063611961649130777e+118,
  +0.1699427460930766201e+174,
  +8.3374317849572216870e-145,
  -5.2355330480469580072e+081,
  -3.8510045942194147919e+141,
  -6.3513622544326339887e-147,
  +2.3869303484454428988e+049,
  +3.8352715871620360268e-165,
  -3.1263120493136887902e+035,
  -5.5794797002556490823e+051,
  -8.8109874479595604379e+142,
  -4.3727360120203216922e+070,
  -3.1109951189668539974e+170,
  -9.4841878031704268232e+011,
  -3.7398451668304407277e+067,
  +4.8984042008915959963e-091,
};
#define NN (sizeof(aVal)/sizeof(aVal[0]))

#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
  int i;
  int cnt;
  int fg;
  char zBuf[1000];

  if( argc!=3 ){
    fprintf(stderr, "Usage:  %s FLAG COUNT\n", argv[0]);
    return 1;
  }
  cnt = atoi(argv[2]);
  fg = atoi(argv[1]);

  switch( fg % 3 ){
    case 0: {
      printf("Doing %d calls to C-lib sprintf()\n", cnt);
      for(i=0; i<cnt; i++){
        sprintf(zBuf, "%.26g", aVal[i%NN]);
      }
      break;
    }
    case 1: {
      printf("Doing %d calls to sqlite3_snprintf()\n", cnt);
      for(i=0; i<cnt; i++){
        sqlite3_snprintf(sizeof(zBuf), zBuf, "%!.26g", aVal[i%NN]);
      }
      break;
    }
    case 2: {
      printf("Doing %d calls to sqlite3_snprintf() without long double\n", cnt);
      sqlite3_test_control(SQLITE_TESTCTRL_USELONGDOUBLE, 0);
      for(i=0; i<cnt; i++){
        sqlite3_snprintf(sizeof(zBuf), zBuf, "%!.26g", aVal[i%NN]);
      }
      break;
    }

  }
  return 0;
}
