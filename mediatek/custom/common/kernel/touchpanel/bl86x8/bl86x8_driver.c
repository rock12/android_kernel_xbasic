#include "tpd.h"
#include <linux/interrupt.h>
#include <cust_eint.h>
#include <linux/i2c.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/rtpm_prio.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/delay.h>



#include <linux/buffer_head.h>
#include <linux/workqueue.h>


#include <mach/mt_pm_ldo.h>
#include <mach/mt_typedefs.h>
#include <mach/mt_boot.h>


#include "cust_gpio_usage.h"

#include "tpd_custom_bl86x8.h"

extern struct tpd_device *tpd;

static struct i2c_client *i2c_client = NULL;
static struct task_struct *thread = NULL;
static struct proc_dir_entry * bl546x_proc_node;
u16   CMDIndex = 0;
u8    tx_num = 0;
u8    rx_num = 0;


static DECLARE_WAIT_QUEUE_HEAD(waiter);
static DEFINE_MUTEX(i2c_access);


static void tpd_eint_interrupt_handler(void);


extern void mt65xx_eint_unmask(unsigned int line);
extern void mt65xx_eint_mask(unsigned int line);
extern void mt65xx_eint_set_hw_debounce(unsigned int eint_num, unsigned int ms);
extern unsigned int mt65xx_eint_set_sens(unsigned int eint_num, unsigned int sens);
extern void mt65xx_eint_registration(unsigned int eint_num, unsigned int is_deb_en, unsigned int pol, void (EINT_FUNC_PTR)(void), unsigned int is_auto_umask);


static int __devinit tpd_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int tpd_detect (struct i2c_client *client, struct i2c_board_info *info);
static int __devexit tpd_remove(struct i2c_client *client);
static int touch_event_handler(void *unused);


static int tpd_flag = 0;
static int tpd_halt=0;
static int point_num = 0;
static int p_point_num = 0;

//#define TPD_CLOSE_POWER_IN_SLEEP

#define CONFIG_BL546X_POWER_M

#define TPD_OK 0
//register define

#define DEVICE_MODE 0x00
#define GEST_ID 0x01
#define TD_STATUS 0x02

#define TOUCH1_XH 0x03
#define TOUCH1_XL 0x04
#define TOUCH1_YH 0x05
#define TOUCH1_YL 0x06

#define TOUCH2_XH 0x09
#define TOUCH2_XL 0x0A
#define TOUCH2_YH 0x0B
#define TOUCH2_YL 0x0C

#define TOUCH3_XH 0x0F
#define TOUCH3_XL 0x10
#define TOUCH3_YH 0x11
#define TOUCH3_YL 0x12
//register define

#define TPD_RESET_ISSUE_WORKAROUND

#define TPD_MAX_RESET_COUNT 3

#ifdef TPD_HAVE_BUTTON
static int tpd_keys_local[TPD_KEY_COUNT] = TPD_KEYS;
static int tpd_keys_dim_local[TPD_KEY_COUNT][4] = TPD_KEYS_DIM;
#endif
#if (defined(TPD_WARP_START) && defined(TPD_WARP_END))
static int tpd_wb_start_local[TPD_WARP_CNT] = TPD_WARP_START;
static int tpd_wb_end_local[TPD_WARP_CNT]   = TPD_WARP_END;
#endif
#if (defined(TPD_HAVE_CALIBRATION) && !defined(TPD_CUSTOM_CALIBRATION))
static int tpd_calmat_local[8]     = TPD_CALIBRATION_MATRIX;
static int tpd_def_calmat_local[8] = TPD_CALIBRATION_MATRIX;
#endif

#define VELOCITY_CUSTOM_BL86X8
#ifdef VELOCITY_CUSTOM_BL86X8
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

// for magnify velocity********************************************

#ifndef TPD_VELOCITY_CUSTOM_X
#define TPD_VELOCITY_CUSTOM_X 10
#endif
#ifndef TPD_VELOCITY_CUSTOM_Y
#define TPD_VELOCITY_CUSTOM_Y 10
#endif

#define TOUCH_IOC_MAGIC 'A'

#define TPD_GET_VELOCITY_CUSTOM_X _IO(TOUCH_IOC_MAGIC,0)
#define TPD_GET_VELOCITY_CUSTOM_Y _IO(TOUCH_IOC_MAGIC,1)

static int g_v_magnify_x =TPD_VELOCITY_CUSTOM_X;
static int g_v_magnify_y =TPD_VELOCITY_CUSTOM_Y;

#include "tpd_custom_bl86x8.h"

#define MTK_PLATFORM

#ifdef CONFIG_ONLINE_UPDATE

#if defined BLTECH_5460
#include "bl5460_fw.h"
#elif defined BLTECH_5461
#include "bl5461_bl.h"
#include "bl5461_fw.h"
#endif
#endif 

#if defined (CONFIG_BL546X_POWER_M)

#define FIRST_DELAY 10000
#define SCHE_DELAY  2000

static u8 ac_online_status = 0;

struct monitor_timer_data {
  struct timer_list timer;
  struct workqueue_struct *work_queue;
  unsigned long prev_jiffies;
};

struct monitor_timer_data monitor_data;
static struct workqueue_struct *bl546x_monitor__workqueue;

const char ac_status_file[]= "/sys/class/power_supply/ac/online";
const char usb_status_file[]= "/sys/class/power_supply/usb/online";
static void inquire_ac_status(struct work_struct *work);
static DECLARE_WORK(monitor_work, inquire_ac_status);

/*----------------------------------------------------------------------*/
void monitor_timer_fn(unsigned long arg)
{
  struct monitor_timer_data *data = (struct monitor_timer_data *) arg;
  unsigned long j = jiffies;
  data->timer.expires += msecs_to_jiffies(SCHE_DELAY);
  data->prev_jiffies = j;
  add_timer(&data->timer);
  if (queue_work(bl546x_monitor__workqueue, &monitor_work) == 0) {
    P("Timer (0) add work queue failed\n");
  }
}

