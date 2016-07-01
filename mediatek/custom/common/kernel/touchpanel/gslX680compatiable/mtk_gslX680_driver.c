
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/bitops.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/byteorder/generic.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/rtpm_prio.h>
#include "tpd.h"
#include <cust_eint.h>
#ifdef MT6573
#include <mach/mt6573_boot.h>
#include <mt6573_kpd.h>
#endif
#ifdef MT6575
#include <mt6575_kpd.h>
#include <mach/mt6575_pm_ldo.h>
#include <mach/mt6575_typedefs.h>
#include <mach/mt6575_boot.h>
#endif

#ifdef MT6577
#include <mach/mt6577_pm_ldo.h>
#include <mach/mt6577_typedefs.h>
#include <mach/mt6577_boot.h>
#endif
#include <mach/mt_pm_ldo.h>
#include <mach/mt_typedefs.h>
#include <mach/mt_boot.h>

#include "mtk_gslX680_driver.h"
#include "tpd_custom_gslX680.h"

//#define GSL_DEBUG
//#define GSL_MONITOR

#define GSL1688E_NAME	"gsl1688E"
#define GSLX680_ADDR	0x46
#define MAX_FINGERS	  	5
#define MAX_CONTACTS	10
#define DMA_TRANS_LEN	0x20
#define SMBUS_TRANS_LEN	0x01
#define GSL_PAGE_REG		0xf0
#define ADD_I2C_DEVICE_ANDROID_4_0
//#define HIGH_SPEED_I2C
#define FILTER_POINT
#ifdef FILTER_POINT
#define FILTER_MAX	9
#endif

//#define TPD_PROC_DEBUG
#ifdef TPD_PROC_DEBUG
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
static struct proc_dir_entry *gsl_config_proc = NULL;
#define GSL_CONFIG_PROC_FILE "gsl_config"
#define CONFIG_LEN 31
static char gsl_read[CONFIG_LEN];
static u8 gsl_data_proc[4] = {0};
static u8 gsl_proc_flag = 0;
#endif

static int tpd_flag = 0;
static int tpd_halt=0;
static char eint_flag = 0;
extern struct tpd_device *tpd;
static struct i2c_client *i2c_client = NULL;
static struct task_struct *thread = NULL;
#ifdef GSL_MONITOR
static struct delayed_work gsl_monitor_work;
static struct workqueue_struct *gsl_monitor_workqueue = NULL;
static char int_1st[4] = {0};
static char int_2nd[4] = {0};
#endif

static kal_uint32 id_sign[MAX_CONTACTS+1] = {0};
static kal_uint8 id_state_flag[MAX_CONTACTS+1] = {0};
static kal_uint8 id_state_old_flag[MAX_CONTACTS+1] = {0};
static kal_uint16 x_old[MAX_CONTACTS+1] = {0};
static kal_uint16 y_old[MAX_CONTACTS+1] = {0};
static kal_uint16 x_new = 0;
static kal_uint16 y_new = 0;


