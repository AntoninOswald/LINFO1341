#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdint.h>
#include <netinet/in.h>
#include <unistd.h>



int send_hello_to_peer(int sock, struct sockaddr *peer_addr, size_t peer_addr_len) {
    ssize_t sent = sendto(sock, "hello", strlen("hello"), 0, peer_addr, peer_addr_len);
    if (sent == -1) {
        printf("could not send the message, error: %s\n", strerror(errno));
        return errno;
    }
    return 0;
}


#define MAX_MESSAGE_SIZE 2500
int bind_and_receive_from_peer(int sock, struct sockaddr *local_addr, socklen_t local_addr_len) {
    int err = bind(sock, local_addr, local_addr_len);  // assign our address to the socket
    if (err == -1) {
        printf("could not bind on the socket, error: %s\n", strerror(errno));
        return errno;
    }
    char buffer[MAX_MESSAGE_SIZE];  // allocate a buffer of MAX_MESSAGE_SIZE bytes on the stack
    ssize_t n_received = recv(sock, buffer, MAX_MESSAGE_SIZE, 0);   // equivalent to do: read(sock, buffer, MAX_MESSAGE_SIZE);
    if (n_received == -1) {
        printf("could not receive the message, error: %s\n", strerror(errno));
        return errno;
    }

    // let's print what we received !
    printf("received %ld bytes:\n", n_received);
    for (int i = 0 ; i < n_received ; i++) {
        printf("0x%hhx ('%c') ", buffer[i], buffer[i]);
    }
    printf("\n");
    return 0;
}


#define MAX_MESSAGE_SIZE 2500
int bind_and_receive_from_peer_with_addr(int sock,struct sockaddr *local_addr, socklen_t local_addr_len) {
    int err = bind(sock, local_addr, local_addr_len);  // assign our address to the socket
    if (err == -1) {
        printf("could not bind on the socket, error: %s\n", strerror(errno));
        return errno;
    }
    struct sockaddr_storage peer_addr;  // allocate the peer's address on the stack. It will be initialized when we receive a message
    socklen_t peer_addr_len = sizeof(struct sockaddr_storage); // variable that will contain the length of the peer's address
    char buffer[MAX_MESSAGE_SIZE];  // allocate a buffer of MAX_MESSAGE_SIZE bytes on the stack
    ssize_t n_received = recvfrom(sock, buffer, MAX_MESSAGE_SIZE, 0, (struct sockaddr *) &peer_addr, &peer_addr_len);
    if (n_received == -1) {
        printf("could not receive the message, error: %s\n", strerror(errno));
        return errno;
    }

    // let's print what we received !
    printf("received %ld bytes:\n", n_received);
    for (int i = 0 ; i < n_received ; i++) {
        printf("0x%hhx ('%c') ", buffer[i], buffer[i]);
    }
    printf("\n");

    // let's now print the address of the peer
    uint8_t *peer_addr_bytes = (uint8_t *) &peer_addr;
    printf("the socket address of the peer is (%ld bytes):\n", peer_addr_len);
    for (int i = 0 ; i < peer_addr_len ; i++) {
        printf("0x%hhx ", peer_addr_bytes[i]);
    }
    printf("\n");
    return 0;
}

#define MAX_MESSAGE_SIZE 2500
int send_hello_to_and_read_reply_from_connected_peer(int sock, struct sockaddr *peer_addr, size_t peer_addr_len) {
    int err = connect(sock, peer_addr, peer_addr_len); // connect the socket to the peer
    if (err == -1) {
        printf("cound not connect the socket: %s\n", strerror(errno));
        return errno;
    }
    ssize_t written = write(sock, "hello", strlen("hello"));  // we can use the generic write(2) system call: we do not need to specify the destination socket address
    if (written == -1) {
        printf("could not send the message, error: %s\n", strerror(errno));
        return errno;
    }
    uint8_t buffer[MAX_MESSAGE_SIZE]; // allocate the receive buffer on the stack
    ssize_t amount_read = read(sock, buffer, MAX_MESSAGE_SIZE);
    if (amount_read == -1) {
        printf("could not read on the socket, error: %s\n", strerror(errno));
        return errno;
    }
    // let's print what we received !
    printf("received %ld bytes:\n", amount_read);
    for (int i = 0 ; i < amount_read ; i++) {
        printf("0x%hhx ('%c') ", buffer[i], buffer[i]);
    }
    return 0;
}



