#ifndef CLIENT_H
#define CLIENT_H

/** Prints an error message and exits the program */
void error(const char *msg);

typedef struct client_struct *Client;

/** Constructor */
Client client_ctor(const char * hostname, unsigned int port);
/** Destructor */
void client_dtor(Client obj);


#endif // CLIENT_H
