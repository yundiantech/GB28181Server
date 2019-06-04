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
  FILE *content_types_file;


  osip_content_type_t *content_type;
  char *a_content_type;
  char *dest;
  char *res;

  content_types_file = fopen (argv[1], "r");
  if (content_types_file == NULL) {
    fprintf (stdout, "Failed to open %s file.\nUsage: tcontent_type content_types.txt\n", argv[1]);
    exit (0);
  }

  a_content_type = (char *) osip_malloc (200);
  res = fgets (a_content_type, 200, content_types_file);        /* lines are under 200 */
  while (res != NULL) {

    int errcode;

    /* remove the last '\n' before parsing */
    strncpy (a_content_type + strlen (a_content_type) - 1, "\0", 1);

    if (0 != strncmp (a_content_type, "#", 1)) {
      /* allocate & init content_type */
      osip_content_type_init (&content_type);
      printf ("=================================================\n");
      printf ("CONTENT_TYPE TO PARSE: |%s|\n", a_content_type);
      errcode = osip_content_type_parse (content_type, a_content_type);
      if (errcode != -1) {
        if (osip_content_type_to_str (content_type, &dest) != -1) {
          printf ("result:                |%s|\n", dest);
          osip_free (dest);
        }
      }
      else
        printf ("Bad content_type format: %s\n", a_content_type);
      osip_content_type_free (content_type);
      printf ("=================================================\n");
    }
    res = fgets (a_content_type, 200, content_types_file);      /* lines are under 200 */
  }
  osip_free (a_content_type);
  return 0;
}
