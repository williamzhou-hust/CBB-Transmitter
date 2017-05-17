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

//#define RUNMAINDPDK
//#ifdef RUNMAINDPDK

#define RTE_LOGTYPE_APP RTE_LOGTYPE_USER1
		
static const char *Mempool_Beforescramble = "Mempool_Beforescramble";
static const char *Mempool_scramble_2_BCC = "Mempool_scramble_2_BCC";
static const char *Mempool_BCC_2_modulation = "Mempool_BCC_2_modulation";
static const char *Mempool_modulation_2_CSD = "Mempool_modulation_2_CSD";
static const char *Mempool_AfterCSD = "Mempool_AfterCSD";
static const char *Beforescramble = "Beforescramble";
static const char *scramble_2_BCC = "scramble_2_BCC";
static const char *BCC_2_modulation = "BCC_2_modulation";
static const char *modulation_2_CSD = "modulation_2_CSD";
static const char *AfterCSD = "AfterCSD";
const unsigned string_size = 512;
const unsigned APEP_LEN = 512; 
static int i=0; 
struct rte_ring *Ring_Beforescramble,*Ring_scramble_2_BCC,*Ring_BCC_2_modulation,*Ring_modulation_2_CSD,*Ring_AfterCSD;
struct rte_mempool *message_pool1;
struct rte_mempool *message_pool2;////
struct rte_mempool *message_pool3;
struct rte_mempool *message_pool4;
struct rte_mempool *message_pool5;
volatile int quit = 0;

static int GenDataAndScramble_encode_dpdk (__attribute__((unused)) void *adb,__attribute__((unused)) void *arg);
static int bcc_encode_dpdk (__attribute__((unused)) void *adb,__attribute__((unused)) void *arg); 
static int modulate_encode_dpdk (__attribute__((unused)) void *adb,__attribute__((unused)) void *arg); 
static int CSD_encode_dpdk (__attribute__((unused)) void *adb,__attribute__((unused)) void *arg); 
static int ReadData(__attribute__((unused)) void *Data); 
static int GenDataAndScramble (__attribute__((unused)) void *Data_In,__attribute__((unused)) void *Data_Out); 
static int BCC_encoder (__attribute__((unused)) void *Data_In,__attribute__((unused)) void *Data_Out);
static int modulate(__attribute__((unused)) void *Data_In,__attribute__((unused)) void *Data_Out);
static int Data_CSD(__attribute__((unused)) void *Data_In,__attribute__((unused)) void *Data_Out);
static int ReadData_Loop();
static int GenDataAndScramble_Loop();
static int BCC_encoder_Loop();
static int modulate_Loop();
static int Data_CSD_Loop();  

static int GenDataAndScramble_encode_dpdk (__attribute__((unused)) void *adb,__attribute__((unused)) void *arg) 
{
	memcpy((unsigned char *)arg, (unsigned char *)adb, APEP_LEN);//从DataIn拷贝数据到DataOut
	printf("GenDataAndScramble success \n");
	rte_mempool_put(message_pool1, adb);//将DataIn刷回内存池message_pool1

	return 0;
}
static int bcc_encode_dpdk (__attribute__((unused)) void *adb,__attribute__((unused)) void *arg) 
{
	memcpy((unsigned char *)arg, (unsigned char *)adb,APEP_LEN);
	printf("BCCencode success\n");
	rte_mempool_put(message_pool2, adb);

	return 0;
}
static int modulate_encode_dpdk (__attribute__((unused)) void *adb,__attribute__((unused)) void *arg) 
{
	memcpy((unsigned char *)arg,(unsigned char *)adb, APEP_LEN);
	printf("modulate success\n");
	rte_mempool_put(message_pool3, adb);

	return 0;
}
static int CSD_encode_dpdk (__attribute__((unused)) void *adb,__attribute__((unused)) void *arg) 
{
	memcpy((unsigned char *)arg,(unsigned char *)adb, APEP_LEN);
	printf("CSD success\n");
	rte_mempool_put(message_pool4, adb);

	return 0;
}

