#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/******
Algorithms for optimum psi voting.   Warren D. Smith, Nov. 2015.  
Compile with
   gcc -Wall -O6 CleanOptPRVote.c     (also -DNDEBUG if want to turn off asserts)
Run with
    time ./a.out
*******/

#define real   double
#define uint8  uint8_t
#define uint32 uint32_t
#define uint64 uint64_t
#define uint   unsigned int
#define ln(x) log(x)
#define Rand01()  drand48()
#define SeedRand(x)  srand48(x)

#define PI          3.14159265358979323846
#define EulerMasch  0.57721566490153286061
#define Zeta2       1.64493406684822643647   /*  PI^2 / 6  */
#define Zeta3       1.20205690315959428540
#define Zeta4       1.08232323371113819152   /*  PI^4 / 90  */
#define AbsPsiHalf  1.96351002602142347944   /*  -Psi(1/2) = EulerMasch+ln(4)  */
#define LN4         1.38629436111989061883   /*  ln(4)  */

/************
Simple algorithm to compute Psi(x) accurate to within +-1e-15
for any x>0 (except that the discreteness of the floating point computer-representable
subset of real numbers prevents any method from achieving that accuracy for very small x).
Worst(?) accuracy at x=0.25.
If one also wanted x<0 (which we, for election purposes, do not)
then one could use the reflection identity 
    Psi(x)=Psi(1-x)-PI*cot(PI*x)
to get there.
*****************/
real PsiAcc(real x){ // psi(x) = Gamma'(x) / Gamma(x).  We ASSUME x>0. 
  real p, y, r;
  assert(x>0.0);  
  if(x < 0.000286648){ return((Zeta2-x*(Zeta3-x*Zeta4))*x-EulerMasch-1.0/x); }
  for(y=x, p=0.0; y<14.9; y += 1.0){ p -= 1.0/y; }
  r = 0.5/y;
  p += ln(y) - r;
  r *= r;
  p -= r * (1/3.0 - r * (2/15.0 - r * (16/63.0 - r * (16/15.0 - r * 256/33.0))));
  return(p);
}

real FsimmonsAcc(real x){ // [Psi(x+1/2)-Psi(1/2)]/2.  Assumes x>=0.  Accuracy +-4e-16.
  real p, y, r;
  assert(x>=0.0);  
  for(y=x+0.5, p=AbsPsiHalf; y<14.9; y += 1.0){ p -= 1.0/y; }
  r = 0.5/y;
  p += ln(y) - r;
  r *= r;
  p -= r * (1/3.0 - r * (2/15.0 - r * (16/63.0 - r * (16/15.0 - r * 256/33.0))));
  return(0.5*p);
}

#define C1 0.244417244017665
#define C2 0.133307727351213
#define C3 0.333333316834594
#define C4 3.79344885805465e-12
/************
Simple algorithm to compute Psi(x) accurate to within +-9e-12
for any x>=0.0001 and accurate to within +-7e-12 for any x>=0.0003.
Worst(?) accuracy at x=0.644504 and x=0.000212217556989 and x=0.01.
PsiFast for uniform random x in [0,5] runs in 120 nanoseconds (240 cycles)
average time on 2GHz iMac.
*****************/
real PsiFast(real x){ // psi(x) = Gamma'(x) / Gamma(x).  We ASSUME x>0.
  real p, y, r;
  if(x < 0.00017){ return((Zeta2-Zeta3*x)*x-EulerMasch-1.0/x); } 
  p=C4;
  for(y=x; y<7.01; y += 1.0){ p = p - 1.0/y; }
  r = 0.5/y;
  p = p + ln(y) - r;
  r = r*r;
  p = p - r*((C1*r-C2)*r+C3);
  return(p);
}

real FsimmonsFast(real x){ // psi(x) = Gamma'(x) / Gamma(x).  We ASSUME x>0.  Accuracy +-4e-12.
  real p, y, r;
  assert(x>=0.0);
  p=C4+AbsPsiHalf;
  for(y=x+0.5; y<7.01; y += 1.0){ p = p - 1.0/y; }
  r = 0.5/y;
  p = p + ln(y) - r;
  r = r*r;
  p = p - r*((C1*r-C2)*r+C3);
  return(0.5*p);
}
#undef C1
#undef C2
#undef C3
#undef C4