static DECLARE_WAIT_QUEUE_HEAD(waiter);
/*
extern void mt65xx_eint_unmask(unsigned int line);
extern void mt65xx_eint_mask(unsigned int line);
extern void mt65xx_eint_set_hw_debounce(kal_uint8 eintno, kal_uint32 ms);
extern kal_uint32 mt65xx_eint_set_sens(kal_uint8 eintno, kal_bool sens);
extern void mt65xx_eint_registration(kal_uint8 eintno, kal_bool Dbounce_En,
		kal_bool ACT_Polarity, void (EINT_FUNC_PTR)(void),
		kal_bool auto_umask);
*/
#ifdef GSL_DEBUG 
#define print_info(fmt, args...)   \
        do{                              \
                printk(fmt, ##args);     \
        }while(0)
#else
#define print_info(fmt, args...)
#endif

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


static void startup_chip(struct i2c_client *client)
{
	char write_buf = 0x00;
#if 1
	u8 buf[4] = {0x00};
	buf[3] = 0x01;
	buf[2] = 0xfe;
	buf[1] = 0x10;
	buf[0] = 0x00;	
	i2c_smbus_write_i2c_block_data(client, 0xf0, sizeof(buf), buf);
	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x00;
	buf[0] = 0x0f;	
	i2c_smbus_write_i2c_block_data(client, 0x04, sizeof(buf), buf);
	msleep(20);	
#endif	
	i2c_smbus_write_i2c_block_data(client, 0xe0, 1, &write_buf); 	
	msleep(10);		
}

static void reset_chip(struct i2c_client *client)
{
	char write_buf[4]	= {0};

	write_buf[0] = 0x88;
	i2c_smbus_write_i2c_block_data(client, 0xe0, 1, &write_buf[0]); 	
	msleep(20);

	write_buf[0] = 0x04;
	i2c_smbus_write_i2c_block_data(client, 0xe4, 1, &write_buf[0]); 	
	msleep(10);

	write_buf[0] = 0x00;
	write_buf[1] = 0x00;
	write_buf[2] = 0x00;
	write_buf[3] = 0x00;
	i2c_smbus_write_i2c_block_data(client, 0xbc, 4, write_buf); 	
	msleep(10);
}

static void clr_reg(struct i2c_client *client)
{
	char write_buf[4]	= {0};

	write_buf[0] = 0x88;
	i2c_smbus_write_i2c_block_data(client, 0xe0, 1, &write_buf[0]); 	
	msleep(20);

	write_buf[0] = 0x01;
	i2c_smbus_write_i2c_block_data(client, 0x80, 1, &write_buf[0]); 	
	msleep(5);
	
	write_buf[0] = 0x04;
	i2c_smbus_write_i2c_block_data(client, 0xe4, 1, &write_buf[0]); 	
	msleep(5);

	write_buf[0] = 0x00;
	i2c_smbus_write_i2c_block_data(client, 0xe0, 1, &write_buf[0]); 	
	msleep(20);
}

#ifdef HIGH_SPEED_I2C

static int i2c_master_send_ext(struct i2c_client *client,const char *buf ,int count)
{
	u8 *buf_dma = NULL;
	u32 old_addr = 0;
	int ret = 0;
	int retry = 3;

	if (count > 255) {
		printk("[i2c_master_send_ext] exceed the max write length \n");
		return -1;
	}

	buf_dma= kmalloc(count,GFP_KERNEL);

	old_addr = client->addr;
	client->addr |= I2C_ENEXT_FLAG ;

	memcpy(buf_dma, buf, count);

	do {
		ret = i2c_master_send(client, (u8*)buf_dma, count);
		retry --;
		if (ret != count) {
			printk("[i2c_master_send_ext] Error sent I2C ret = %d\n", ret);
		}
	}while ((ret != count) && (retry > 0));

	client->addr = old_addr;
	kfree(buf_dma);

	return ret;

}



void gsl_write_interface(struct i2c_client *client,u8 addr, u8* data, u16 size)
{
	u8 addr_bak;
       u8 buff[256];
	   buff[0] = addr;
	   memcpy(buff,data,size);
	addr_bak = client->addr;
	//client->addr = addr;
	client->addr |= I2C_ENEXT_FLAG;
	i2c_master_send_ext(client,buff,size+1);
	client->addr = addr_bak;
}

static u32 gsl_read_interface__(struct i2c_client *client, u8 reg, u8 *buf, u32 num)
{
	struct i2c_msg xfer_msg[2];

	xfer_msg[0].addr = client->addr;
	xfer_msg[0].len = 1;
	xfer_msg[0].flags = client->flags & I2C_M_TEN;
	xfer_msg[0].buf = &reg;
	xfer_msg[0].timing = 400;

	xfer_msg[1].addr = client->addr;
	xfer_msg[1].len = num;
	xfer_msg[1].flags |= I2C_M_RD;
	xfer_msg[1].buf = buf;
	xfer_msg[1].timing = 400;

	if (reg < 0x80) {
		i2c_transfer(client->adapter, xfer_msg, ARRAY_SIZE(xfer_msg));
		msleep(5);
	}

	return i2c_transfer(client->adapter, xfer_msg, ARRAY_SIZE(xfer_msg)) == ARRAY_SIZE(xfer_msg) ? 0 : -EFAULT;
}

static u32 gsl_write_interface__(struct i2c_client *client, const u8 reg, u8 *buf, u32 num)
{
	struct i2c_msg xfer_msg[1];

	buf[0] = reg;

	xfer_msg[0].addr = client->addr;
	xfer_msg[0].len = num + 1;
	xfer_msg[0].flags = client->flags & I2C_M_TEN;
	xfer_msg[0].buf = buf;
	xfer_msg[0].timing = 400;

	return i2c_transfer(client->adapter, xfer_msg, 1) == 1 ? 0 : -EFAULT;
}

static __inline__ void fw2buf(u8 *buf, const u32 *fw)
{
	u32 *u32_buf = (int *)buf;
	*u32_buf = *fw;
}

static void gsl_load_fw(struct i2c_client *client)
{
	u8 buf[DMA_TRANS_LEN*4 + 1] = {0};
	u8 send_flag = 1;
	u8 *cur = buf + 1;
	u32 source_line = 0;
	u32 source_len;
	struct fw_data *ptr_fw;

	printk("=============gsl_load_fw start==============\n");

	ptr_fw = GSLX680_FW;
	source_len = ARRAY_SIZE(GSLX680_FW);
	for (source_line = 0; source_line < source_len; source_line++) 
	{
		/* init page trans, set the page val */
		if (GSL_PAGE_REG == ptr_fw[source_line].offset)
		{
			fw2buf(cur, &ptr_fw[source_line].val);
			gsl_write_interface(client, GSL_PAGE_REG, buf, 4);
			send_flag = 1;
		}
		else 
		{
			if (1 == send_flag % (DMA_TRANS_LEN < 0x20 ? DMA_TRANS_LEN : 0x20))
	    			buf[0] = (u8)ptr_fw[source_line].offset;

			fw2buf(cur, &ptr_fw[source_line].val);
			cur += 4;

			if (0 == send_flag % (DMA_TRANS_LEN < 0x20 ? DMA_TRANS_LEN : 0x20)) 
			{
	    			gsl_write_interface(client, buf[0], buf, cur - buf - 1);
	    			cur = buf + 1;
			}

			send_flag++;
		}
	}

	printk("=============gsl_load_fw end==============\n");

}
#else
static void gsl_load_fw(struct i2c_client *client)
{
	char buf[SMBUS_TRANS_LEN*4] = {0};
	char reg = 0, send_flag = 1, cur = 0;
	
	unsigned int source_line = 0;
	unsigned int source_len = ARRAY_SIZE(GSLX680_FW);

	printk("=============gsl_load_fw start==============\n");

	for (source_line = 0; source_line < source_len; source_line++) 
	{
		if(1 == SMBUS_TRANS_LEN)
		{
			reg = GSLX680_FW[source_line].offset;

			buf[0] = (char)(GSLX680_FW[source_line].val & 0x000000ff);
			buf[1] = (char)((GSLX680_FW[source_line].val & 0x0000ff00) >> 8);
			buf[2] = (char)((GSLX680_FW[source_line].val & 0x00ff0000) >> 16);
			buf[3] = (char)((GSLX680_FW[source_line].val & 0xff000000) >> 24);
			//client.timing =400;
			i2c_smbus_write_i2c_block_data(client, reg, 4, buf); 	
		}
		else
		{
			/* init page trans, set the page val */
			if (GSL_PAGE_REG == GSLX680_FW[source_line].offset)
			{
				buf[0] = (char)(GSLX680_FW[source_line].val & 0x000000ff);
				i2c_smbus_write_i2c_block_data(client, GSL_PAGE_REG, 1, &buf[0]); 	
				send_flag = 1;
			}
			else 
			{
				if (1 == send_flag % (SMBUS_TRANS_LEN < 0x08 ? SMBUS_TRANS_LEN : 0x08))
					reg = GSLX680_FW[source_line].offset;

				buf[cur + 0] = (char)(GSLX680_FW[source_line].val & 0x000000ff);
				buf[cur + 1] = (char)((GSLX680_FW[source_line].val & 0x0000ff00) >> 8);
				buf[cur + 2] = (char)((GSLX680_FW[source_line].val & 0x00ff0000) >> 16);
				buf[cur + 3] = (char)((GSLX680_FW[source_line].val & 0xff000000) >> 24);
				cur += 4;

				if (0 == send_flag % (SMBUS_TRANS_LEN < 0x08 ? SMBUS_TRANS_LEN : 0x08)) 
				{
					i2c_smbus_write_i2c_block_data(client, reg, SMBUS_TRANS_LEN*4, buf); 	
					cur = 0;
				}

				send_flag++;

			}
		}
	}

	printk("=============gsl_load_fw end==============\n");

}


#endif

static int test_i2c(struct i2c_client *client)
{
	char read_buf = 0;
	char write_buf = 0x12;
	int ret, rc = 1;
	client->addr = 0x40;
	ret = i2c_smbus_read_i2c_block_data( client, 0xf0, 1, &read_buf );
	if  (ret  < 0)  
    		rc --;
	else
		printk("I read reg 0xf0 is %x\n", read_buf);

	msleep(2);
	ret = i2c_smbus_write_i2c_block_data( client, 0xf0, 1, &write_buf );
	if(ret  >=  0 )
		printk("I write reg 0xf0 0x12\n");
	
	msleep(2);
	ret = i2c_smbus_read_i2c_block_data( client, 0xf0, 1, &read_buf );
	if(ret <  0 )
		rc --;
	else
		printk("I read reg 0xf0 is 0x%x\n", read_buf);

	return rc;
}
static void init_chip(struct i2c_client *client)
{
	int rc;
	
	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
	msleep(20); 	
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
	msleep(20); 		
	rc = test_i2c(client);
	if(rc < 0)
	{
		printk("------gslX680 test_i2c error------\n");	
		return;
	}	
	clr_reg(client);
	reset_chip(client);
	gsl_load_fw(client);			
	startup_chip(client);
	reset_chip(client);
	startup_chip(client);		
}

static void check_mem_data(struct i2c_client *client)
{
	char read_buf[4]  = {0};
	
	msleep(30);
	i2c_smbus_read_i2c_block_data(client,0xb0, sizeof(read_buf), read_buf);
	
	if (read_buf[3] != 0x5a || read_buf[2] != 0x5a || read_buf[1] != 0x5a || read_buf[0] != 0x5a)
	{
		printk("#########check mem read 0xb0 = %x %x %x %x #########\n", read_buf[3], read_buf[2], read_buf[1], read_buf[0]);
		init_chip(client);
	}
}

#ifdef TPD_PROC_DEBUG
static int char_to_int(char ch)
{
    if(ch>='0' && ch<='9')
        return (ch-'0');
    else
        return (ch-'a'+10);
}

static int gsl_config_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    char *ptr = page;
    //char temp_data[4] = {0};
    char temp_data[5] = {0};
    int i;
    ptr += sprintf(ptr,"====gslX68X===start\n");
    if(gsl_data_proc[0] == 0xaa)
    {
#ifdef GSL_NOID_VERSION
        for(i=0;i<95;i++)
        {
            ptr +=sprintf(ptr,"[%d] = ",i);
            ptr +=sprintf(ptr,"%d\n",gsl_config_data_id[i]);        
        }   
        *eof = 1;
    }
    else if(gsl_data_proc[0] == 0xbb)
    {
        ptr +=sprintf(ptr,"gsl_config_data_id[%d] = ",gsl_data_proc[1]);
        if(gsl_data_proc[1] < 95 && gsl_data_proc[1] >= 0)
            ptr +=sprintf(ptr,"%d\n",gsl_config_data_id[gsl_data_proc[1]]);         
        *eof = 1;
#endif
    }
    else{
        ptr += sprintf(ptr,"page : {0xf0,0x%02X};\n",gsl_data_proc[0]);
        temp_data[1] = gsl_data_proc[0];
        temp_data[0] = 0xf0;
        i2c_master_send(i2c_client, temp_data, 5);
    
        //gsl_read_interface( ddata->client, gsl_data_proc[1], temp_data, 4);
		i2c_smbus_read_i2c_block_data(i2c_client, gsl_data_proc[1], 4, temp_data);
        ptr +=sprintf(ptr,"offset : {0x%02x,0x",gsl_data_proc[1]);
        ptr +=sprintf(ptr,"%02x",temp_data[3]);
        ptr +=sprintf(ptr,"%02x",temp_data[2]);
        ptr +=sprintf(ptr,"%02x",temp_data[1]);
        ptr +=sprintf(ptr,"%02x};\n",temp_data[0]);
        ptr +=sprintf(ptr,"====gslX68X===end\n");
        *eof = 1;
    }
    return (ptr - page);

}
static int gsl_config_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
    u8 buf[5] = {0};
    u8 addr = 0;
    u32 tmp = 0;
    print_info("[tp-gsl][%s] \n",__func__);
    /*
    if(count != CONFIG_LEN)
    {
        print_info("size not match [%d:%ld]\n", CONFIG_LEN, count);
            return -EFAULT;
    }
    */
    if(copy_from_user(gsl_read, buffer, count))
    {
        print_info("copy from user fail\n");
            return -EFAULT;
    }
    print_info("[tp-gsl][%s][%s]\n",__func__,gsl_read);

    addr = char_to_int(gsl_read[3])<<4|char_to_int(gsl_read[4]);
    if(addr == 0xaa)
    {
        gsl_proc_flag = 1;
        reset_chip(i2c_client);
        /*
        msleep(20);
        mt_set_gpio_out(GPIO_CTP_RST_PIN, 0);
        msleep(20);
        mt_set_gpio_out(GPIO_CTP_RST_PIN, 1);
        msleep(20);
        */
        //gsl_start_core(ddata->client);
        return count;
    }   
    else if(addr == 0xbb)
    {
        msleep(20);
        reset_chip(i2c_client);
        startup_chip(i2c_client);
        gsl_proc_flag = 0;
        return count;
    }
    else if(addr == 0xcc)
    {
        gsl_data_proc[0] = char_to_int(gsl_read[8])<<4|char_to_int(gsl_read[9]);
            gsl_data_proc[1] = char_to_int(gsl_read[10])<<4|char_to_int(gsl_read[11]);
        return count;
    }
