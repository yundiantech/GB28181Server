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
#include <osipparser2/osip_parser.h>
#include <osipparser2/sdp_message.h>

int test_message (char *msg, size_t len, int verbose, int clone, int perf);
static void usage (void);

static void
usage ()
{
  fprintf (stderr, "Usage: ./torture_test torture_file [-v (verbose)] [-c (clone)] [-p (performance: loop 100.000]\n");
  exit (1);
}

static int
read_binary (char **msg, int *len, FILE * torture_file)
{
  *msg = (char *) osip_malloc (100000); /* msg are under 100000 */

  *len = fread (*msg, 1, 100000, torture_file);
  return ferror (torture_file) ? -1 : 0;
}

int
main (int argc, char **argv)
{
  int success = 1;
  int loop = 1;
  int verbose = 0;              /* 1: verbose, 0 (or nothing: not verbose) */
  int clone = 0;                /* 1: verbose, 0 (or nothing: not verbose) */
  FILE *torture_file;
  char *msg;
  char *ptr;
  int pos;
  int len;
  int expected_error=OSIP_SUCCESS;
  
  for (pos = 2; pos < argc; pos++) {
    if (0 == strncmp (argv[pos], "-v", 2))
      verbose = 1;
    else if (0 == strncmp (argv[pos], "-c", 2))
      clone = 1;
    else if (0 == strncmp (argv[pos], "-p", 2))
      loop = 100000;
    else
      usage ();
  }

  if (argc < 2) {
    usage ();
  }

  torture_file = fopen (argv[1], "r");
  if (torture_file == NULL) {
    usage ();
  }

  /* initialize parser */
  parser_init ();

  if (read_binary (&msg, &len, torture_file) < 0) {
    fprintf (stdout, "test %s : ============================ FAILED (cannot read file)\n", argv[1]);
    return -999;
  }
  
  ptr=msg;
  if (osip_strncasecmp(msg, "expected_error:", strlen("expected_error:"))==0) {
    ptr+=strlen("expected_error:");
    expected_error=atoi(ptr);
    while (*ptr!='\r' && *ptr!='\n')
      ptr++;
    while (*ptr=='\r' || *ptr=='\n')
      ptr++;
  }
  success = test_message (ptr, len, verbose, clone, loop);
  if (verbose) {
    fprintf (stdout, "test %s : ============================ \n", argv[1]);
    fwrite (msg, 1, len, stdout);
  }
  if (success==expected_error)
    fprintf (stdout, "test %s : ============================ OK (error_code=%i)\n", argv[1], expected_error);
  else
    fprintf (stdout, "test %s : ============================ FAILED (expected=%i error_code=%i)\n", argv[1], expected_error, success);

  osip_free (msg);
  fclose (torture_file);
#ifdef __linux
  if (success!=expected_error)
    exit (EXIT_FAILURE);
  else
    exit (EXIT_SUCCESS);
#endif
  return success;
}

int
test_message (char *msg, size_t len, int verbose, int clone, int perf)
{
  osip_message_t *sip;
  int err=0;
  char *result;
  
  int j = perf;
  
  if (verbose)
    fprintf (stdout, "Trying %i sequentials calls to osip_message_init(), osip_message_parse() and osip_message_free()\n", j);
  while (j != 0) {
    j--;
    osip_message_init (&sip);
    err = osip_message_parse (sip, msg, len);
      if (err != 0) {
	if (verbose)
	  fprintf (stdout, "ERROR: failed while parsing!\n");
        osip_message_free (sip);
        return err;
      }
      osip_message_free (sip);
  }
  
  osip_message_init (&sip);
  err = osip_message_parse (sip, msg, len);
  if (err != 0) {
    if (verbose)
      fprintf (stdout, "ERROR: failed while parsing!\n");
    osip_message_free (sip);
    return err;
  }
  else {
    size_t length;
    
#if 0
    sdp_message_t *sdp;
    osip_body_t *oldbody;
    int pos;
    
    pos = 0;
    while (!osip_list_eol (&sip->bodies, pos)) {
      oldbody = (osip_body_t *) osip_list_get (&sip->bodies, pos);
      pos++;
      sdp_message_init (&sdp);
      err = sdp_message_parse (sdp, oldbody->body);
      sdp_message_free (sdp);
      sdp = NULL;
      if (err != 0) {
	if (verbose)
	  fprintf (stdout, "ERROR: Bad SDP!\n");
	break;
      }
      else if (verbose)
	fprintf (stdout, "SUCCESS: Correct SDP!\n");
    }
#endif
    
    osip_message_force_update (sip);
    err = osip_message_to_str (sip, &result, &length);
    if (err != OSIP_SUCCESS) {
      if (verbose)
	fprintf (stdout, "ERROR: failed while printing message!\n");
      osip_message_free (sip);
      return err;
    }
    else {
      if (verbose)
	fwrite (result, 1, length, stdout);
      if (clone) {
	int j = perf;
	
	if (verbose)
	  fprintf (stdout, "Trying %i sequentials calls to osip_message_clone() and osip_message_free()\n", j);
	while (j != 0) {
	  osip_message_t *copy;
	  
	  j--;
	  err = osip_message_clone (sip, &copy);
	  if (err != OSIP_SUCCESS) {
	    if (verbose)
	      fprintf (stdout, "ERROR: failed while creating copy of message!\n");
	    break;
	  }
	  else {
	    char *tmp;
	    size_t length;
	    
	    osip_message_force_update (copy);
	    err = osip_message_to_str (copy, &tmp, &length);
	    if (err != OSIP_SUCCESS) {
	      if (verbose)
		fprintf (stdout, "ERROR: failed while printing message!\n");
	    }
	    else {
	      if (0 == strcmp (result, tmp)) {
		if (verbose)
		  printf ("The osip_message_clone method works perfectly\n");
	      }
	      else {
		printf ("ERROR: The osip_message_clone method DOES NOT works\n");
		err=-1;
		if (verbose) {
		  printf ("Here is the copy: \n");
		  fwrite (tmp, 1, length, stdout);
		  printf ("\n");
		}
	      }
	      osip_free (tmp);
	    }
	    osip_message_free (copy);
	  }
	}
	if (verbose)
	  fprintf (stdout, "sequentials calls: done\n");
      }
      osip_free (result);
    }
    osip_message_free (sip);
  }

  return err;
}
