#include "mcu_def.h"

typedef struct
{
    comm_port_t *port;
} zxa07_ctb_t;

int zxa07_set_cnt();

void zxa07_thread(void *);

void zxa07_init(void *pname);

void zxa07_retrieve();

void zxa07_publish();
#define zxa07_nrs	600