#ifdef GSL_NOID_VERSION
    else if(addr == 0xdd)
    {   
        buf[0] = char_to_int(gsl_read[8])<<4|char_to_int(gsl_read[9]);
        tmp = (char_to_int(gsl_read[13])<<28) | (char_to_int(gsl_read[14])<<24) | 
            (char_to_int(gsl_read[15])<<20) | (char_to_int(gsl_read[16])<<16) | 
            (char_to_int(gsl_read[17])<<12) | (char_to_int(gsl_read[18])<<8) |
            (char_to_int(gsl_read[19])<<4) |char_to_int(gsl_read[20]);
        gsl_config_data_id[buf[0]] = (int)tmp;
        return count;   
    }
#endif
    else if(addr == 0xf0)
    {
        buf[1] = char_to_int(gsl_read[8])<<4|char_to_int(gsl_read[9]);
        buf[2] = 0;
        buf[3] = 0;
        buf[4] = 0;
    }else
    {
        buf[4] = char_to_int(gsl_read[8])<<4|char_to_int(gsl_read[9]);
        buf[3] = char_to_int(gsl_read[10])<<4|char_to_int(gsl_read[11]);
        buf[2] = char_to_int(gsl_read[12])<<4|char_to_int(gsl_read[13]);
        buf[1] = char_to_int(gsl_read[14])<<4|char_to_int(gsl_read[15]);
    }
    buf[0] = addr;
    i2c_master_send(i2c_client, buf, 5);
    print_info("[tp-gsl][%s] addr =%x, %x ,%x,%x,%x",__func__,addr,buf[4],buf[3],buf[2],buf[1]);

    return count;

}
#endif

