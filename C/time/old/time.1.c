#include <stdio.h>
#include <windows.h>
#include <time.h>

#define timezone 8
#define output_path kk.mm.ss.txt

int main ()
{
	FILE *out;
	out = fopen("kk.mm.ss.txt","w");
	char buffer[] = {'0','0',':','0','0',':','0','0'};

	//fprintf(out, "%c[  ]",'w');
	//fclose(out);

	long s_before = 0;
	long s = 0;
	while(1)
	{
		s = time(NULL);
		if(s > s_before)
		{
			//printf("%d%d:%d%d:%d%d\n", (s/3600+timezone)%24/10,(s/3600+timezone)%24%10,s/60%60/10,s/60%60%10,s%60/10,s%60%10);

			//buffer[0] = 0x30;  char(0x30) == '0'
			buffer[0] = 0x30 + (s/3600+timezone)%24/10;
			buffer[1] = 0x30 + (s/3600+timezone)%24%10;
			buffer[3] = 0x30 + s/60%60/10;
			buffer[4] = 0x30 + s/60%60%10;
			buffer[6] = 0x30 + s%60/10;
			buffer[7] = 0x30 + s%60%10;
			for(int i=0;i<8;i++)
			{
				printf("%c",buffer[i]);
			}
			printf("\n");
			out = fopen("kk.mm.ss.txt","w");
			fwrite(buffer,1,sizeof(buffer),out);
			fclose(out);
			//fprintf(out, "%c[  ]",'w');


			s_before = s;
		}
		Sleep(50);
	}
  	return(0);
}
