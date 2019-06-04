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
  FILE *routes_file;


  osip_route_t *route;
  char *a_route;
  char *dest;
  char *res;

  routes_file = fopen (argv[1], "r");
  if (routes_file == NULL) {
    fprintf (stdout, "Failed to open %s file.\nUsage: troute routes.txt\n", argv[1]);
    exit (0);
  }

  a_route = (char *) osip_malloc (200);
  res = fgets (a_route, 200, routes_file);      /* lines are under 200 */
  while (res != NULL) {

    int errcode;

    /* remove the last '\n' before parsing */
    strncpy (a_route + strlen (a_route) - 1, "\0", 1);

    if (0 != strncmp (a_route, "#", 1)) {
      /* allocate & init route */
      osip_route_init (&route);
      printf ("=================================================\n");
      printf ("ROUTE TO PARSE: |%s|\n", a_route);
      errcode = osip_route_parse (route, a_route);
      if (errcode != -1) {
        if (osip_route_to_str (route, &dest) != -1) {
          printf ("result:         |%s|\n", dest);
          osip_free (dest);
        }
      }
      else
        printf ("Bad route format: %s\n", a_route);
      osip_route_free (route);
      printf ("=================================================\n");
    }
    res = fgets (a_route, 200, routes_file);    /* lines are under 200 */
  }
  osip_free (a_route);
  return 0;
}