/*----------------------------------------------------------------------*/
static void inquire_ac_status(struct work_struct *work)
{
  struct file *fd;
  char buffer[10];
  mm_segment_t old_fs = get_fs();
  u8 ac_online = 0;
  u8 usb_online = 0;

  P("%s \n",__func__);
  memset(buffer,0, 10);
  set_fs(KERNEL_DS);

  fd = filp_open(ac_status_file,O_RDONLY,0);

  if (IS_ERR(fd)) {
    P("can't open ac the file");
    set_fs(old_fs);
    return ;
  }

  if (fd->f_op->read(fd, buffer, 2, &fd->f_pos) <0)
    P("read file error?\n");
  filp_close(fd,NULL);

  P("ac status:%s \n",buffer);
  if (buffer[0] == '1') {
    P("ac online \n");
    ac_online = 1;
    if (ac_online != ac_online_status){    	
      if (bl546x_write_reg(BL546X_REG_AC,1) < 0)
      	P("write ac reg error !\n");     
      else 
        ac_online_status = ac_online;
    }
  } else {
    P("ac offline \n");
    ac_online = 0;
    if (ac_online != ac_online_status){
      if (bl546x_write_reg(BL546X_REG_AC,0) < 0)
      	P("write ac reg error !\n");     
      else 
        ac_online_status = ac_online;
     }
  }

  if (!ac_online) 
  {
  	fd = filp_open(usb_status_file,O_RDONLY,0);

    if (IS_ERR(fd)) {
      P("can't open ac the file");
      set_fs(old_fs);
      //ac_online_status = ac_online;
      return ;
    }

    if (fd->f_op->read(fd, buffer, 2, &fd->f_pos) <0)
      P("read file error?\n");
    filp_close(fd,NULL);

    P("usb status:%s \n",buffer);
    if (buffer[0] == '1') {
      P("usb online \n");
      usb_online = 1;
      if (usb_online != ac_online_status) {      
        if (bl546x_write_reg(BL546X_REG_AC,1) < 0)
        	P("write ac reg error !\n");        	
        else 
          ac_online_status = usb_online;
      }
    } else {
      P("usb offline \n");
      usb_online = 0;
      if (ac_online != ac_online_status) {
        if (bl546x_write_reg(BL546X_REG_AC,0) < 0)
        	P("write ac reg error !\n");  
        else       	
          ac_online_status = usb_online;
      }
    }
  }
  //ac_online_status = ac_online;
  set_fs(old_fs);
}

/*----------------------------------------------------------------------*/
static int bl546x_monitor_timer_init(void)
{
  unsigned long j = jiffies;

  init_timer(&monitor_data.timer);
  monitor_data.prev_jiffies = j;
  monitor_data.timer.function = monitor_timer_fn;
  monitor_data.timer.data = (unsigned long)(&monitor_data);
  monitor_data.timer.expires = j + msecs_to_jiffies(FIRST_DELAY);  //first delay 30 seconds
  add_timer(&monitor_data.timer);
  bl546x_monitor__workqueue = create_singlethread_workqueue("monitor-ac");

  return 0;
}

static int bl546x_destroy_monitor_workqueue(void)
{

  del_timer(&monitor_data.timer);
  if (!bl546x_monitor__workqueue) {
    cancel_work_sync(&monitor_work);
    destroy_workqueue(bl546x_monitor__workqueue);
  }


  return 1;
}

#endif
/*----------------------------------------------------------------------
*
*   BLTECH Touch panel upgrade related driver
*
*
----------------------------------------------------------------------*/

#ifdef CONFIG_APK_SUPPORT
static int bl546x_ReadFlah(u16 wAddr, u8 *pbBuf, u16 wBufSize, BL5460_FLASHTYPE FlashType)
{
    u8              abCmd[6];
    u16             wEndAddr;
    int             iRetryTimes;
    struct          i2c_msg  msg;
    int             ret = -1;

    //P("%s ++",__func__);

    switch (FlashType) {
    case BL5460_FLASHTYPE_CODE:
#if defined BLTECH_5460
        abCmd[0] = I2C_READ_FLASH_5460;
#elif defined BLTECH_5461
        abCmd[0] = I2C_READ_FLASH_5461;
#endif
        abCmd[1] = ~abCmd[0];
        if (wAddr >= 32768) return 0;
        if (wAddr + wBufSize > 32768) return 0;
        break;
#if defined BLTECH_5461
    case BL5460_FLASHTYPE_RAM:
        abCmd[0] = I2C_READ_RAM_5461;
        abCmd[1] = ~abCmd[0];
        if (wAddr >= 32768) return 0;
        if (wAddr + wBufSize > 32768) return 0;
        break;
#endif
    default:
        return 0;
    }
    wEndAddr = wAddr + wBufSize - 1;
    abCmd[2] = (u8)(wAddr >> 8);
    abCmd[3] = (u8)(wAddr & 0xff);
    abCmd[4] = (u8)(wEndAddr >> 8);
    abCmd[5] = (u8)(wEndAddr & 0xff);

    for (iRetryTimes = 3; iRetryTimes > 0; iRetryTimes --, mdelay(50)) {
        msg.addr  = 0x2c;
        msg.flags = 0;
        msg.len   = sizeof abCmd;
        msg.buf   = abCmd;
        msg.timing = 300;
        
        ret = i2c_transfer (i2c_client->adapter, &msg, 1);
        if (ret < 0) {
            P ("I2C read-write error: %d !!!\n", ret);
            continue;
        }

        msg.addr  = 0x2c;
        msg.flags = I2C_M_RD;
        msg.len   = wBufSize;
        msg.buf   = pbBuf;
        msg.timing = 300;
        
        ret = i2c_transfer (i2c_client->adapter, &msg, 1);
        if (ret < 0) {
            P ("I2C read-data error: %d !!!\n", ret);
            continue;
        }
        udelay (10);
        break;
    }

    return (iRetryTimes != 0) ? 0:-1;
}


/*--------------------------------------------------------------------*/
static int bl546x_WriteFlah(u16 wAddr, u8 *pbData, u16 wDataSize, BL5460_FLASHTYPE FlashType)
{
    u8              abCmd[6];
    u16             wEndAddr;
    int             iRetryTimes;
    struct          i2c_msg  msg;
    int             ret = -1;

    switch (FlashType) {
    case BL5460_FLASHTYPE_CODE:
#if defined BLTECH_5460
        abCmd[0] = I2C_WRITE_FLASH_5460;
#elif defined BLTECH_5461
        abCmd[0] = I2C_WRITE_FLASH_5461;
#endif
        abCmd[1] = ~abCmd[0];
        if (wAddr >= 32768) return 0;
        if (wAddr + wDataSize > 32768) return 0;
        break;
#if defined BLTECH_5461
    case BL5460_FLASHTYPE_RAM:
        abCmd[0] = I2C_WRITE_RAM_5461;
        abCmd[1] = ~abCmd[0];
        if (wAddr >= 32768) return 0;
        if (wAddr + wDataSize > 32768) return 0;
        break;
#endif
    default:
        return 0;
    }
    wEndAddr = wAddr + wDataSize - 1;
    abCmd[2] = (u8)(wAddr >> 8);
    abCmd[3] = (u8)(wAddr & 0xff);
    abCmd[4] = (u8)(wEndAddr >> 8);
    abCmd[5] = (u8)(wEndAddr & 0xff);
       

    for (iRetryTimes = 3; iRetryTimes > 0; iRetryTimes --, mdelay(50)) {
        msg.addr  = 0x2c;
        msg.flags = 0;
        msg.len   = sizeof abCmd;
        msg.buf   = abCmd;
        msg.timing = 300;
        
        ret = i2c_transfer (i2c_client->adapter, &msg, 1);
        if (ret < 0) {
            P ("I2C write-command error: %d !!!\n", ret);
            continue;
        }

        msg.addr  = 0x2c;
        msg.flags = 0;
        msg.len   = wDataSize;
        msg.buf   = pbData;
        msg.timing = 300;
        ret = i2c_transfer (i2c_client->adapter, &msg, 1);
        if (ret < 0) {
            P ("I2C  write-data error: %d !!!\n", ret);
            continue;
        }
        udelay (90);
        break;
    }

    return (iRetryTimes != 0) ? 0:-1;
}


