#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iostrm.h"

/** Prints an error message and exits the program */
void error(const char *msg);

typedef struct client_struct *Client;

/** Constructor */
Client client_ctor(const char * hostname, unsigned int port);
/** Destructor */
void client_dtor(Client obj);

/** The Client declaration */
struct client_struct
{
    IOStream stream;
};

#endif // CLIENT_H
