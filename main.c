#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
int q[4];  //把出好的題目放在這裡，需四個位置
char a[500];  //放讓使用者輸入的數字 
int a_int[4];  //把使用者輸入的字串轉成數字後放在這四個位置 
int answerBase[5040][4];  /*答案庫，會裝入所有重複的四位數，從1234到9876
                            5040是因為10*9*8*7，4是因為每組答案是一個四位數*/
int isOver,restart;  //當作布林值，判斷是否遊戲結束以及是否要繼續玩
int remain;  //答案庫裡面的答案數 
int l,m;  //拿來裝使用者回答的幾a幾b
int randNum;  //裝亂數，隨機從答案庫裡面拿答案 
int ifclear;

void makeQ(void);
void getA();
void outcome(void);
void intro(void);
void menu(void);
void prepare(void);  //準備工作之函數 
void ask(void);  //電腦問之函數 
void answer(void);  //人類答幾a幾b之函數 
void think(void);  //電腦思考之函數（其實就是從答案庫中排除不可能的答案） 

int main(void){
    intro();
    while(1==1){
        ifclear =0;
        isOver=0;
        prepare();
        while(!isOver||!ifclear){
            makeQ();
            ROOT:getA();
            if(ifclear) break;
            ask();
            answer();
            if(isOver) break;
            think();
            if(isOver) break;
            if(!ifclear) goto ROOT;
        }
        menu();
        if(restart) break;
    }
    return 0;
}
/****** 介紹文 ******/
void intro(void){
    printf("*************************************\n");
    printf("歡迎遊玩電腦與人的猜數字比賽程式\n");
    printf("\n");
    printf("遊戲規則：\n");
    printf("請你在心中想一組4位不重複的數字\n");
    printf("同時電腦也出一組4位不重複的數字\n");
    printf("你每次需輸入一個數字不重複的四位數\n");
    printf("同時電腦也會猜測你心裡想的數字\n");
    printf("當你輸入完你猜的數字後電腦會顯示幾A幾B\n");
    printf("幾A代表你有幾個數字正確且位置正確\n");
    printf("幾B代表你有幾個數字正確但位置錯誤\n");
    printf("電腦同時也會顯示他猜的數字\n");
    printf("你要告訴電腦他猜的結果是幾A幾B\n");
    printf("如果你比電腦先猜中你就贏了\n");
    printf("反之如果電腦先猜中你就輸了\n");
    printf("注意!電腦一定能在7回合之內猜中喔!\n");
    printf("祝好運！\n");
    printf("\n");
    printf("**  操作說明：                                    \n");
    printf("**  輸入四個數字直到猜出來或電腦先猜中                \n");
    printf("************************************\n");
}
/****** 出題函數 ******/
void makeQ(void){
    int i,j;
    srand(time(0));
    for(i=0;i<4;i++){
        *(q+i) = rand()%10;
        for(j=0;j<i;j++){
            while(*(q+i) == *(q+j)){  //這個while是用來當數字重複出現時讓就再一次取亂數
                *(q+i) = rand()%10;
            }
        }

    }
}
/****** 讀數字函數 ******/
void getA(){
    LOOP:printf("\n請輸入: ");
    int i,j;  //迴圈專用 
    int boolin = 1;  //用來表示使用者是否正常輸入
    fgets(a,sizeof(a),stdin);
    *(a+strlen(a)-1)= '\0';  //空字元，用於字串的結束
    //因為fget只要成功讀取，會自動在'\0'前面加上'\n'，所以要把'\n'變成'\0'，不然結果會不如預期地多出1


    if(!(strlen(a)==4)){  //判斷使用者是否輸入了四個字元 
        boolin *=0;  //表示使用者不正常輸入 
        printf("請不要輸入太多或太少\n");
    }
    for(i=0;i<strlen(a);i++){  //判斷讓使用者是否輸入0到9 
        if(!(*(a+i) >= '0' && *(a+i) <= '9')){
            boolin *= 0;
            printf("請輸入數字0~9\n");
            goto LOOP;
        }
    }

    for(i=0;i<4;i++){  //判斷使用者是否輸入重複數字 
        for(j=0;j<i;j++){
            while(*(a+i) == *(a+j)){  //這裡和上述出題用的是一樣的原理 
                printf("請不要輸入重複的數字\n");
                boolin *= 0;
                goto LOOP;
            }
            if(!boolin) goto LOOP;
        }
        if(!boolin) goto LOOP;
    }

    if(boolin){  //boolin如果等於1，也就是使用者正常輸入，才開始把字串轉成數字 
        for(i=0;i<4;i++){
            *(a_int+i) = *(a+i) - '0';
        }
        outcome();  //轉成數字後呼叫outcome看看結果為幾A幾B
    }
}