real Ftab[512];
/*********
For even greater accuracy, the Ftab could be precomputed to accuracy exceeding
full machine accuracy, and simply typed in.  For example, here are the values of 
Fsimmons(j/9) for  j=0,1,2,...,9  to very high accuracy, and values for greater j
could be got using the shift-by-1 recurrence F(x+1)=F(x)+1/(2*x+1):
          j     Fsimmons(j/9)
*******************************/
#define FK0     0
#define FK1     0.231344398261451616064873648180400480003643139525917758784844
#define FK2     0.402461181265483029158748201667014700281522437624437363175421
#define FK3     0.536390306674581119399124765539722520727431767702493342876392
#define FK4     0.645501525962868133052710550655173858364905640019988029663089
#define FK5     0.737072875488692532109319255818931414457565419789726902211828
#define FK6     0.815691260323254343508007379076488922301332395563382479521565
#define FK7     0.884406562797259071986922242399804666842510686827450062579203
#define FK8     0.945335576927708646448801110074017809479387778256144709861120
#define FK9     1
real FK[] = {FK0,FK1,FK2,FK3,FK4,FK5,FK6,FK7,FK8,FK9};

void CreateFtable(uint TOPSCORE){
  uint j;
  real scal=1.0/TOPSCORE;
  for(j=0; j<512; j++){
    Ftab[j] = FsimmonsAcc(j*scal);
  }
}

/*****
record error for  doubling  PsiFast test:  1.30416e-11 at x=10.000033540032444
record error for  doubling  PsiFast test:  4.65661e-10 at x=0.000000151773634
record error for shift-by-1 PsiFast test:  1.20299e-11 at x=7.010000308187760
record error for shift-by-1 PsiFast test:  5.05734e-10 at x=0.000000115166010
record error for  PsiAcc-PsiFast    test:  7.27596e-12 at x=0.000170964996759
record error for  PsiAcc-PsiFast    test:  7.27596e-12 at x=0.000026208026647
record error for  PsiAcc-PsiFast    test:  8.18545e-12 at x=0.000212217556989
Psi function testing done (300000000 tests of domains x>0 and x>0.0001)
time 73 seconds

record error for  doubling  PsiFast test:  1.27471e-11 at x=0.005113554019236
record error for  doubling  PsiFast test:  1.27471e-11 at x=0.005046858460489
record error for shift-by-1 PsiFast test:    1.106e-12 at x=0.000382260723211
record error for shift-by-1 PsiFast test:  1.43818e-12 at x=0.000463437671663
record error for shift-by-1 PsiFast test:  1.49047e-12 at x=0.000318528751742
record error for  PsiAcc-PsiFast    test:  6.92069e-12 at x=0.010153554194734
record error for  PsiAcc-PsiFast    test:   6.9349e-12 at x=0.010033532819176
Psi function testing done (300000000 tests of domain 0.0003<x<5.0)
time 95 seconds
*****************/
void TestPsi(uint T, real L, real U){ //T+1 tests for x in domain L<x<U+L
  uint j;
  real p,x,e1,e2,e3,rec1=0.0,rec2=0.0,rec3=0.0;
  printf("Psi function accuracy tests:\n");
  printf("j   FsimmonsAcc(j/9)     error        FsimmonsFast(j/9)    error\n");
  for(j=0; j<=9; j++){
    printf("%d  %19.16f %12g  %19.16f %12g\n", 
	   j, FsimmonsAcc(j/9.0), FsimmonsAcc(j/9.0)-FK[j],
	   FsimmonsFast(j/9.0), FsimmonsFast(j/9.0)-FK[j] );
  }
  printf("\n");
  for(j=0; j<=T; j++){
    x = U*Rand01() + L;
    p = PsiFast(x);
    e1 = fabs(p + 1.0/x - PsiFast(x+1.0));
    e2 = fabs(PsiFast(2*x) - 0.5*(PsiFast(x+0.5) + p + LN4));
    e3 = fabs(p - PsiAcc(x));
    if(e1>=rec1){
      rec1=e1; 
      printf("record error for shift-by-1 PsiFast test: %12g at x=%.15f\n", rec1, x);
    }
    if(e2>=rec2){
      rec2=e2; 
      printf("record error for  doubling  PsiFast test: %12g at x=%.15f\n", rec2, x);
    }
    if(e3>=rec3){
      rec3=e3; 
      printf("record error for  PsiAcc-PsiFast    test: %12g at x=%.15f\n", rec3, x);
    }
  }
  printf("Psi function testing done (%u tests of domain %g<x<%g)\n", j*3, L, L+U);
}

