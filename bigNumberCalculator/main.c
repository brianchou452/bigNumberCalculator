//
//  main.c
//  bigNumberCalculator
//
//  Created by Brian Chou on 2022/4/8.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define NUM_MAX 100 //在此定義所能計算的最大位數

void reverseStr(char str[]);
int toInt(char charNum);
char toChar(int num);
void add(char a[], char b[], char ans[]);
void minus(char a[], char b[], char ans[]);
int findDot(char num[]);
int max(unsigned long a, unsigned long b){
	return a > b ? (int)a : (int)b;
}
bool minusIsNegative(char a[], char b[]);


int main(int argc, const char * argv[]) {

    printf("這是一個可以計算%d位數加減法的計算機\n", NUM_MAX);
    printf("輸入數字僅接受正整數或正小數，\n");
    printf("輸入錯誤會導致程式壞掉歐!!!\n\n\n");
    printf("###############################\n");
	bool continueCal = true;
	do {
		char numA[NUM_MAX + 1] = {};// +1 是因為有'\0'
		char numB[NUM_MAX + 1] = {};
		char ans[NUM_MAX * 2 + 1] = {};//有可能a的位數都在小數點左邊,b的位數都在小數點右邊（a = 123.1 b = 1.234）
		printf("請輸入第一個數字：");
		scanf("%s",numA);
		printf("請輸入第二個數字：");
        setbuf(stdin, NULL);//清空輸入緩衝區
		scanf("%s",numB);
		printf("1. 加法\n2. 減法\n請輸入數字選擇要進行的運算: ");
        setbuf(stdin, NULL);//清空輸入緩衝區
		int calculateType;
		scanf("%d",&calculateType);

		if(calculateType == 1) {
			add(numA, numB, ans);
			printf("答案為：%s\n\n", ans);
		}else if (calculateType == 2) {
			minus(numA, numB, ans);
			printf("答案為：%s\n\n", ans);
		}else{
			printf("喔哦！ 輸入錯誤餒！\n\n");
		}

		printf("1. 繼續算！\n2. 才不要！掰掰！\n是否繼續？");
		int continueType;
		scanf("%d",&continueType);
		continueCal = continueType == 1 ? true : false;
		printf("\n\n");
        printf("========================\n");
	} while (continueCal);


	return 0;
}

void reverseStr(char str[]){
	char tmp[NUM_MAX * 2 + 1] = {};
	int arrLength = (int)strlen(str);
	strcpy(tmp, str);
	for(int i = 0; i < arrLength; i++) {
		str[i] = tmp[arrLength - i - 1];
	}
}

int toInt(char charNum){
	return charNum == '\0' ? 0 : charNum - 48;
}
char toChar(int num){
	return num + 48;
}

int findDot(char num[]){
	for(int i = 0; i < strlen(num); i++) {
		if(num[i] == '.') return i;
	}
	return -1;
}

void add(char a[], char b[], char ans[]){
	if(strlen(a) > NUM_MAX || strlen(b) > NUM_MAX) {
		strcpy(ans, "哎呀！ 超出所能計算的最大數 :(");
		return;
	}
	reverseStr(a);//將陣列倒過來，方便計算
	reverseStr(b);
	int dotA = findDot(a);//從右邊數來小數點的位置(123.4, dotA = 2)
	int dotB = findDot(b);
	int offsetA = 0;//利用偏移量使a b兩數的小數點對齊
	int offsetB = 0;

	if(dotA < dotB) {//根據小數點的位置判斷是a或b偏移
		offsetA = dotB - dotA;
	}else if (dotA > dotB) {
		offsetB = dotA - dotB;
	}

	int addToNext = 0;//進位到下一位的數
	for(int i = 0; i <= max(strlen(a), strlen(b)) + offsetA + offsetB; i++) {
		int indexA = i - offsetA;//減去offset計算真正的index
		int indexB = i - offsetB;
		if(i == NUM_MAX  && addToNext == 1) {
			strcpy(ans, "哎呀！ 超出所能計算的最大數 :(");
			return;
		}
		if(i == max(strlen(a), strlen(b)) + offsetA + offsetB && addToNext == 0) {//for loop 最後一輪，判斷是否進位
			break;
		}
		if(a[indexA] == '\0' && b[indexB] == '\0' && addToNext == 0) {//如果a和b都無數字且無進位(50 + 50 = 100)
			break;
		}

		if(a[indexA] == '.' || b[indexB] == '.') {//遇到小數點時跳過
			ans[i] = '.';
			continue;
		}
		int ai = indexA >= 0 ? toInt(a[indexA]) : 0;//a b 兩數位數不同時index有可能為負,
		int bi = indexB >= 0 ? toInt(b[indexB]) : 0;//因為有扣除偏移量(offsetA B)
		int tmp = ai + bi + addToNext;
		if(tmp > 9) {//判斷是否進位
			ans[i] = toChar(tmp - 10);
			addToNext = 1;
		}else{
			ans[i] = toChar(tmp);
			addToNext = 0;
		}

	}
	reverseStr(ans);//將陣列倒回來，成為最後的答案
}