#ifdef GSL_MONITOR
static void gsl_monitor_worker(void)
{
	char write_buf;
	char read_buf[4]  = {0};
	
	print_info("----------------gsl_monitor_worker-----------------\n");	

	i2c_smbus_read_i2c_block_data(i2c_client, 0xb4, 4, read_buf);	
	
	int_2nd[3] = int_1st[3];
	int_2nd[2] = int_1st[2];
	int_2nd[1] = int_1st[1];
	int_2nd[0] = int_1st[0];
	int_1st[3] = read_buf[3];
	int_1st[2] = read_buf[2];
	int_1st[1] = read_buf[1];
	int_1st[0] = read_buf[0];

	if (int_1st[3] == int_2nd[3] && int_1st[2] == int_2nd[2] &&int_1st[1] == int_2nd[1] && int_1st[0] == int_2nd[0]) 
	{
		printk("======int_1st: %x %x %x %x , int_2nd: %x %x %x %x ======\n",int_1st[3], int_1st[2], int_1st[1], int_1st[0], int_2nd[3], int_2nd[2],int_2nd[1],int_2nd[0]);
		init_chip(i2c_client);
	}
	queue_delayed_work(gsl_monitor_workqueue, &gsl_monitor_work, 100);
}
#endif

#ifdef FILTER_POINT
static void filter_point(u16 x, u16 y , u8 id)
{
	u16 x_err =0;
	u16 y_err =0;
	u16 filter_step_x = 0, filter_step_y = 0;
	
	id_sign[id] = id_sign[id] + 1;
	if(id_sign[id] == 1)
	{
		x_old[id] = x;
		y_old[id] = y;
	}
	
	x_err = x > x_old[id] ? (x -x_old[id]) : (x_old[id] - x);
	y_err = y > y_old[id] ? (y -y_old[id]) : (y_old[id] - y);

	if( (x_err > FILTER_MAX && y_err > FILTER_MAX/3) || (x_err > FILTER_MAX/3 && y_err > FILTER_MAX) )
	{
		filter_step_x = x_err;
		filter_step_y = y_err;
	}
	else
	{
		if(x_err > FILTER_MAX)
			filter_step_x = x_err; 
		if(y_err> FILTER_MAX)
			filter_step_y = y_err;
	}

	if(x_err <= 2*FILTER_MAX && y_err <= 2*FILTER_MAX)
	{
		filter_step_x >>= 2; 
		filter_step_y >>= 2;
	}
	else if(x_err <= 3*FILTER_MAX && y_err <= 3*FILTER_MAX)
	{
		filter_step_x >>= 1; 
		filter_step_y >>= 1;
	}	
	else if(x_err <= 4*FILTER_MAX && y_err <= 4*FILTER_MAX)
	{
		filter_step_x = filter_step_x*3/4; 
		filter_step_y = filter_step_y*3/4;
	}	
	
	x_new = x > x_old[id] ? (x_old[id] + filter_step_x) : (x_old[id] - filter_step_x);
	y_new = y > y_old[id] ? (y_old[id] + filter_step_y) : (y_old[id] - filter_step_y);

	x_old[id] = x_new;
	y_old[id] = y_new;
}
#else

