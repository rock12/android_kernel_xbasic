#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define APK_COMMON_PATH			""
#define OUT_FOLDER				"bird/renovateguard_apk_list/out/"
#define APK_SPECIAL_LIST		"bird/renovateguard_apk_list/out/apk_fullpath_list.txt"
#define OUT_APK_COMPILE_INFO	"bird/renovateguard_apk_list/out/apk_compile_info"
#define OUT_APK_LIST_INFO		"bird/renovateguard_apk_list/out/apk_notice.txt"
#define APK_INFO_BLOCK_SIZE	60

//reference file :frameworks\base\bird\RenovateGuard\RenovateGuard.c
int rom_getline(FILE* stream,char s[],int max_length)
{
	char c;
	int len;

	for(len=0;(len<max_length -1)&&((c=fgetc(stream))!=EOF)&&(c!='\n')&&(c!='\r');++len) 
	{
		s[len]=c;
	}
	if(c=='\n'|| c== '\r')
	{
		//printf("debug: 1\n");
		s[len]='\n';
		++len;
	}
	//s[len]='\0';
	return len;
}

int ApkInfoCalcCRC8(unsigned char *p_MemAddr, int BlockSize)
{
	unsigned char CrcReg = 0xFF; 
	unsigned char MemByte;
	unsigned char BitNo;	

	while (BlockSize)
	{
		MemByte = *p_MemAddr;
		for (BitNo = 0; BitNo < 8; BitNo++)
		{
			if ((CrcReg^MemByte) & 0x80)
			{
				CrcReg = (CrcReg << 1) ^ 0x11D; 
			}
			else
			{
				CrcReg <<= 1;
			}
			MemByte <<= 1;
		}
		BlockSize--;
		p_MemAddr++;
	}

	CrcReg = ~CrcReg;
	return CrcReg;
}


