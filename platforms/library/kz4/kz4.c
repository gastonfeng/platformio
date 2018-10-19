#include "mcu_def.h"
#include "kz4.h"
#include <stdio.h>
kz4_ctb_t kz4;
u16 LKZ4_BIAOZHUN, kz4_biaozhun;     //检测标准
u16 LKZ4_LIJING, kz4_lijing;         //粒径
u16 LKZ4_ZHOUQI, kz4_zhouqi;         //周期
u16 LKZ4_LIUSU_MODE, kz4_liusu_mode; //流速模式
u16 LKZ4_LIUSU, kz4_liusu;           //流速
u16 LKZ4_ERROR;                      //错误代码
u16 LKZ4_WORK, kz4_work;             //工作模式
u16 kz4_step;
int kz4_vals[8];
u16 kz4_level[8];
u16 LKZ4_LEVEL0, LKZ4_LEVEL1, LKZ4_LEVEL2, LKZ4_LEVEL3, LKZ4_LEVEL4, LKZ4_LEVEL5, LKZ4_LEVEL6, LKZ4_LEVEL7;
u32 LKZ4_VAL0, LKZ4_VAL1, LKZ4_VAL2, LKZ4_VAL3, LKZ4_VAL4, LKZ4_VAL5, LKZ4_VAL6, LKZ4_VAL7;
u16 *KZ4_BIAOZHUN = &LKZ4_BIAOZHUN;     //检测标准
u16 *KZ4_LIJING = &LKZ4_LIJING;         //粒径
u16 *KZ4_ZHOUQI = &LKZ4_ZHOUQI;         //周期
u16 *KZ4_LIUSU_MODE = &LKZ4_LIUSU_MODE; //流速模式
u16 *KZ4_LIUSU = &LKZ4_LIUSU;           //流速
u16 *KZ4_ERROR = &LKZ4_ERROR;           //错误代码
u16 *KZ4_WORK = &LKZ4_WORK;             //工作模式
u16 *KZ4_LEVEL0 = &LKZ4_LEVEL0, *KZ4_LEVEL1 = &LKZ4_LEVEL1, *KZ4_LEVEL2 = &LKZ4_LEVEL2, *KZ4_LEVEL3 = &LKZ4_LEVEL3, *KZ4_LEVEL4 = &LKZ4_LEVEL4, *KZ4_LEVEL5 = &LKZ4_LEVEL5, *KZ4_LEVEL6 = &LKZ4_LEVEL6, *KZ4_LEVEL7 = &LKZ4_LEVEL7;
u32 *KZ4_VAL0 = &LKZ4_VAL0, *KZ4_VAL1 = &LKZ4_VAL1, *KZ4_VAL2 = &LKZ4_VAL2, *KZ4_VAL3 = &LKZ4_VAL3, *KZ4_VAL4 = &LKZ4_VAL4, *KZ4_VAL5 = &LKZ4_VAL5, *KZ4_VAL6 = &LKZ4_VAL6, *KZ4_VAL7 = &LKZ4_VAL7;