static void record_point(u16 x, u16 y , u8 id)
{
	u16 x_err =0;
	u16 y_err =0;

	id_sign[id]=id_sign[id]+1;
	
	if(id_sign[id]==1){
		x_old[id]=x;
		y_old[id]=y;
	}

	x = (x_old[id] + x)/2;
	y = (y_old[id] + y)/2;
		
	if(x>x_old[id]){
		x_err=x -x_old[id];
	}
	else{
		x_err=x_old[id]-x;
	}

	if(y>y_old[id]){
		y_err=y -y_old[id];
	}
	else{
		y_err=y_old[id]-y;
	}

	if( (x_err > 3 && y_err > 1) || (x_err > 1 && y_err > 3) ){
		x_new = x;     x_old[id] = x;
		y_new = y;     y_old[id] = y;
	}
	else{
		if(x_err > 3){
			x_new = x;     x_old[id] = x;
		}
		else
			x_new = x_old[id];
		if(y_err> 3){
			y_new = y;     y_old[id] = y;
		}
		else
			y_new = y_old[id];
	}

	if(id_sign[id]==1){
		x_new= x_old[id];
		y_new= y_old[id];
	}
	
}
#endif

void tpd_down_comp( int id, int x, int y, int p) 
{
	print_info("------tpd_down id: %d, x:%d, y:%d------ \n", id, x, y);

	input_report_key(tpd->dev, BTN_TOUCH, 1);
	input_report_abs(tpd->dev, ABS_MT_TOUCH_MAJOR, 1);
	input_report_abs(tpd->dev, ABS_MT_POSITION_X, x);
	input_report_abs(tpd->dev, ABS_MT_POSITION_Y, y);
	input_report_abs(tpd->dev, ABS_MT_TRACKING_ID, id); 	
	input_mt_sync(tpd->dev);

	if (FACTORY_BOOT == get_boot_mode()|| RECOVERY_BOOT == get_boot_mode())
	{   
	#ifdef TPD_HAVE_BUTTON 
		tpd_button(x, y, 1);  
	#endif
	}
	if(y > TPD_RES_Y) //virtual key debounce to avoid android ANR issue
	{
		 msleep(50);
		 print_info("D virtual key \n");
	}	
}

