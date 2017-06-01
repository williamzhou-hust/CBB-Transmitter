#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/queue.h>

#include <rte_common.h>
#include <rte_memory.h>
#include <rte_memzone.h>
#include <rte_launch.h>
#include <rte_eal.h>
#include <rte_per_lcore.h>
#include <rte_lcore.h>
#include <rte_debug.h>
#include <rte_atomic.h>
#include <rte_branch_prediction.h>
#include <rte_ring.h>
#include <rte_log.h>
#include <rte_mempool.h>
#include <cmdline_rdline.h>
#include <cmdline_parse.h>
#include <cmdline_socket.h>
#include <cmdline.h>
#include "mp_commands.h"
#include <rte_mbuf.h>
//#define RUNMAINDPDK
//#ifdef RUNMAINDPDK

#define RTE_LOGTYPE_APP RTE_LOGTYPE_USER1
// #define MEMPOOL_F_SP_PUT         0x0

#define MBUF_CACHE_SIZE 250
#define NUM_MBUFS 512
static const char *MBUF_POOL = "MBUF_POOL";

static const char *Beforescramble = "Beforescramble";
static const char *scramble_2_BCC = "scramble_2_BCC";
static const char *BCC_2_modulation = "BCC_2_modulation";
static const char *modulation_2_CSD = "modulation_2_CSD";
static const char *AfterCSD = "AfterCSD";

const unsigned APEP_LEN = 512; 

// static int i=0; 
struct rte_ring *Ring_Beforescramble,*Ring_scramble_2_BCC,*Ring_BCC_2_modulation,*Ring_modulation_2_CSD,*Ring_AfterCSD;
struct rte_mempool *mbuf_pool;
	
volatile int quit = 0;

static int GenDataAndScramble_encode_dpdk (__attribute__((unused)) struct rte_mbuf *adb);
static int bcc_encode_dpdk(__attribute__((unused)) struct rte_mbuf *adb);
static int modulate_encode_dpdk(__attribute__((unused)) struct rte_mbuf *adb); 
static int CSD_encode_dpdk(__attribute__((unused)) struct rte_mbuf *adb);
static int ReadData(__attribute__((unused)) struct rte_mbuf *adb);
static int GenDataAndScramble(__attribute__((unused)) struct rte_mbuf *adb);
static int BCC_encoder(__attribute__((unused)) struct rte_mbuf *adb);
static int modulate(__attribute__((unused)) struct rte_mbuf *adb);
static int Data_CSD(__attribute__((unused)) struct rte_mbuf *adb);
static int ReadData_Loop();
static int GenDataAndScramble_Loop();
static int BCC_encoder_Loop();
static int modulate_Loop();
static int Data_CSD_Loop();  

static int GenDataAndScramble_encode_dpdk (__attribute__((unused)) struct rte_mbuf *adb) 
{
	//memcpy((unsigned char *)arg, (unsigned char *)adb, APEP_LEN);//从DataIn拷贝数据到DataOut
	printf("GenDataAndScramble success \n");
	//rte_mempool_put(message_pool1, adb);//将DataIn刷回内存池message_pool1

	return 0;
}
static int bcc_encode_dpdk (__attribute__((unused)) struct rte_mbuf *adb) 
{
	///memcpy((unsigned char *)arg, (unsigned char *)adb,APEP_LEN);
	printf("BCCencode success\n");
	//rte_mempool_put(message_pool2, adb);
	return 0;
}
static int modulate_encode_dpdk (__attribute__((unused)) struct rte_mbuf *adb) 
{
	//memcpy((unsigned char *)arg,(unsigned char *)adb, APEP_LEN);
	printf("modulate success\n");
	//rte_mempool_put(message_pool3, adb);

	return 0;
}
static int CSD_encode_dpdk (__attribute__((unused)) struct rte_mbuf *adb) 
{
	//memcpy((unsigned char *)arg,(unsigned char *)adb, APEP_LEN);
	printf("CSD success\n");
	// printf("sizeof Data_In_CSD %d\n", strlen(adb));
	//printf("sizeof Data_In_CSD befroe put  %d\n", strlen(adb));
	// rte_mempool_free_count(const struct rte_mempool *mp)
	// printf("data_len %d\n", (*adb).data_len);
	printf("the number of free entries in the mempool befroe put %d\n", rte_mempool_free_count(mbuf_pool));
	//__rte_mbuf_raw_free(adb);
	rte_mempool_put(mbuf_pool, &adb);
	//rte_pktmbuf_free(&adb);

	printf("the number of free entries in the mempool after put %d\n", rte_mempool_free_count(mbuf_pool));
	//printf("sizeof Data_In_CSD after put  %d\n", strlen(adb));
	
	//rte_pktmbuf_free(adb);
	return 0;
}

