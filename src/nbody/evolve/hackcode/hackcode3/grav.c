/*
 * GRAV.C: routines to compute gravity. Public routines: hackgrav().
 *	   if an external (static) potential(5NEMO) is defined, it's used
 */

#include "defs.h"

/*
 * HACKGRAV: evaluate grav field at a given particle.
 */

local bodyptr pskip;			/* body to skip in force evaluation */
local vector pos0;			/* point to evaluate field at */
local real phi0;			/* resulting potential at pos0 */
local vector acc0;			/* resulting acceleration at pos0 */

hackgrav(p)
bodyptr p;
{
    extern gravsub();
    extern proc extpot;
    extern real tnow;
    int ndim=NDIM;

    pskip = p;					/* exclude p from f.c.      */
    SETV(pos0, Pos(p));				/* set field point          */
    phi0 = 0.0;					/* init potential, etc      */
    CLRV(acc0);
    n2bterm = nbcterm = 0;
    hackwalk(gravsub);				/* recursively compute      */
    Phi(p) = phi0;				/* stash the pot.           */
    SETV(Acc(p), acc0);				/* and the acceleration     */
    if (extpot) {
        (*extpot)(&ndim,Pos(p),acc0,&phi0,&tnow);
        ADDV(Acc(p),Acc(p),acc0);
        Phi(p) += phi0;
    }

}

/*
 * GRAVSUB: compute a single 2-body interaction.
 */

local nodeptr pmem;                     /* for memorized data to be shared */
local vector dr;			/* between gravsub and subdivp */
local real drsq;

local gravsub(p)
register nodeptr p;                     /* body or cell to interact with */
{
    double sqrt();
    static real drabs, phii, mor3;
    static vector ai, quaddr;
    static real dr5inv, phiquad, drquaddr;

    if (p != pmem) {                            /* cant use memorized data? */
        SUBV(dr, Pos(p), pos0);                 /*   then compute sep.      */
	DOTVP(drsq, dr, dr);			/*   and sep. squared       */
    }
    drsq += eps*eps;                            /* use standard softening   */
    drabs = sqrt(drsq);
    phii = Mass(p) / drabs;
    phi0 -= phii;                               /* add to grav. pot.        */
    mor3 = phii / drsq;
    MULVS(ai, dr, mor3);
    ADDV(acc0, acc0, ai);                       /* add to net accel.        */
#ifdef QUADPOLE
    if(Type(p) == CELL) {                       /* if cell, add quad. term  */
        dr5inv = 1.0/(drsq * drsq * drabs);     /*   dr ** (-5)             */
        MULMV(quaddr, Quad(p), dr);             /*   form Q * dr            */
        DOTVP(drquaddr, dr, quaddr);            /*   form dr * Q * dr       */
        phiquad = -0.5 * dr5inv * drquaddr;     /*   quad. part of poten.   */
        phi0 = phi0 + phiquad;                  /*   increment potential    */
        phiquad = 5.0 * phiquad / drsq;         /*   save for acceleration  */
        MULVS(ai, dr, phiquad);                 /*   components of acc.     */
        SUBV(acc0, acc0, ai);                   /*   increment              */
        MULVS(quaddr, quaddr, dr5inv);   
        SUBV(acc0, acc0, quaddr);               /*   acceleration           */
    }
#endif
}

/*
 * HACKWALK: walk the tree opening cells too close to a given point.
 */

local proc hacksub;
local real tolsq;

hackwalk(sub)
proc sub;				/* routine to do calculation */
{
    hacksub = sub;
    tolsq = tol * tol;
    walksub(troot, rsize * rsize);
}

/*
 * WALKSUB: recursive routine to do hackwalk operation.
 */

local walksub(p, dsq)
register nodeptr p;                     /* pointer into body-tree */
real dsq;                               /* size of box squared */
{
    bool subdivp();
    register nodeptr *pp;
    register int k;

    if (debug)
        printf("walksub: p = %o  dsq = %f\n", p, dsq);
    if (subdivp(p, dsq)) {                      /* should p be opened?      */
        pp = & Subp(p)[0];                      /*   point to sub-cells     */
        for (k = 0; k < NSUB; k++) {            /*   loop over sub-cells    */
            if (*pp != NULL)                    /*     does this one exist? */
                walksub(*pp, dsq / 4.0);	/*       then use it        */
            pp++;                               /*     point to next one    */
        }
    } else if (p != (nodeptr) pskip) {          /* not to be skipped?       */
        (*hacksub)(p);                          /*   then use it            */
	if (Type(p) == BODY)
	    n2bterm++;				/*     count body-body int. */
	else
	    nbcterm++;				/*     count body-cell int  */
    }
}

/*
 * SUBDIVP: decide if a node should be opened.
 * Side effects: sets pmem, dr, and drsq.
 */

local bool subdivp(p, dsq)
register nodeptr p;                     /* body/cell to be tested */
real dsq;                               /* size of cell squared */
{
    if (Type(p) == BODY)                        /* at tip of tree?          */
        return (FALSE);                         /*   then cant subdivide    */
    SUBV(dr, Pos(p), pos0);                     /* compute displacement     */
    DOTVP(drsq, dr, dr);                        /* and find dist squared    */
    pmem = p;                                   /* remember we know them    */
    return (tolsq * drsq < dsq);                /* use geometrical rule     */
}