void tpd_up_comp(void) 
{
	print_info("------tpd_up------ \n");

	input_report_key(tpd->dev, BTN_TOUCH, 0);
	input_mt_sync(tpd->dev);
	
	if (FACTORY_BOOT == get_boot_mode()|| RECOVERY_BOOT == get_boot_mode())
	{   
	#ifdef TPD_HAVE_BUTTON 
	    tpd_button(0, 0, 0);  
	#endif
	}
}

static void report_data_handle(void)
{
	char touch_data[MAX_FINGERS * 4 + 4] = {0};
	char buf[4] = {0};
	char id, point_num = 0;
	unsigned int x, y, temp_a, temp_b, i;
#ifdef GSL_NOID_VERSION
	struct gsl_touch_info cinfo;
#endif
	
#ifdef TPD_PROC_DEBUG
    if(gsl_proc_flag == 1)
        return;
#endif

	i2c_smbus_read_i2c_block_data(i2c_client, 0x80, 8, &touch_data[0]);
	point_num = touch_data[0];
	if(point_num > 1)
	{
		i2c_smbus_read_i2c_block_data(i2c_client, 0x88, 8, &touch_data[8]);
	}
	if(point_num > 3)
	{
		i2c_smbus_read_i2c_block_data(i2c_client, 0x90, 8, &touch_data[16]);
	}
	
#ifdef GSL_NOID_VERSION
	cinfo.finger_num = point_num;
	print_info("tp-gsl  finger_num = %d\n",cinfo.finger_num);
	for(i = 0; i < (point_num < MAX_CONTACTS ? point_num : MAX_CONTACTS); i ++)
	{
		temp_a = touch_data[(i + 1) * 4 + 3] & 0x0f;
		temp_b = touch_data[(i + 1) * 4 + 2];
		cinfo.x[i] = temp_a << 8 |temp_b;
		temp_a = touch_data[(i + 1) * 4 + 1];
		temp_b = touch_data[(i + 1) * 4 + 0];
		cinfo.y[i] = temp_a << 8 |temp_b;		
		print_info("tp-gsl  x = %d y = %d \n",cinfo.x[i],cinfo.y[i]);
	}
	gsl_alg_id_main(&cinfo);
	point_num = cinfo.finger_num;
#endif

	for(i = 1 ;i <= MAX_CONTACTS; i ++)
	{
		if(point_num == 0)
			id_sign[i] = 0;	
		id_state_flag[i] = 0;
	}			
	for(i = 0; i < (point_num < MAX_FINGERS ? point_num : MAX_FINGERS); i ++)
	{
	#ifdef GSL_NOID_VERSION
		id = cinfo.id[i];
		x =  cinfo.x[i];
		y =  cinfo.y[i];	
	#else
		id = touch_data[(i + 1) * 4 + 3] >> 4;
		temp_a = touch_data[(i + 1) * 4 + 3] & 0x0f;
		temp_b = touch_data[(i + 1) * 4 + 2];
		x = temp_a << 8 |temp_b;
		temp_a = touch_data[(i + 1) * 4 + 1];
		temp_b = touch_data[(i + 1) * 4 + 0];
		y = temp_a << 8 |temp_b;	
	#endif
	
		if(1 <= id && id <= MAX_CONTACTS)
		{
		#ifdef FILTER_POINT
			filter_point(x, y ,id);
		#else
			record_point(x, y , id);
		#endif
			tpd_down_comp(id, x_new, y_new, 10);
			id_state_flag[id] = 1;
		}
	}
	for(i = 1; i <= MAX_CONTACTS; i ++)
	{	
		if( (0 == point_num) || ((0 != id_state_old_flag[i]) && (0 == id_state_flag[i])) )
		{
			id_sign[i]=0;
		}
		id_state_old_flag[i] = id_state_flag[i];
	}			
	if(0 == point_num)
	{
		tpd_up_comp();
	}
	input_sync(tpd->dev);
}

static int touch_event_handler(void *unused)
{
	struct sched_param param = { .sched_priority = RTPM_PRIO_TPD };
	sched_setscheduler(current, SCHED_RR, &param);
	
	do
	{
		mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);
		set_current_state(TASK_INTERRUPTIBLE);
		wait_event_interruptible(waiter, tpd_flag != 0);
		tpd_flag = 0;
		TPD_DEBUG_SET_TIME;
		set_current_state(TASK_RUNNING);
		print_info("===touch_event_handler, task running===\n");

		eint_flag = 0;
		report_data_handle();
		
	} while (!kthread_should_stop());
	
	return 0;
}

void tpd_eint_interrupt_handler_comp(void)
{
	print_info("===tpd irq interrupt===\n");

	eint_flag = 1;
	tpd_flag=1; 
	wake_up_interruptible(&waiter);
}

static int tpd_i2c_detect_comp(struct i2c_client *client, int kind, struct i2c_board_info *info) {
	strcpy(info->type, TPD_DEVICE);
	return 0;
}

#define GPIO_TP_ID0    GPIO137
#define GPIO_TP_ID1    GPIO138

