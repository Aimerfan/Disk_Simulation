#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int mode = 0;
int rlimit = 15;
int m_algo = -1;
char* algo[] = { "FCFS", "SSTF", "SCAN", "LOOK", "CSCAN", "CLOOK" };

void help(){
	puts("/A 表示產生檔名為\"advence.txt\"的進階測資");
	puts("/B 表示產生檔名為\"basic.txt\"的基本測資");
	puts("/? 顯示說明\n");
	puts("任意數字參數，將修改隨機產生request的數量");
	return;
}

int det_algo(char* m_algo){
	int i;
	char *upper = m_algo;
	while(*upper) *upper = toupper(*upper);
	for(i = 0; i < 6; i++) if(!strcmp(m_algo, algo[i])) break;
	if(i == 6) return -1;
	else return i;
}

int main(int argc, char *argv[]){
	
	FILE* output;
	int cylinders;
	srand(time(NULL));
	
	int i;
	for(i = 1; i < argc; i++){
		if(argv[i][0] == '/'){
			if(argv[i][1] == 'A' || argv[i][1] == 'a') mode = 1;
			else if(argv[i][1] == 'B' || argv[i][1] == 'b') mode = 0;
			else if(argv[i][1] == '?') help();
			else if((m_algo = det_algo(&argv[i][1])) > 0) printf("%d\n", m_algo);
			else goto error_argu;
		}
		else if('0' <= argv[i][0] && argv[i][0] <= '9'){
			int lim = atoi(argv[i]);
			if(lim == 0) goto error_argu;
			rlimit = lim;
		}
		else{
			error_argu:
			printf("\"%s\" is not a valid argument.\n", argv[i]);
		}
	}
	
	if(mode == 0) output = fopen("basic.txt", "w");
	else if(mode == 1) output = fopen("advence.txt", "w");
	
	cylinders = (rand()%10) * 100;
	fprintf(output, "%s\n", m_algo > 0 ? algo[m_algo] : algo[rand()%6]);
	fprintf(output, "%d\n%d\n%d\n", cylinders, rand()%cylinders, rand()&1);
	if(mode == 0){
		for(i = 0; i < rlimit; i++) fprintf(output, "%d,", rand()%cylinders);
		fprintf(output, "-1");
	}
	else if(mode == 1){
		for(i = 0; i < rlimit; i++) fprintf(output, "(%d.%d,%d)", rand()%(rlimit*10), rand()%10, rand()%cylinders);
		fprintf(output, "(-1,-1)");
	}
	fclose(output);
	
	return 0;
}
