/*
 * PL/0 complier program for win32 platform (implemented in C)
 *
 * The program has been test on Visual C++ 6.0, Visual C++.NET and
 * Visual C++.NET 2003, on Win98, WinNT, Win2000, WinXP and Win2003
 *
 * ʹ�÷�����
 * ���к�����PL/0Դ�����ļ�?
 * �ش��Ƿ�������������
 * �ش��Ƿ�������ֱ�
 * fa.tmp������������
 * fa1.tmp���Դ�ļ�������ж�Ӧ���׵�ַ
 * fa2.tmp�����?
 * fas.tmp������ֱ�
 */

#include <stdio.h>
#include <stdlib.h>
#include "pl0_extend.h"
#include "string.h"

#define stacksize 500
#define readlength 120

int get_the_second(char* str); 
int get_the_third(char* str);
float get_the_fourth(char* str);
void get_the_fifth(char* str, char* fifth);

int main(int argc,char** argv)
{
    // ��������forѭ������
    int iter;
    
    // �򿪴���м������ļ�
    FILE* pFile = fopen("virCode.tmp", "r");
    if(!pFile)
        return;
    // �������ڴ���ļ��ַ���������
    char str[readlength] = { 0 };
    // ���������������code���鸳��ֵΪnan, nan�����Զ�������������, �������κο�ִ�еĴ���
    for(iter=0 ; iter<cxmax; iter++){
        code[iter].f = nan;
    }
    // num��¼�����������code�е�index
    int num = 0;
    char* a;
    while ( a = fgets(str, readlength, pFile) )
    {
        // ��ȡ���������Ĳ�����
        char op[4] = "000";
        op[3] = 0;
        for(iter=0 ; iter<3 ; iter++){
            op[iter] = str[iter];
        }
        // �ж���ʲô���͵������ָ�������
        if(strcmp(op,"lit")==0){
            code[num].f = lit;
        }else if(strcmp(op,"opr")==0){
            code[num].f = opr;
        }else if(strcmp(op,"lod")==0){
            code[num].f = lod;
        }else if(strcmp(op,"sto")==0){
            code[num].f = sto;
        }else if(strcmp(op,"cal")==0){
            code[num].f = cal;
        }else if(strcmp(op,"int")==0){
            code[num].f = inte;
        }else if(strcmp(op,"jmp")==0){
            code[num].f = jmp;
        }else if(strcmp(op,"jpc")==0){
            code[num].f = jpc;
        }else if(strcmp(op,"lda")==0){
            code[num].f = lda;
        }else if(strcmp(op,"sta")==0){
            code[num].f = sta;
        }else if(strcmp(op,"lpl")==0){
            code[num].f = lpl;
        }else if(strcmp(op,"lmi")==0){
            code[num].f = lmi;
        }else if(strcmp(op,"end")==0){
            code[num].f = end;
        }else if(strcmp(op,"mov")==0){
            code[num].f = mov;
        }else{
            printf("nan\n");
        }

        // ��ȡ�ڶ�������������
        int second = 0;
        int third = 0;
        float fourth = 0;
        char fifth[strmax] = {0};
        // ��ȡ��code��
        second = get_the_second(str);
        third = get_the_third(str);
        code[num].l = second;
        code[num].a = third;

        // ֻ��oprָ������A��ֵΪ17��19ʱ���Ż��漰����3�����������ַ�������ʽ��ȡ
        if(code[num].f==opr && (code[num].a==17 || code[num].a==19)){
            get_the_fifth(str, code[num].str);
        }else{
            fourth = get_the_fourth(str);
            // ��¼��code����
            code[num].real = fourth;
        }
        num+=1;
    }
    // �ر��ļ�
    fclose(pFile);

	interpret();
    return 0;
}

// ���ڻ�ȡ��1���������ĺ���
int get_the_second(char* str){
	int ins;
	int second = 0;
		for(ins=4 ; ins<readlength ; ins++){
            // 32��ʾ�ո�
			if((int)str[ins]!=32){
				second = second*10 + (int)(str[ins])-48;
			}else{
				break;
			}
		}
	return second;
}

// ���ڻ�ȡ��2���������ĺ���
int get_the_third(char* str){
	int ins;
	int third = 0;
		for(ins=6 ; ins<readlength ; ins++){
            // 32��ʾ�ո�
			if((int)str[ins]!=32){
				third = third*10 + (int)(str[ins])-48;
			}else{
				break;
			}
		}
	return third;
}

