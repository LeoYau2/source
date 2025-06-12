#include <stdio.h>
#include <windows.h>
#include <time.h>

#define timezone 8
#define output_path kk.mm.ss.txt

int main()
{
	FILE *out, *date_out;
	out = fopen("kk.mm.ss.txt", "w");
	//char buffer[] = {'0','0',':','0','0',':','0','0'};
	int n = 0;
	long s_before = 0;
	long s = 0;
	int mday_before = 0;
	while (1)
	{
		s = time(NULL);
		if (s > s_before)
		{
			time_t t = time(NULL);
			struct tm tm = *localtime(&t);
			//printf("%d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
			//printf("%d%d:%d%d:%d%d\n", (s/3600+timezone)%24/10,(s/3600+timezone)%24%10,s/60%60/10,s/60%60%10,s%60/10,s%60%10);

			//buffer[0] = 0x30;  char(0x30) == '0'
			/*buffer[0] = 0x30 + (s/3600+timezone)%24/10;
			buffer[1] = 0x30 + (s/3600+timezone)%24%10;
			buffer[3] = 0x30 + s/60%60/10;
			buffer[4] = 0x30 + s/60%60%10;
			buffer[6] = 0x30 + s%60/10;
			buffer[7] = 0x30 + s%60%10;
			for(int i=0;i<8;i++)
			{
				printf("%c",buffer[i]);
			}
			printf("\n");*/
			out = fopen("kk.mm.ss.txt", "w");
			fprintf(out, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
			//fwrite(buffer,1,sizeof(buffer),out);
			fclose(out);
			if (tm.tm_mday != mday_before)
			{
				date_out = fopen("YY.MM.DD.txt", "w");
				//fprintf(out,"%02d-%02d-%02d", tm.tm_year%100, tm.tm_mon + 1, tm.tm_mday);
				fprintf(out, "%02d.%02d.%02d", tm.tm_year % 100, tm.tm_mon + 1, tm.tm_mday);
				fclose(date_out);
				mday_before = tm.tm_mday;
				printf("----%02d.%02d.%02d----\n", tm.tm_year % 100, tm.tm_mon + 1, tm.tm_mday);
			}
			printf("%02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
			s_before = s;
		}
		Sleep(50);
		n++;
	}
	return (0);
}