/*--------------------------------------------------------------------*/
static int bl546x_EraseFlash(u8 fCodeFlash)
{
    u8              abCmd[4];
    u8              bBytes;
    int             iRetryTimes;
    struct          i2c_msg  msg;
    int             ret = -1;

    //P("%s ++",__func__);

    if (fCodeFlash) {
#if defined BLTECH_5460
        abCmd[0] = I2C_ERASE_FLASH_5460_W;
#elif defined BLTECH_5461
        abCmd[0] = I2C_ERASE_FLASH_5461_W;
#endif
        abCmd[1] = ~abCmd[0];
        bBytes = 2;
    } else {
        abCmd[0] = I2C_ERASE_FLASH_5460_P;
        abCmd[1] = ~abCmd[0];
        abCmd[2] = 0;
        abCmd[3] = 0;
        bBytes = 4;
    }

    for (iRetryTimes = 3; iRetryTimes > 0; iRetryTimes --, mdelay(50)) {
        msg.addr  = 0x2c;
        msg.flags = 0;
        msg.len   = bBytes;
        msg.buf   = abCmd;
        msg.timing = 300;
        ret = i2c_transfer (i2c_client->adapter, &msg, 1);
        if (ret < 0) {
            P ("I2C erase error: %d !!!\n", ret);
            continue;
        }
        mdelay (150);
        break;
    }
    return (iRetryTimes != 0) ? 0:-1;
}

/*--------------------------------------------------------------------*/
static int bl5460_EraseFlashByPage(u16 usPageNO)
{
  u8              abCmd[4];
  u8              bBytes;
  int             iRetryTimes;
  struct          i2c_msg  msg;
  int             ret = -1;

  printk("pageno:%d\n",usPageNO);

#if defined BLTECH_5460
  abCmd[0] = I2C_ERASE_PAGE_5460;
#elif defined BLTECH_5461
  abCmd[0] = I2C_ERASE_PAGE_5461;
#endif

  abCmd[1] = ~abCmd[0];
#if defined BLTECH_5460
  abCmd[2] = (u8)(((usPageNO >> 8) & 0xff) << 2);
  abCmd[3] = (u8)((usPageNO & 0xff) << 2);
#elif defined BLTECH_5461
  abCmd[2] = (u8)((usPageNO >> 8) & 0xff);
  abCmd[3] = (u8)(usPageNO & 0xff);
#endif
  bBytes = 4;
  printk("%d,%d,%d,%d\n",abCmd[0],abCmd[1],abCmd[2],abCmd[3]);

  for (iRetryTimes = 3; iRetryTimes > 0; iRetryTimes --, mdelay(50)) {
    msg.addr  = 0x2c;
    msg.flags = 0;
    msg.len   = bBytes;
    msg.buf   = abCmd;
    msg.timing = 300;
    ret = i2c_transfer(i2c_client->adapter, &msg, 1);
    if (ret < 0) {
      P("I2C erase error: %d !!!\n", ret);
      continue;
    }
    mdelay(40);
    break;
  }
  mdelay(200);
  return (iRetryTimes != 0) ? 0:-1;
}

/*--------------------------------------------------------------------*/
static int download_fw_hwmode(unsigned char *pdata, int length)
{
    int i;
    int ret = -1;
    unsigned char *preadbuf;
    char retries = 3;
    u16 wSize;
    u16 addr;


    preadbuf = kzalloc(sizeof(unsigned char)*32*1024, GFP_KERNEL);
    if (preadbuf == 0) return ret;



//#if  defined BLTECH_5460
    mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
//#elif defined BLTECH_5461
//    mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_GPIO);
//    mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_OUT);
//    mt_set_gpio_out(GPIO_CTP_EINT_PIN, GPIO_OUT_ZERO);
//#endif

#ifndef MTK_PLATFORM
    bl546x_SetWake(0);
#endif


    while (retries > 0) {
        //read old fw from flash
        for (i = 0; i < HEADER_SIZE;) {

            if (HEADER_SIZE - i > I2C_TRANSFER_RSIZE ) wSize = I2C_TRANSFER_RSIZE;
            else wSize = HEADER_SIZE - i;
#if  defined BLTECH_5460
            if (i >= 0x4000)
                addr = i & 0x3fff;
            else
                addr = i | 0x4000;
#elif defined BLTECH_5461
            addr = i;
#endif
            if (bl546x_ReadFlah(addr,&preadbuf[i],wSize,BL5460_FLASHTYPE_CODE) < 0)
                break;
            P("read -  addr:0x%04x, size :%d",i,wSize);
            i += wSize;
        }
        if (i != HEADER_SIZE) {
            retries --;
            P("read old firmware error, retry ...");
            continue;
        }
        break;
    }
    if (retries == 0) goto clean;

    /*---------------*/
    retries = 3;
    while (retries > 0) {
        memset (preadbuf, 0 ,32 *1024*sizeof (char));
        //erase flash
        P("erase flash ...\n");
        if (bl546x_EraseFlash(1) < 0) {
            retries--;
            P("erase flash fail !\n");
            continue;
        }

        //write fw to flash
        P("write firmware to flash ...\n");
        for (i = 0; i < length;) {
            if (length - i > I2C_TRANSFER_WSIZE ) wSize = I2C_TRANSFER_WSIZE;
            else wSize = length - i;
#if  defined BLTECH_5460
            if (i >= 0x4000)
                addr = i & 0x3fff;
            else
                addr = i | 0x4000;
#elif defined BLTECH_5461
            addr = i;
#endif
            if (bl546x_WriteFlah(addr,(u8 *)&pdata[i],wSize, BL5460_FLASHTYPE_CODE) < 0)
                break;

            //P("write - addr:0x%04x, size :%d",i,wSize);
            i += wSize;
            udelay(65);
        }
        if (i != length) {
            retries--;
            P("write firmware error, retry ...");
            continue;
        }

        // read fw from flash
        P("read firmware from flash ...\n");
        for (i = 0; i < length;) {
            if (length - i > I2C_TRANSFER_RSIZE ) wSize = I2C_TRANSFER_RSIZE;
            else wSize = length - i;
#if defined BLTECH_5460
            if (i >= 0x4000)
                addr = i & 0x3fff;
            else
                addr = i | 0x4000;
#elif defined BLTECH_5461
            addr = i;

#endif
            if (bl546x_ReadFlah(addr,&preadbuf[i],wSize,BL5460_FLASHTYPE_CODE) < 0)
                break;
            //P("read -  addr:0x%04x, size :%d",i,wSize);
            i += wSize;
        }
        if (i != length) {
            retries --;
            P("read firmware error, retry ...");
            continue;
        }

        // verify fw
        P("verify firmware ...\n");
        for (i = 0; i < length;) {
            if (length - i > I2C_VERFIY_SIZE ) wSize = I2C_VERFIY_SIZE;
            else wSize = length - i;
            //P("verify addr: 0x%04x\n",i);
            if (memcmp(&pdata[i],&preadbuf[i],wSize))break;
            i += wSize;
        }

        if (i == length) {
            ret = 0;
            retries = 0;
            printk("download firmware success !\n");
            break;
        } else {
            printk("verify firmware fail !\n");
            retries--;
        }
    }

clean:


#ifdef MTK_PLATFORM
#if  defined BLTECH_5460
    mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
#endif 
#else 
#if  defined BLTECH_5460
    bl546x_SetWake(0);
#endif
#endif 
    mdelay(30);
    kfree(preadbuf);

#ifdef MTK_PLATFORM    
#if  defined BLTECH_5461
    mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_EINT);
    mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(GPIO_CTP_EINT_PIN, GPIO_PULL_ENABLE);
    mt_set_gpio_pull_select(GPIO_CTP_EINT_PIN, GPIO_PULL_UP);