// �Ը���������ʽ��ȡ��3������
float get_the_fourth(char* str){
    int ins;
    float fourth = 0;
    int flag = 0;	// flag=1ָʾĿǰ������С����֮�� 
    float afterDot = 0.1;	// ���ڼ���С�������� 
        for(ins=9 ; ins<readlength ; ins++){
            // 10��ʾ��β
            if((int)str[ins]!=10){
                if((int)str[ins]!=46){
                	// �����ж��Ƿ��Ѿ���С��λ 
                	if(flag==0){
                		// ��������λ 
                		fourth = fourth*10 + (int)(str[ins])-48;
					}else{
						// �Ѿ���С��λ 
						fourth = fourth + ((int)(str[ins])-48)*afterDot;
						afterDot = afterDot * 0.1;
					}
				}else{
					flag = 1;
				}
                
            }else{
                break;
            }
        }
    return fourth;
}

// ���ַ�������ʽ��ȡ��3������
void get_the_fifth(char* str, char* fifth){
    int ins;
    for(ins=0 ; ins<strmax ; ins++){
        fifth[ins] = 0;
    }
    int index = 0;
    for(ins=9 ; ins<readlength ; ins++){
        // 10��ʾ��β
        if((int)str[ins]!=10){
            fifth[index] = str[ins];
            index++;
        }else{
            break;
        }
    }
}

// read��ʽ�����봦����
int FormatRead(struct instruction* ins){
    int num = 0,iter;
    // �����ַ���str
    for(iter=0 ; iter<strmax ; iter++){
        // ����β�����˳�
        if(ins->str[iter]==0)
            break;
        // ���������%����ֱ��װ��tempstr�ݴ�
        if(ins->str[iter]!='%'){
            tempstr[index_temp] = ins->str[iter];
            index_temp++; 
        }else{
            // ��������%
            tempstr[index_temp] = '%';
            index_temp++;
            iter++;
            // ֱ�Ӱ����������Ͷ��룬֮���ٰ����ͱ���
            tempstr[index_temp] = 'f';
            // ����ʱ���������ȫ��������ٵ���ŵ�ջ��
            scanf(tempstr, &(tempfloat[num]));
            num++;
            // ����tempstr
            memset(tempstr,0,strmax);
            index_temp = 0;
        }
    }
    // ���ã��Ա��´�ʹ��
    memset(tempstr,0,strmax);
    index_temp = 0;
    return num;
}

// ��ʽ�����������
void PrintStr(struct instruction* ins){
	int iter;
    // ͬ������strÿ���ַ�
    for(iter=0 ; iter<strmax ; iter++){
        // ����β�����˳�
        if(ins->str[iter]==0)
            break;
        // ֻҪ�����Ĳ���ת���ַ�ʱ��ֱ�Ӵ���tempstr
        if(ins->str[iter]!='\\'){
            tempstr[index_temp] = ins->str[iter];
            index_temp++; 
        }else{
            // ������ת���ַ�����֮ǰ�����������ʼ��tempstr
            printf("%s",tempstr);
            memset(tempstr,0,strmax);
            index_temp = 0;
            iter++;
            // �ж���һ���ǲ���n�����ǣ��������Ǹ����з�
            if(ins->str[iter]=='n'){
                printf("\n");
            }else{  // ������n����ԭ���Ľ������
                printf("%c%c",ins->str[iter-1],ins->str[iter]);
            }
        }
    }
    // �������tempstr
    printf("%s",tempstr);
    // ����
    memset(tempstr,0,strmax);
    index_temp = 0;
}