/****************
Carries out V-voter C-candidate W-winner election, 0<W<C, 0<V.
Algorithm employs "algorithm R" from Knuth volume 4's 7.2.1.3 to generate all W-element subsets
of the C candidates.
Its mission is, from among all W-element subsets of the C candidates, to find the one maximizing
   Quality = SUM(v=1..V) Psi( Delta + SUM(j=1..W) score[winner[j]*V+k] )
Here Delta is a positive constant pre-chosen by the voting system designer (1/2<=Delta<=1
is recommended and conjecturally Delta=1/2 is the best choice), and Psi(x) is the 
digamma function.  Score[j*V+k] is the score awarded to candidate j on voter k's ballot,
where 0<=k<V and 0<=j<C.
This routine ASSUMES all these scores have been prescaled to lie within the 
real interval [0, 1] where 0 is the score a voter would award to a hated candidate 
and 1 to an excellent candidate.
For voters who do not score some candidate, that blank entry of the score array 
must be filled in with that candidate's average score (among those voters who did score him). 
In that way the abstaining voters will leave that candidate's average unaffected. 
The value Q of the quality function for the optimum winner-set, is returned.
   The indices of the winning candidates are returned in the array winner[0..W-1]. 
Each such index lies in the integer interval [0, C-1]. The algorithm employs the 
real array S[0..V-1] and the uint (unsigned integer) array x[0..W], as workspace,
and the real array score[0..C*V-1] as input.
   Uses PsiFast(x).
*************/
real OptVote(uint V, uint C, uint W, real score[], real Delta, //the inputs
	     uint x[], real S[],   //used as workspace
	     uint winner[]){   //winner[0..W-1] and the returned value are the outputs
  uint k, j, prv=0, scc=0;
  uint64 ctr=0;
  real A, Q, Qrecord = -HUGE;
  assert(0.0<Delta);  assert(Delta<=2.0);
  assert(0<W);  assert(W<C);  assert(0<V);
  for(j=0; j<W; j++){ x[j] = j; } x[j]=C; //initial subset: {0,1,2,...,W-1}.
  for(k=0; k<V; k++){ 
    A = Delta; 
    for(j=0; j<C; j++){  
      assert(0<=score[j*V+k] && score[j*V+k]<=1.0);
      if(j<W){ A += score[j*V+k]; }
    }
    S[k] = A;
  }
 R2:  
  ctr++;  Q = 0.0;  scc *= V;  prv *= V;
  for(k=0; k<V; k++){  //inner loop
    S[k] += score[scc+k] - score[prv+k];  
    Q += FsimmonsFast(S[k]);
  }
  if(Q > Qrecord){ //winner set with highest quality yet seen:
    Qrecord = Q;   //print Q then the indices of the winners:
    printf("%16.8f:", Q);
    for(k=0; k<W; k++){ winner[k]=x[k]; printf(" %2d", x[k]); }
    printf("\n");
  }
  if(W&1){ //W is odd
    if(x[0]+1 < x[1]){ prv=scc=x[0]; scc++; x[0]=scc; goto R2; }else{ j=1; goto R4; }
  }else{ //W is even
    if(  x[0] > 0   ){ prv=scc=x[0]; scc--; x[0]=scc; goto R2; }else{ j=1; goto R5; }
  }
 R4: 
  if(x[j] >= j+1){ prv=x[j]; x[j]=x[j-1]; scc=j-1; x[j-1]=scc; goto R2; }else{ j++; }
 R5:
  if(x[j]+1 < x[j+1]){ prv=x[j-1]; x[j-1]=x[j]; scc=x[j]; scc++; x[j]=scc; goto R2; }
  else{ j++; if(j<W){ goto R4; }}
  printf("Exhaustively searched %llu subsets.\n", ctr);
  return(Qrecord);
}