#endif 
#else 
#if defined BLTECH_5461
    bl546x_SetIRQGPIO();
#endif 
#endif 

    return ret;

}


#if defined BLTECH_5460
static int bl5460_fw_download(unsigned char *pdata, int length)
{
    struct timeval t1,t2;
    do_gettimeofday(&t1);
    if (download_fw_hwmode(pdata,length) < 0) return -1;
    do_gettimeofday(&t2);
    P("cost time: %lu \n", t2.tv_sec - t1.tv_sec);

    return 0;
}

#endif

#if defined BLTECH_5461
static int bl5461_fw_download(unsigned char *pdata, int length)
{
    struct timeval t1,t2;
    do_gettimeofday(&t1);

    P("%s \n", __func__);

    if (download_fw_hwmode(pdata,length) < 0) return -1;
    do_gettimeofday(&t2);
    P("cost time: %lu \n", t2.tv_sec - t1.tv_sec);
    return 0;
}
#endif

static int boot_fw_download(void)
{
    printk ("%s \n", __func__);
#if defined BLTECH_5460
    bl5460_fw_download((unsigned char *)fwbin,FW_SIZE);
#elif defined 	BLTECH_5461
    bl5461_fw_download((unsigned char *)fwbin,FW_SIZE);
#endif
    return 1;
}


/*--------------------------------------------------------------------*/
static int apk_fw_download(unsigned char *pdata, int length)
{

  P("%s \n", __func__);
#if defined BLTECH_5460
  bl5460_fw_download(pdata,length);
#elif defined   BLTECH_5461
  bl5461_fw_download(pdata,length);
#endif
  return 1;
}

/*--------------------------------------------------------------------*/
static int apk_data_download(unsigned char *pdata, int length,int iFlashAddr)
{

  u16 bsPageNO,bePageNO,PageNO,i,wSize,addr;
  u8  retries = 3;
  u8  *preadbuf;
  int ret = 0;

  P("%s \n", __func__);


  if (iFlashAddr + length > 0x8000) return 0;
  preadbuf = kzalloc(sizeof(unsigned char) *1*1024, GFP_KERNEL);
  if (preadbuf == 0) return 0;

#ifdef MTK_PLATFORM
  mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
  mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
  mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
#else   
  bl546x_SetWake(0);
#endif 

#if defined BLTECH_5460
  if (iFlashAddr < 0x4000) {
    bsPageNO = ((iFlashAddr | 0X4000) >> 10);
    bePageNO = (((iFlashAddr + length-1) | 0X4000) >> 10);
  } else {
    bsPageNO = ((iFlashAddr & 0x3fff) >> 10);
    bePageNO = (((iFlashAddr + length-1) & 0x3fff) >> 10);
  }
#elif defined BLTECH_5461
  bsPageNO = iFlashAddr >> 10;
  bePageNO = (iFlashAddr + length - 1) >> 10;

#endif
  PageNO = (bsPageNO << 8) + bePageNO;
  printk("pageno: %d, %d, %d\n",PageNO,bsPageNO,bePageNO);
  bl5460_EraseFlashByPage(PageNO);



  //write fw to flash
  while (retries > 0) {
    P("write data to flash ...\n");
    for (i = 0; i < length;) {
      if (length - i > I2C_TRANSFER_WSIZE)
        wSize = I2C_TRANSFER_WSIZE;
      else wSize = length - i;
      addr = i + iFlashAddr;
#if  defined BLTECH_5460
      if (addr >= 0x4000)
        addr &= 0x3fff;
      else
        addr |= 0x4000;
#elif defined BLTECH_5461
      addr = i;
#endif
      if (bl546x_WriteFlah(addr, (u8 *) &pdata[i],wSize, BL5460_FLASHTYPE_CODE) < 0)
        break;

      i += wSize;
      udelay(65);
    }
    if (i != length) {
      retries--;
      P("write firmware error, retry ...");
      continue;
    }
    // read fw from flash
    P("read firmware from flash ...\n");
    for (i = 0; i < length;) {
      if (length - i > I2C_TRANSFER_RSIZE)
        wSize = I2C_TRANSFER_RSIZE;
      else wSize = length - i;
      addr = i + iFlashAddr;
#if defined BLTECH_5460
      if (addr >= 0x4000)
        addr &= 0x3fff;
      else
        addr |= 0x4000;
#elif defined BLTECH_5461
      addr = i;

#endif
      if (bl546x_ReadFlah(addr,&preadbuf[i],wSize,BL5460_FLASHTYPE_CODE) < 0)
        break;
      //P("read - addr:0x%04x, size :%d",i,wSize);
      i += wSize;
    }
    if (i != length) {
      retries --;
      P("read firmware error, retry ...");
      continue;
    }

    // verify fw
    P("verify firmware ...\n");
    for (i = 0; i < length;) {
      if (length - i > I2C_VERFIY_SIZE) wSize = I2C_VERFIY_SIZE;
      else wSize = length - i;
      //P("verify addr: 0x%04x\n",i);
      if (memcmp(&pdata[i],&preadbuf[i],wSize)) break;
      i += wSize;
    }


    if (i == length) {
      ret = 0;
      retries = 0;
      P("download data success !\n");
      break;
    } else {
      printk("verify data fail !\n");
      retries--;
    }
  }
  kfree(preadbuf);
#if defined BLTECH_5460
#ifdef MTK_PLATFORM   
   mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
   mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
   mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
#else    
   bl546x_SetWake(1);
#endif    
#endif 
  mdelay(20);
  return ret;
}

static struct file * open_fw_file(char * path, mm_segment_t * old_fs_p) {
  struct file * filp = NULL;
  int errno = -1;
  P("open_fw_file \n");
  *old_fs_p = get_fs();
  set_fs(KERNEL_DS);