static int ReadData_Loop() 
{
	// unsigned lcore_id = rte_lcore_id();
	// printf("ReadData Starting core %u\n", lcore_id);
	// const char *message ="ReadData_Loop";
	//struct rte_mbuf *Data =NULL;
	//snprintf((char *)Data, string_size, "%s", message);
	/*入队前，先获取内存池的一块内存*/
	// const char *message ="ReadData_Loop";
	struct rte_mbuf *Data =NULL;
	//Data=rte_pktmbuf_alloc(mbuf_pool);
	while (!quit){
		
		if (((Data=rte_pktmbuf_alloc(mbuf_pool))==NULL)|(rte_ring_enqueue(Ring_Beforescramble, Data) < 0))
		{
			// printf("Failed to send message - message discarded\n");
			//__rte_mbuf_raw_free(Data);
			//rte_mempool_put(mbuf_pool,&Data);
			//rte_pktmbuf_free(Data);
			GenDataAndScramble_Loop();
		}
		else 
		{
			//rte_pktmbuf_free(Data);
			// rte_mempool_put(mbuf_pool, Data);
			// printf("sizeof Data %d\n", strlen(Data));
			// printf("sizeof Data %d\n", sizeof(*Data));
			//printf("the number of free entries in the mempool after ReadData before put %d\n", rte_mempool_free_count(mbuf_pool));
			// printf("data_len %d\n", Data->data_len);
				ReadData(Data);
		}
	
	}
	return 0;
}
static int ReadData(__attribute__((unused)) struct rte_mbuf *Data) 
{
	
	//printf("the number of free entries in the mempool befroe put %d\n", rte_mempool_free_count(mbuf_pool));
	//rte_mempool_put(mbuf_pool,&Data);
	//printf("the number of free entries in the mempool after put %d\n", rte_mempool_free_count(mbuf_pool));

	  FILE *fp=fopen("send_din_dec.txt","rt");
		unsigned char* databits=(unsigned char*)malloc(APEP_LEN*sizeof(unsigned char));
	    unsigned int datatmp=0;
		int i=0;
	    for(i=0;i<APEP_LEN;i++){
	            fscanf(fp,"%ud",&datatmp);
	            databits[i]=datatmp&0x000000FF;
	    }
		memcpy((unsigned char *)Data, (unsigned char *)databits, APEP_LEN);//将文件读取数据复制给Data即原始数据流

	 fclose(fp);
	printf("ReadData success\n");
	free(databits);
	printf("data_len %d\n", Data->data_len);
	return 0;
}
static int GenDataAndScramble_Loop() 
{

	void *Data_In_Scramble=NULL;

	while (!quit){
	if (rte_ring_dequeue(Ring_Beforescramble, &Data_In_Scramble) < 0){
			ReadData_Loop();
			continue;
		}
	else if (rte_ring_enqueue(Ring_scramble_2_BCC, Data_In_Scramble) < 0) 
		{
			rte_ring_enqueue(Ring_Beforescramble, Data_In_Scramble);
			BCC_encoder_Loop();
		}
	else 
	{
		// printf("sizeof Data_In_Scramble %d\n", strlen(Data_In_Scramble));
		GenDataAndScramble(Data_In_Scramble);
	}
	
	}
	return 0;
}
static int GenDataAndScramble (__attribute__((unused)) struct rte_mbuf *Data_In) 
{

			//rte_mempool_put(mbuf_pool, Data_In);
			GenDataAndScramble_encode_dpdk(Data_In);
		return 0;
}
static int BCC_encoder_Loop() 
{

	void *Data_In_BCC=NULL;
	while (!quit)
	{
		if (rte_ring_dequeue(Ring_scramble_2_BCC, &Data_In_BCC) < 0)
		{
				GenDataAndScramble_Loop();
				continue;
		}
		else if (rte_ring_enqueue(Ring_BCC_2_modulation, Data_In_BCC) < 0) 
		{
			rte_ring_enqueue(Ring_scramble_2_BCC, Data_In_BCC);
			modulate_Loop();
		}
		else 
		{
			// printf("sizeof Data_In_BCC %d\n", strlen(Data_In_BCC));
			BCC_encoder(Data_In_BCC);
		}
		
	}
	return 0;
}
static int BCC_encoder (__attribute__((unused)) struct rte_mbuf *Data_In)
{
		// printf("sizeof Data BCC %d\n", sizeof(Data_In));

		bcc_encode_dpdk(Data_In);
		return 0;
}
static int modulate_Loop() 
{

	void *Data_In_modulate=NULL;
	while (!quit)
	{
		if (rte_ring_dequeue(Ring_BCC_2_modulation, &Data_In_modulate) < 0)
		{
				BCC_encoder_Loop();
				continue;
		}
		else if (rte_ring_enqueue(Ring_modulation_2_CSD, Data_In_modulate) < 0) 
		{
			rte_ring_enqueue(Ring_BCC_2_modulation, Data_In_modulate);
			Data_CSD_Loop();
		}		
		else 
		{
				// printf("sizeof Data_In_modulate %d\n", strlen(Data_In_modulate));
				modulate(Data_In_modulate);
		}

	}
	return 0;
}
static int modulate(__attribute__((unused)) struct rte_mbuf *Data_In)
{

		modulate_encode_dpdk(Data_In);
		return 0;
}
static int Data_CSD_Loop() 
{

	void *Data_In_CSD=NULL;
	while (!quit)
	{
		if (rte_ring_dequeue(Ring_modulation_2_CSD, &Data_In_CSD) < 0)
		{
				modulate_Loop();
				continue;
		}
		else 
		{
			// printf("sizeof Data_In_CSD %d\n", strlen(Data_In_CSD));
			Data_CSD(Data_In_CSD);
		}
	
	}
	return 0;
}
static int Data_CSD(__attribute__((unused)) struct rte_mbuf *Data_In)
{
/*		// printf("sizeof Data_In %d\n", strlen(Data_In));
		if (rte_ring_enqueue(Ring_AfterCSD, Data_In) < 0) {//Ring_AfterCSD已满将Ring_AfterCSD队列元素全部出队
		printf("sizeof Data_In_CSD befroe put  %d\n", strlen(Data_In));
		rte_mempool_put(mbuf_pool, &Data_In);
		printf("sizeof Data_In_CSD after put%d\n", strlen(Data_In));
		//rte_pktmbuf_free(Data_In);
		return 0;
		printf("Clear Ring_AfterCSD\n");
		Data_CSD_Loop();
		}
		else */
		CSD_encode_dpdk(Data_In);		
		return 0;
}	
int
main(int argc, char **argv)
{
	const unsigned flags = 0;
	const unsigned ring_size = 512;
	const unsigned pool_size = 256;
	const unsigned pool_cache = 32;
	const unsigned priv_data_sz = 0;
	int ret;
	// 运行一次得到preamble和HeLTF.
	//generatePreambleAndHeLTF_csd()
	// 运行一次得到比特干扰码表。
	//Creatnewchart();
	// 运行一次得到BCC编码表。
	//init_BCCencode_table();
	// 运行一次得到生成导频的分流交织表
	//initial_streamwave_table();
	// 运行一次得到CSD表。
	//initcsdTableForHeLTF();
	// 初始化函数，计算OFDM符号个数，字节长度
	//int N_CBPS, N_SYM, ScrLength, valid_bits;
   // GenInit(&N_CBPS, &N_SYM, &ScrLength, &valid_bits);
	///////////////////////////////////////////////////////////////////////////////////
	//unsigned lcore_id;
	ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Cannot init EAL\n");
		Ring_Beforescramble = rte_ring_create(Beforescramble , ring_size, rte_socket_id(), flags);
		Ring_scramble_2_BCC = rte_ring_create(scramble_2_BCC, ring_size, rte_socket_id(), flags);
		Ring_BCC_2_modulation = rte_ring_create(BCC_2_modulation, ring_size, rte_socket_id(), flags);
		Ring_modulation_2_CSD = rte_ring_create(modulation_2_CSD, ring_size, rte_socket_id(), flags);
		Ring_AfterCSD = rte_ring_create(AfterCSD, ring_size, rte_socket_id(), flags);		
	
	if (Ring_Beforescramble == NULL)
		rte_exit(EXIT_FAILURE, "Problem getting sending ring\n");
	if (Ring_scramble_2_BCC == NULL)
		rte_exit(EXIT_FAILURE, "Problem getting receiving ring\n");
	if (Ring_BCC_2_modulation == NULL)
		rte_exit(EXIT_FAILURE, "Problem getting sending ring\n");
	if (Ring_modulation_2_CSD == NULL)
		rte_exit(EXIT_FAILURE, "Problem getting receiving ring\n");
	if (Ring_AfterCSD == NULL)
		rte_exit(EXIT_FAILURE, "Problem getting sending ring\n");
	/* Creates a new mempool in memory to hold the mbufs. */
	mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL", NUM_MBUFS,
		MBUF_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());

	if (mbuf_pool == NULL)
		rte_exit(EXIT_FAILURE, "Cannot create mbuf pool\n");

	RTE_LOG(INFO, APP, "Finished Process Init.\n");

	rte_eal_remote_launch(ReadData_Loop, NULL,1);
	rte_eal_remote_launch(GenDataAndScramble_Loop, NULL, 1);
	rte_eal_remote_launch(BCC_encoder_Loop, NULL, 3);
	rte_eal_remote_launch(BCC_encoder_Loop, NULL,4);
	rte_eal_remote_launch(BCC_encoder_Loop, NULL, 5);
	rte_eal_remote_launch(modulate_Loop, NULL, 6);
	rte_eal_remote_launch(modulate_Loop, NULL,7);
	rte_eal_remote_launch(modulate_Loop, NULL,8);
	rte_eal_remote_launch(Data_CSD_Loop, NULL,9);
	Data_CSD_Loop(NULL);
	rte_eal_mp_wait_lcore();
	return 0;
}
//#endif // RUN