#define BLANKSCORE 10
/****************
Carries out V-voter C-candidate W-winner election, 0<W<C, 0<V.
Algorithm employs "algorithm R" from Knuth volume 4's 7.2.1.3 to generate all W-element subsets
of the C candidates.
Its mission is, from among all W-element subsets of the C candidates, to find the one maximizing
   Quality = SUM(v=1..V) Fsimmons( (1/TOPSCORE) * SUM(j=1..W) score[winner[j]*V+k] )
Score[j*V+k] is integer score awarded to candidate j on voter k's ballot,
where 0<=k<V and 0<=j<C, and this score must lie in the integer interval [0, TOPSCORE].
Here 0 is the score a voter would award to a hated candidate and TOPSCORE to 
an excellent candidate.
   Uses precomputed table of Fsimmons(x) which causes this to be about 50X faster
than OptVote() on my machine.  Specifically will run in about binomial(C,W)*V*4.5nanosec.
That precomputation could be done by  CreateFtable(TOPSCORE).
Unfortunately with this routine, voters MUST score every candidate, blanks forbidden.
[The problem is that blank entries of the score array would have to be filled in with that 
candidate's average score (among those voters who did score him), which usually
would not be an integer, in which case the lookup table will not work.]
The value Q of the quality function for the optimum winner-set, is returned.
   The indices of the winning candidates are returned in the array winner[0..W-1]. 
Each such index lies in the integer interval [0, C-1]. The algorithm employs the 
uint (unsigned integer) arrays S[0..V-1] and x[0..W], as workspace,
and the uint array score[0..C*V-1] as input.
*************/
real FastOptVote(uint V, uint C, uint W, uint8 score[],  //the inputs
	     uint x[], int S[],   //used as workspace
             uint SumScore[],   //average score for candidate j for j=0..C-1
             uint NonBlankCount[],   //number of nonblank scores for candidate j for j=0..C-1
	     uint winner[]){   //winner[0..W-1] and the returned value are the outputs
  uint sum, A, k, j, ct, prv=0, scc=0, hasblank=0;
  uint64 ctr=0;
  real Q, Qrecord = -HUGE;
  assert(0<W);  assert(W<C);  assert(0<V);
  for(j=0; j<W; j++){ x[j] = j; } x[j]=C; //initial subset: {0,1,2,...,W-1}.
  for(k=0; k<V; k++){ 
    A = 0;
    for(j=0; j<C; j++){  
      if(j<W){ A += score[j*V+k]; }
    }
    S[k] = A;
  }
  for(j=0; j<C; j++){ 
    sum = 0; ct = 0;
    for(k=0; k<V; k++){ 
      if( score[j*V+k]<BLANKSCORE ){ sum += score[j*V+k];  ct++; }else{ hasblank=1; }
    }
    SumScore[j] = sum;   NonBlankCount[j] = ct;
  }
  if(hasblank){ return(0); }  //this code cannot handle blank scores, so early exit
 R2:  
  ctr++;  Q = 0.0;  scc *= V;  prv *= V;
  for(k=0; k<V; k++){  //inner loop
    S[k] += score[scc+k] - score[prv+k];  
    assert(S[k]>=0);
    assert(S[k]<512);
    Q += Ftab[ S[k] ];
  }
  if(Q > Qrecord){ //winner set with highest quality yet seen:
    Qrecord = Q;   //print Q then the indices of the winners:
    printf("%16.8f:", Q);
    for(k=0; k<W; k++){ winner[k]=x[k]; printf(" %2d", x[k]); }
    printf("\n");
  }
  if(W&1){ //W is odd
    if(x[0]+1 < x[1]){ prv=scc=x[0]; scc++; x[0]=scc; goto R2; }else{ j=1; goto R4; }
  }else{ //W is even
    if(  x[0] > 0   ){ prv=scc=x[0]; scc--; x[0]=scc; goto R2; }else{ j=1; goto R5; }
  }
 R4: 
  if(x[j] >= j+1){ prv=x[j]; x[j]=x[j-1]; scc=j-1; x[j-1]=scc; goto R2; }else{ j++; }
 R5:
  if(x[j]+1 < x[j+1]){ prv=x[j-1]; x[j-1]=x[j]; scc=x[j]; scc++; x[j]=scc; goto R2; }
  else{ j++; if(j<W){ goto R4; }}
  printf("Exhaustively searched %llu subsets.\n", ctr);
  return(Qrecord);
}

main(){
  uint x[400+2], j, k, winner[9], SS[29], NBC[29];
  uint8 Iscore[400*29];
  int IS[400];
  real Q;
  SeedRand(161703501943);
  TestPsi(999999, 0.0003, 5.0);
  CreateFtable(9);
  winner[8]=0;

  for(j=0; j<400*29; j++){ Iscore[j] = Rand01()*10; }
  printf("\n400-voter, 29-canddt, 8-winner election (random integer scores 0..9):\n");
  printf("     QUALITY       %d-WINNER-SET\n", 8);
  fflush(stdout);
  Q = FastOptVote(400, 29, 8, Iscore, x, IS, SS, NBC, winner);
  printf("binomial(29,8)=4292145.\n\n");
  printf("Canddt   SumScore   NonBlankCount   AvgScore   Won?\n");
  for(j=0, k=0; j<29; j++){ 
    printf("%2d   %8u  %8u             %.4f     %c\n",
	   j, SS[j], NBC[j], SS[j]/(real)NBC[j], j==winner[k]?'Y':'N' );
    if(j==winner[k]){ k++; }
  }
  printf("final winner set ");
  for(j=0; j<8; j++){ printf("%d ", winner[j]); }
  printf("quality=%.15f\n", Q);
}
