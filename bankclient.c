/*
 * bankclient.c
 * Adapted from code/netp/echoclient.c in CSAPP
 */

#include "csapp.h"
#include "bank.h"

int parse_buf(char buf[], msg_t *msg);

void print_response(msg_t *response);

int main(int argc, char **argv) 
{
    int clientfd, port;
    char *host, buf[MAX_LINE];
    
    if (argc == 2 || argc == 3) {
        host = argv[1];
        port = (argc == 2) ? DEFAULT_PORT : atoi(argv[2]);
    }
    else {
        fprintf(stderr, "usage: %s <host>\n", argv[0]);
        exit(0);
    }    
    
    clientfd = Open_clientfd(host, port);
    
    printf("Bank client v.%d connected to %s on port %d\n", VERSION, host, port);
    
    msg_t *request = new_msg();
    msg_t *response = new_msg();
    
    printf("> "); fflush(stdout);
    while (Fgets(buf, MAX_LINE, stdin) != NULL) {
        if (parse_buf(buf, request)) {
            Rio_writen(clientfd, (void *) request, sizeof(msg_t)); // Send bits to server
            Rio_readn(clientfd, (void *) response, sizeof(msg_t)); // Get bits back from server
            print_response(response);    
        }
        clear_msg(request);
        clear_msg(response);
        printf("> "); fflush(stdout);
    }
    
    Close(clientfd); 
    free(request);
    free(response);
    exit(0);
}

int parse_buf(char buf[], msg_t *msg)
{
    if (strncmp(buf, "create", 6) == 0) {
        msg->opcode = 0x10;
        msg->amt = atoi(buf + 7);
        msg->acct = 0xffffffff;
        strncpy(msg->error, "Who's there?", 12);
        fflush(stdout);
        return 1;
    }
    
    else if (strncmp(buf, "deposit", 7) == 0) {
        msg->opcode = 0x20;
        return 1;
    }
    
    else if (strncmp(buf, "withdraw", 8) == 0) {
        msg->opcode = 0x30;
        return 1;
    }
    
    else if (strncmp(buf, "balance", 7) == 0) {
        msg->opcode = 0x40;
        return 1;
    }
    
    else if (strncmp(buf, "close", 5) == 0) {
        msg->opcode = 0x50;
        return 1;
    }
    else {
        printf("Command not understood.\n");
        return 0;
    }
}

void print_response(msg_t *response)
{   
    // printf("response: "); hex_dump(response);
    // printf("Client received a message of length %d bytes\n", (int) message_len(response));
    
    if (response->version != VERSION) {
        printf("Unknown server version %d\n", response->version);
        return;
    }
    
    if (response->opcode == 0x11) {
        printf("Server successfully created account %d with initial deposit %lld\n", 
                response->acct, response->amt);
    }
    else if (response->opcode == 0x21) {
        
    }
    else if (response->opcode == 0x31) {
    
    }
    else if (response->opcode == 0x41) {
    
    }
    else if (response->opcode == 0x51) {
    
    }
    else {
        printf("Didn't understand server response\n");
        // raise_exception(connfd, 0x91, request, request->opcode, "Unknown opcode");
    }
    
    fflush(stdout);
}