static int tpd_check_tp_gpio_id(void)
{
	int id0,id1;
	mt_set_gpio_mode(GPIO_TP_ID1, GPIO_MODE_00);
	mt_set_gpio_dir(GPIO_TP_ID1, GPIO_DIR_IN);
	mt_set_gpio_pull_enable(GPIO_TP_ID1, GPIO_PULL_ENABLE);
	mt_set_gpio_pull_select(GPIO_TP_ID1, GPIO_PULL_UP);	

		mt_set_gpio_mode(GPIO_TP_ID0, GPIO_MODE_00);
	mt_set_gpio_dir(GPIO_TP_ID0, GPIO_DIR_IN);
	mt_set_gpio_pull_enable(GPIO_TP_ID0, GPIO_PULL_ENABLE);
	mt_set_gpio_pull_select(GPIO_TP_ID0, GPIO_PULL_UP);	
	id0=mt_get_gpio_in(GPIO_TP_ID0);  
	id1=mt_get_gpio_in(GPIO_TP_ID1);  
	printk("MYCAT COMA TP PIN ID:[%d,%d]\n",id0,id1);
		if((id0 ==1)&&(id1 ==1))
		return 1;
	else
		return -1;
}

static int tpd_i2c_probe_comp(struct i2c_client *client, const struct i2c_device_id *id) {
	int err = 0;
	char buffer[5];
	int status=0;

	printk("mycat tpd_i2c_probe comp==\n");
	       err = tpd_check_tp_gpio_id();
	if(err < 0)
		{
		printk("mycat tpd_i2c_probe GSL1688E error\n");
		return -1;

		}
	else
		printk("mycat tpd_i2c_probe use GSL1688E\n");
	
	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
	msleep(100);

      if(TPD_POWER_SOURCE != MT65XX_POWER_NONE)
       {
	    hwPowerDown(TPD_POWER_SOURCE,"TP");
	    hwPowerOn(TPD_POWER_SOURCE,VOL_2800,"TP");
	    msleep(100);	
      }	
	  
	msleep(100);

	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);

	mt_set_gpio_mode(GPIO_CTP_EINT_PIN, GPIO_CTP_EINT_PIN_M_EINT);
	mt_set_gpio_dir(GPIO_CTP_EINT_PIN, GPIO_DIR_IN);
	mt_set_gpio_pull_enable(GPIO_CTP_EINT_PIN, GPIO_PULL_ENABLE);
	mt_set_gpio_pull_select(GPIO_CTP_EINT_PIN, GPIO_PULL_UP);
	msleep(50);

	i2c_client = client;	
	init_chip(i2c_client);
	check_mem_data(i2c_client);
	
	mt65xx_eint_set_sens(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_SENSITIVE);
	mt65xx_eint_set_hw_debounce(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_CN);
	mt65xx_eint_registration(CUST_EINT_TOUCH_PANEL_NUM, CUST_EINT_TOUCH_PANEL_DEBOUNCE_EN, CUST_EINT_TOUCH_PANEL_POLARITY, tpd_eint_interrupt_handler_comp, 1);
	mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);
	
	tpd_load_status = 1;
	thread = kthread_run(touch_event_handler, 0, TPD_DEVICE);
	if (IS_ERR(thread)) {
		err = PTR_ERR(thread);
		TPD_DMESG(TPD_DEVICE " failed to create kernel thread: %d\n", err);
	}

#ifdef GSL_MONITOR
	printk( "tpd_i2c_probe () : queue gsl_monitor_workqueue\n");

	INIT_DELAYED_WORK(&gsl_monitor_work, gsl_monitor_worker);
	gsl_monitor_workqueue = create_singlethread_workqueue("gsl_monitor_workqueue");
	queue_delayed_work(gsl_monitor_workqueue, &gsl_monitor_work, 1000);
#endif

#ifdef GSL_NOID_VERSION
	gsl_DataInit(gsl_config_data_id);
#endif

#ifdef TPD_PROC_DEBUG
	gsl_config_proc = create_proc_entry(GSL_CONFIG_PROC_FILE, 0666, NULL);
	printk("[tp-gsl] [%s] gsl_config_proc = %x \n",__func__,gsl_config_proc);
	if (gsl_config_proc == NULL)
	{
		print_info("create_proc_entry %s failed\n", GSL_CONFIG_PROC_FILE);
	}
	else
	{
		gsl_config_proc->read_proc = gsl_config_read_proc;
		gsl_config_proc->write_proc = gsl_config_write_proc;
	}
	gsl_proc_flag = 0;
#endif

	printk("==tpd_i2c_probe end==\n");
		
	return 0;
}

static int tpd_i2c_remove_comp(struct i2c_client *client)
{
	printk("==tpd_i2c_remove==\n");
	
	return 0;
}


static const struct i2c_device_id tpd_i2c_id_comp[] = {{GSL1688E_NAME,0},{}};
#ifdef ADD_I2C_DEVICE_ANDROID_4_0
static struct i2c_board_info __initdata gslX680_i2c_tpd_comp={ I2C_BOARD_INFO(GSL1688E_NAME, (GSLX680_ADDR))};
#else
static unsigned short force[] = {0, (GSLX680_ADDR << 1), I2C_CLIENT_END,I2C_CLIENT_END};
static const unsigned short * const forces[] = { force, NULL };
static struct i2c_client_address_data addr_data_comp = { .forces = forces,};
#endif

