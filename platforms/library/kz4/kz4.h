#include "mcu_def.h"

typedef struct {
    comm_port_t *port;
} kz4_ctb_t;

int kz4_set_biaozhun();

int kz4_set_lijing();

int kz4_set_zhouqi();

int kz4_set_liusu();

int kz4_get_setup();

int kz4_get_lijing();

int kz4_get_zhouqi();

int kz4_get_liusu();

int kz4_get_error();
int kz4_towork(u8);
void kz4_thread(void *);

void kz4_init();

void kz4_retrieve();

void kz4_publish();
