#include "mcu_def.h"
#include "zxa_07.h"
#include <stdio.h>
zxa07_ctb_t zxa07;

u16 zxa07_buf[zxa07_nrs][8],zxa07_save_index;
u32 Lzxa07_VAL0,Lzxa07_VAL1,Lzxa07_VAL2,Lzxa07_VAL3,Lzxa07_VAL4,Lzxa07_VAL5,Lzxa07_VAL6,Lzxa07_VAL7,zxa07_sum[8];
u32 *ZXA07_VAL0 = &Lzxa07_VAL0, *ZXA07_VAL1 = &Lzxa07_VAL1, *ZXA07_VAL2 = &Lzxa07_VAL2, *ZXA07_VAL3 = &Lzxa07_VAL3, *ZXA07_VAL4 = &Lzxa07_VAL4, *ZXA07_VAL5 = &Lzxa07_VAL5, *ZXA07_VAL6 = &Lzxa07_VAL6, *ZXA07_VAL7 = &Lzxa07_VAL7;
u32 dzxa07_VAL0,dzxa07_VAL1,dzxa07_VAL2,dzxa07_VAL3,dzxa07_VAL4,dzxa07_VAL5,dzxa07_VAL6,dzxa07_VAL7,zxa_val[8];
u32 *ZXA07_V0 = &dzxa07_VAL0, *ZXA07_V1 = &dzxa07_VAL1, *ZXA07_V2 = &dzxa07_VAL2, *ZXA07_V3 = &dzxa07_VAL3, *ZXA07_V4 = &dzxa07_VAL4, *ZXA07_V5 = &dzxa07_VAL5, *ZXA07_V6 = &dzxa07_VAL6, *ZXA07_V7 = &dzxa07_VAL7;

u8 zxa07_inited, cmdindex,zxa07_cnt;
u16 Lzxa07_cnt,Lzxa07_error;
u16 *ZXA07_CNT=&Lzxa07_cnt,*ZXA07_ERROR=&Lzxa07_error;
u32 zxa07_lasttick = 0;
int zxa07_set_cnt()
{
    u8 buf[12];
    buf[0] = 0xaa;
    buf[1] = 0x55;
    buf[2] = cmdindex;
    buf[3] = 0xc;
    buf[4]=0;
    buf[5]=0;
    buf[6]=0;
    buf[7]=zxa07_cnt;
    buf[8]=0;
    buf[9]=0;
    buf[10]=calc_xor(buf, 10);
    buf[11]=0-calc_checsum( buf,11);
    zxa07.port->write(zxa07.port, buf, 12, 0);
    delay_ms(20);
    return 0;
}
void zxa07_retrieve()
{
	*ZXA07_V0=zxa_val[0];
	*ZXA07_V1=zxa_val[1];
	*ZXA07_V2=zxa_val[2];
	*ZXA07_V3=zxa_val[3];
	*ZXA07_V4=zxa_val[4];
	*ZXA07_V5=zxa_val[5];
	*ZXA07_V6=zxa_val[6];
	*ZXA07_V7=zxa_val[7];
    *ZXA07_VAL0 = zxa07_sum[0];
    *ZXA07_VAL1 = zxa07_sum[1];
    *ZXA07_VAL2 = zxa07_sum[2];
    *ZXA07_VAL3 = zxa07_sum[3];
    *ZXA07_VAL4 = zxa07_sum[4];
    *ZXA07_VAL5 = zxa07_sum[5];
    *ZXA07_VAL6 = zxa07_sum[6];
    *ZXA07_VAL7 = zxa07_sum[7];
}

void zxa07_publish()
{
#if 0
    if ((rt_tick_get() - zxa07_lasttick) < 5000)
        return;
    if (*ZXA07_CNT != zxa07_cnt)
    {
        zxa07_set_cnt();
        zxa07_lasttick = rt_tick_get();
    }
    cmdindex++;
#endif
}
int zxa07_reciver();
void zxa07_thread(void *arg)
{
    while (1)
    {
        zxa07_reciver();
    }
}


int zxa07_reciver()
{
    u8 buf[12];
    buf_data_clear(&zxa07.port->rx_buf);
    buf[0]=0xaa;
    buf[1]=0x55;
    int len = read_head(zxa07.port,  2,buf, 120000);
    if (len <= 0)
    {
        rt_kprintf("no data\n");
        *ZXA07_ERROR = 100;
        return -1;
    }
    len=read_context(zxa07.port, 23, 0, 500);
    if(len!=23)
    {
        rt_kprintf("data len %d\n",len);
        *ZXA07_ERROR = 101;
        return -2;
    }
    buf[0]=calc_xor(zxa07.port->rx_buf.buf, 25);
    len=read_cmp_context(zxa07.port, 1, buf, 5);
    if (len <= 0)
    {
        rt_kprintf("checsum 1 error\n");
        *ZXA07_ERROR = 102;
        return -1;
    }
    buf[1]=calc_checsum(zxa07.port->rx_buf.buf, 26);
    buf[0]=256-buf[1];
    len=read_cmp_context(zxa07.port, 1, buf, 5);
    if (len <= 0)
    {
        rt_kprintf("checsum 2 error\n");
        *ZXA07_ERROR = 103;
        return -1;
    }
    for(len=0; len<8; len++)
    {
        u16 val=zxa07.port->rx_buf.buf[8+len*2]+zxa07.port->rx_buf.buf[8+len*2+1]*256;
		zxa_val[len]=val;
        zxa07_sum[len]-=zxa07_buf[zxa07_save_index][len];
        zxa07_buf[zxa07_save_index][len]=val;
        zxa07_sum[len]+=zxa07_buf[zxa07_save_index][len];

    }
    zxa07_save_index++;
    if(zxa07_save_index>=Lzxa07_cnt)
        zxa07_save_index=0;
    *ZXA07_ERROR = 0;

    return 0;
}

void zxa07_init(void *pname)
{
    int len,i;
    char *portname = pname;
    zxa07_inited = 0;
    for( len=0; len<8; len++)
    {
        zxa07_sum[len]=0;
        for( i=0; i<Lzxa07_cnt; i++)
            zxa07_buf[i][len]=0;

    }
    *ZXA07_CNT=100;
#ifdef TESTCASE
    zxa07.port = comm_port_win(portname);
#else
    zxa07.port = comm_port_rt(portname);
#endif
    zxa07.port->open(zxa07.port, 9600, 8, 1, PARITY_NONE);
    rt_thread_t thread = rt_thread_create("zxa07", zxa07_thread, &zxa07, 2048, PRIO_THREAD_DEVICE, RT_TICK_PER_SECOND);
    rt_thread_startup(thread);
}
struct plc_device plc_zxa07 =
{
    zxa07_init,
    0,
    zxa07_retrieve,
    zxa07_publish,
    0,
    "zxa07",
};

#ifdef TESTCASE
#include <embUnit/embUnit.h>
static void setUp(void)
{
    rtthread_startup();
}
static void tearDown(void)
{
    rtthread_exit();
}

static void testGetValue(void *arg)
{
    zxa07_init("COM8");

}

static void test_function()
{
    testGetValue(0);
    rtthread_exit();
}
static void testInit(void)
{

    thread_new(test_function, 0);
    rtthread_run();
}
TestRef tests_zxa07(void);
TestRef tests_zxa07(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures)
    {
        new_TestFixture("testInit", testInit),
                        //                        new_TestFixture("testGetValue",testGetValue),
    };
    EMB_UNIT_TESTCALLER(tests_zxa07, "tests_zxa07", setUp, tearDown, fixtures);

    return (TestRef)&tests_zxa07;
}

#endif
