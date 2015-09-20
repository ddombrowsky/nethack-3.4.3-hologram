#include "hologram.h"
#include "../rdsh/gemserver.h"

#include <sys/socket.h>
#include <linux/un.h>
#include <unistd.h>
#include <string.h>

static int gemsocket_fd = -1;

/**
 *
 * @returns 1 if OK to read, 0 if not,
 *          -1 if can't connect
 */
int has_hologram(struct obj *tgem)
{
    if (tgem->spe >= 0) {
        struct sockaddr_un addr;
        char buf[GEM_MAXPACKET];
        int nbytes;

        /*
         * connect to gemserver
         */
        gemsocket_fd = socket(PF_UNIX, SOCK_STREAM, 0);
        if (gemsocket_fd < 0) {
            return -1;
        }

        memset(&addr, 0, sizeof(addr));
        addr.sun_family=AF_UNIX;
        snprintf(addr.sun_path, UNIX_PATH_MAX, GEM_SERVER);

        if(connect(gemsocket_fd,
                   (struct sockaddr *)&addr,
                   sizeof(struct sockaddr_un)) != 0) {
            return -1;
        }
        memset(buf, 0, sizeof(buf));
        /* read challenge packet */
        nbytes = read(gemsocket_fd, buf, sizeof(buf));
        if (nbytes > 0) {
            /* TODO */
        } else {
            close(gemsocket_fd);
            return -1;
        }

        /* write response packet */
        /* TODO */
        if (write(gemsocket_fd, buf, sizeof(buf)) < 0) {
            close(gemsocket_fd);
            return -1;
        }

        /* read success/fail */
        nbytes = read(gemsocket_fd, buf, sizeof(buf));
        if (nbytes > 0) {
            /* TODO */
        } else {
            close(gemsocket_fd);
            return -1;
        }

        /* can only read once... */
        tgem->spe = -1;
        return 1;
    }

    /* already read */
    return 0;
}

void rdsh_getnextkey(char *rbuf, size_t buflen)
{
    char buf[GEM_MAXPACKET];
    int nbytes;

    nbytes = read(gemsocket_fd, buf, sizeof(buf));

    if (nbytes > 0) {
        memcpy(rbuf, buf, nbytes);
    }

    close(gemsocket_fd);
}