u8 kz4_inited, cmdindex;
;
u32 kz4_lasttick = 0;
int kz4_set_biaozhun()
{
    u8 buf[8];
    buf[0] = 'W';
    buf[1] = 'H';
    buf[2] = *KZ4_BIAOZHUN + 0x30;
    buf[3] = 0xd;
    kz4.port->write(kz4.port, buf, 4, 0);
    delay_ms(20);
    return 0;
}
int kz4_set_lijing()
{
    u8 buf[8];
    buf[0] = 'W';
    buf[1] = 'U';
    buf[2] = *KZ4_LIJING + 0x30;
    buf[3] = 0xd;
    kz4.port->write(kz4.port, buf, 4, 0);
    delay_ms(20);
    return 0;
}
int kz4_set_zhouqi()
{
    u8 buf[8];
    buf[0] = 'W';
    buf[1] = 'D';
    buf[2] = *KZ4_ZHOUQI / 10 + 0x30;
    buf[3] = *KZ4_ZHOUQI % 10 + 0x30;
    buf[4] = 0xd;
    kz4.port->write(kz4.port, buf, 5, 0);
    delay_ms(20);
    return 0;
}
int kz4_set_liusu()
{
    u8 buf[8];
    buf[0] = 'W';
    buf[1] = 'V';
    buf[2] = *KZ4_LIUSU_MODE + 0x30;
    buf[3] = *KZ4_LIUSU / 10 + 0x30;
    buf[4] = *KZ4_LIUSU % 10 + 0x30;
    buf[5] = 0xd;
    kz4.port->write(kz4.port, buf, 6, 0);
    delay_ms(20);
    return 0;
}
int kz4_get_error()
{
    u8 buf[8];
    buf[0] = 'R';
    buf[1] = 'M';
    buf[2] = 0xd;
    kz4.port->write(kz4.port, buf, 3, 0);
    delay_ms(20);
    return 0;
}
int kz4_get_setup()
{
    u8 buf[8];
    buf[0] = 'R';
    buf[1] = 'S';
    buf[2] = 0xd;
    kz4.port->write(kz4.port, buf, 3, 0);
    delay_ms(20);
    return 0;
}
int kz4_towork(u8 mode)
{
    u8 buf[8], i;
    buf[0] = 'W';
    if (mode)
    {
        buf[1] = 'K';
        for (i = 0; i < 8; i++)
        {
            kz4_level[i] = 0;
            kz4_vals[i] = 0;
        }
    }
    else
        buf[1] = 'G';
    buf[2] = 0xd;
    kz4.port->write(kz4.port, buf, 3, 0);
    delay_ms(20);
    return 0;
}
void kz4_retrieve()
{
    *KZ4_LEVEL0 = kz4_level[0];
    *KZ4_LEVEL1 = kz4_level[1];
    *KZ4_LEVEL2 = kz4_level[2];
    *KZ4_LEVEL3 = kz4_level[3];
    *KZ4_LEVEL4 = kz4_level[4];
    *KZ4_LEVEL5 = kz4_level[5];
    *KZ4_LEVEL6 = kz4_level[6];
    *KZ4_LEVEL7 = kz4_level[7];
    *KZ4_VAL0 = kz4_vals[0];
    *KZ4_VAL1 = kz4_vals[1];
    *KZ4_VAL2 = kz4_vals[2];
    *KZ4_VAL3 = kz4_vals[3];
    *KZ4_VAL4 = kz4_vals[4];
    *KZ4_VAL5 = kz4_vals[5];
    *KZ4_VAL6 = kz4_vals[6];
    *KZ4_VAL7 = kz4_vals[7];
#if 0
    KZ4_BIAOZHUN = kz4_biaozhun;
    KZ4_WORK = 0;
    KZ4_LIJING = kz4_lijing;
    KZ4_ZHOUQI = kz4_zhouqi;
    KZ4_LIUSU = kz4_liusu;
    KZ4_LIUSU_MODE = kz4_liusu_mode;
#endif
}

void kz4_publish()
{
    if ((rt_tick_get() - kz4_lasttick) < 5000)
        return;
    if (cmdindex == 0)
    {
        if (*KZ4_WORK != kz4_work)
        {
            kz4_towork(*KZ4_WORK);
            kz4_lasttick = rt_tick_get();
        }
        else if (*KZ4_BIAOZHUN != kz4_biaozhun)
        {
            kz4_set_biaozhun();
            kz4_lasttick = rt_tick_get();
        }
        else if (*KZ4_LIJING != kz4_lijing)
        {
            kz4_set_lijing();
            kz4_lasttick = rt_tick_get();
        }
        else if (*KZ4_ZHOUQI != kz4_zhouqi)
        {
            kz4_set_zhouqi();
            kz4_lasttick = rt_tick_get();
        }
        else if ((*KZ4_LIUSU_MODE != kz4_liusu_mode) || ((*KZ4_LIUSU != kz4_liusu) && (*KZ4_LIUSU_MODE)))
        {
            kz4_set_liusu();
            kz4_lasttick = rt_tick_get();
        }
    }
    if (!kz4_work && cmdindex == 1)
    {
        kz4_get_error();
        kz4_lasttick = rt_tick_get();
    }
    if (!kz4_work && cmdindex == 2)
    {
        kz4_get_setup();
        kz4_lasttick = rt_tick_get();
    }
    cmdindex++;
    if (cmdindex > 2)
        cmdindex = 0;
}
int kz4_reciver();
void kz4_thread(void *arg)
{
    while (1)
    {
        kz4_reciver();
    }
}
void kz4_read_level(u8 *buf);

