#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<iostream>
using namespace std;
//用结构体存放操作数 
struct Digit{
    int x;
    int y;
};  
char ss[4]={'+','-','x','%'}; //操作符
//返回操作符的优先级
int mPriority(char op){
    if(op=='+') return 1;
    else if(op=='-') return 1;
    else if(op=='x') return 2;
    else if(op=='%') return 2;
} 
//求最大公约数
int gcd(int a,int b)
{
    if(b==0) return a;
    else return gcd(b,a%b);    
}  
//根据操作符计算两个操作数
Digit Calculate(Digit num1,Digit num2,char op){
    Digit res;
    int f;
    switch(op){
        case '+':{
            res.x=num1.x*num2.y+num1.y*num2.x;
            res.y=num1.y*num2.y;
            break;
        }
        case '-':{
            res.x=num1.x*num2.y-num1.y*num2.x;
            res.y=num1.y*num2.y;
            break;
        }
        case 'x':{
            res.x=num1.x*num2.x;
            res.y=num1.y*num2.y;
            break;
        }
        case '%':{
            res.x=num1.x*num2.y;
            res.y=num1.y*num2.x;
            break;
        }
        default:{
            res.x=0;
            res.y=1;
        }
    }
    if(res.x<res.y) 
        f=gcd(res.y,res.x);
    else 
        f=gcd(res.x,res.y);
    res.x=res.x/f;res.y=res.y/f;
    //如果分母为负数，取反
    if(res.y<0){
        res.x=-res.x;
        res.y=-res.y;
    } 
    return res;
} 
//随机生成1个操作数并打印 
Digit getNum()
{
    int i,j,f;
    Digit res;
    if(rand()%3==1)
    {
        i=rand()%11+1;
        j=rand()%11+1;
        if(i>j) {int temp=i;i=j;j=temp;}
        f=gcd(j,i);
        i=i/f;j=j/f;
        printf("%d/%d",i,j);
    }else{
        i=rand()%101+1;
        j=1;
        printf("%d",i);
    }
    res.x=i;
    res.y=j;
    return res;
}
//随机生成一个操作符并打印
char getOperator(){
    char op=ss[rand()%4];
    if(op=='%') 
        printf("÷");
    else if(op=='x')
        printf("×");
    else printf("%c",op);
    return op;
} 

int main()
{
    srand((unsigned)time(NULL));  //每次运行进行初始化 
    int times; //控制生成题目的个数 
    float score=100; //题目得分 
    printf("请输入题目个数(例如输入:-n 5,将生成5个题目)：");
    scanf("-n %d",&times);
    printf("本次共%d题，满分100分\n",times);
    //第一个for循环，每次生成一个题目 
    for(int j=0;j<times;j++){
        printf("%d: ",j+1);
        int t=0,q=0,p=0,top=0;
        Digit opNum[10],numStack[10];
        char op,operatorStack[10],expQueue[20];
        opNum[q++]=getNum();
        expQueue[p++]=q-1+'0';
        //加入附加功能，运算符个数随机生成
		int op_Num=rand()%5+1; 
		//附加功能，控制括号的生成
		int braket_Max=2,braket=0,flag=0;//分别代表生成括号的个数和当前左括号个数 
        //得到后缀表达式 
        for(t=0;t<op_Num;t++)
        {
            op=getOperator();  //生成运算符  
            if(t==0){
                operatorStack[top++]=op;
                 //随机决定是否生成左括号
	            if(rand()%3==1&&t<op_Num-1){
	            	printf("(");//打印括号 
	            	braket_Max--;
	            	braket++;
					operatorStack[top++]='('; //左括号入栈 
	            }
				opNum[q++]=getNum();
                expQueue[p++]=q-1+'0';
                continue;
            }
           
            //当符号栈顶不是左括号，根据优先级判断出栈 
            if(operatorStack[top-1]!='('){
            	while(mPriority(op)<=mPriority(operatorStack[top-1])&&top>0&&operatorStack[top-1]!='('){
	                top--;
	                expQueue[p++]=operatorStack[top];
	            }
            }
            operatorStack[top++]=op;
             //随机决定是否生成左括号
            if(rand()%3==1&&t<op_Num-1){
            	if(braket_Max<0) break;//如果已经生成三对括号，就不再生成 
            	printf("(");//打印括号 
            	flag=t;
            	braket_Max--;
            	braket++;
				operatorStack[top++]='('; //左括号入栈 
            }
            opNum[q++]=getNum();//产生一个随机数 
            expQueue[p++]=q-1+'0';
            //随机决定是否生成右括号
            if(flag!=t&&rand()%3==1){
            
            	if(braket<=0) break; 
            	printf(")");//打印右括号
				braket--;
				//一直出栈直到遇到左括号
				while(operatorStack[top-1]!='(') {
					top--;
					expQueue[p++]=operatorStack[top];
				}
				top--;
            }
        } 
        //如果还有左括号还未匹配
		while(braket>0){
			braket--;
			printf(")");
			while(operatorStack[top-1]!='(') {
					top--;
					expQueue[p++]=operatorStack[top];
			}
			top--;
		} 
        while(top>0){
            top--;
            expQueue[p++]=operatorStack[top];
        }
       
        //根据后缀表达式计算结果 
        top=0;
        for(int i=0;i<p;i++){
            if(expQueue[i]>='0'&&expQueue[i]<='9')
                {
                    int ch=expQueue[i]-'0';
                    numStack[top].x=opNum[ch].x;
                    numStack[top].y=opNum[ch].y;
                    top++;
                }else{
                    top--;
                    numStack[top-1]=Calculate(numStack[top-1],numStack[top],expQueue[i]);
                }
        }
        printf("=");
        //用户输入计算结果
        char userAns[100],rightAns[100];
//        printf("%d/%d\n",numStack[top-1].x,numStack[top-1].y);
        cin>>userAns;
        int c=getchar();
        //得到的正确结果 
        if(numStack[top-1].y!=1){
            sprintf(rightAns,"%d%s",numStack[top-1].x,"/");
            sprintf(rightAns,"%s%d",rightAns,numStack[top-1].y);
        }    
        else 
            sprintf(rightAns,"%d",numStack[top-1].x);
        //printf("%s\n",rightAns);
        //判断对错 
        if(strcmp(userAns,rightAns)==0)
            printf("正确！\n");
        else {
            printf("不正确！正确答案= %s\n",rightAns);
            //扣分
            score-=100*1.0/times; 
        }
        
    }
    printf("本次得分%.2f",score);    
    return 0;    
}
