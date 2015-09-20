
#include "gemserver.h"

#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <linux/un.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int handshake(int sock_fd, int *value)
{
    char buf[GEM_MAXPACKET];
    int nbytes;
    int success = false;

    /* write challenge */
    /* TODO */
    nbytes = write(sock_fd, buf, sizeof(buf));

    printf("wrote %d bytes\n", nbytes);

     /* read response */
    /* TODO */
    nbytes = read(sock_fd, buf, sizeof(buf));

    printf("read %d bytes\n", nbytes);

    if (nbytes > 0) {
        /* TODO */
        *value = 1;
        success = true;

        nbytes = write(sock_fd, buf, sizeof(buf));

        printf("wrote %d bytes\n", nbytes);
    }

    return success;
}

void sendkey(int sock_fd, int value)
{
    char buf[88];

    strncpy(buf,
        "1111111111111111111111111111111111:"
        "1111111111111111111111111111111111000000000000000000",
        sizeof(buf));

    write(sock_fd, buf, sizeof(buf));
}

int main(void)
{
    int sock_fd, con_fd;
    struct sockaddr_un addr;
    socklen_t addr_len;

    sock_fd = socket(PF_UNIX, SOCK_STREAM, 0);

    if (sock_fd<0) {
        printf("socket() failed, returned %d\n", sock_fd);
        return -1;
    }

    unlink(GEM_SERVER);

    memset(&addr, 0, sizeof(addr));

    addr.sun_family=AF_UNIX;
    snprintf(addr.sun_path, UNIX_PATH_MAX, GEM_SERVER);

    /* TODO create with correct perms and group */

    if (bind(sock_fd,
             (struct sockaddr *)&addr,
             sizeof(struct sockaddr_un)) != 0) {
        printf("bind() failed\n");
        return -1;
    }

    if (listen(sock_fd, 5 /* max connections */) != 0) {
        printf("listen() failed\n");
        return -1;
    }

    printf("listening for connections at %s...\n", GEM_SERVER);
    while ((con_fd = accept(sock_fd,
                           (struct sockaddr *)&addr,
                           &addr_len)) > -1) {
        int req;

        /* TODO this should be a log function */
        char stime[200];
        time_t t;
        struct tm *tmp;

        t = time(NULL);
        tmp = localtime(&t);
        strftime(stime, sizeof(stime), "%Y-%m-%d %T %z", tmp);

        /* We do not fork.  We use the system's 
         * queueing mechanism to make this a critical
         * section and process requests one-by-one. */
        printf("%s Got connection\n", stime);

        if (handshake(con_fd, &req)) {
            sendkey(con_fd, req);
        }

        close(con_fd);
    }

    close(sock_fd);
    unlink(GEM_SERVER);

    return 0;
}
