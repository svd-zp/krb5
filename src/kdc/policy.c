/*
 * $Source$
 * $Author$
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 *
 * Policy decision routines for KDC.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_policy_c[] =
"$Id$";
#endif	/* !lint & !SABER */

#include <krb5/copyright.h>


#include <krb5/krb5.h>
#include <krb5/kdb.h>

#include "kdc_util.h"

/*ARGSUSED*/
krb5_boolean
against_postdate_policy(fromtime)
krb5_timestamp fromtime;
{
    return FALSE;
}

krb5_boolean
against_flag_policy_as(request)
const register krb5_kdc_req *request;
{
    if (isflagset(request->kdc_options, KDC_OPT_FORWARDED) ||
	isflagset(request->kdc_options, KDC_OPT_PROXY) ||
	isflagset(request->kdc_options, KDC_OPT_RENEW) ||
	isflagset(request->kdc_options, KDC_OPT_VALIDATE) ||
	isflagset(request->kdc_options, KDC_OPT_ENC_TKT_IN_SKEY))
	return TRUE;			/* against policy */

    return FALSE;			/* not against policy */
}

krb5_boolean
against_flag_policy_tgs(request, ticket)
const register krb5_kdc_req *request;
const register krb5_ticket *ticket;
{

    if (((isflagset(request->kdc_options, KDC_OPT_FORWARDED) ||
	  isflagset(request->kdc_options, KDC_OPT_FORWARDABLE)) &&
	 !isflagset(ticket->enc_part2->flags,
		TKT_FLG_FORWARDABLE)) || /* TGS must be forwardable to get
					    forwarded or forwardable ticket */

	((isflagset(request->kdc_options, KDC_OPT_PROXY) ||
	  isflagset(request->kdc_options, KDC_OPT_PROXIABLE)) &&
	 !isflagset(ticket->enc_part2->flags,
		TKT_FLG_PROXIABLE)) ||	/* TGS must be proxiable to get
					   proxiable ticket */

	((isflagset(request->kdc_options, KDC_OPT_ALLOW_POSTDATE) ||
	  isflagset(request->kdc_options, KDC_OPT_POSTDATED)) &&
	 !isflagset(ticket->enc_part2->flags,
		TKT_FLG_MAY_POSTDATE)) || /* TGS must allow postdating to get
					   postdated ticket */
	 
	(isflagset(request->kdc_options, KDC_OPT_VALIDATE) &&
	 !isflagset(ticket->enc_part2->flags,
		TKT_FLG_INVALID)) || 	/* can only validate invalid tix */

	((isflagset(request->kdc_options, KDC_OPT_RENEW) ||
	  isflagset(request->kdc_options, KDC_OPT_RENEWABLE)) &&
	 !isflagset(ticket->enc_part2->flags,
		TKT_FLG_RENEWABLE))) 	/* can only renew renewable tix */

	return TRUE;			/* against policy */

    return FALSE;			/* XXX not against policy */
}