  P("file %s \n", path);
  filp = filp_open(path, O_RDONLY, 0644);

  if (!filp || IS_ERR(filp)) {
    if (!filp)
      errno = -ENOENT;
    else
      errno = PTR_ERR(filp);
    pr_err("The update file for Guitar open error.n");
    return NULL;
  }


  filp->f_op->llseek(filp,0,0);
  return filp ;
}

static void close_fw_file(struct file * filp, mm_segment_t old_fs)
{
  set_fs(old_fs);
  if (filp)
    filp_close(filp, NULL);
}

static int read_fw_file(unsigned char *pfilename, unsigned char *pdata, int *plen)
{
  struct file *fp;
  int size;
  int length = 0;
  int ret = -1;
  mm_segment_t old_fs;

  if (!pdata) return ret;
  fp = open_fw_file(pfilename,&old_fs);
  if (fp == NULL) goto clean;
  length = fp->f_op->llseek(fp, 0, SEEK_END);
  P("fw size:%d \n",length);
  fp->f_op->llseek(fp, 0, 0);
  size = fp->f_op->read(fp, pdata, length, &fp->f_pos);
  if (size == length) {
    P("read ok \n");
    ret = 0;
    *plen = length;
  } else
    P("read size:%d \n",size);

clean:
  close_fw_file(fp,old_fs);
  return ret;
}

static int bl546x_update_write(struct file * filp,const char __user * buff,unsigned long len,void * data)
{
  unsigned char cmd[28*16*2];
  unsigned char *pdata = NULL;
  int length = 32*1024;
  unsigned char val;
  unsigned short address = 0;

  memset(cmd,0,128);

  if (copy_from_user(cmd, buff, len))
    return -EFAULT;

  P("%s \n", cmd);
  switch (cmd[0]) {
    case '1': //update file;
      P("update file \n");
      pdata = kzalloc(sizeof(char) *length, GFP_KERNEL);
      if (!pdata) {
        pr_err("can't allocate buffer for data\n");
        break;
      }
      if (read_fw_file(&cmd[1],pdata,&length)) break;
      if (pdata != NULL || length != 0) {
        apk_fw_download(pdata,length);
        kfree(pdata);
        pdata= NULL;
      }
      break;
    case '2': //transfer to enginner mode
      if (bl546x_read_reg(BL546X_REG_WORKMODE,&val,1) < 0) {
        pr_err("read work mode error !\n");
        return 0;
      }
      if (val != BL546X_MODE_TEST) {
        if (bl546x_write_reg(BL546X_REG_WORKMODE,BL546X_MODE_TEST) < 0) {
          pr_err("set work mode error !\n");
          return 0;
        }
      }
      break;
    case '3': //transfer to normal mode
      if (bl546x_read_reg(BL546X_REG_WORKMODE,&val,1) < 0) {
        pr_err("read work mode error !\n");
        return 0;
      }
      if (val != BL546X_MODE_OPERATION) {
        if (bl546x_write_reg(BL546X_REG_WORKMODE,BL546X_MODE_OPERATION) < 0) {
          pr_err("set work mode error !\n");
          return 0;
        }
      }
      break;
    case '4': //set read cmd index
      CMDIndex = cmd[1] - 0x30;
      break;
    case '5'://clear cread cmd index
      CMDIndex = 0;
      break;
    case '6'://write compensation data
      CMDIndex = 6;
      address = cmd[2] + (cmd[1] << 8);

      printk("addr: %x\n",address);
      apk_data_download(&cmd[3],len-3,address);
      break;


    default:
      break;
  }
  return len;

}

static int bl546x_update_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
  unsigned char cmd[128];
  unsigned char val;
  int len = 0;
  unsigned short *pdata = NULL;
  unsigned char bRetries = 0, bTxCount;
  int i,j;

  memset(cmd,0,128);
  P("%s %d\n", __func__,CMDIndex);
  if (CMDIndex == 0) {
    sprintf(cmd,"bltech ctp driver 2.0 \n");
    strcpy(page, cmd);
    *eof = 1;
    len = strlen(cmd);
  } else if (CMDIndex == 2) {
    pdata = kzalloc(sizeof(short) *1024, GFP_KERNEL);
    if (pdata == NULL) {
      pr_err("read work mode error !\n");
      return 0;
    }
    if (bl546x_read_reg(BL546X_REG_WORKMODE,&val,1) < 0) {
      pr_err("read work mode error !\n");
      return 0;
    }
    if (val != BL546X_MODE_TEST) {
      P("write TEST workmode\n");
      if (bl546x_write_reg(BL546X_REG_WORKMODE,BL546X_MODE_TEST) < 0) {
        pr_err("set work mode error !\n");
        return 0;
      }
    }
    mdelay(100);
    for (j = 0; j < 1; j++) {
      if (bl546x_read_reg(BL546X_REG_WORKMODE,&val,1) < 0) {
        pr_err("read work mode error !\n");
        return 0;
      }
      if (val == BL546X_MODE_TEST) {
        if (bl546x_write_reg(BL546X_REG_WORKMODE,val|BL546X_MODE_READTOGGLE) < 0) {
          pr_err("set scan mode error !\n");
          return 0;
        }
      } else return 0;
      for (bRetries=0; bRetries<5; bRetries++) {
        mdelay(20);
        if (bl546x_read_reg(BL546X_REG_WORKMODE,&val,1) < 0) {
          pr_err("read work mode error !\n");
          return 0;
        }
        if ((val & BL546X_MODE_READTOGGLE) == 0) break;
      }
      if (bRetries == 5)
        P("scan timeout %d \n",val);
      for (bTxCount=0; bTxCount<tx_num; bTxCount++) {
        if (bl546x_write_reg(BL546X_TSREG_READ_ROW_ADDR,bTxCount) < 0) {
          pr_err("write read address error !\n");
          return 0;
        }

        if (bl546x_read_reg(BL546X_TSREG_RAWDATA_BEGIN_ADDR, (unsigned char *) &pdata[bTxCount*rx_num],rx_num*sizeof(short)) < 0) {
          pr_err("read raw address error !\n");
          return 0;
        }

        for (i = 0; i < rx_num; i++)
          pdata[bTxCount*rx_num+i] = mfSwapWord(pdata[bTxCount*rx_num+i]);
      }
#if 0
      for (i = 0; i < p_ts_data->tx_num; i++) {
        for (j = 0; j < p_ts_data->rx_num; j++)
          printk("%d ",pdata[i*p_ts_data->rx_num+j]);
        printk("\n");

      }
      mdelay(20);
#endif
    }
    memcpy(page,pdata,tx_num*rx_num*2);
    if (pdata != NULL)
      kfree(pdata);
    len = tx_num*rx_num*2;
    *eof = 1;

  } else if (CMDIndex == 1) {

    unsigned char buf[10];
    if (bl546x_read_reg(BL546X_REG_WORKMODE,&val,1) < 0) {
      pr_err("read work mode error !\n");
      return 0;
    }
    if (val != BL546X_MODE_TEST) {
      return 0;
    }
    if (bl546x_read_reg(BL546X_TSREG_PANEL_ROWS, (unsigned char *) &buf,2) < 0) {
      pr_err("read work mode error !\n");
      return 0;
    }
    printk("%d,%d\n",buf[0],buf[1]);
    tx_num = buf[0];
    rx_num = buf[1];
    memcpy(page,buf,2);
    len = 2;
    *eof = 1;
  } else if (CMDIndex == 3) {

    unsigned char buf[20];
    if (bl546x_read_reg(BL546X_REG_WORKMODE,&val,1) < 0) {
      pr_err("read work mode error !\n");
      return 0;
    }
    if (val != BL546X_MODE_TEST) {
      return 0;
    }
    if (bl546x_read_reg(BL546X_TSREG_COL_OFFSET0, (unsigned char *) &buf,rx_num) < 0) {
      pr_err("read work mode error !\n");
      return 0;
    }
    for (i = 0; i < rx_num; i++)
      printk("%d, ",buf[i]);
    memcpy(page,buf,rx_num);
    len = rx_num;
    *eof = 1;

  }
  printk("len:%d \n",len);
  return len;
}