bool minusIsNegative(char a[], char b[]){
	int lengthA = (int)strlen(a);
	int lengthB = (int)strlen(b);
    int findDotA = findDot(a);
    int findDotB = findDot(b);
    int dotA = findDotA == -1 ? 0 : lengthA - findDotA;//從右邊數來小數點在第幾位,
	int dotB = findDotB == -1 ? 0 : lengthB - findDotB;//如果使用者輸入的數無小數點findDot()會回傳-1,因此需判斷！
	if(lengthA - dotA > lengthB - dotB ) {//比較小數點左邊的位數
		return false;
	}else if (lengthA - dotA < lengthB - dotB) {
		return true;
	}else{//小數點左邊位數相等，無需增加偏移量
		for(int i = 0; i < max(lengthA, lengthB); i++) {
			if(toInt(a[i]) > toInt(b[i])) {
				return false;
			}else if (toInt(a[i]) < toInt(b[i])) {
				return true;
			}
		}
	}
	return false;//代表兩數相等
}

void minus(char a[], char b[], char ans[]){
	if(strlen(a) > NUM_MAX || strlen(b) > NUM_MAX) {
		strcpy(ans, "哎呀！ 超出所能計算的最大數 :(");
		return;
	}
	bool isNegative = minusIsNegative(a, b);
	if(isNegative) {//如果相減為負，則a、b互換再加上負號
		minus(b, a, ans);
		if(strlen(ans) < NUM_MAX) {
			char tmp[NUM_MAX] = {};
			strcpy(tmp, ans);
			strcpy(ans, "-");
			strcat(ans, tmp);
		}else{
			strcpy(ans, "哎呀！ 加上負號超過最大位數 :(");
		}
		return;
	}
	reverseStr(a);//將陣列倒過來，方便計算
	reverseStr(b);
	int dotA = findDot(a);
	int dotB = findDot(b);
	int offsetA = 0;//利用偏移量使a b兩數的小數點對齊
	int offsetB = 0;

	if(dotA < dotB) {//根據小數點的位置判斷是a或b偏移
		offsetA = dotB - dotA;
	}else if (dotA > dotB) {
		offsetB = dotA - dotB;
	}

	int previousBorrow = 0;//上一位的借位
	for(int i = 0; i <= max(strlen(a), strlen(b)) + offsetA + offsetB; i++) {
		int indexA = i - offsetA;//減去offset計算真正的index
		int indexB = i - offsetB;
		if(i == NUM_MAX  && previousBorrow == 1) {
			strcpy(ans, "哎呀！ 超出所能計算的最大數 :(");
			return;
		}
		
		if(a[indexA] == '\0' && b[indexB] == '\0') {
			break;
		}

		if(a[indexA] == '.' || b[indexB] == '.') {//遇到小數點時跳過
			ans[i] = '.';
			continue;
		}
		int ai = indexA >= 0 ? toInt(a[indexA]) : 0;
		int bi = indexB >= 0 ? toInt(b[indexB]) : 0;
		int tmp = ai - bi - previousBorrow;
		if(tmp < 0) {//判斷是否需借位
			ans[i] = toChar(tmp + 10);
			previousBorrow = 1;
		}else{
            //避免 100 - 20 ＝ 080
			if(i == max(strlen(a), strlen(b)) + offsetA + offsetB - 1 && tmp == 0 && ans[i - 1] != '.') {
				ans[i] = '\0';
				break;
			}else{
				ans[i] = toChar(tmp);
				previousBorrow = 0;
			}

		}

	}
	reverseStr(ans);
}

/*
   測資：
   加法：
   20 80
   900 99
   20 2.1
   1.1 1

   減法：
   12.1 10.1
   0.45 0.78
   12.5 2.1
   123.1 2.456
 */
