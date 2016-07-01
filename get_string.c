#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

char outbuf[100];
char *main(int argc, char *argv[])
{
	FILE *fp,*outfp;
	int iFileLen,i,j,k,isSearchStarted;
	char *bf;

	fp = fopen(argv[1],"rt");
	if(fp==NULL)
	{
		printf("file : %s not exit\n",argv[1]);
		return;
	}
	outfp = fopen("tmp","wc");
	if(outfp==NULL)
	{
		printf("file : %s not exit\n",argv[1]);
		return;
	}
	fseek(fp,0,SEEK_END);
	iFileLen = ftell(fp);
	fseek(fp,0,SEEK_SET);
	//printf("file length = %d\n",iFileLen);

	bf = (char *)malloc(iFileLen+4);
	fread(bf,iFileLen,1,fp);

	
	outbuf[0]=0;
	isSearchStarted = 0;
	if(argc == 4)
		isSearchStarted = 1;	
	for(i=0;i<iFileLen-strlen(argv[2]);i++)
	{
		if(strncmp(&bf[i],(char *)"AUTO_ADD_GLOBAL_DEFINE_BY_NAME_VALUE",strlen("AUTO_ADD_GLOBAL_DEFINE_BY_NAME_VALUE"))==0)
		{
			for(;;i++)
			{			
				if(bf[i]==0x0d || bf[i]==0x0a)
					break;
			}
			isSearchStarted = 1;			
		}
	
		if(strncmp(&bf[i],(char *)argv[2],strlen(argv[2]))==0 && isSearchStarted==1)
		{
			if(bf[i-1]=='#'||bf[i-2]=='#'||bf[i-3]=='#'||bf[i-4]=='#')
				break;
			//add by zhuyaozhong 20110720
			if(bf[i+strlen(argv[2])]!=' '&&bf[i+strlen(argv[2])]!='=')
				continue;
			//add by zhuyaozhong 20120612
			k = 3;
			if(bf[i+strlen(argv[2])]=='=' && bf[i+strlen(argv[2])+1]!='=' && bf[i+strlen(argv[2])+1]!=' ')	// PLATFORM=123
				k = 1;
			// jiali add begin 20130301
			else if (bf[i+strlen(argv[2])]==' ' && bf[i+strlen(argv[2])+1]=='=' && bf[i+strlen(argv[2])+2]==' ' && bf[i+strlen(argv[2])+3]!=' ')	// PLATFORM = 123
				k = 3;
			else if (bf[i+strlen(argv[2])]==' ' && bf[i+strlen(argv[2])+1]=='=' && bf[i+strlen(argv[2])+2]!=' ')	// PLATFORM =123
				k = 2;
			else if (bf[i+strlen(argv[2])]=='=' && bf[i+strlen(argv[2])+1]==' ' && bf[i+strlen(argv[2])+2]!=' ')	// PLATFORM= 123
				k = 2;
			else if (bf[i+strlen(argv[2])]==' ' && bf[i+strlen(argv[2])+1]!='=' && bf[i+strlen(argv[2])+1]!=' ')	// PLATFORM OTHERDEFINE
			{
				continue;
			}
			else
			{
				fprintf(stderr, "%s format error!\nonly support formats below:\n  ", argv[1]);
				fprintf(stderr, "%s=123\n  ", argv[2]);
				fprintf(stderr, "%s = 123\n  ", argv[2]);
				fprintf(stderr, "%s= 123\n  ", argv[2]);
				fprintf(stderr, "%s =123\n", argv[2]);
				return 0;
			}
			// jiali add end 20130301
			for(j=0;j<100;j++)
			{	
				outbuf[j] = bf[i+strlen(argv[2])+k+j];
				if(bf[i+strlen(argv[2])+k+j]==' '
					|| bf[i+strlen(argv[2])+k+j]==0x0d
					|| bf[i+strlen(argv[2])+k+j]==0x0a
					|| bf[i+strlen(argv[2])+k+j]=='\t'
					|| bf[i+strlen(argv[2])+k+j]=='#'
					|| bf[i+strlen(argv[2])+k+j]==0xFF
					)	// jiali modify 20130609
				{
					outbuf[j]=0;
					printf("%s",outbuf);
					fwrite(outbuf,strlen(outbuf),1,outfp);	
					break;
				}
			}
			break;
		}
	}

	if(outbuf[0]==0)
		printf("%s",argv[2]);

	free(bf);
	fclose(fp);
	fclose(outfp);
	return &outbuf[0];
}