struct i2c_driver tpd_i2c_driver_comp = {
	.driver = {
		.name = GSL1688E_NAME,
	#ifndef ADD_I2C_DEVICE_ANDROID_4_0	 
		.owner = THIS_MODULE,
	#endif
	},
	.probe = tpd_i2c_probe_comp,
	.remove = tpd_i2c_remove_comp,
	.id_table = tpd_i2c_id_comp,
	.detect = tpd_i2c_detect_comp,
	#ifndef ADD_I2C_DEVICE_ANDROID_4_0
	.address_data = &addr_data_comp,
	#endif
};

int tpd_local_init_comp(void)
{
	printk("==tpd_local_init==\n");

	if(i2c_add_driver(&tpd_i2c_driver_comp)!=0) {
		TPD_DMESG("mycat unable to add i2c driver.\n");
		return -1;
	}
	/*
	if(tpd_load_status == 0)
	{
		TPD_DMESG("add error touch panel driver.\n");
		i2c_del_driver(&tpd_i2c_driver);
		return -1;
	}
	*/
#ifdef TPD_HAVE_BUTTON
	tpd_button_setting(TPD_KEY_COUNT, tpd_keys_local, tpd_keys_dim_local);// initialize tpd button data
#endif

#if (defined(TPD_WARP_START) && defined(TPD_WARP_END))
	TPD_DO_WARP = 1;
	memcpy(tpd_wb_start, tpd_wb_start_local, TPD_WARP_CNT*4);
	memcpy(tpd_wb_end, tpd_wb_start_local, TPD_WARP_CNT*4);
#endif

#if (defined(TPD_HAVE_CALIBRATION) && !defined(TPD_CUSTOM_CALIBRATION))
	memcpy(tpd_calmat, tpd_calmat_local, 8*4);
	memcpy(tpd_def_calmat, tpd_def_calmat_local, 8*4);
#endif
	tpd_type_cap = 1;

	printk("==tpd_local_init end==\n");
	return 0;
}

/* Function to manage low power suspend */
void tpd_suspend_comp(struct early_suspend *h)
{
	printk("==tpd_suspend comp==\n");
	
#ifdef TPD_PROC_DEBUG
#ifdef GSL_NOID_VERSION
	gsl_DataInit(gsl_config_data_id);
#endif
	if(gsl_proc_flag == 1)
		return;
#endif

	tpd_halt = 1;
	mt65xx_eint_mask(CUST_EINT_TOUCH_PANEL_NUM);
#ifdef GSL_MONITOR
	printk( "gsl_ts_suspend () : cancel gsl_monitor_work\n");
	cancel_delayed_work_sync(&gsl_monitor_work);
#endif
	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ZERO);
}

/* Function to manage power-on resume */
void tpd_resume_comp(struct early_suspend *h)
{
	printk("==tpd_resume==\n");
	
#ifdef TPD_PROC_DEBUG
    if(gsl_proc_flag == 1)
        return;
#endif
	mt_set_gpio_mode(GPIO_CTP_RST_PIN, GPIO_CTP_RST_PIN_M_GPIO);
	mt_set_gpio_dir(GPIO_CTP_RST_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_CTP_RST_PIN, GPIO_OUT_ONE);
	msleep(20);	
	reset_chip(i2c_client);
	startup_chip(i2c_client);
	check_mem_data(i2c_client);	
#ifdef GSL_MONITOR
	printk( "gsl_ts_resume () : queue gsl_monitor_work\n");
	queue_delayed_work(gsl_monitor_workqueue, &gsl_monitor_work, 300);
#endif	
	mt65xx_eint_unmask(CUST_EINT_TOUCH_PANEL_NUM);
	tpd_halt = 0;
}

static struct tpd_driver_t tpd_device_driver_comp = {
	.tpd_device_name = "GSL1688E",
	.tpd_local_init = tpd_local_init_comp,
	.suspend = tpd_suspend_comp,
	.resume = tpd_resume_comp,
#ifdef TPD_HAVE_BUTTON
	.tpd_have_button = 1,
#else
	.tpd_have_button = 0,
#endif
};

/* called when loaded into kernel */
static int __init tpd_driver_init(void) {
	printk("mycat gsl1688e touch panel driver init\n");
	
#ifdef ADD_I2C_DEVICE_ANDROID_4_0
	i2c_register_board_info(1, &gslX680_i2c_tpd_comp, 1);	
#endif
	if(tpd_driver_add(&tpd_device_driver_comp) < 0)
		printk("mycat add gsl16bbe driver failed\n");
	return 0;
}

/* should never be called */
static void __exit tpd_driver_exit(void) {
	printk("mycat gsl1688e touch panel driver exit\n");
	//input_unregister_device(tpd->dev);
	tpd_driver_remove(&tpd_device_driver_comp);
}

module_init(tpd_driver_init);
module_exit(tpd_driver_exit);


