/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _CALC_H_RPCGEN
#define _CALC_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct operands {
	double first_value;
	double second_value;
};
typedef struct operands operands;

#define CALC 55555555
#define VERSION 100

#if defined(__STDC__) || defined(__cplusplus)
#define ADD 1
extern  double * add_100(operands *, CLIENT *);
extern  double * add_100_svc(operands *, struct svc_req *);
#define SUB 2
extern  double * sub_100(operands *, CLIENT *);
extern  double * sub_100_svc(operands *, struct svc_req *);
#define MULTIPLY 3
extern  double * multiply_100(operands *, CLIENT *);
extern  double * multiply_100_svc(operands *, struct svc_req *);
#define DIVIDE 4
extern  double * divide_100(operands *, CLIENT *);
extern  double * divide_100_svc(operands *, struct svc_req *);
extern int calc_100_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define ADD 1
extern  double * add_100();
extern  double * add_100_svc();
#define SUB 2
extern  double * sub_100();
extern  double * sub_100_svc();
#define MULTIPLY 3
extern  double * multiply_100();
extern  double * multiply_100_svc();
#define DIVIDE 4
extern  double * divide_100();
extern  double * divide_100_svc();
extern int calc_100_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_operands (XDR *, operands*);

#else /* K&R C */
extern bool_t xdr_operands ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_CALC_H_RPCGEN */