#endif 
/*--------------------------------------------------------------------*/
static int bl546x_write_reg(unsigned char reg, unsigned char val)
{
  int             ret = 0;
/*  
  unsigned char   abCmd[2];
  

  abCmd[0] = reg;
  abCmd[1] = val;

  ret = i2c_master_send(i2c_client,abCmd,2);
  if (ret < 0) {
    P("I2C write reg error: %d !!!\n",ret);
    return -1;
  }
*/  
  if (i2c_smbus_write_i2c_block_data(i2c_client, reg, 1, &val) < 0)
  	return -1;
  ret = 0;
  return ret;
}

/*--------------------------------------------------------------------*/
static int bl546x_read_reg(unsigned char reg, unsigned char *pBuf, int iBytes)
{
	
  int             ret = 0;
/*  
  unsigned char   abCmd[1];
  abCmd[0] = reg;
  ret = i2c_master_send(i2c_client,abCmd,1);
  if (ret < 0) {
    P("I2C read - write error: %d !!!\n", ret);
    return -1;
  }
  ret  = i2c_master_recv(i2c_client, pBuf, iBytes);
  if (ret < 0) {
    P("I2C read - read error: %d !!!\n", ret);
    return -1;
  }
*/  
  if (i2c_smbus_read_i2c_block_data(i2c_client, reg, iBytes, pBuf) < 0)
  	return -1;
  ret = 0;
  return ret;
}

static int tpd_misc_open(struct inode *inode, struct file *file)
{
    return nonseekable_open(inode, file);
}
/*----------------------------------------------------------------------------*/
static int tpd_misc_release(struct inode *inode, struct file *file)
{
    //file->private_data = NULL;
    return 0;
}
/*----------------------------------------------------------------------------*/
static long tpd_unlocked_ioctl(struct file *file, unsigned int cmd,
                               unsigned long arg)
{
    void __user *data;

    long err = 0;

    if (_IOC_DIR(cmd) & _IOC_READ)
    {
        err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
    }
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
    {
        err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
    }

    if (err)
    {
        printk("tpd: access error: %08X, (%2d, %2d)\n", cmd, _IOC_DIR(cmd), _IOC_SIZE(cmd));
        return -EFAULT;
    }

    switch (cmd)
    {
    case TPD_GET_VELOCITY_CUSTOM_X:
        data = (void __user *) arg;
        if (data == NULL)
        {
            err = -EINVAL;
            break;
        }

        if (copy_to_user(data, &g_v_magnify_x, sizeof(g_v_magnify_x)))
        {
            err = -EFAULT;
            break;
        }
        break;

    case TPD_GET_VELOCITY_CUSTOM_Y:
        data = (void __user *) arg;
        if (data == NULL)
        {
            err = -EINVAL;
            break;
        }

        if (copy_to_user(data, &g_v_magnify_y, sizeof(g_v_magnify_y)))
        {
            err = -EFAULT;
            break;
        }
        break;


    default:
        printk("tpd: unknown IOCTL: 0x%08x\n", cmd);
        err = -ENOIOCTLCMD;
        break;

    }

    return err;
}


static struct file_operations tpd_fops = {
            //	.owner = THIS_MODULE,
            .open = tpd_misc_open,
                    .release = tpd_misc_release,
                               .unlocked_ioctl = tpd_unlocked_ioctl,
                                             };
/*----------------------------------------------------------------------------*/
static struct miscdevice tpd_misc_device = {
            .minor = MISC_DYNAMIC_MINOR,
                     .name = "touch",
                             .fops = &tpd_fops,
                                 };

//**********************************************
#endif

struct touch_info {
    int y[5];
    int x[5];
    int p[5];
    int id[5];
    int count;
};

static const struct i2c_device_id bl86x8_tpd_id[] = {{"bl86x8",0},{}};
static struct i2c_board_info __initdata bl86x8_i2c_tpd={ I2C_BOARD_INFO("bl86x8", (0x50>>1))};


static struct i2c_driver tpd_i2c_driver = {
            .driver = {
                          .name = "bl86x8",//.name = TPD_DEVICE,
                          //	 .owner = THIS_MODULE,
                      },
                      .probe = tpd_probe,
                               .remove = __devexit_p(tpd_remove),
                                         .id_table = bl86x8_tpd_id,
                                                     .detect = tpd_detect,
                                                               //  .address_data = &addr_data,
                                                               //   .address_list = (const unsigned short*) forces,
                                                           };


static  void tpd_down(int x, int y, int p) {
    // input_report_abs(tpd->dev, ABS_PRESSURE, p);
    input_report_key(tpd->dev, BTN_TOUCH, 1);
    input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 20);
    input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
    input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);
    //printk("D[%4d %4d %4d] ", x, y, p);
    /* track id Start 0 */
    input_report_abs(tpd->dev, ABS_MT_TRACKING_ID, p);
    input_mt_sync(tpd->dev);

    if (FACTORY_BOOT == get_boot_mode()|| RECOVERY_BOOT == get_boot_mode())
    {
        tpd_button(x, y, 1);
    }

    if (y > TPD_RES_Y) //virtual key debounce to avoid android ANR issue
    {
        msleep(50);
        printk("D virtual key \n");
    }

    TPD_EM_PRINT(x, y, x, y, p-1, 1);
}

static  void tpd_up(int x, int y,int *count) {
    //if(*count>0) {
    //input_report_abs(tpd->dev, ABS_PRESSURE, 0);
    input_report_key(tpd->dev, BTN_TOUCH, 0);
    //input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 0);
    //input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
    //input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);
    //printk("U[%4d %4d %4d] ", x, y, 0);
    input_mt_sync(tpd->dev);
    TPD_EM_PRINT(x, y, x, y, 0, 0);
    //	 (*count)--;
    if (FACTORY_BOOT == get_boot_mode()|| RECOVERY_BOOT == get_boot_mode())
    {
        tpd_button(x, y, 0);
    }

}