/****** 顯示結果函數 ******/
void outcome(void){
    int a=0;  //幾A的A 
    int b=0;  //幾B的B 
    int i,j;  //迴圈專用 
    for(i=0;i<4;i++){  //使用兩個for，裡面的if作16次 
        for(j=0;j<4;j++){
            if(*(a_int+i)==*(q+j)){  //先判斷有沒有遇到相同的數字 
                if(i==j){  //如果遇到相同的數字，再判斷是不是在相同的位置 
                    a++;  //相同數字且相同位置，那麼就得到一個A 
                }else{  //相同數字但沒有相同位置，那麼就得到一個B
                    b++;
                }
            }
        }
    }

    if(a==4){  //如果有4A代表使用者過關了！
        printf("恭喜！你贏電腦了！\n");
        ifclear = 1;  //因為過關了，所以從0變成1 
        menu();  //過關後跳出選單看不要繼續玩
    }else{
        printf("%dA%dB\n",a,b);  //沒過關，那就顯示幾A幾B
    }

}
/****** 選單函數 ******/
void menu(void){
    printf("再玩一次請輸入'r'，離開請輸入'q'\n");
    fgets(a,sizeof(a),stdin);
    a[strlen(a)-1]='\0';

    if(*a == 'r' && strlen(a) == 1){ //選擇再玩一次 
        restart = 0;  //跳出後且不能離開，這樣才可以再開新局
    }else if(*a == 'q' && strlen(a) == 1){
        restart = 1;
    }else{
        menu();
    }

}
/****** 準備工作之函數 ******/
void prepare(void){
    int i,tmp,unit_1,unit_10,unit_100,unit_1000;
    remain=5040;
    tmp=0;  //tmp只是一個拿來計數變數 
    isOver=0;  //判斷結束與否 
    for(i=0;i<10000;i++){  //這個for產生答案庫 
        unit_1 = (i/1)%10;  //以下四行把四位數拆成四個各位數分別存到四個變數 
        unit_10 = (i/10)%10;
        unit_100 = (i/100)%10;
        unit_1000 = (i/1000)%10;
        if(unit_1!=unit_10 && unit_1!=unit_100 && unit_1!=unit_1000 && unit_10!=unit_100 && unit_10!=unit_1000 && unit_100!=unit_1000){
            answerBase[tmp][0] = unit_1000;
            answerBase[tmp][1] = unit_100;
            answerBase[tmp][2] = unit_10;
            answerBase[tmp][3] = unit_1;
            tmp++; //判斷各個位數的數字彼此不重複，程式才會繼續執行
        }
    }
}
/****** 電腦問之函數 ******/
void ask(void){
    int i;
    printf("電腦:請問是");
    srand(time(0));
    while(1==1){
        randNum=0;
        for(i=0;i<4;i++){
            randNum *= 10;
            randNum += rand()%10; //電腦猜的數字跟出題的時候一樣，只能是四位數不重複的數字
        }
        if(randNum<remain) break;
    }
    for(i=0;i<4;i++) printf("%d",answerBase[randNum][i]);
    printf("?\n");
}/****** 人類答之函數 ******/
void answer(void){
    char input[500];
    int right=0;  //當布林值，判斷是否正確的輸入 
    do{  //取得a,b值 
        do{  //取得a值
            printf("A:");
            fgets(input,sizeof(input),stdin);
            *(input+strlen(input)-1)='\0';
            if(strlen(input)==1 && *input-'0'<= 9 && *input-'0'>=0){ //判斷輸入的A值在合理範圍內
                l = *input-'0';
                right = 1;
            }else printf("輸入錯誤\n\n"); //若輸入錯誤則顯示，並要求使用者重新輸入
        }while(!right);
        do{  //取得b值 
            printf("B:");
            fgets(input,sizeof(input),stdin);
            *(input+strlen(input)-1)='\0';
            if(strlen(input)==1 && *input-'0'<= 9 && *input-'0'>=0){ //判斷輸入的B值在合理範圍內
                m = *input-'0';
                right = 1;
            }else printf("輸入錯誤\n\n"); //若輸入錯誤則顯示，並要求使用者重新輸入
        }while(!right);

        if(l+m>4){  //如果a和b相加大於4當然就不正確
            printf("輸入錯誤\n\n");
            right = 0;
        }
    }while(!right);
    if(l==4){
        int i;
        printf("你輸囉!\n");
        printf("答案是：");
        for(i=0;i<4;i++){
            printf("%d",*(q+i)); //公布電腦出的正確答案
        }
        printf("\n");
        isOver=1;
    }else if(remain==1){  //如果答案庫只剩一個答案，但是這答案還是錯的，就是玩家作弊！ 
        printf("你作弊！\n");
        isOver=1;
    }
}
/****** 電腦思考之函數 ******/
void think(void){
    int i,j,k,aa,bb;
    int refer[4];
    for(i=0;i<4;i++){
        refer[i]=answerBase[randNum][i];
    }
    for(i=0;i<remain;i++){
        aa = bb = 0;  //填裝a值和b值，但要區別於之前的人類輸入的a值b值 
        for(j=0;j<4;j++){  //以下兩個for用來比對第i個答案和亂數取的答案的是幾a幾b 
            for(k=0;k<4;k++){
                if(answerBase[i][j]==refer[k]){
                    if(j==k) aa++;
                    else bb++;
                }
            }
        }
        if(!((aa==l)&&(bb==m))){  //把a值b值相等的答案保留下來 
            for(j=i;j<remain;j++){  //開始搬運 
                for(k=0;k<4;k++){
                    answerBase[j][k]=answerBase[j+1][k];
                }
            }
            remain--;
            i--;
        }
    }
    if(remain==0){  //發現沒有答案符合玩家想的數字，就是玩家作弊！
        printf("你作弊！\n");
        isOver=1;
    }
}