void kz4_read_level(u8 *buf)
{
    int ch = 0;
    u8 a, b, r;
    while (1)
    {
        if ((*buf < 0x20) || (*buf > '9'))
            return;
        a = *buf++;
        b = *buf++;
        if (a == 0x20)
            r = 0;
        else
            r = a - '0';
        r = r * 10;
        if (b >= 0x30)
            r += b - '0';
        kz4_level[ch] = r;
        ch++;
    }
}
int kz4_reciver()
{
    const u8 xd = 0xd;
    char buf[12];
    int i, j, a;
    buf_data_clear(&kz4.port->rx_buf);
    int len = read_until(kz4.port, &xd, 1, 120000);
    if (len <= 0)
    {
        rt_kprintf("no data\n");
        *KZ4_ERROR = 100;
        return -1;
    }
    // *KZ4_ERROR = 0;
    rt_kprintf((const char *)kz4.port->rx_buf.buf);
    switch (kz4.port->rx_buf.buf[0])
    {
    case 'H':
        for (i = 0; i < len; i++)
        {
            if (kz4.port->rx_buf.buf[i] == 'D' && kz4.port->rx_buf.buf[i + 1] == 'A' &&
                kz4.port->rx_buf.buf[i + 2] == 'T')
            {
                kz4_work = 1;
                i += 3;
                for (a = 0; a < 8; a++)
                {
                    for (j = 0; j < sizeof(buf); j++)
                    {
                        if (kz4.port->rx_buf.buf[i + j] == 'T')
                        {
                            buf[j] = 0;
                            rt_kprintf(buf);
                            sscanf((const char *)buf, "%d", &kz4_vals[a]);
                            i += j + 1;
                            break;
                        }
                        if (kz4.port->rx_buf.buf[i + j] == 'L')
                        {
                            buf[j] = 0;
                            rt_kprintf(buf);
                            sscanf((const char *)buf, "%d", &kz4_vals[a]);
                            i += j;
                            break;
                        }
                        buf[j] = kz4.port->rx_buf.buf[i + j];
                    }
                }
            }
            if (kz4.port->rx_buf.buf[i] == 'L')
            {
                kz4_read_level(&kz4.port->rx_buf.buf[i + 1]);
            }
        }
        return 'H';
    case 'W':
        //kz4_work = 0;
        if (kz4.port->rx_buf.buf[1] == 'M')
        {
            *KZ4_ERROR = kz4.port->rx_buf.buf[2] - 0x30;
            return -6;
        }
        if (kz4.port->rx_buf.buf[1] == 'K' && kz4.port->rx_buf.buf[2] == 0xd)
        {
            kz4_work = *KZ4_WORK;
            return -7;
        }
        if (kz4.port->rx_buf.buf[1] == 'G' && kz4.port->rx_buf.buf[2] == 0xd)
        {
            kz4_work = *KZ4_WORK;
            return -8;
        }
        return kz4.port->rx_buf.buf[1];
    case 'R':
        kz4_work = 0;
        if (kz4.port->rx_buf.buf[1] == 'S' && kz4.port->rx_buf.buf[2] == 'T')
        {
            kz4_biaozhun = kz4.port->rx_buf.buf[3] - 0x30;
            kz4_lijing = kz4.port->rx_buf.buf[4] - 0x30;
            kz4_zhouqi = ((kz4.port->rx_buf.buf[5] - 0x30) * 10) + (kz4.port->rx_buf.buf[6] - 0x30);
            kz4_liusu_mode = kz4.port->rx_buf.buf[7] - 0x30;
            if (kz4.port->rx_buf.buf[8] == 'V')
            {
                kz4_liusu = ((kz4.port->rx_buf.buf[9] - 0x30) * 10) + (kz4.port->rx_buf.buf[10] - 0x30);
            }
            if (!kz4_inited)
            {
                *KZ4_BIAOZHUN = kz4_biaozhun;
                *KZ4_WORK = 0;
                *KZ4_LIJING = kz4_lijing;
                *KZ4_ZHOUQI = kz4_zhouqi;
                *KZ4_LIUSU = kz4_liusu;
                *KZ4_LIUSU_MODE = kz4_liusu_mode;
                kz4_inited = 1;
            }
            return 'R';
        }
        return -2;
    default:
        *KZ4_ERROR = 0xf0;
        return -3;
    }
    return 0;
}

