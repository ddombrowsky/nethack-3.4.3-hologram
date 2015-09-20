/*
 *
 * Simple server based on a named socket, which will
 * give out pub/priv key pairs based on the requested
 * amount.
 *
 * The server is intended to run as root, and read
 * files readable only by root.
 *
 */

#define GEM_SERVER "/var/run/gemserver"
#define GEM_MAXPACKET 256