// �����������ͺ���
void interpret()
{
	int iter,num;
    printf("start interpret:\n");
    int ins_addr, base_addr, top_addr;
    struct instruction ins;				// ָ��Ĵ���
    float s[stacksize];					// ����ջ
    memset(s,-1,stacksize);
    top_addr = 0;						// ջ���Ĵ���
    base_addr = 0;						// ��ַ�Ĵ���
    ins_addr = 0;						// ָ���ַ�Ĵ��������ָ����code[]�������λ��
    s[0] = s[1] = s[2] = 0;
    do {
        // ����ǰָ��
        ins = code[ins_addr];
        ins_addr++;
        // code�ĸ�ʽ��F L A��F�ǲ����룬L�����ò���������Ĳ��������þ�Ϊ0��A�����ָ�����ͬ
        switch (ins.f)
        {
        case lit:                                               // ��a��ֵȡ��ջ����ջ��ָ��t+1
            switch (ins.l)
            {
                case 0:     // ������A����top��ָ��Ԫ��top+1
                    //printf("����:%d\n", ins.a);
					s[top_addr] = ins.a;
					//printf("����:%d\n", ins.a);
					//printf("��ʱջ��:%d\n", s[top_addr]);
                    top_addr++;
                    break;
                case 1:     // ������F����top��ָ��Ԫ��top+1
                    s[top_addr] = ins.real;
                    //printf("��ʱջ��:%d\n", s[top_addr]);
                    top_addr++;
                    break;
            }
            
            break;
        case opr:                                               // ��ѧ���߼�����
            switch (ins.a)
            {
            case 0:                                         // ���������ù��̣����ص��õ㲢��ջ
                // ջ��λ��ָ�����ڹ��̵Ļ�ַ����Ϊ��ջ����
                top_addr = base_addr;
                // ��һ��ָ��ָ��p ָ��ǰ���̵� ���ص�ַ
                ins_addr = s[top_addr+2];
                // ��ַ ָ��ǰ���̵� ��̬������ʶ��λ��
                base_addr = s[top_addr+1];
                break;
            case 1:                                         // ��ջ��Ԫ���෴������������ջ��
                s[top_addr-1] = -s[top_addr-1];
                break;
            case 2:                                         // ��ջ����ջ����ӣ������ջ����t-1
                top_addr--;
                s[top_addr-1] = s[top_addr-1]+s[top_addr];
                break;
            case 3:                                         // ��ջ����ȥջ���������ջ����t-1
                top_addr--;
                s[top_addr-1] = s[top_addr-1]-s[top_addr];
                break;
            case 4:                                         // ��ջ�� �� ջ���������ջ����t-1
                top_addr--;
                s[top_addr-1] = s[top_addr-1]*s[top_addr];
                break;
            case 5:                                         // ��ջ�� ���� ջ���������ջ����t-1
                top_addr--;
                s[top_addr-1] = s[top_addr-1]/s[top_addr];
                break;
            case 6:                                         // ջ��������Ϊ���� ���Ϊ1��Ϊż����Ϊ0
                s[top_addr-1] = (int)(s[top_addr-1])%2;
                break;
            case 8:                                         // ��ջ����ջ��������ȣ�1���ڴ�ջ����t-1
                top_addr--;
                s[top_addr-1] = (s[top_addr-1] == s[top_addr]);
                break;
            case 9:                                         // ��ջ����ջ�����ݲ���ȣ�1���ڴ�ջ����t-1
                top_addr--;
                s[top_addr-1] = (s[top_addr-1] != s[top_addr]);
                break;
            case 10:                                        // ��ջ������С��ջ�����ݣ�1���ڴ�ջ����t-1
                top_addr--;
                s[top_addr-1] = (s[top_addr-1] < s[top_addr]);
                break;
            case 11:                                        // ��ջ������ �� ջ�����ݣ�1���ڴ�ջ����t-1
                top_addr--;
                s[top_addr-1] = (s[top_addr-1] >= s[top_addr]);
                break;
            case 12:                                        // ��ջ������ ���� ջ�����ݣ�1���ڴ�ջ����t-1
                top_addr--;
                s[top_addr-1] = (s[top_addr-1] > s[top_addr]);
                break;
            case 13:                                        // ��ջ������ �� ջ�����ݣ�1���ڴ�ջ����t-1
                top_addr--;
                s[top_addr-1] = (s[top_addr-1] <= s[top_addr]);
                break;
            case 14:                                        // ջ���������̨(�ж�float��int)��t-1
                if ((int)(s[top_addr-1]*10000)%10000==0)
                    printf("%d", (int)s[top_addr-1]);
                else
                    printf("%f", s[top_addr-1]);
                top_addr--;
                break;
            case 15:                                        // ����̨��Ļ���һ������
                printf("\n");
                break;
            case 16:                                        // �ӿ���̨����һ�����룬����ջ����t+1
                printf("\n>>>");
                scanf("%f", &(s[top_addr]));
                top_addr++;
                break;
            case 17:                                        // ����ַ���, ������ж���Ϊ�˻�ȡ�����з�
                PrintStr(&ins);
                break;
            case 18:                                         // ǿ����ĳ�ָ�ʽ���ջ��
                if((int)ins.real==0){
                    printf("%d",(int)s[top_addr-1]);
                }else if((int)ins.real==1){
                    printf("%f",(float)s[top_addr-1]);
                }else{
                    printf("����ʱ���󣡣�");
                }
                top_addr--;
                break;
            case 19:
                printf("\n>>>");
                num=FormatRead(&ins);
                // ��ʱ�ٴ��������ǰ ��ջ����, ���һ���ʱ���ľ���ջ������ֱ�ӻ�ȡ��
                for(iter=(num-1) ; iter>=0 ; iter--){
                    s[top_addr] = tempfloat[iter];
                    top_addr++;
                }
                break;
            }
            break;
        case lod:                                               // ȡ��Ե�ǰ���� ���ΪL ����ƫ����ΪA �Ĵ洢��Ԫ��ֵ��ջ��, t+1
            s[top_addr] = s[base(ins.l,s,base_addr)+ins.a];
            top_addr++;
            //printf("��ʱջ��:%d\n", s[top_addr-1]);
            break;
        case sto:                                               // ջ����ֵ�浽 ��Ե�ǰ���̲��ΪL ����ƫ����ΪA �Ĵ洢��Ԫ, t-1
            //printf("��ʱջ��:%d\n", s[top_addr-1]);
            //printf("�ж�:%d\n", ((int)ins.real)%10);
            switch ((((int)ins.real)%10)){
                case 0:
                    top_addr--;
                    s[base(ins.l, s, base_addr) + ins.a] = (int)s[top_addr];
                    break;
                case 1:
                    top_addr--;
                    s[base(ins.l, s, base_addr) + ins.a] = s[top_addr];
                    break;
            }
            break;
        case cal:                                               // �����ӹ���
            // �������̻���ַ��ջ����̬����
            s[top_addr] = base(ins.l, s, base_addr);
            // �������̻���ַ��ջ������������base��������̬����
            s[top_addr+1] = base_addr;
            // ����ǰָ��ָ����ջ �����ص�ַ��
            s[top_addr+2] = ins_addr; 
            // �ı����ַָ��ֵΪ�¹��̵Ļ���ַ
            base_addr = top_addr;
            // ��ת
            ins_addr = ins.a;
            break;
        case inte:                                              // ջ������A���洢��Ԫ
            top_addr += ins.a;
            break;
        case jmp:                                               // ֱ����ת
            ins_addr = ins.a;
            break;
        case jpc:                                               // ������ת����ջ��Ϊ0����ת��code�ĵ�ַA
            top_addr--;
            if (s[top_addr] == 0)
            {
                ins_addr = ins.a;
            }
            break;
        case lda:                                               // ȡ�����ж�Ӧλ�õ�ֵ����ǰջ����topָ�벻��
            s[top_addr-1] = s[base(ins.l, s, base_addr) + ins.a + (int)s[top_addr-1]];
            break;
        case sta:
            switch ((((int)ins.real)%10)){
                case 0:
                    top_addr--;
                    s[base(ins.l, s, base_addr) + ins.a + (int)s[top_addr-1]] = (int)s[top_addr];
                    break;
                case 1:
                    top_addr--;
                    s[base(ins.l, s, base_addr) + ins.a + (int)s[top_addr-1]] = s[top_addr];
                    break;
                default:
                    printf("sta wrong!\n");
                    break;
            top_addr--;
            }
            break;
        case lpl:           // ȡ��Ե�ǰ���� ���ΪL ����ƫ����ΪA+��ջ����ֵ �Ĵ洢��Ԫ��ֵ ���� ��ǰջ����ֵ������ջ��, t����
            s[top_addr-1] = s[base(ins.l, s, base_addr) + ins.a + (int)s[top_addr-2]] + s[top_addr-1];
            break;
        case lmi:           // ȡ��Ե�ǰ���� ���ΪL ����ƫ����ΪA+��ջ����ֵ �Ĵ洢��Ԫ��ֵ ��ȥ ��ǰջ����ֵ������ջ��, t����
            s[top_addr-1] = s[base(ins.l, s, base_addr) + ins.a + (int)s[top_addr-2]] - s[top_addr-1];
            break;
        case end:           // ȡ��Ե�ǰ���� ���ΪL ����ƫ����ΪA+��ջ����ֵ �Ĵ洢��Ԫ��ֵ ��ȥ ��ǰջ����ֵ������ջ��, t����
            printf("\nThe program exited...\n");
            break;
        case mov:           // ��ջ��A��Ԫ��������3��λ�ã�Ϊ���̵��������ڳ�λ�ã�t-A
            for(iter=ins.a ; iter>0 ; iter--){
                s[top_addr+2] = s[top_addr-1];
                top_addr--;
            }
            break;
        }
        if (ins.f == end){
            break;
        }
    } while (ins_addr != 0);
    
}

// �����ַ�ĺ���
int base(int l, float* s, int base_addr)
{
    int b1;
    b1 = base_addr;
    while (l > 0)
    {
        b1 = s[b1];
        l--;
    }
    return b1;
}

// end