void kz4_init(void *pname)
{
    char *portname = pname;
    kz4_inited = 0;
    *KZ4_BIAOZHUN = 0;
    *KZ4_WORK = 0;
    *KZ4_LIJING = 0;
    *KZ4_ZHOUQI = 0;
    *KZ4_LIUSU = 0;
    *KZ4_LIUSU_MODE = 0;
    kz4_work = 0;
    kz4_biaozhun = 0;
    kz4_lijing = 0;
    kz4_zhouqi = 0;
    kz4_liusu_mode = 0;
    kz4_liusu = 0;
    kz4.port = comm_port_rt(portname);
    kz4.port->open(kz4.port, 9600, 8, 1, PARITY_NONE);
    rt_thread_t thread = rt_thread_create("kz4", kz4_thread, &kz4, 2048, PRIO_THREAD_DEVICE, RT_TICK_PER_SECOND);
    rt_thread_startup(thread);
}
struct plc_device plc_kz4 =
    {
        kz4_init,
        0,
        kz4_retrieve,
        kz4_publish,
        0,
        "KZ4",
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
    int ret;
    kz4_init("COM1");

    *KZ4_ZHOUQI = 1;
    kz4_set_zhouqi();
    ret = kz4_reciver();
    TEST_ASSERT('D' == ret || ret == 'M');
    delay_ms(3000);

    *KZ4_ZHOUQI = 12;
    kz4_set_zhouqi();
    ret = kz4_reciver();
    TEST_ASSERT('D' == ret || ret == 'M');
    delay_ms(3000);

    *KZ4_BIAOZHUN = 0;
    kz4_set_biaozhun();
    ret = kz4_reciver();
    TEST_ASSERT('H' == ret || ret == 'M');
    delay_ms(5000);

    *KZ4_LIJING = 0;
    kz4_set_lijing();
    ret = kz4_reciver();
    TEST_ASSERT('U' == ret || ret == 'M');
    delay_ms(5000);

    *KZ4_LIJING = 1;
    kz4_set_lijing();
    ret = kz4_reciver();
    TEST_ASSERT('U' == ret || ret == 'M');
    delay_ms(3000);

    *KZ4_LIUSU_MODE = 0;
    *KZ4_LIUSU = 10;
    kz4_set_liusu();
    ret = kz4_reciver();
    TEST_ASSERT('V' == ret || ret == 'M');
    delay_ms(3000);
    *KZ4_LIUSU_MODE = 1;
    *KZ4_LIUSU = 10;
    kz4_set_liusu();
    ret = kz4_reciver();
    TEST_ASSERT('V' == ret || ret == 'M');
    delay_ms(3000);
    *KZ4_LIUSU_MODE = 1;
    *KZ4_LIUSU = 50;
    kz4_set_liusu();
    ret = kz4_reciver();
    TEST_ASSERT('V' == ret || ret == 'M');

    delay_ms(3000);
    kz4_get_error();
    ret = kz4_reciver();
    TEST_ASSERT_EQUAL_INT('M', ret);

    delay_ms(3000);
    kz4_get_setup();
    ret = kz4_reciver();
    TEST_ASSERT_EQUAL_INT('R', ret);
    delay_ms(5000);

    *KZ4_BIAOZHUN = 5;
    kz4_set_biaozhun();
    ret = kz4_reciver();
    TEST_ASSERT('H' == ret || ret == 'M');
    delay_ms(5000);
    kz4_towork(1);
    while (1)
        delay_ms(3000);
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
TestRef tests_kz4(void);
TestRef tests_kz4(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures){
        new_TestFixture("testInit", testInit),
        //                        new_TestFixture("testGetValue",testGetValue),
    };
    EMB_UNIT_TESTCALLER(tests_kz4, "tests_kz4", setUp, tearDown, fixtures);

    return (TestRef)&tests_kz4;
}

#endif
