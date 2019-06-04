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
  FILE *vias_file;


  osip_via_t *via;
  char *a_via;
  char *dest;
  char *res;

  vias_file = fopen (argv[1], "r");
  if (vias_file == NULL) {
    fprintf (stdout, "Failed to open %s file.\nUsage: tvia vias.txt\n", argv[1]);
    exit (0);
  }

  a_via = (char *) osip_malloc (200);
  res = fgets (a_via, 200, vias_file);  /* lines are under 200 */
  while (res != NULL) {

    int errcode;

    /* remove the last '\n' before parsing */
    osip_strncpy (a_via + strlen (a_via) - 1, "\0", 1);

    if (0 != strncmp (a_via, "#", 1)) {
      /* allocate & init via */
      osip_via_init (&via);
      printf ("=================================================\n");
      printf ("VIA TO PARSE: |%s|\n", a_via);
      errcode = osip_via_parse (via, a_via);
      if (errcode != -1) {
        if (osip_via_to_str (via, &dest) != -1) {
          printf ("result:       |%s|\n", dest);
          osip_free (dest);
        }
      }
      else
        printf ("Bad via format: %s\n", a_via);
      osip_via_free (via);
      printf ("=================================================\n");
    }
    res = fgets (a_via, 200, vias_file);        /* lines are under 200 */
  }
  osip_free (a_via);

  return 0;
}