static int tpd_touchinfo(struct touch_info *cinfo, struct touch_info *pinfo)
{

    int i = 0;
    //char data[30] = {0};
    char data[32] = {0};//------ben change
    //u16 high_byte,low_byte;
    u8  offset,report_rate =0;

    p_point_num = point_num;
    mutex_lock(&i2c_access);
    if (tpd_halt)
    {
        mutex_unlock(&i2c_access);
        TPD_DMESG( "tpd_touchinfo return ..\n");
        return false;
    }
    i2c_smbus_read_i2c_block_data(i2c_client, 0x00, 8, &(data[0]));
    i2c_smbus_read_i2c_block_data(i2c_client, 0x08, 8, &(data[8]));
    i2c_smbus_read_i2c_block_data(i2c_client, 0x10, 8, &(data[16]));
    //i2c_smbus_read_i2c_block_data(i2c_client, 0x18, 6, &(data[24]));
    i2c_smbus_read_i2c_block_data(i2c_client, 0x18, 8, &(data[24]));//------ben change


    ////i2c_smbus_read_i2c_block_data(i2c_client, 0xa6, 1, &(data[24]));
    ////i2c_smbus_read_i2c_block_data(i2c_client, 0x88, 1, &report_rate);

    //we have  to re update report rate
    // TPD_DMESG("report rate =%x\n",report_rate);
    if (report_rate < 8)
    {
        report_rate = 0x8;
        if ((i2c_smbus_write_i2c_block_data(i2c_client, 0x88, 1, &report_rate))< 0)
        {
            TPD_DMESG("I2C read report rate error, line: %d\n", __LINE__);
        }
    }

    mutex_unlock(&i2c_access);
    /* Device Mode[2:0] == 0 :Normal operating Mode*/
    if ((data[0] & 0x50) != 0) return false;  ///0x70

    /*get the number of the touch points*/
    point_num= data[2] & 0x0f;

    for (i = 0; i < point_num; i++)
    {
        offset          = 3+6*i;
        cinfo->p[i] = data[offset] >> 6; //event flag
        cinfo->id[i] = data[offset+2]>>4; //touch id

        cinfo->x[i] = ((data[offset] & 0x0F)<<8) |(data[offset + 1]);
        cinfo->y[i] = ((data[offset + 2] & 0x0F)<<8) |(data[offset + 3]);
        cinfo->count++;



        //event.y = (((s16)(buf[offset+0] & 0x0F))<<8) | ((s16)buf[offset+1]);
        //event.x = (((s16)(buf[offset+2] & 0x0F))<<8) | ((s16)buf[offset+3]);
    }

    TPD_DEBUG(" bl86x8:point_num = %d\n", point_num);
    TPD_DEBUG(" cinfo->x[0] = %d, cinfo->y[0] = %d, cinfo->p[0] = %d,cinfo->id[0] = %d \n", cinfo->x[0], cinfo->y[0], cinfo->p[0],cinfo->id[0]);
    TPD_DEBUG(" cinfo->x[1] = %d, cinfo->y[1] = %d, cinfo->p[1] = %d,cinfo->id[1] = %d \n", cinfo->x[1], cinfo->y[1], cinfo->p[1],cinfo->id[1]);
    TPD_DEBUG(" cinfo->x[2]= %d, cinfo->y[2]= %d, cinfo->p[2] = %d,cinfo->id[2] = %d \n", cinfo->x[2], cinfo->y[2], cinfo->p[2],cinfo->id[2]);
    TPD_DEBUG(" cinfo->x[3]= %d, cinfo->y[3]= %d, cinfo->p[3] = %d,cinfo->id[3] = %d \n", cinfo->x[3], cinfo->y[3], cinfo->p[3],cinfo->id[3]);
    TPD_DEBUG(" cinfo->x[4]= %d, cinfo->y[4]= %d, cinfo->p[4] = %d,cinfo->id[4] = %d \n", cinfo->x[4], cinfo->y[4], cinfo->p[4],cinfo->id[4]);

    return true;

};

static int touch_event_handler(void *unused)
{

    struct touch_info cinfo, pinfo;
    int i=0;

    struct sched_param param = { .sched_priority = RTPM_PRIO_TPD };
    sched_setscheduler(current, SCHED_RR, &param);

    do
    {
        mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);
        set_current_state(TASK_INTERRUPTIBLE);
        wait_event_interruptible(waiter,tpd_flag!=0);

        tpd_flag = 0;
        set_current_state(TASK_RUNNING);

        if (tpd_touchinfo(&cinfo, &pinfo))
        {
            //TPD_DEBUG("point_num = %d\n",point_num);
            TPD_DEBUG_SET_TIME;
            if (point_num >0)
            {
                for (i =0; i<point_num && i<5; i++)//only support 3 point
                {
                    tpd_down(cinfo.x[i], cinfo.y[i], cinfo.id[i]);
                }
                input_sync(tpd->dev);
            }
            else
            {
                tpd_up(cinfo.x[0], cinfo.y[0], 0);
                TPD_DEBUG("bl86x8:release --->\n");
                //input_mt_sync(tpd->dev);
                input_sync(tpd->dev);
            }
        }

        if (tpd_mode==12)
        {
            //power down for desence debug
            //power off, need confirm with SA
            hwPowerDown(MT6323_POWER_LDO_VGP1,  "TP");
            hwPowerDown(MT6323_POWER_LDO_VIO28,  "TP");
            msleep(20);

        }

    }while (!kthread_should_stop());

    return 0;
}

static int tpd_detect (struct i2c_client *client, struct i2c_board_info *info)
{
    strcpy(info->type, "bl86x8");
    return 0;
}

static void tpd_eint_interrupt_handler(void)
{
    //TPD_DEBUG("TPD interrupt has been triggered\n");
    TPD_DEBUG_PRINT_INT;
    tpd_flag = 1;
    wake_up_interruptible(&waiter);

}
static int __devinit tpd_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int retval = TPD_OK;
    char data;
    u8 report_rate=0;
    int err=0;
    int reset_count = 0;

reset_proc:
    i2c_client = client;


    //i2c_client->timing = 100;
    //power on, need confirm with SA
    hwPowerDown(MT6323_POWER_LDO_VGP1,"TP");
    hwPowerDown(MT6323_POWER_LDO_VIO28,"TP");

    hwPowerOn(MT6323_POWER_LDO_VGP1, VOL_2800, "TP");
    hwPowerOn(MT6323_POWER_LDO_VIO28, VOL_2800, "TP");
    msleep(20);

#ifdef TPD_CLOSE_POWER_IN_SLEEP
    hwPowerDown(TPD_POWER_SOURCE,"TP");
    hwPowerOn(TPD_POWER_SOURCE,VOL_3300,"TP");
    msleep(100);
