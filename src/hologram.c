#include "hologram.h"



boolean has_hologram(struct obj *tgem)
{
    boolean ret = 0;

    if (tgem->spe >= 0) {
        /* TODO: security */

        /* can only read once... */
        tgem->spe = -1;

        ret = 1;
    }

    return ret;
}

void rdsh_getnextkey(char *buf, size_t buflen)
{
    buf[0] = 'X';
    buf[1] = '\0';
}
