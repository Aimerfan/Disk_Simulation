# Disk_Simulation

共三份主要檔案:
OSIIHW.cpp : 作業本體，單檔，使用C++，編譯環境Dev C++ TDM-GCC 4.9.2，語言標準GNU C++11
request_generate.c : 測資檔產生器，使用C語言，編譯環境Dev C++ TDM-GCC 4.9.2，語言標準GNU C99
run_basic.bat : 執行批次檔，會執行一次request_generate.exe，預設產生"basic.txt"檔案後，執行OSIIHW.exe在cmd視窗中印出結果

OSIIHW.cpp :

輸入檔案格式如下:
1.OSIIHW.cpp : 
  預設輸入檔名"basic.txt"(功能未開發完成，只能讀取預設檔名)，包含以下項目 : 
	Disk scheduling algorithms:
	Number of disk cylinders:
	Initial disk head cylinder: 
	Direction of disk head:
	Disk requests:(-1做結尾)

	EXAMPLE:

	CLOOK
	200
	55
	0
	100,181,39,120,16,122,67,69,-1
	
  印出格式範例 : 
	DISK SCHEDULE START WITH INIT AS : 
	
	Algorithm : CLOOK
	Total cylinders : 200
	Init head seat : 55
	Init direct : 0
	Disk requests : 
	(0,100)
	(0,181)
	(0,39)
	(0,120)
	(0,16)
	(0,122)
	(0,67)
	(0,69)
	
	Head track : 
	(0.0,67)
	(0.0,69)
	(0.0,100)
	(0.0,120)
	(0.0,122)
	(0.0,181)
	(0.0,16)
	(0.0,39)
	
	total moved cylinders : 314
	
2.request_generate.c : 
  在命令列環境下使用'/?'獲取使用說明 : 
	/A 表示產生檔名為"advence.txt"的進階測資
	/B 表示產生檔名為"basic.txt"的基本測資
	/? 顯示說明
	
	任意數字參數，將修改隨機產生request的數量(預設值20)
	
3.run_basic.bat : 
  1.無使用參數執行一次request_generate.exe，產生"basic.txt"檔案
  2.執行OSIIHW.exe在cmd視窗中印出結果
  3.pause