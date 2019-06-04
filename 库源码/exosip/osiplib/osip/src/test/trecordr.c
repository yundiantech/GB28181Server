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
  FILE *record_routes_file;


  osip_record_route_t *record_route;
  char *a_record_route;
  char *dest;
  char *res;

  record_routes_file = fopen (argv[1], "r");
  if (record_routes_file == NULL) {
    fprintf (stdout, "Failed to open %s file.\nUsage: trecord_route record_routes.txt\n", argv[1]);
    exit (0);
  }

  a_record_route = (char *) osip_malloc (200);
  res = fgets (a_record_route, 200, record_routes_file);        /* lines are under 200 */
  while (res != NULL) {

    int errcode;

    /* remove the last '\n' before parsing */
    strncpy (a_record_route + strlen (a_record_route) - 1, "\0", 1);

    if (0 != strncmp (a_record_route, "#", 1)) {
      /* allocate & init record_route */
      osip_record_route_init (&record_route);
      printf ("=================================================\n");
      printf ("RECORD_ROUTE TO PARSE: |%s|\n", a_record_route);
      errcode = osip_record_route_parse (record_route, a_record_route);
      if (errcode != -1) {
        if (osip_record_route_to_str (record_route, &dest) != -1) {
          printf ("result:                |%s|\n", dest);
          osip_free (dest);
        }
      }
      else
        printf ("Bad record_route format: %s\n", a_record_route);
      osip_record_route_free (record_route);
      printf ("=================================================\n");
    }
    res = fgets (a_record_route, 200, record_routes_file);      /* lines are under 200 */
  }
  osip_free (a_record_route);
  return 0;
}