#else

    mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
    msleep(20);

    TPD_DMESG(" bl86x8 reset\n");

    mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
#endif

    mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_EINT);
    mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(GPIO_CTP_EINT_PIN, GPIO_PULL_ENABLE);
    mt_set_gpio_pull_select(GPIO_CTP_EINT_PIN, GPIO_PULL_UP);

    mt65xx_eint_set_sens(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_SENSITIVE);
    mt65xx_eint_set_hw_debounce(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_CN);
    mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_TOUCH_PANEL_POLARITY, tpd_eint_interrupt_handler, 1);
    mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);

    msleep(100);

    if ((i2c_smbus_read_i2c_block_data(i2c_client, 0x00, 1, &data))< 0)
    {
        TPD_DMESG("I2C transfer error, line: %d\n", __LINE__);
#ifdef TPD_RESET_ISSUE_WORKAROUND
        if ( reset_count < TPD_MAX_RESET_COUNT )
        {
            reset_count++;
            goto reset_proc;
        }
#endif
        return -1;
    }

    //set report rate 80Hz
    report_rate = 0x8;
    if ((i2c_smbus_write_i2c_block_data(i2c_client, 0x88, 1, &report_rate))< 0)
    {
        if ((i2c_smbus_write_i2c_block_data(i2c_client, 0x88, 1, &report_rate))< 0)
        {
            TPD_DMESG("I2C read report rate error, line: %d\n", __LINE__);
        }

    }

    tpd_load_status = 1;

#ifdef VELOCITY_CUSTOM_BL86X8
    if ((err = misc_register(&tpd_misc_device)))
    {
        printk("mtk_tpd: tpd_misc_device register failed\n");

    }
#endif

    thread = kthread_run(touch_event_handler, 0, TPD_DEVICE);
    if (IS_ERR(thread))
    {
        retval = PTR_ERR(thread);
        TPD_DMESG(TPD_DEVICE " failed to create kernel thread: %d\n", retval);
    }

    TPD_DMESG("bl86x8 Touch Panel Device Probe %s\n", (retval < TPD_OK) ? "FAIL" : "PASS");
    bl546x_monitor_timer_init();   
#if defined(CONFIG_APK_SUPPORT)
  bl546x_proc_node = create_proc_entry("bl546x", 0666, NULL);
  if (bl546x_proc_node == NULL) {
    TPD_DMESG("Couldn't create proc entry!\n");    
  } else {
    TPD_DMESG("Create proc entry success!\n");
    bl546x_proc_node->write_proc = bl546x_update_write;
    bl546x_proc_node->read_proc = bl546x_update_read;
  }
#endif
    return 0;

}

static int __devexit tpd_remove(struct i2c_client *client)
{

    TPD_DEBUG("TPD removed\n");
    return 0;
}


static int tpd_local_init(void)
{


    TPD_DMESG("Blestech BL86x8 I2C Touchscreen Driver (Built %s @ %s)\n", __DATE__, __TIME__);

    if (i2c_add_driver(&tpd_i2c_driver)!=0)
    {
        TPD_DMESG("bl86x8 unable to add i2c driver.\n");
        return -1;
    }

    if (tpd_load_status == 0)
    {
        TPD_DMESG("bl86x8 add error touch panel driver.\n");
        i2c_del_driver(&tpd_i2c_driver);
        return -1;
    }

#ifdef TPD_HAVE_BUTTON
    tpd_button_setting(TPD_KEY_COUNT, tpd_keys_local, tpd_keys_dim_local);// initialize tpd button data
#endif

#if (defined(TPD_WARP_START) && defined(TPD_WARP_END))
    TPD_DO_WARP = 1;
    memcpy(tpd_wb_start, tpd_wb_start_local, TPD_WARP_CNT*4);
    memcpy(tpd_wb_end, tpd_wb_start_local, TPD_WARP_CNT*4);
#endif

#if (defined(TPD_HAVE_CALIBRATION) && !defined(TPD_CUSTOM_CALIBRATION))
    memcpy(tpd_calmat, tpd_def_calmat_local, 8*4);
    memcpy(tpd_def_calmat, tpd_def_calmat_local, 8*4);
#endif
    TPD_DMESG("end %s, %d\n", __FUNCTION__, __LINE__);
    tpd_type_cap = 1;
    return 0;
}

static void tpd_resume( struct early_suspend *h )
{
    //int retval = TPD_OK;
    char data;
    msleep(100);///ÐÞ¸Ä´¦SKY
    TPD_DMESG("TPD wake up\n");
#ifdef TPD_CLOSE_POWER_IN_SLEEP
    hwPowerOn(TPD_POWER_SOURCE,VOL_3300,"TP");
#else

    mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
    msleep(30);
    mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
    mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
    mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
#endif
    mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);
    msleep(20);
    tpd_halt = 0;
    if ((i2c_smbus_read_i2c_block_data(i2c_client, 0x00, 1, &data))< 0)
    {
        TPD_DMESG("resume I2C transfer error, line: %d\n", __LINE__);

    }
    tpd_up(0,0,0);
    input_sync(tpd->dev);
    TPD_DMESG("TPD wake up done\n");
    //return retval;
}

static void tpd_suspend( struct early_suspend *h )
{
    // int retval = TPD_OK;
    static char data = 0x3;
    tpd_halt = 1;
    TPD_DMESG("TPD enter sleep\n");
    ac_online_status = 0;
    mt65xx_eint_mask(CUST_EINT_TOUCH_PANEL_NUM);
    mutex_lock(&i2c_access);
#ifdef TPD_CLOSE_POWER_IN_SLEEP
    hwPowerDown(TPD_POWER_SOURCE,"TP");
#else
    i2c_smbus_write_i2c_block_data(i2c_client, 0xA5, 1, &data);  //TP enter sleep mode
#endif
    mutex_unlock(&i2c_access);

    TPD_DMESG("TPD enter sleep done\n");
    //return retval;
}


static struct tpd_driver_t tpd_device_driver = {
            .tpd_device_name = "bl86x8",
                               .tpd_local_init = tpd_local_init,
                                                 .suspend = tpd_suspend,
                                                            .resume = tpd_resume,
#ifdef TPD_HAVE_BUTTON
                                                                      .tpd_have_button = 1,
#else
                                                                      .tpd_have_button = 0,
#endif
                                                                                     };
/* called when loaded into kernel */
static int __init tpd_driver_init(void) {
    printk(" bl86x8 touch panel driver init\n");
    i2c_register_board_info(1, &bl86x8_i2c_tpd, 1);
    if (tpd_driver_add(&tpd_device_driver) < 0)
        TPD_DMESG("add bl86x8 driver failed\n");
    return 0;
}

/* should never be called */
static void __exit tpd_driver_exit(void) {
    TPD_DMESG(" bl86x8 touch panel driver exit\n");
    //input_unregister_device(tpd->dev);
    tpd_driver_remove(&tpd_device_driver);
}

module_init(tpd_driver_init);
module_exit(tpd_driver_exit);