int create_socket_and_send_message() {
    int sock = socket(AF_INET6, SOCK_DGRAM, 0);             // create a socket using IPv6 addresses
    if (sock == -1) {
        printf("could not create the IPv6 SOCK_DGRAM socket, error: %s\n", strerror(errno));
        return errno;
    }
    struct sockaddr_in6 peer_addr;                          // allocate the address on the stack
    memset(&peer_addr, 0, sizeof(peer_addr));               // fill the address with 0-bytes to avoid garbage-values
    peer_addr.sin6_family = AF_INET6;                       // indicate that the address is an IPv6 address
    peer_addr.sin6_port = htons(55555);                     // indicate that the programm is running on port 55555
    inet_pton(AF_INET6, "::1", &peer_addr.sin6_addr);       // indicate that the program is running on the computer identified by the ::1 IPv6 address

    send_hello_to_peer(sock, (struct sockaddr *) &peer_addr, sizeof(peer_addr));    // use the send_hello_to_peer function that we defined previously
    close(sock);                                            // release the resources used by the socket
    return 0;
}



//EXO INGINIOUS


int create_and_send_message(const struct sockaddr *dest_addr, socklen_t addrlen) {
    // TODO: Create a IPv6 socket supporting datagrams

    int sock = socket(AF_INET6, SOCK_DGRAM, 0);             // create a socket using IPv6 addresses
    if (sock == -1) {
        return -1;
    }
    // TODO: Connect it to the destination

    int err = connect(sock, dest_addr, addrlen);

    if (err == -1)
    {
        return -1;
    }
    // TODO: Send the required message (cfr. statement) through the socket

    int buffer[5] = {1,3,5,7,9};

    for (int i = 0; i < 5; i++)
    {
        buffer[i] = htonl(buffer[i]); //Vérufier endianness
    }

    int res = send(sock,buffer,sizeof(buffer),0);
    if(res == -1){
        return -1;
    }

    return 0;

}

int recv_and_handle_message(const struct sockaddr *src_addr, socklen_t addrlen) {
    // TODO: Create a IPv6 socket supporting datagrams

    int sock = socket(AF_INET6, SOCK_DGRAM, 0);             // create a socket using IPv6 addresses
    if (sock == -1) {
        return -1;
    }

    // TODO: Bind it to the source

    int err = bind(sock, src_addr, addrlen);  // assign our address to the socket
    if (err == -1) {
        return -1;
    }

    // TODO: Receive a message through the socket

    struct sockaddr_storage peer_addr;  // allocate the peer's address on the stack. It will be initialized when we receive a message
    socklen_t peer_addr_len = sizeof(struct sockaddr_storage); // variable that will contain the length of the peer's address
    char buffer[1024];  // allocate a buffer of MAX_MESSAGE_SIZE bytes on the stack
    ssize_t n_received = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *) &peer_addr, &peer_addr_len);
    if (n_received == -1) {
        return -1;
    }
    // TODO: Perform the computation

    int sum = 0;

    for (int i = 0; i < n_received; i+= sizeof(int))
    {
        sum = sum + ntohl(*(int *)(buffer+i)); //convertir dans la convention de la machine
    }

    // TODO: Send back the result

    //attention,il faut reconvertir le résultat dans l'autre sens

    int res = htonl(sum);
    ssize_t sent = sendto(sock, &res, sizeof(int),0, (struct sockaddr *) &peer_addr, peer_addr_len);
    if (sent == -1) {
        return -1;
    }
    

    return 0;
}




int main(int argc, char **argv){


    return 0;
}