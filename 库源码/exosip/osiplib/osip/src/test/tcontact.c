/*
  The oSIP library implements the Session Initiation Protocol (SIP -rfc3261-)
  Copyright (C) 2001-2012 Aymeric MOIZARD amoizard@antisip.com
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#ifdef ENABLE_MPATROL
#include <mpatrol.h>
#endif

#include <osipparser2/internal.h>
#include <osipparser2/osip_port.h>
#include <osipparser2/osip_message.h>


int
main (int argc, char **argv)
{
  FILE *contacts_file;


  osip_contact_t *contact;
  char *a_contact;
  char *dest;
  char *res;

  contacts_file = fopen (argv[1], "r");
  if (contacts_file == NULL) {
    fprintf (stdout, "Failed to open %s file.\nUsage: tcontact contacts.txt\n", argv[1]);
    exit (0);
  }

  a_contact = (char *) osip_malloc (500);
  res = fgets (a_contact, 500, contacts_file);  /* lines are under 200 */
  while (res != NULL) {

    int errcode;

    /* remove the last '\n' before parsing */
    strncpy (a_contact + strlen (a_contact) - 1, "\0", 1);

    if (0 != strncmp (a_contact, "#", 1)) {
      /* allocate & init contact */
      osip_contact_init (&contact);
      printf ("=================================================\n");
      printf ("CONTACT TO PARSE: |%s|\n", a_contact);
      errcode = osip_contact_parse (contact, a_contact);
      if (errcode != -1) {
        if (osip_contact_to_str (contact, &dest) != -1) {
          printf ("result:           |%s|\n", dest);

	  {
	    int pos = 0;
	    osip_generic_param_t *u_param;
	    while (!osip_list_eol (&contact->gen_params, pos)) {
	      u_param = (osip_generic_param_t *) osip_list_get (&contact->gen_params, pos);
	      
	      if (u_param->gvalue == NULL)
		printf ("result:       ;%s\n", u_param->gname);
	      else
		printf ("result:       ;%s=%s\n", u_param->gname, u_param->gvalue);
	      pos++;
	    }
	  }

          osip_free (dest);
        }
      }
      else
        printf ("Bad contact format: %s\n", a_contact);
      osip_contact_free (contact);
      printf ("=================================================\n");
    }
    res = fgets (a_contact, 500, contacts_file);        /* lines are under 200 */
  }
  osip_free (a_contact);
  return 0;
}