int main(int argc, char *argv[])
{
	int nCount;
	char key[13];
	int i=0;
    int i_compile = 0;
    
    system(">"OUT_APK_COMPILE_INFO);
	system(">"OUT_APK_LIST_INFO);
	char new_bird_special_apk_list[1024*40];
	new_bird_special_apk_list[0]=0;
	memset(key, 0, 13);
	strncpy(key, "runfingerprint",13); // \u5f97\u5230fingerprint\u7684\u503c
	//printf("key[%d][%d][%d][%d][%d] [%d][%d][%d][%d][%d] [%d][%d][%d]", key[0], key[1], key[2], key[3], key[4], key[5], key[6], key[7], key[8], key[9], key[10], key[11], key[12]);

	//read apk info b
	FILE *fp_list;
	FILE *fp_apk_in;
	FILE *fp_apk_out;
	FILE *fp_apk_warning;

	char list_path_line[100]="";
	char list_path_whole[100]="";
	char warning_name[100]="";
	unsigned char apk_info[APK_INFO_BLOCK_SIZE];
	unsigned char crc8 = 0;
	unsigned char empty_list_info[] = {0xee,0xee,0xee};
		
	int line_flag = 1;	
    	int rc = 0;
	int count = 0;
	int start=0;
	int apk_count = 0;
	int info_compile_size = 0;
	int linenum=0;
	int success=1;
	if((fp_list = fopen(APK_SPECIAL_LIST, "r"))==NULL){
		printf("huangrunming fp_list open fail: "APK_SPECIAL_LIST"\n");
	    	return 0;
	}

	if((fp_apk_out = fopen(OUT_APK_COMPILE_INFO, "wb" ))==NULL){
		printf("huangrunming fp_apk_out open fail: " OUT_APK_COMPILE_INFO "\n" );
	    return 0;
	}
	
	if((fp_apk_warning = fopen(OUT_APK_LIST_INFO, "w" ))==NULL){
		printf("huangrunming fp_apk_warning open fail:"OUT_APK_LIST_INFO"\n");
	    return 0;
	}

  
	while(line_flag){
		linenum++;
		memset(list_path_line,0,strlen(list_path_line));
		memset(list_path_whole,0,strlen(list_path_whole));
		memset(warning_name,0,strlen(warning_name));
		memset(apk_info,0x00,APK_INFO_BLOCK_SIZE);

		count = rom_getline(fp_list,list_path_line,100);
	
		if ('\n' == list_path_line[0])
		{
			continue;
		}
		//printf("huangrunming list_path_line = (%s)\n",list_path_line);
		if (strlen(list_path_line) ==0) // last line
		{
			line_flag	= 0;
			//printf("debug: over!\n");
			break;
		}
		if (strlen(list_path_line) < strlen("a.apk\n"))
		{
			line_flag=0;
			printf(APK_SPECIAL_LIST" error in line(%d): %s\n", linenum, list_path_line);
			success = 0;
			break;
		}
		
		if (list_path_line[strlen(list_path_line)-1] != '\n')
		{
			strcat(list_path_line, "\n");
			count++;
		}
		
		if(('k' != list_path_line[count-2])&&('p' != list_path_line[count-3])&&('a' != list_path_line[count-4])){
			line_flag = 0;
			printf(APK_SPECIAL_LIST" error in line(%d):%s\n", linenum, list_path_line);
			success = 0;
			break;
		}
		list_path_line[strlen(list_path_line)-1] = 0;
		if (0 != access(list_path_line, 0))
		{
			line_flag=0;
			printf(APK_SPECIAL_LIST" error in line(%d): file not exist %s\n", linenum, list_path_line);
			success = 0;
			break;
		}
		strcat(list_path_line, "\n");
		strcat(new_bird_special_apk_list, list_path_line);
		apk_count++;
		//warning file b
		for(i = count-2;i>=0;i--){
			if('/' == list_path_line[i]){
				start = i;
				break;
			}
		}
		//printf("huangrunming start = %d\n",start);
		if(start){
			for(i = 0;i<count-start;i++){
				warning_name[i] = list_path_line[start+1+i];
				//printf("huangrunming i = %d,warning_name[i] = %c\n",i,warning_name[i]);
			}
		}
		rc= fwrite(warning_name, strlen(warning_name), 1, fp_apk_warning );
		//printf("huangrunming warning_name, rc =%d\n", rc);
		//warning file d

		strcat(list_path_whole,APK_COMMON_PATH);
		strncat(list_path_whole,list_path_line,strlen(list_path_line)-1);
		printf("%s\n",list_path_whole);
		
		if((fp_apk_in = fopen(list_path_whole, "rb"))==NULL){
			printf("huangrunming list_path_whole = %s\n", list_path_whole);
			success = 0;
	    		return 0;
		}

		if((rc = fread(apk_info,APK_INFO_BLOCK_SIZE, 1,fp_apk_in)) != 0 ){ 
			//printf("huangrunming read =%d\n", rc);
			//make crc8 to first byte of every apk
			crc8 = ApkInfoCalcCRC8(apk_info,APK_INFO_BLOCK_SIZE);
			apk_info[0] = crc8;
			//printf("crc8=%x\n",crc8);
		
		//^13
		nCount = APK_INFO_BLOCK_SIZE/13*13;	//times of 13
		while(nCount >= 0)
		{
			int jji;
			for (jji = 0; jji < 13 && nCount + jji < APK_INFO_BLOCK_SIZE; jji++)
			apk_info[nCount + jji] ^= (unsigned char)key[jji];
			nCount -= 13;
		}
		
    		rc= fwrite(apk_info, APK_INFO_BLOCK_SIZE, 1, fp_apk_out );
		}
		fclose(fp_apk_in);
	}
	
	if(apk_count==0){
		//if special_apk_list is empty or not exist.
		//write 0xee,0xee,0xee to fp_apk_out
		printf("apk list is empty\n");
		fwrite(empty_list_info, 3, 1, fp_apk_out );
	}
	fclose(fp_apk_out);
	
	//warning file
	memset(warning_name,0,strlen(warning_name));
	strcat(warning_name,"Warning! These apks cannot be modified!And this file should not be modified!!\n");
	rc= fwrite(warning_name, strlen(warning_name), 1, fp_apk_warning );
	
	fclose(fp_list);
	fclose(fp_apk_warning);
	//read apk info d
	//printf("new_bird_special_apk_list=\n%s", new_bird_special_apk_list);
	if (success)	// overwrite bird_special_apk_list
	{
		if((fp_list = fopen(APK_SPECIAL_LIST, "w"))==NULL){
			printf("huangrunming fp_list open fail: "APK_SPECIAL_LIST"\n");
	    	return 0;
		}
		fwrite(new_bird_special_apk_list, strlen(new_bird_special_apk_list), 1, fp_list);
		fclose(fp_list);

	}
	return 0;
}
