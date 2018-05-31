#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int mode = 0;
int rlimit = 15;
char* algo[] = { "FCFS", "SSTF", "SCAN", "LOOK", "CSCAN", "CLOOK" };

void help(){
	puts("/A 表示產生檔名為\"advence.txt\"的進階測資");
	puts("/B 表示產生檔名為\"basic.txt\"的基本測資");
	puts("/? 顯示說明\n");
	puts("任意數字參數，將修改隨機產生request的數量(預設值20)");
	return;
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
	
	cylinders = (rand()%20) * 500;
	fprintf(output, "%s\n", algo[rand()%6]);
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
