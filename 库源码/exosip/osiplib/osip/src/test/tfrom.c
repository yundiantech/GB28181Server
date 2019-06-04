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
  FILE *froms_file;


  osip_from_t *from;
  char *a_from;
  char *dest;
  char *res;

  froms_file = fopen (argv[1], "r");
  if (froms_file == NULL) {
    fprintf (stdout, "Failed to open %s file.\nUsage: tfrom froms.txt\n", argv[1]);
    exit (0);
  }

  a_from = (char *) osip_malloc (200);
  res = fgets (a_from, 200, froms_file);        /* lines are under 200 */
  while (res != NULL) {

    int errcode;

    /* remove the last '\n' before parsing */
    strncpy (a_from + strlen (a_from) - 1, "\0", 1);

    if (0 != strncmp (a_from, "#", 1)) {
      /* allocate & init from */
      osip_from_init (&from);
      printf ("=================================================\n");
      printf ("FROM TO PARSE: |%s|\n", a_from);
      errcode = osip_from_parse (from, a_from);
      if (errcode != -1) {
        if (osip_from_to_str (from, &dest) != -1) {
          printf ("result:        |%s|\n", dest);
          osip_free (dest);
        }
      }
      else
        printf ("Bad from format: %s\n", a_from);
      osip_from_free (from);
      printf ("=================================================\n");
    }
    res = fgets (a_from, 200, froms_file);      /* lines are under 200 */
  }
  osip_free (a_from);
  return 0;
}
