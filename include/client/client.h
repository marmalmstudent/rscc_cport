#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iostrm.h"


void error(const char *msg);

typedef struct client_struct *Client;

Client client_ctor(const char * hostname, unsigned int port);
void client_dtor(Client obj);

struct client_struct
{
    IOStream stream;
};

#endif // CLIENT_H
