/*
  eXosip - This is the eXtended osip library.
  Copyright (C) 2001-2012 Aymeric MOIZARD amoizard@antisip.com
  
  eXosip is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  eXosip is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#include "eXosip2.h"

#ifndef MINISIZE

#include <eXosip2/eXosip.h>

int
eXosip_refer_build_request (struct eXosip_t *excontext, osip_message_t ** refer, const char *refer_to, const char *from, const char *to, const char *proxy)
{
  int i;

  *refer = NULL;
  i = _eXosip_generating_request_out_of_dialog (excontext, refer, "REFER", to, "UDP", from, proxy);
  if (i != 0) {
    return i;
  }

  osip_message_set_header (*refer, "Refer-to", refer_to);
  return OSIP_SUCCESS;
}

int
eXosip_refer_send_request (struct eXosip_t *excontext, osip_message_t * refer)
{
  osip_transaction_t *transaction;
  osip_event_t *sipevent;
  int i;

  if (refer == NULL)
    return OSIP_BADPARAMETER;

  i = _eXosip_transaction_init (excontext, &transaction, NICT, excontext->j_osip, refer);
  if (i != 0) {
    osip_message_free (refer);
    return i;
  }

  osip_list_add (&excontext->j_transactions, transaction, 0);

  sipevent = osip_new_outgoing_sipmessage (refer);
  sipevent->transactionid = transaction->transactionid;

  osip_transaction_add_event (transaction, sipevent);
  _eXosip_wakeup (excontext);
  return OSIP_SUCCESS;
}

#endif