static int ReadData_Loop() 
{
	// unsigned lcore_id = rte_lcore_id();
	// printf("ReadData Starting core %u\n", lcore_id);
	// const char *message ="ReadData_Loop";
	void *Data =NULL;
	//snprintf((char *)Data, string_size, "%s", message);
	/*入队前，先获取内存池的一块内存*/
	//printf("ReadData_Loop launch");
	while (!quit){
		if ((rte_mempool_get(message_pool1, &Data) < 0)){
			GenDataAndScramble_Loop();
		}
		else {
			ReadData(Data);
		}
		
	}
	return 0;
}
static int ReadData(__attribute__((unused)) void *Data) 
{
	if (rte_ring_enqueue(Ring_Beforescramble, Data) < 0) 
	{
		// printf("Failed to send message - message discarded\n");
		rte_mempool_put(message_pool1, Data);
		GenDataAndScramble_Loop();
	}
	else
	{
	    FILE *fp=fopen("send_din_dec.txt","rt");
		unsigned char* databits=(unsigned char*)malloc(APEP_LEN*sizeof(unsigned char));
	    unsigned int datatmp=0;
		int i=0;
	    for(i=0;i<APEP_LEN;i++){
	            if(fscanf(fp,"%ud",&datatmp) < 0){
	            	free(databits);
	            	printf("fscanf data error");
	            	return 0;
	            }
	            databits[i]=datatmp&0x000000FF;
	    }
		memcpy((unsigned char *)Data, (unsigned char *)databits, APEP_LEN);//将文件读取数据复制给Data即原始数据流
	    fclose(fp);
		printf("ReadData success\n");
		free(databits);
		i++;
		printf("%d\n",i);
	}
	return 0;
}
static int GenDataAndScramble_Loop() 
{
	// unsigned lcore_id = rte_lcore_id();
	// printf("GenDataAndScramble Starting core %u\n", lcore_id);
	// const char *message ="GenDataAndScramble";
	void *Data_In_Scramble=NULL;
	void *Data_Out_Scramble=NULL;
	while (!quit){
	if (rte_ring_dequeue(Ring_Beforescramble, &Data_In_Scramble) < 0){
			ReadData_Loop();
			continue;
		}
	else if ((rte_mempool_get(message_pool2, &Data_Out_Scramble) < 0)) 
	{
		BCC_encoder_Loop ();
	}
	else 
	GenDataAndScramble(Data_In_Scramble,Data_Out_Scramble);
	}
	return 0;
}
static int GenDataAndScramble (__attribute__((unused)) void *Data_In,__attribute__((unused)) void *Data_Out) 
{
		// void GenDataAndScramble(unsigned char *data_scramble, int ScrLength, unsigned char *databits, int valid_bits);
		// 接口：输入为原始数据流和有效数据位，输出为byte转换为bit存储并加扰码后的数据流。
		// 操作：通过查比特扰码表，对原始数据进行比特化并加扰。
		if (rte_ring_enqueue(Ring_scramble_2_BCC, Data_Out) < 0) {
		// printf("Failed to send message - message discarded\n");
		rte_mempool_put(message_pool2, Data_Out);
		ReadData_Loop();
		}
		else 
		GenDataAndScramble_encode_dpdk(Data_In,Data_Out);
		return 0;
}
static int BCC_encoder_Loop() 
{
	// unsigned lcore_id = rte_lcore_id();
	// printf("BCC_encoder Starting core %u\n", lcore_id);
	// const char *message ="BCC_encoder";
	// void *Data =NULL;
	void *Data_In_BCC=NULL;
	void *Data_Out_BCC=NULL;
	while (!quit){
	if (rte_ring_dequeue(Ring_scramble_2_BCC, &Data_In_BCC) < 0){
			GenDataAndScramble_Loop();
			continue;
		}
	else if ((rte_mempool_get(message_pool3, &Data_Out_BCC) < 0)) 
	{
		modulate_Loop();
		continue;
	}
	else 
	BCC_encoder(Data_In_BCC,Data_Out_BCC);
	}
	return 0;
}
static int BCC_encoder (__attribute__((unused)) void *Data_In,__attribute__((unused)) void *Data_Out)
{
		// void BCC_encoder_OPT(unsigned char *data_scramble, int ScrLength, int N_SYM, unsigned char **code_out, int CodeLength);
		// 接口：输入为扰码后的数据流及其有效数据长度和符号数，输出为BCC编码后的数据此时数据。
		if (rte_ring_enqueue(Ring_BCC_2_modulation, Data_Out) < 0) {
		// printf("Failed to send message - message discarded\n");
		rte_mempool_put(message_pool3, Data_Out);
		GenDataAndScramble_Loop();
		}
		else 
		bcc_encode_dpdk(Data_In,Data_Out);
		return 0;
}
static int modulate_Loop() 
{
	// unsigned lcore_id = rte_lcore_id();
	// printf("modulate Starting core %u\n", lcore_id);
	// const char *message ="modulate";
	// void *Data =NULL;
	void *Data_In_modulate=NULL;
	void *Data_Out_modulate=NULL;
	while (!quit){
	if (rte_ring_dequeue(Ring_BCC_2_modulation, &Data_In_modulate) < 0){
			BCC_encoder_Loop();
			continue;
		}
	else if ((rte_mempool_get(message_pool4, &Data_Out_modulate) < 0)) 
	{
		Data_CSD_Loop();
		continue;
	}
	else 
	modulate(Data_In_modulate,Data_Out_modulate);
	}
	return 0;
}
static int modulate(__attribute__((unused)) void *Data_In,__attribute__((unused)) void *Data_Out)
{
		// void modulate_mapping(unsigned char *BCC_output, complex32 **subcar_map_data);
		// 接口：输入为BCC编码后的数据，输出为经过分流交织，调制映射和插入导频后的数据。
		// 操作：对BCC编码后的数据查分流交织表进行分流交织，调制映射和插入导频。
		if (rte_ring_enqueue(Ring_modulation_2_CSD, Data_Out) < 0) {
		// printf("Failed to send message - message discarded\n");
		rte_mempool_put(message_pool4, Data_Out);
		BCC_encoder_Loop();
		}
		modulate_encode_dpdk(Data_In,Data_Out);
		return 0;
}
static int Data_CSD_Loop() 
{
	// unsigned lcore_id = rte_lcore_id();
	// printf("BCC_encoder Starting core %u\n", lcore_id);
	// const char *message ="BCC_encoder";
	// void *Data =NULL;
	void *Data_In_CSD=NULL;
	void *Data_Out_CSD=NULL;
	while (!quit){
	if (rte_ring_dequeue(Ring_modulation_2_CSD, &Data_In_CSD) < 0){
			modulate_Loop();
			continue;
		}
	//else if (rte_mempool_full(message_pool5)) //内存池已满将内存池清空rte_mempool_full(const struct rte_mempool *mp)
	else if ((rte_mempool_get(message_pool5, &Data_Out_CSD) < 0)) //内存池已满将内存池清空
	{
		void *obj_table=NULL;
		struct rte_mempool_cache *cache=NULL;
		unsigned n=APEP_LEN;
		rte_mempool_generic_put(message_pool5, &obj_table,n,cache,1);
		//__mempool_put_bulk(message_pool5, obj_table,n,struct rte_mempool_cache *cache,1);
		//rte_mempool_put(message_pool5, Data_Out);
		//free(Data_Out_CSD);
		rte_mempool_put(message_pool4, Data_In_CSD);
		printf("Clear message_pool5 \n");
		continue;
	}
	else 
	{
	//rte_mempool_get(message_pool5, &Data_Out_CSD);
	Data_CSD(Data_In_CSD,Data_Out_CSD);
	}
	
	}
	return 0;
}
static int Data_CSD(__attribute__((unused)) void *Data_In,__attribute__((unused)) void *Data_Out)
{
		// __Data_CSD_aux(complex32 **subcar_map_data, int N_SYM, complex32 **csd_data,int NTXindex);
		// 接口：输入数据为调制映射并插入导频后的数据，输出为CSD后的数据。
		// 操作：初始化CSD表，通过查表利用向量指令进行CSD计算。
		if (rte_ring_enqueue(Ring_AfterCSD, Data_Out) < 0) {//Ring_AfterCSD已满将Ring_AfterCSD队列元素全部出队
		// printf("Failed to send message - message discarded\n");
		rte_mempool_put(message_pool5, Data_Out);

		//void *obj_table=NULL;
		//unsigned n=APEP_LEN;
		//rte_ring_mc_dequeue_burst(Ring_AfterCSD, &obj_table,n);
		printf("Clear Ring_AfterCSD\n");
		rte_mempool_put(message_pool4, Data_In);
		Data_CSD_Loop();
		}
		else 
		CSD_encode_dpdk(Data_In,Data_Out);		
		return 0;
}	
int
main(int argc, char **argv)
{
	const unsigned flags = 0;
	const unsigned ring_size = 256;
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
		// ring1to2 = rte_ring_create(_one_2_two, ring_size, rte_socket_id(), flags);
		// ring2to3 = rte_ring_create(_two_2_three, ring_size, rte_socket_id(), flags);
		
		message_pool1 = rte_mempool_create(Mempool_Beforescramble, pool_size,
				string_size, pool_cache, priv_data_sz,
				NULL, NULL, NULL, NULL,
				rte_socket_id(), flags);
		message_pool2 = rte_mempool_create(Mempool_scramble_2_BCC, pool_size,
				string_size, pool_cache, priv_data_sz,
				NULL, NULL, NULL, NULL,
				rte_socket_id(), flags);
		message_pool3 = rte_mempool_create(Mempool_BCC_2_modulation, pool_size,
				string_size, pool_cache, priv_data_sz,
				NULL, NULL, NULL, NULL,
				rte_socket_id(), flags);
		message_pool4 = rte_mempool_create(Mempool_modulation_2_CSD, pool_size,
				string_size, pool_cache, priv_data_sz,
				NULL, NULL, NULL, NULL,
				rte_socket_id(), flags);
		message_pool5 = rte_mempool_create(Mempool_AfterCSD, pool_size,
				string_size, pool_cache, priv_data_sz,
				NULL, NULL, NULL, NULL,
				rte_socket_id(), flags);				
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

	if (message_pool1 == NULL)
		rte_exit(EXIT_FAILURE, "Problem getting message pool\n");
	if (message_pool2 == NULL)
		rte_exit(EXIT_FAILURE, "Problem getting message pool\n");
	if (message_pool3 == NULL)
		rte_exit(EXIT_FAILURE, "Problem getting message pool\n");
	if (message_pool4 == NULL)
		rte_exit(EXIT_FAILURE, "Problem getting message pool\n");
	if (message_pool5 == NULL)
		rte_exit(EXIT_FAILURE, "Problem getting message pool\n");
	RTE_LOG(INFO, APP, "Finished Process Init.\n");
	rte_eal_remote_launch(ReadData_Loop, NULL,1);
	rte_eal_remote_launch(GenDataAndScramble_Loop, NULL, 2);
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
