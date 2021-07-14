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

/* ����ִ��ʱʹ�õ�ջ */
#define stacksize 500

int main(int argc,char** argv)
{
	// symnumΪ48, ����48�����
    bool nxtlev[symnum];

    // ��һ���ļ�, �����ļ�ָ��: FILE���͵�ָ�����, ָ����ļ�
    Pl0_SourceCode = fopen(argv[1], "r");

    // ������ļ�������ok
    if (Pl0_SourceCode)
    {
        init();     /* ��ʼ�� */

        err = 0;    // ��������� 
        // cc��ll: getchʹ�õļ�����, cc��ʾ��ǰ�ַ�
        // cx���������ָ��, ȡֵ��Χ[0, cxmax-1]
        cc = cx = ll = 0;
        // �������л�ȡ�ַ���getch ʹ��, ��ʼ��Ϊһ���ո�
        ch = ' ';

        // �״ζ�һ���ַ�, �������block�����, ���ǵݹ����
        if(-1 != getsym())
        {
            // nxtlev������Ϊ32��bool����, declbegsys������һ��������ʼ�ķ��ż��ϣ�statbegsys������俪ʼ�ķ��ż���
            // ����nxtlev��ʹ���3��������6�����Ŀ�ʼ����, ������һ�־���true
            addset(nxtlev, declbegsys, statbegsys, symnum);
            // �ټ�һ����������� .
            nxtlev[period] = true;

            // 4������, 1: ��ǰ�ֳ������ڲ�, 2: ���ֱ�ǰβָ��, ���������Ǳ�ʶ��3��������6������bool����
            // 4: param_num�������� 
            if(-1 == block(0, 0, nxtlev, 0))   /* ���ñ������ */
            {
                fclose(Pl0_SourceCode);
                printf("\n");
                return 0;
            }

            // ������һ�����Ų���period, �ͱ�ʾû����ȷ����
            if (sym != period)
            {
                error(9);	// �����β���� . ����������
            }

            // ���ֻ��0������, �Ϳ��Ե��ý��ͳ���ʼ������
            if (err == 0)
            {
            	// ���������������ļ�
            	StoreToFile();
            }
            else // ���򱨴�, �޷��ý��ͳ���������
            {
                printf("Errors in pl/0 program");
            }
        }

        fclose(Pl0_SourceCode); // �ر�Դ�ļ�
    }
    else	// �ļ����޷�������
    {
        printf("Can't open file!\n");
    }

    printf("\n");
    // ���������ͣһ��
    // system("pause");
    return 0;
}

/*
* ��ʼ�������÷���ֵ�������ű�ʶ�����͹��ࡪ�����ͱ�ʶ
*/
void init()
{
    int i;

    /* ���õ��ַ����� */
    for (i=0; i<=255; i++)
    {
        ssym[i] = nul;      // nul = 0
    }
    for(i=0 ; i<cxmax; i++){
    	code[i].f = nan;
    }
    // ����14�� ���ַ��ķ���ֵ ��ASCII��Ϊ����
    ssym['+'] = plus;       // 3
    ssym['-'] = minus;      // 4
    ssym['*'] = times;      // 5
    ssym['/'] = slash;      // 6
    ssym['('] = lparen;     // 14
    ssym[')'] = rparen;     // 15
    ssym['='] = eql;        // 8
    ssym[','] = comma;      // 16
    ssym['.'] = period;     // 18
    ssym['#'] = neq;        // 9 ������
    ssym[';'] = semicolon;  // 17 �ֺ�
    ssym[':'] = colon;  	// 33 ð��
    ssym['['] = lbrakt;     // 39 ��������
    ssym[']'] = rbrakt;     // 40 ��������

    /* ���ñ���������,������ĸ˳��abcdefghijklmnoprstuvwxyz�������۰���� */
    // ��22�������֣�word���������֣�word�ĵ�2��[]�Ǵ�ű�������һ��������ĸ�����10��
	// ����break��else ��case��of��exit��step��until��real 
    strcpy(&(word[0][0]), "begin");
    strcpy(&(word[1][0]), "break");     // break���������־
    strcpy(&(word[2][0]), "call");
    strcpy(&(word[3][0]), "case");      // case���������־ 
    strcpy(&(word[4][0]), "const");
    strcpy(&(word[5][0]), "do");
    strcpy(&(word[6][0]), "else");      // else���������־ 
    strcpy(&(word[7][0]), "end");		
    strcpy(&(word[8][0]), "exit");		// exit���������־
    strcpy(&(word[9][0]), "for");       // for��������ı�־
    strcpy(&(word[10][0]), "if");
    strcpy(&(word[11][0]), "odd");
    strcpy(&(word[12][0]), "of");      // of�ı�־ 
    strcpy(&(word[13][0]), "procedure");
    strcpy(&(word[14][0]), "read");
    strcpy(&(word[15][0]), "real");		// ���������ı�־
    strcpy(&(word[16][0]), "step");     // step�ı�־
    strcpy(&(word[17][0]), "then");
    strcpy(&(word[18][0]), "until");    // until�ı�־
    strcpy(&(word[19][0]), "var");
    strcpy(&(word[20][0]), "while");
    strcpy(&(word[21][0]), "write");
    
    
    /* ���ñ����ַ��� */
    // ����22�������ֵķ���ֵ
    wsym[0] = beginsym;     // 20 begin
    wsym[1] = breaksym;     // 41 break����
    wsym[2] = callsym;      // 28 call
    wsym[3] = casesym;      // 45 case
    wsym[4] = constsym;     // 29 const
    wsym[5] = dosym;        // 27 do
    wsym[6] = elsesym;      // 47 else 
    wsym[7] = endsym;       // 21 end
    wsym[8] = exitsym;		// 44 exit����
    wsym[9] = forsym;       // 36 for����
    wsym[10] = ifsym;       // 22 if
    wsym[11] = oddsym;      // 7 odd
    wsym[12] = ofsym;       // 46 of
    wsym[13] = procsym;     // 31 procedure
    wsym[14] = readsym;     // 26 read
	wsym[15] = realsym;     // 34 real����
    wsym[16] = stepsym;     // 37 step
    wsym[17] = thensym;     // 23 then
    wsym[18] = untilsym;    // 38 until
    wsym[19] = varsym;      // 30 var
    wsym[20] = whilesym;    // 24 while
    wsym[21] = writesym;    // 25 write


    /* ���÷��ż� */
    // symnum = 32������32������(��ȥ��P_CODEָ��)
    for (i=0; i<symnum; i++)
    {
        declbegsys[i] = false;  // ��ʾ������ʼ�ķ��ż���
        statbegsys[i] = false;  // ��ʾ��俪ʼ�ķ��ż���
        facbegsys[i] = false;   // ��ʾ���ӿ�ʼ�ķ��ż���
    }

    // 9��������
    /* ����������ʼ���ż� */
    declbegsys[constsym] = true;    // ��������
    declbegsys[varsym] = true;      // ��������
    declbegsys[procsym] = true;     // ��������

    /* ������俪ʼ���ż� */
    statbegsys[beginsym] = true;    // �������Ŀ�ʼbegin
    statbegsys[callsym] = true;     // ���̵������
    statbegsys[ifsym] = true;       // if�������
    statbegsys[whilesym] = true;    // whileѭ�����
    statbegsys[readsym] = true;     // �����
    statbegsys[writesym] = true;    // д���
    statbegsys[forsym] = true;      // forѭ�����
    statbegsys[exitsym] = true;     // exit���
    statbegsys[casesym] = true;     // caseѡ����� 
	statbegsys[breaksym] = true;    // break��� 

    /* ��������(����ʽ�ұߵĲ���)��ʼ���ż� */
    facbegsys[ident] = true;        // ��ʶ��
    facbegsys[number] = true;       // �޷�������
    facbegsys[lparen] = true;       // �����ţ�left parenthesis ��Բ����
    facbegsys[real] = true;			// ������real
}

/*
* ������ʵ�ּ��ϵļ�������
*/
int inset(int e, bool* s)
{
    return s[e];
}

// sr: ����Ϊ32��bool����, s1: ����һ��������ʼ�ķ��ż��ϣ�s2: ������俪ʼ�ķ��ż���, n = 32
// ����sr��ʹ���3��������6�����Ŀ�ʼ����, ������һ�־���true
int addset(bool* sr, bool* s1, bool* s2, int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        sr[i] = s1[i]||s2[i];
    }
    return 0;
}

int subset(bool* sr, bool* s1, bool* s2, int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        sr[i] = s1[i]&&(!s2[i]);
    }
    return 0;
}

int mulset(bool* sr, bool* s1, bool* s2, int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        sr[i] = s1[i]&&s2[i];
    }
    return 0;
}

/*
*   ��������ӡ����λ�� �� �������n
*/
void error(int n)
{
    char space[81];     // ���81�Ͷ�ȡ�л�������һ�µ�, Ҳ��81���ַ�
    // ��ʼ��space, ���ڱ�ʾ������һ��Line����ֵ�λ��
    memset(space,32,81);    // ����81��char(ռ1���ֽ�)��λ��ȫ������32: ������Ϊ0010 0000, ��ASCII���� �ո�
    int i;
	for(i=0;i<cc-1;i++)
	{
		space[i] = line[i];
	}
    // ��ֵΪ0, ��ʾ'\0'����, ��˿ո�ĳ���(��Ϊ��һ���ո�λ����0)���ǳ����λ��(�����һ���ո����, Ϊcc-1)
    space[cc-1]=0; //����ʱ��ǰ�����Ѿ����꣬����cc-1	
    printf("****%d�е�%s��֮ǰ�ķǿ���\n����%d:",hs, space, n);
    switch(n)
    {
        case 1 :printf("����˵���в�������\":=\". \n");break;
        case 2 :printf("����˵���е�\"=\"��Ӧ��������.  \n");break;
        case 3 :printf("ȱ��\"=\". \n");break;
        case 4 :printf("const ��var ��procedure��ӦΪ��ʶ��. \n");break;
        case 5 :printf("©����\",\"������\";\". \n");break;
        case 6 :printf("����˵����ķ��Ų���ȷ(Ӧ�Ǿ��ӵĿ�ʼ��,���̶����)��\n");break;
        case 7 :printf("����˳������ӦΪ[<����˵������>][<����˵������>] [<����˵������>]<���>��\n");break;
        case 8 :printf("�������ڵ���䲿�ֵķ��Ų���ȷ��\n");break;
        case 9 :printf("�����ĩβ�����˾��\".\"��\n");break;
        case 10 :printf("����֮��©����\";\"��\n");break;
        case 11 :printf("��ʶ��δ˵��.\n");break;
        case 12 :printf("��ֵ�����ӦΪ������\n");break;
        case 13 :printf("ӦΪ \":=/+=/-=\" ֮һ��\n");break;
        case 14 :printf("call ��ӦΪ��ʶ����\n");break;
        case 15 :printf("call ���ʶ������Ӧ�ǹ��̡�\n");break;
        case 16 :printf("ȱ��\"then\"��\n");break;
        case 17 :printf("ȱ��\"end\"��\";\"��\n");break;
        case 18 :printf("do while ��ѭ�����ȱ��do��\n");break;
        case 19 :printf("����ı�Ų���ȷ��\n");break;
        case 20 :printf("ӦΪ��ϵ�������\n");break;
        case 21 :printf("���ʽ�ڵı�ʶ�����Բ������޷���ֵ�Ĺ��̡�\n");break;
        case 22 :printf("���ʽ��©�������š�\n");break;
        case 23 :printf("�Ƿ����š�\n");break;
        case 24 :printf("���ʽ�Ŀ�ʼ��Ϊ�Ƿ����ŷ��š�\n");break;
        case 25 :printf("������ĺ���ǳ�����\n");break;
        case 26 :printf("�����ڴ˲�������\n");break;
        case 27 :printf("��������ĳ���ӦΪ���������ǳ�����\n");break;
        case 28 :printf("���������ظ���\n");break;
        case 29 :printf("δ�ҵ���Ӧ��������\n");break;
        case 30 :printf("��֧�ֹ��̵��жϡ�\n");break;
        case 31 :printf("������ַ�Ͻ硣\n");break;
        case 32 :printf("��������������Ƕ������������\n");break;
        case 33 :printf("ȱ�٣���");break;
        case 34 :printf("read��Ӧ��Ϊ����ֵ��\n");break;
        case 35 :printf("read��û��ʶ�������ʶ��δ���������߱�ʶ�����Ǳ�����\n");break;
        case 36 :printf("δ֪�������͡�\n");break;
        case 37 :printf("�ַ���������\n");break;
        case 38 :printf("write���ַ�����ӦΪ���š�\n");break;
        case 39 :printf("write��ʽ���������Ӧ��\n");break;
        case 40 :printf("��ע��δ����������\n");break;
        case 41 :printf("for�� ӦΪ��ʶ����\n");break;
        case 42 :printf("ȱ��step��\n");break;
        case 43 :printf("ȱ��until��\n");break;
        case 44 :printf("ȱ��do��\n");break;
        case 45 :printf("�����С�����ǳ�������������\n");break;
        case 46 :printf("�������������Ų�������\n");break;
        case 47 :printf("������ʴ���\n");break;
        case 48 :printf("�������ȱ�������š�\n");break;
        case 49 :printf("break����ѭ������ڡ�\n");break;
        case 50 :printf("ѭ�������涨��������\n");break;
        case 51 :printf("��ʽ�������������\n");break;
        case 52 :printf("read��ȱ�������� (��\n");break;
        case 53 :printf("δ֪��ʽ�����͡�\n");break;
        case 54 :printf("read��������������ơ�\n");break;
        case 55 :printf("���̴����﷨����\n");break;
        case 56 :printf("���̵��ò�����һ�¡�\n");break;
        default :printf("�Ҳ������ִ���\n");
    }

    err++;
}


/*
*   �м�����¼д��virCode.tmp�ļ��� 
*/
void StoreToFile(){
	virCode = fopen("virCode.tmp", "w");
 	int length = sizeof(code)/sizeof(code[0]);
 	int iter;
	for(iter=0 ; iter<length ; iter++){
		switch(code[iter].f){
			case 0:
                if(code[iter].a/10>0)
				    fprintf(virCode,"lit %d %d %f\n", code[iter].l, code[iter].a, code[iter].real);
                else
                    fprintf(virCode,"lit %d %d  %f\n", code[iter].l, code[iter].a, code[iter].real);
				break;
			case 1:
				if(code[iter].a/10>0){
                    if(code[iter].a==17 || code[iter].a==19){
                        fprintf(virCode,"opr %d %d %s\n", code[iter].l, code[iter].a, code[iter].str);
                    }else{
                        fprintf(virCode,"opr %d %d %f\n", code[iter].l, code[iter].a, code[iter].real);
                    }
                }
                else
                    fprintf(virCode,"opr %d %d  %f\n", code[iter].l, code[iter].a, code[iter].real);
                break;
			case 2:
				if(code[iter].a/10>0)
                    fprintf(virCode,"lod %d %d %f\n", code[iter].l, code[iter].a, code[iter].real);
                else
                    fprintf(virCode,"lod %d %d  %f\n", code[iter].l, code[iter].a, code[iter].real);
                break;
			case 3:
				if(code[iter].a/10>0)
                    fprintf(virCode,"sto %d %d %f\n", code[iter].l, code[iter].a, code[iter].real);
                else
                    fprintf(virCode,"sto %d %d  %f\n", code[iter].l, code[iter].a, code[iter].real);
                break;
			case 4:
				if(code[iter].a/10>0)
                    fprintf(virCode,"cal %d %d %f\n", code[iter].l, code[iter].a, code[iter].real);
                else
                    fprintf(virCode,"cal %d %d  %f\n", code[iter].l, code[iter].a, code[iter].real);
                break;
			case 5:
				if(code[iter].a/10>0)
                    fprintf(virCode,"int %d %d %f\n", code[iter].l, code[iter].a, code[iter].real);
                else
                    fprintf(virCode,"int %d %d  %f\n", code[iter].l, code[iter].a, code[iter].real);
                break;
			case 6:
				if(code[iter].a/10>0)
                    fprintf(virCode,"jmp %d %d %f\n", code[iter].l, code[iter].a, code[iter].real);
                else
                    fprintf(virCode,"jmp %d %d  %f\n", code[iter].l, code[iter].a, code[iter].real);
                break;
			case 7:
				if(code[iter].a/10>0)
                    fprintf(virCode,"jpc %d %d %f\n", code[iter].l, code[iter].a, code[iter].real);
                else
                    fprintf(virCode,"jpc %d %d  %f\n", code[iter].l, code[iter].a, code[iter].real);
                break;
            case 9:
                if(code[iter].a/10>0)
                    fprintf(virCode,"lda %d %d %f\n", code[iter].l, code[iter].a, code[iter].real);
                else
                    fprintf(virCode,"lda %d %d  %f\n", code[iter].l, code[iter].a, code[iter].real);
                break;
            case 10:
                if(code[iter].a/10>0)
                    fprintf(virCode,"sta %d %d %f\n", code[iter].l, code[iter].a, code[iter].real);
                else
                    fprintf(virCode,"sta %d %d  %f\n", code[iter].l, code[iter].a, code[iter].real);
                break;
            case 11:
            	if(code[iter].a/10>0)
                    fprintf(virCode,"lpl %d %d %f\n", code[iter].l, code[iter].a, code[iter].real);
                else
                    fprintf(virCode,"lpl %d %d  %f\n", code[iter].l, code[iter].a, code[iter].real);
            	break;
            case 12:
            	if(code[iter].a/10>0)
                    fprintf(virCode,"lmi %d %d %f\n", code[iter].l, code[iter].a, code[iter].real);
                else
                    fprintf(virCode,"lmi %d %d  %f\n", code[iter].l, code[iter].a, code[iter].real);
            	break;
            case 13:
                fprintf(virCode,"end %d %d  %f\n", code[iter].l, code[iter].a, code[iter].real);
            	break;
            case 14:
                if(code[iter].a/10>0)
                    fprintf(virCode,"mov %d %d %f\n", code[iter].l, code[iter].a, code[iter].real);
                else
                    fprintf(virCode,"mov %d %d  %f\n", code[iter].l, code[iter].a, code[iter].real);
            	break;
			default:
				break;
		}
	}
    fclose(virCode);
}

/*
*  ��ӡ���ֱ� 
*/
void print_nameTable(int tx0, int tx){
    printf("TABLE:\n");
    int i;
    if (tx0+1 > tx)
    {
        printf("    NULL\n");
    }
    printf("number \tkind \tname \tval/lev \taddr \ttype \tsize");
    for (i=1; i<=tx; i++)
    {
        switch (table[i].kind)
        {
        case constant:
            printf("\n%d \tconst \t%s \t", i, table[i].name);
            printf("val=%d \taddr=%d \ttype=%c", table[i].val, table[i].adr, table[i].type);
            break;
        case variable:
            printf("\n%d \tvar \t%s \t", i, table[i].name);
            printf("lev=%d \taddr=%d \ttype=%c", table[i].level, table[i].adr, table[i].type);
            break;
        case procedur:
            printf("\n%d \tproc \t%s \t", i, table[i].name);
            printf("lev=%d \taddr=%d \tsize=%d", table[i].level, table[i].adr, table[i].size);
            break;
        case array:
            printf("\n%d \tarray \t%s \t", i, table[i].name);
            printf("lev=%d \taddr=%d \ttype=%c \tsize=%d", table[i].level, table[i].adr, table[i].type, table[i].size);
            break;
        }
    }
    printf("\n\n");
}

/*
* ©���ո񣬶�ȡһ���ַ���
*
* ÿ�ζ�һ�У�����line��������line��getsymȡ�պ��ٶ�һ��
*
* ������getsym���á�
*/
int getch()
{
    // ll��cc ����ʼ��Ϊ0, cc��ʾ��ǰ�ַ�(ch)��λ��
    // ��������, ˵���ϴδ����ж�����һ���ַ�(��line��)�Ѿ���������, �͵��ٶ�һ�г��� 
    if (cc == ll)
    {
        // feof: ������ϵ��ļ��Ƿ��ѽ������������ɶҲû�У��򷵻ط�0ֵ�������ж���(����EOF), ����0
        // Pl0_SourceCode: Դ�����ļ���ָ��
        if (feof(Pl0_SourceCode))
        {
            printf("program incomplete");
            return -1;
        }
        // ���¸�ֵΪ0
        ll=0;
        cc=0;
        hs++;   /* ��ȡ��һ�У�����+1 */ 
        // printf("%d ", cx);          // cx, ���������ָ��(��ʵ�Ǹ�����, ��ʾ���ǵڼ������������)
        ch = ' ';               // �ո�, ��ӦASCII 32
        while (ch != 10)        // �տ�ʼ32�϶�������10, �����ѭ��, 10�ǻ��з�line feed(��˼�ǰ����ж���)
        {
            // �����EOF�ļ�������, ��line�и�λ��Ϊ'\0'��β
            if (EOF == fscanf(Pl0_SourceCode,"%c", &ch))   // �����ж�ȡһ��%c�������ַ���ch
            {
                line[ll] = 0;
                break;
            }
            // ��������ע�� 
            if(ch=='/' && ll!=0 && line[ll-1]=='/'){
                ll--;
                // ��������Ŀ϶�Ҳ������10��ֻΪ�˰�ʣ�µĶ���
                while (ch != 10){
                	// EOF����˼�ǣ���ȡ���󣬼���һ��û�������� 
                    if (EOF == fscanf(Pl0_SourceCode,"%c", &ch))   // �����ж�ȡһ��%c�������ַ���ch
		            {
		            	// �������ǿ�����line[0]��λ��Ϊ0��������ע�� 
		                line[ll] = 0;
		                break;
		            }
                }
                break;
            }else if(ch=='*' && ll!=0 && line[ll-1]=='/'){			// �����ǿ�ע�� 
            	ll--;
            	char tempch;			// tempch��Ϊ��ǰch��ǰһ��ֵ 
            	fscanf(Pl0_SourceCode,"%c", &ch);
            	// ���� */ Ϊֹ 
            	do{
            		tempch = ch; 
            		if (EOF == fscanf(Pl0_SourceCode,"%c", &ch))   // �����ж�ȡһ��%c�������ַ���ch
		            {
		                line[ll] = 0;
		                break;
		            }
				}while(!(ch == '/' && tempch=='*')); 
                break;
			} 
            //end richard
            line[ll] = ch;
            ll++;
        }
    }
    // line: ��ȡ�л�����(����Դ�����е�һ��), ���81��char�ַ�(����'\0')
    ch = line[cc];  // �ѵ�ǰccָ��Ķ�ȡ�л������е��ַ���ch
    cc++;           // cc ��1, ָ����һ��λ��, �Ա��´ζ�ȡ
    // ���ll=0��������ע�ͻ��ע�����ڵ�һ��λ�ã������� chҲ����������Ȼ���� 
	if(ll==0){
    	ch = ' ';
    	cc=0;
	}
    return 0;
}

/*
* �ʷ���������ȡһ������(��)
*/
int getsym()
{
    int i,j,k;

    /* the original version lacks "\r", thanks to foolevery */
    while (ch==' ' || ch==10 || ch==13 || ch==9)  /* ���Կո�10:���С�13:�س�(Carriage Return)��9:TAB(ˮƽ�Ʊ��) */
    {
        getchdo; // ץȡһ���ַ���ch
    }
    /* ����(��ʶ��)��������a..z��ͷ */
    if (ch>='a' && ch<='z')
    {           
        k = 0; // ��ʾ��ǰ������Ѿ��ж��ٸ��ַ���
        do {
            if(k<al) // al: ���ŵ���󳤶�, Ϊ10
            {
                // a: ������ʱ��ȡ��һ����(����)
                a[k] = ch;
                k++;
            }
            // else: ������滹����ĸ����, kȴ�Ѿ�>=al, �������(Ŀǰ�����Ϊ)
            getchdo;    // ��ȡһ���ַ�
            //printf("\n-%c ",ch);
        } while (ch>='a' && ch<='z' || ch>='0' && ch<='9');
        a[k] = 0;       // �����'\0'��β
        strcpy(id, a);  // ��ʱcopy��id��, id: ��ŵ�ǰ��ʶ��
        i = 0;
        j = norw-1;     // norw: �ؼ��ָ���, 13��
        do {    /* ������ǰ�����Ƿ�Ϊ������, �۰���� */
            k = (i+j)/2; 
            if (strcmp(id,word[k]) <= 0) // ��id<word[k]ʱ������Ϊ����(��ASCIIֵ��С�Ƚ�)
            {
                j = k - 1;
            }
            if (strcmp(id,word[k]) >= 0) // ��id>word[k]ʱ������Ϊ����
            {
                i = k + 1;
            }
        } while (i <= j); 
        // ���i��j������2, ˵���Ǳ�����
        if (i-1 > j)
        {
            // sym��¼Ϊ��Ӧ�����ֵı�ʶ
            sym = wsym[k]; // sym, ��ŵ�ǰ���ŵı�ʶ(����)
        }
        else // ������
        {
            // sym��¼Ϊ��Ӧ��ʶ��(ident)�ı�ʶ, Ϊident=1
            sym = ident; /* ����ʧ�ܣ������� */
        }
    }
    else // ������ĸ��ͷ, ��ֻ�������ֻ����
    {
        if (ch>='0' && ch<='9')
        {           /* ����Ƿ�Ϊ���֣���0..9��ͷ */
            k = 0;          // ��ʾ����������м�λ��
            num = 0;        // ��¼������
            sym = number;   // ���ֵı�ʶ, Ϊ2
            do {
                num = 10*num + ch - '0';    // �ۼ�
                k++;
                getchdo;    // �ٶ�һ��, �����������˳�ѭ��
            } while (ch>='0' && ch<='9'); /* ��ȡ���ֵ�ֵ */
            
            // �����ж��ǲ��Ǹ�����
            if (ch=='.'){
            	// �������ʾ�Ǹ�����
            	sym = real;
				getchdo;
				float temp = 0.1;
				float_ = (float)num;
				while(ch>='0'&& ch<='9'){
					float_ = float_ + temp*(ch - '0'); //float_��¼�ø����� 
					temp = temp*0.1;
					k++;
					getchdo;
				}
            }
            // k--; ɾ����������
            if (k > nmax)   // nmax: number�����λ��, Ϊ14
            {
                error(30);  // �������Ϊ30, ���ֹ���!!!
            }
        }
        else // �Ƿ���, �����ǵ��ַ�, Ҳ������˫�ַ�
        {
            if (ch == ':')      /* ��⸳ֵ���� */
            {
                getchdo;  // ��ȡһ���ַ�
                if (ch == '=')  // �����=, ˵�������� := ��ֵ
                {
                    sym = becomes;  // ��ֵ���ŵı�ʶ, Ϊ19
                    getchdo;    // �ٶ�һ��, �´�ֱ�Ӽ��, �������ͬ��
                }
                else
                {
                    sym = colon;  /* Ϊð�� */
                }
            }
            else    // �Ǹ�ֵ����
            {
                if (ch == '<')      /* ���С�ڻ�С�ڵ��ڷ��� */
                {
                    getchdo;
                    if (ch == '=')
                    {
                        sym = leq;  // С�ڵ��ڷ��ű�ʶΪ11
                        getchdo;    // �ٶ�һ��, �´�ֱ�Ӽ��, �������ͬ��
                    }
                    else
                    {
                        sym = lss;  // С�ڷ��ű�ʶΪ10
                    }
                }
                else    // ����С�ڿ�ͷ�ķ���
                {
                    if (ch=='>')        /* �����ڻ���ڵ��ڷ��� */
                    {
                        getchdo;
                        if (ch == '=')
                        {
                            sym = geq;  // ���ڵ��ڷ��ű�ʶΪ13
                            getchdo;    // �ٶ�һ��, �´�ֱ�Ӽ��, �������ͬ��
                        }
                        else
                        {
                            sym = gtr;  // ����, ��ʶΪ12
                        }
                    }
                    else
                    {
                        // ����ַ���
                        if (ch=='"')
                        {
                            for(k=0 ; k<strmax ; k++){
                                str_[k] = '\0';
                            }
                            k=0;
                            sym = str;
                            getchdo;
                            while(ch!='"')
                            {
                                str_[k] = ch;
                                k++;
                                getchdo;
                                if(k>(strmax-1)){
                                    break;
                                }
                            }
                            getchdo;
                            if(k>(strmax-1)){
                                error(37);  /* �ַ������� */
                            }
                        }
                        else
                        {
                        	if(ch == '+')   /* ���+/+=���� */
                        	{
                        		getchdo;
		                        if (ch == '=')
		                        {
		                            sym = pluseq;  // +=���ű�ʶΪ42
		                            getchdo;    // �ٶ�һ��, �´�ֱ�Ӽ��, �������ͬ��
		                        }
		                        else
		                        {
		                            sym = plus;  // +, ��ʶΪ3
		                        }
                        	}
                        	else
                        	{
                        		if(ch=='-')  /* ���-/-=���� */
                        		{
                        			getchdo;
			                        if (ch == '=')
			                        {
			                            sym = minuseq;  // -=���ű�ʶΪ43
			                            getchdo;    // �ٶ�һ��, �´�ֱ�Ӽ��, �������ͬ��
			                        }
			                        else
			                        {
			                            sym = minus;  // -, ��ʶΪ4
			                        }
                        		}
                        		else    // ʣ�µ�ֻ���ǵ��ַ�
		                        {
		                            // �����ʼ��ʱ�иõ��ַ�, ��ֵ��Ӧ��ʶ, ����Ϊ��ʼ��ʱ��nul��ʶ(0,��unknown)
		                            sym = ssym[ch];     /* �����Ų�������������ʱ��ȫ�����յ��ַ����Ŵ��� */
		                            //getchdo;
		                            //richard
		                            if (sym != period)  // ֻҪ����������������һ��������(.),�򶼵��ٶ�һ���Թ��´�ʹ��
		                            {
		                                getchdo;
		                            }
		                            //end richard
		                        }
		                    } 
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/*
* �������������
*
* x: instruction.f;
* y: instruction.l;
* z: instruction.a;
*/
int gen(enum fct x, int y, int z , float real)
{
    if (cx >= cxmax)
    {
        printf("Program too long"); /* ������� */
        return -1;
    }
    code[cx].f = x;
    code[cx].l = y;
    code[cx].a = z;
    code[cx].real = real;
//    if ((int)(real*10)%10==0)
//        printf("%d", code[cx].real);
//    else{
//    	printf("���룺%d", code[cx].f);
//		printf("%f--", code[cx].real);
//	} 
        
    cx++;
    return 0;
}


/*
* ���Ե�ǰ�����Ƿ�Ϸ�
*
* ��ĳһ���֣���һ����䣬һ�����ʽ����Ҫ����ʱʱ����ϣ����һ����������ĳ��?
* ���ò��ֵĺ�����ţ���test���������⣬���Ҹ��𵱼�ⲻͨ��ʱ�Ĳ��ȴ�ʩ��
* ��������Ҫ���ʱָ����ǰ��Ҫ�ķ��ż��ϺͲ����õļ��ϣ���֮ǰδ��ɲ��ֵĺ��
* ���ţ����Լ���ⲻͨ��ʱ�Ĵ���š�
*
* s1:   ������Ҫ�ķ���
* s2:   �������������Ҫ�ģ�����Ҫһ�������õļ�?
* n:    �����
*/
int test(bool* s1, bool* s2, int n)
{
    if (!inset(sym, s1))
    {
        error(n);
        /* ����ⲻͨ��ʱ����ͣ��ȡ���ţ�ֱ����������Ҫ�ļ��ϻ򲹾ȵļ��� */
        // �����ų����в�����FIRST��FOLLOW�ķ���
        while ((!inset(sym,s1)) && (!inset(sym,s2)))
        {
            getsymdo;
        }
        // *************************����Ҫ�Ӷ��������������FIRST����FOLLOW
        // �����FOLLOW��þ䵱��������getsymdo��ȡ��һ������ʼ�µķ���
        // �����FIRST�������ٶ���ֱ�ӿ�ʼ��һ�ֵķ���
    }
    return 0;
}

/*
* ���������?
*
* lev:    ��ǰ�ֳ������ڲ�
* tx:     ���ֱ�ǰβָ��
* fsys:   ��main������block�ǣ�3������ + 6�����Ŀ�ʼ���� + .���������ǵ�ǰģ���FIRST���ϣ�
* param_num: ���β������� 
*/
int block(int lev, int tx, bool* fsys, int thisParamNum)
{
    int i;
	
    int dx;                 /* ���ַ��䵽����Ե�ַ */
    int tx0;                /* ������ʼtx, tx�����ֱ������һ�����ֵ�ָ��, ���ǿ�λ��ָ�� */
    // ���� cx�����������ָ�룬ָ����ǿ�λ��
    bool nxtlev[symnum];    /* �¼�������FIRST���� */

    dx = 3 + thisParamNum;
    tx0 = tx;               /* ��¼�������ֵĳ�ʼλ�� */
    // table���λ�ò������������֣�����main����procedure�������ڴ��jmp��code���λ�ã��������������˺󣬲źøģ�Ȼ���ٸĳ�inte��λ��
    table[tx-thisParamNum].adr = cx;
    // ���Ⱦ�������������תjmp 0 0, Ϊ��������������������봦��������
    gendo(jmp, 0, 0, 0);

    if (lev > levmax)
    {
        error(32);
    }

    do {

        if (sym == constsym)    /* �յ������������ţ���ʼ���������� */
        {
            getsymdo;

            /* the original do...while(sym == ident) is problematic, thanks to calculous */
            /* do { */
            constdeclarationdo(&tx, lev, &dx);  /* dx��ֵ�ᱻconstdeclaration�ı䣬ʹ��ָ�� */
            while (sym == comma)
            {
                getsymdo;
                constdeclarationdo(&tx, lev, &dx);
            }
            if (sym == semicolon)
            {
                getsymdo;
            }
            else
            {
                error(5);   /*©���˶��Ż��߷ֺ�*/
            }
            /* } while (sym == ident); */
        }
		
        if (sym == varsym)      /* �յ������������ţ���ʼ����������� */
        {
            getsymdo;

            /* the original do...while(sym == ident) is problematic, thanks to calculous */
            /* do {  */
            int tx_cp = tx+1;	// ���ڱ���ÿ��var 
            int dx_cp = dx;		// ����array��addr���ٸ�ֵ 
            vardeclarationdo(&tx, lev, &dx);
            while (sym == comma)
            {
                getsymdo;
                vardeclarationdo(&tx, lev, &dx);
            }
            // �ж�����real�����־
            if (sym == colon)
			{
				getsymdo;
				if (sym == realsym)
				{
					for(i=tx_cp ; i<=tx ; i++){
						table[i].type = 'f';
					}
					getsymdo;
				}
				else
				{
					error(36);	// δ֪��������
				}
			}
			// ���û��ָ������Ĭ����integer
			else
			{
				for(i=tx_cp ; i<=tx ; i++){
					table[i].type = 'i';
				}
			}

            if (sym == lbrakt)  /* ��ʶ��֮����'[', ��ʶ��Ϊ���� */
            {
                getsymdo;
                // ����������ҳ����ٵĿռ��С
                arraydeclarationdo(&tx, lev, &dx);
                // ȫ���޸ĳ�array����, ��¼array��size, ��������ջ�￪�ٵĿռ�
                for(i=tx_cp ; i<=tx ; i++){
                    table[i].kind = array;
                    table[i].size = arrSize;
                    table[i].adr = dx_cp;
					dx_cp += table[i].size;
                    dx += (table[i].size - 1);
                }
            }

            if (sym == semicolon)
            {
                getsymdo;
            }
            else
            {
                error(5);	// ©���˷ֺ�
            }
            /* } while (sym == ident);  */
        }
		
        while (sym == procsym) /* �յ������������ţ���ʼ����������� */
        {
            getsymdo;
			
            if (sym == ident)
            {
                enter(procedur, &tx, lev, &dx); /* ��¼�������� */
                getsymdo;
            }
            else
            {
                error(4);   /* procedure��ӦΪ��ʶ�� */
            }
            // �ݴ�dx
            save_dx = dx;
            // dx���3, ʹ�ô��α������ӹ�����dx=3��λ�ÿ�ʼ��¼
            dx = 3;
            int tx_cp = tx+1;
            if (sym==lparen){
            	param_num = 0;
				do{
            		getsymdo;
            		if(sym==ident){
            			vardeclarationdo(&tx, lev+1, &dx); 
            			param_num++;
            		}
            	}while(sym == comma);

            	if (sym==rparen)
            	{
            		getsymdo;
            	}else{
            		error(55);		// ���̴����﷨����
            	}
            	// ��ʱĬ��Ϊ������ 
            	for(i=tx_cp ; i<=tx ; i++){
					table[i].type = 'f';
				}
				// ��¼���̲���������val
				table[tx_cp-1].val = param_num;
            }
            // ��ֵ����
            dx = save_dx;
            
            // procedure�� ��ʶ����Ӧ���и� ;
            if (sym == semicolon)
            {
                getsymdo;
            }
            else
            {
                error(5);   /* ©���˷ֺ� */
            }

            // fsys: ��ǰģ���FIRST����
            memcpy(nxtlev, fsys, sizeof(bool)*symnum);
            // nxtlev�²��FIRST���ϣ��̳�����һ��� 3������ + 6�����Ŀ�ʼ���� + .�����������һ����
            nxtlev[semicolon] = true;                             // ***********************�������Ӧ�ô�����FIRST
            if (-1 == block(lev+1, tx, nxtlev, param_num))
            {
                return -1;  /* �ݹ���� */
            }
            // txҲ�ع鱾���
			tx = tx_cp-1; 
            // ����һ������󣬿�end���ǲ��� ; ��β
            if(sym == semicolon)
            {
                getsymdo;
                // <�ֳ���> ::= [<����˵��>][<����˵��>][<����˵��>]<���>  ע�������<���>ֻ����һ��
                // ������˵������������7��<���>���������ٶ���һ��procedure
                memcpy(nxtlev, statbegsys, sizeof(bool)*symnum);
                nxtlev[ident] = true;
                // �����������˼��: �����Լ��������ڶ���procedure
                nxtlev[procsym] = true;
				nxtlev[constsym] = true;
                // �������õĴ��룬�����Ƕ�<�ֳ���>��������<���>���ֽ���test
                // fsys�Ǵ˲��FIRST��3������ + 6�����Ŀ�ʼ���� + .������
                bool xia[symnum]; 
                xia[semicolon] = true;
                xia[period] = true;
                testdo(nxtlev, xia, 6);                          // ************************�����nxtlevû���⣬FOLLOW��Ӧ����fsys����;��.
            }
            else
            {
                error(5);   /* ©���˷ֺ� */
            }
        }
        // ��������û��<����˵��>������ֱ�ӵ����λ�ã���Ȼ��<����˵��>Ҳ�ᵽ��
        // statbegsys: ��begin��call��if��while��read��write��ռ��7��������6�֣�����<��ֵ���>
        //memcpy(nxtlev, statbegsys, sizeof(bool)*symnum);
        // �ټ��ϸ�ֵ����ident��ͷ
        //nxtlev[ident] = true;
        // declbegsys: ������ʼ���ż�, ��const��var��procedure
        //testdo(nxtlev, declbegsys, 7);                            // nxtlev����***********������ĺ�����ż�FOLLOW ���Բ�Ӧ�����κ�<����>

        // ************�����2��֮�ϵĵط�������ȫ�������ˣ���������Ͳ���ѭ��
    } while (inset(sym, declbegsys));   /* ֱ��û���������� */
	
    // �����table[tx0].adr����jmp��code���λ�ã����޸�jmp��a��ʹֱ֮�������������, ��inte���봦
    code[table[tx0].adr].a = cx;    /* ��ʼ���ɵ�ǰ���̴��� */
    // ����table[tx0] ��ʵ�ǳ���procedure�������������ֱ��е�ʵ��, adr��ֱ��ָ��inte��code�е�λ��, ��Ȼ��ʱcx��ָ��Ŀգ�����������վͻ���inte
    table[tx0-thisParamNum].adr = cx;            /* ��ǰ���̴����ַ */
    // size����dx: 3+����������
    table[tx0-thisParamNum].size = dx;           /* ����������ÿ����һ�����������dx����1�����������Ѿ�������dx���ǵ�ǰ�������ݵ�size */
    // ��ջ������ dx ���洢��Ԫ
    gendo(inte, 0, dx, 0);             /* ���ɷ����ڴ���룬dx����size��С */
    printf("lev=%d, tx0=%d, tx=%d\n",lev, tx0, tx);
    print_nameTable(tx0, tx);
	
    // ����FIRSTΪfsys: 3������ + 6�����Ŀ�ʼ���� + .������
    // **********************���nxtlev��<���>��FIRST�����Բ��ԣ�ֻ����7����䣬��˿���fsys����������Ҫ
    memcpy(nxtlev, fsys, sizeof(bool)*symnum);  /* ÿ��������ż��Ͷ������ϲ������ż��ͣ��Ա㲹�� */
    // ����FIRST���ϷֺŻ�end
    nxtlev[semicolon] = true;       // *******************��Ӧ����ʲô;��end
    nxtlev[endsym] = true;
    // �����statement������䴦��
    statementdo(nxtlev, &tx, lev);
    // opr 0 0���������ù��̣����ص��õ㲢��ջ
    gendo(opr, 0, 0, 0);                       /* ÿ�����̳��ڶ�Ҫʹ�õ��ͷ����ݶ�ָ�� */
    memset(nxtlev, 0, sizeof(bool)*symnum); /*�ֳ���û�в��ȼ��� */
    // ����test�ľ�����û��period . ����
    // *************************�����FIRSTҲ��Ӧ����fsys, ��ֻ���� . ��Ϊ<���>ֻ��һ��, nxtlev��FOLLOW�����ڲ��ȣ���û������
    testdo(fsys, nxtlev, 8);
    return 0;
}

/*
* �����ֱ��м���һ��
*
* k:      ��������const,var or procedure
* ptx:    ���ֱ�βָ���ָ�룬Ϊ�˿��Ըı����ֱ�βָ���ֵ
* lev:    �������ڵĲ��,���Ժ����е�lev��������
* pdx:    dxΪ��ǰӦ����ı�������Ե�ַ�������Ҫ����1
*/
void enter(enum object k, int* ptx, int lev, int* pdx)
{
    // ����Ҫ++ ����������tx����ָ���λ��
    (*ptx)++;
    strcpy(table[(*ptx)].name, id); /* ȫ�ֱ���id���Ѵ��е�ǰ���ֵ����� */
    table[(*ptx)].kind = k;
    switch (k)
    {
    case constant:  /* �������� */
        if (num > amax)
        {
            error(31);  /* ��Խ�� */
            num = 0;
        }
        if(table[(*ptx)].type=='i'){
            table[(*ptx)].val = num;
        }else if(table[(*ptx)].type=='f'){
            table[(*ptx)].valf = float_;
        }
        
        break;
    case variable:  /* �������� */
        table[(*ptx)].level = lev;
        //printf("������ˣ� tx=%d, dx=%d\n",(*ptx),(*pdx));
        table[(*ptx)].adr = (*pdx);
        (*pdx)++;
        break;
    case procedur:  /*���������֡�*/
        table[(*ptx)].level = lev;
        break;
    }
}

/*
* �������ֵ�λ��.
* �ҵ��򷵻������ֱ��е�λ��,���򷵻�0.
*
* idt:    Ҫ���ҵ�����
* tx:     ��ǰ���ֱ�βָ��
*/
int position(char* idt, int tx)
{
    int i;
    strcpy(table[0].name, idt);
    i = tx;
//    printf("------------------------------------%s,%s\n",(table[i].name),idt);
    
    while (strcmp(table[i].name, idt) != 0)
    {
        i--;
//		printf("------------------------------------%s,%s\n",(table[i].name),idt);
    }
    return i;
}

// ��������������
int arraydeclaration(int* ptx, int lev, int* pdx)
{
    int i;
    // ����ǲ���const, �������ڱ�����const��number
    if (sym == ident)
    {
        i = position(id, *ptx);
        if (i == 0)
        {
            error(11);  /* ����δ�ҵ� */
        }
        else
        {
            if (table[i].kind == constant){
                arrSize = table[i].val;
            }else{
                error(45);      // ����: �����С�����ǳ�����������
            }
        }
    }
    // ����Ƿ���integer��������
    else if(sym == number)
    {
        arrSize = num;
    }
    else
    {
        error(45);      // ����: �����С�����ǳ�����������
    }

    getsymdo;
    if (sym != rbrakt)
    {
        error(46);      // �������������Ų�����
    }else{
        getsymdo;
    }
    return 0;
}

/*
* ������������
*/
int constdeclaration(int* ptx, int lev, int* pdx)
{
    if (sym == ident)
    {
        getsymdo;
        if (sym==eql || sym==becomes)
        {
            if (sym == becomes)
            {
                error(1);   /* ��=д����:= */
            }
            getsymdo;
            if (sym == number || sym == real)
            {
                if(sym==number){
                    table[(*ptx+1)].type = 'i';
                }else if(sym==real){
                    table[(*ptx+1)].type = 'f';
                }
                enter(constant, ptx, lev, pdx);
                getsymdo;
            }
            else
            {
                error(2);   /* ����˵��=��Ӧ������ */
            }
        }
        else
        {
            error(3);   /* ����˵����ʶ��Ӧ��= */
        }
    }
    else
    {
        error(4);   /* const��Ӧ�Ǳ�ʶ */
    }
    return 0;
}

/*
* ������������
*/
int vardeclaration(int* ptx,int lev,int* pdx)
{
	// ���ȼ���ǲ��Ǳ�ʶ��
    if (sym == ident)
    {
        enter(variable, ptx, lev, pdx); // ��д���ֱ�, variable��ʾ�Ǳ���var
        getsymdo;
    }
    // ������Ǿͱ���
    else
    {
        error(4);   /* var��Ӧ�Ǳ�ʶ */
    }
    return 0;
}

/*
* ��䴦��
*/
int statement(bool* fsys, int* ptx, int lev)
{
    // k,j����forѭ��, numOfFormat��¼��ʽ������ĸ���, perSegIndex�Ǹ����ֶ��ַ���index
    int i, cx1, cx2,cx3,k,j,perSegIndex,ptx2;  
    struct instruction which;   // ר����for��step�ж�
    bool nxtlev[symnum];

    if (sym == ident)   /* ׼�����ո�ֵ��䴦�� */
    {
        // Ѱ�������ʶ�������ֱ��е�λ��
        i = position(id, *ptx);
        if (i == 0)
        {
            error(11);  /* ����δ�ҵ� */
        }
        // �ҵ������
        else
        {
        	// ˵��������array
            if(table[i].kind != variable)
            {
                getsymdo;
                // ����Ƿ���array
                if (sym == lbrakt) /* �����������ڵı��ʽ */
                {
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool)*symnum);
                    nxtlev[rbrakt] = true;
                    expressiondo(nxtlev, ptx, lev);
                    if (sym != rbrakt)
                    {
                        error(48);  // �������ȱ��������
                    }
                    else
                    {
                        getsymdo;
                        // ��ֵ/+=/-=����
                        if(sym == becomes || sym == pluseq || sym == minuseq)
                        {
                        	sym_op = sym;
                        	getsymdo;
                        }
                        else{
                        	error(13);  /* û�м�⵽��ֵ/+=/-=���� */
                        }
                        memcpy(nxtlev, fsys, sizeof(bool)*symnum);
                        expressiondo(nxtlev, ptx, lev); /* ����ֵ�����Ҳ���ʽ */

                    	// �����+=/-=���һ������
                    	if(sym_op == pluseq){
                        	// ȡ��Ե�ǰ���� ���ΪL ����ƫ����ΪA+��ջ����ֵ �Ĵ洢��Ԫ��ֵ ���� ��ǰջ����ֵ������ջ��, t����
                        	gendo(lpl, lev-table[i].level, table[i].adr, 0);
                        }
                        else if(sym_op == minuseq){
                        	// ȡ��Ե�ǰ���� ���ΪL ����ƫ����ΪA+��ջ����ֵ �Ĵ洢��Ԫ��ֵ ��ȥ ��ǰջ����ֵ������ջ��, t����
                        	gendo(lmi, lev-table[i].level, table[i].adr, 0);
                        }
                        // �ж���ʲô��ʽ�洢
                        if(table[i].type=='i'){
                            // ջ����ֵ(������)�浽 ��Ե�ǰ���̲��ΪL ����ƫ����ΪA+��ջ����ֵ �Ĵ洢��Ԫ, t-2
                            gendo(sta, lev-table[i].level, table[i].adr, 0);   // �����ʹ��ȥ��top-2
                        }
                        else if(table[i].type=='f'){
                            // ջ����ֵ(��������)�浽 ��Ե�ǰ���̲��ΪL ����ƫ����ΪA+��ջ����ֵ �Ĵ洢��Ԫ, t-2
                            gendo(sta, lev-table[i].level, table[i].adr, 1);   // �����������ȥ��top-2
                        }else{
                            printf("wrong!!!\n");
                        }
                    }
                }
                else
                {
                    error(12);  /* ��ֵ����ʽ���� */
                    i = 0;
                }
            }
            // ��ͨ��variable
            else
            {
                getsymdo;
                if(sym == becomes || sym == pluseq || sym == minuseq)
                {
                	sym_op = sym;
                    getsymdo;
                }
                else
                {
                    error(13);  /* û�м�⵽��ֵ/+=/-=���� */
                }

                memcpy(nxtlev, fsys, sizeof(bool)*symnum);
                expressiondo(nxtlev, ptx, lev); /* ����ֵ�����Ҳ���ʽ */

                // �����+=/-=����Ҫ�ȼ�������
                if(sym_op == pluseq || sym_op == minuseq){
                	// ������ȡ��ջ��
                	gendo(lod, lev-table[i].level, table[i].adr, 0);
                	if(sym_op == pluseq){
	                	// ��ջ������ջ��, ��ʱջ�����Ǽ�������
	                	gendo(opr, 0, 2, 0);
	                }
	                else if(sym_op == minuseq){
	                	// ��ջ����ȥջ��������������Ҫ�������෴��
	                	gendo(opr, 0, 3, 0);
	                	// ȡ��, ��ʱջ�����Ǽ�������
	                	gendo(opr, 0, 1, 0);
	                }
                }
                
                // �ж���ʲô��ʽ�洢
                if(table[i].type=='i'){
                    // �����ʹ������
                    gendo(sto, lev-table[i].level, table[i].adr, 0);
                }
                else if(table[i].type=='f'){
                	// ���������������
                    gendo(sto, lev-table[i].level, table[i].adr, 1);
                }else{
                    printf("wrong!!!\n");
                }
            }
        }//if (i == 0)
    }
    else
    {
        if (sym == readsym) /* ׼������read��䴦�� */
        {
            getsymdo;
            if (sym != lparen)
            {
                error(52);  /* ��ʽ����Ӧ�������� */
            }
            else
            {
            	getsymdo;
            	// �����str�����Ǹ�ʽ������
            	// �ֱ��¼read���ID���͸�ʽ������ĸ���
            	ReadNum = 0;
            	ReadFormat = 0;
            	if (sym == str)
                {
                    for(k=0 ; k<strmax ; k++){
                        if(str_[k]=='\0'){ break;}
                        // Ԥ���� �����������
                        code[cx].str[k] = str_[k];
                        // ����Ǹ�ʽ������
                        if(str_[k]=='%'){
                            k++;	// �ٶ�һλ
                            code[cx].str[k] = str_[k];
                            switch(str_[k]){
                                case 'd':
                                	// do nothing
                                    break;
                                case 'f':
                                	// do nothing
                                    break;
                                default:
                                	error(53);			// δ֪��ʽ������
                                	break;
                            }
                            ReadFormat++;	// ��ʽ���ַ���1
                        }
                    }
                    code[cx].str[k] = 0;
                    gendo(opr, ReadFormat, 19, 0);  // ��Ҫ����벢�ŵ�ջ��

                    getsymdo;       // ��������
                    if (sym != comma){
                    	error(5);			// ©���˶���
                    }
                }
                // ����ԭ�������뷽ʽ
                do {
                    if(sym==comma){
                    	getsymdo;
					}
                    if (sym == ident)
                    {
//                    	printf("id=%s������, ptx=%d ",id,*ptx); 
                        i = position(id, *ptx); /* ����Ҫ���ı��� */
//                        printf("i��%d\n",i); 
                        ReadNum++;		// ID����¼+1
                    }
                    else
                    {
                        i=0;
                    }
                    //printf("------------------------------------------------------%d",i); 

                    if (i == 0)
                    { 
                        error(35);  /* read()��Ӧ���������ı����� */
                    }
                    // ��������
                    else if (table[i].kind != variable)
                    {
                        getsymdo;
                        if (sym == lbrakt) /* �����������ڵı��ʽ */
                        {
                            getsymdo;
                            memcpy(nxtlev, fsys, sizeof(bool)*symnum);
                            nxtlev[rbrakt] = true;
                            expressiondo(nxtlev, ptx, lev);
                            if (sym != rbrakt)
                            {
                                error(48);  // �������ȱ��������
                            }
                            else
                            {
                                // ����̨����һ��%f�������ŵ�ջ��, ֻ�е��Ǹ�ʽ������ʱ��Ч
                                if (ReadFormat==0)
                                	gendo(opr, 0, 16, 0);
                                if(table[i].type=='i'){
                                	// ջ����ֵ(������)�浽 ��Ե�ǰ���̲��ΪL ����ƫ����ΪA+��ջ����ֵ �Ĵ洢��Ԫ, t-2
                                    gendo(sta, lev-table[i].level, table[i].adr, 0);   // ���ȥ��top-2
                                }
                                else if(table[i].type=='f'){
                                	// ջ����ֵ(��������)�浽 ��Ե�ǰ���̲��ΪL ����ƫ����ΪA+��ջ����ֵ �Ĵ洢��Ԫ, t-2
                                    gendo(sta, lev-table[i].level, table[i].adr, 1);   // ���ȥ��top-2
                                }else{
                                    printf("wrong!!!\n");
                                }
                                getsymdo;
                            }
                        }
                        else
                        {
                            error(34);  /* read()������ı�ʶ�����Ǳ���, thanks to amd */
                        }
                    }
                    else    // �������ʶ����var, ��������ͨvariable
                    {
                        // ����̨����һ��%f�������ŵ�ջ��, ֻ�е��Ǹ�ʽ������ʱ��Ч
                        if (ReadFormat==0)
                        	gendo(opr, 0, 16, 0);  /* ��������ָ���ȡֵ��ջ�� */
                        if(table[i].type=='i'){
                            gendo(sto, lev-table[i].level, table[i].adr, 0);   // ���ȥ��top-1
                        }
                        else if(table[i].type=='f'){
                            gendo(sto, lev-table[i].level, table[i].adr, 1);   // ���ȥ��top-1
                        }else{
                            printf("wrong!!!\n");
                        }
                        getsymdo;
                    }
                    
                } while (sym == comma); /* һ��read���ɶ�������� */
                if(ReadNum!=ReadFormat && ReadFormat!=0){
                	error(51);		// ��ʽ�������������
                }
                if (ReadNum>readnum){
                	error(54);		// readһ�ζ��벻�ܳ���10
                }
            }
            if(sym != rparen)
            {
                error(33);  /* ��ʽ����Ӧ�������� */
                while (!inset(sym, fsys))   /* �����ȣ�ֱ���յ��ϲ㺯���ĺ������ */
                {
                    getsymdo;
                }
            }
            else
            {
                getsymdo;
            }
        }
        else
        {
            if (sym == writesym)    /* ׼������write��䴦����read���� */
            {
                getsymdo;
                if (sym == lparen)
                {
                    getsymdo;                    
                    if(sym == str){
                        getsymdo;       // �������ţ�����ֱ�Ӷ����˽�β�ģ�
                        char tempStr[strmax];
                        // ��ʼ���ݴ�str������
                        for(k=0 ; k<strmax ; k++){
                            tempStr[k] = '\0';
                        }
                        // ����str��ÿһ�����з���
                        for(k=0 ; k<strmax ; k++){
                            if(str_[k]=='\0'){ break;}
                            // ����Ǹ�ʽ������
                            if(str_[k]=='%'){
                                k++;
                                getsymdo;
                                if(sym!=ident){
                                    error(39);
                                }
                                memcpy(nxtlev, fsys, sizeof(bool)*symnum);
                                nxtlev[rparen] = true;
                                nxtlev[comma] = true;       /* write�ĺ������Ϊ) or , */
                                expressiondo(nxtlev, ptx, lev); /* ���ñ��ʽ�����˴���read��ͬ��readΪ��������ֵ */
                                switch(str_[k]){
                                    case 'd':
                                        gendo(opr, 0, 18, 0);  // ��ջ����ֵ���������̨��top-1
                                        break;
                                    case 'f':
                                        gendo(opr, 0, 18, 1);  // ��ջ����ֵ���������̨��top-1
                                        break;
                                }
                            }
                            else    // ����������
                            {
                                perSegIndex = 0;
                                do{
                                    tempStr[perSegIndex] = str_[k];
                                    perSegIndex++;
                                    k++;
                                }while(str_[k]!='%' && str_[k]!='\0' && k<strmax);
                                // ֱ��ĩβ���߸�ʽ��������ֹͣ
                                // Ԥ���� �����������
                                for(j=0 ; j<perSegIndex ; j++){
                                    code[cx].str[j] = tempStr[j];
                                    tempStr[j] = 0;
                                }
                                gendo(opr, 0, 17, 0);  // ����ַ�����ֵ
                                if(str_[k]=='%'){
                                    k--;
                                }
                            }
                        }
                        if (sym != rparen)
                        {
                            error(33);  /* write()��ӦΪ�������ʽ */
                        }
                        else
                        {
                            getsymdo;
                        }
                    }
                    else        // ֻ��һ������id
                    {
                        do {
                            if(sym == comma){
                                getsymdo;
                            }
                            memcpy(nxtlev, fsys, sizeof(bool)*symnum);
                            nxtlev[rparen] = true;
                            nxtlev[comma] = true;       /* write�ĺ������Ϊ) or , */
                            expressiondo(nxtlev, ptx, lev); /* ���ñ��ʽ�����˴���read��ͬ��readΪ��������ֵ */
                            // ��ջ����ֵ���������̨��top-1
                            gendo(opr, 0, 14, 0);  /* �������ָ����ջ����ֵ */
                        } while (sym == comma);
                        if (sym != rparen)
                        {
                            error(33);  /* write()��ӦΪ�������ʽ */
                        }
                        else
                        {
                            getsymdo;
                        }
                    }
                }
                // ���һ������
                // gendo(opr, 0, 15, 0);  /* ������� */
            }
            else
            {
                if (sym == callsym) /* ׼������call��䴦�� */
                {
                    getsymdo;
                    if (sym != ident)
                    {
                        error(14);  /* call��ӦΪ��ʶ�� */
                    }
                    else
                    {
                        i = position(id, *ptx);
                        if (i == 0)
                        {
                            error(11);  /* ����δ�ҵ� */
                        }
                        else
                        {
                            if (table[i].kind == procedur)
                            {
                            	getsymdo;
								if(sym == lparen){
									// ��¼��������
									param_num = 0;
									do{
										getsymdo;
										memcpy(nxtlev, fsys, sizeof(bool)*symnum);
										expressiondo(nxtlev,ptx,lev);
										param_num++;
									}while(sym == comma);

									if(sym ==rparen){
										getsymdo;
									}else{
										error(55);		// ���̴����﷨����
									}
								}else{
									error(55);		// ���̴����﷨����
								}
								if (param_num != table[i].val){
									error(56);		// ���̵��ò�����һ��
								}
								// ��ջ��A��Ԫ��������3��λ�ã�Ϊ���̵��������ڳ�λ�ã�t-A
								gendo(mov, 0, param_num, 0);
                                // ��cal L A ���õ�ַΪA�Ĺ��̣�����ڱ���飬���ù����뱻���ù��̲��ΪL
                                // �����table[i].adr������code�����λ��, lev�϶����ڵ���table[i].level
                                gendo(cal, lev-table[i].level, table[i].adr, 0);   /* ����callָ�� */
                            }
                            else
                            {
                                error(15);  /* call���ʶ��ӦΪ���� */
                            }
                        }
                    }
                }
                else
                {
                    if (sym == ifsym)   /* ׼������if��䴦�� */
                    {
                        getsymdo;
                        memcpy(nxtlev, fsys, sizeof(bool)*symnum);
                        nxtlev[thensym] = true;
                        nxtlev[dosym] = true;   /* �������Ϊthen��do */
                        conditiondo(nxtlev, ptx, lev); /* �������������߼����㣩���� */
                        if (sym == thensym)
                        {
                            getsymdo;
                        }
                        else
                        {
                            error(16);  /* ȱ��then */
                        }
                        cx1 = cx;   /* ���浱ǰָ���ַ */
                        gendo(jpc, 0, 0, 0);   /* ����������תָ���ת��ַδ֪����ʱд0 */
                        statementdo(fsys, ptx, lev);    /* ����then������ */
                        cx3 = cx; 
                        gendo(jmp, 0, 0, 0);
                        code[cx1].a = cx;   /* ��statement�����cxΪthen�����ִ�����λ�ã�������ǰ��δ������ת��ַ */
                        if(sym == elsesym)
						{
							getsymdo;
							statementdo(fsys, ptx, lev);
							code[cx3].a = cx; 	//��ǰ��else�����������λ�ã�if���ִ�к�Ӧ����ת����
						} 
                    }
                    else
                    {
                        if (sym == beginsym)    /* ׼�����ո�����䴦�� */
                        {
                            getsymdo;
                            memcpy(nxtlev, fsys, sizeof(bool)*symnum);
                            nxtlev[semicolon] = true;
                            nxtlev[endsym] = true;  /* �������Ϊ�ֺŻ�end */
                            /* ѭ��������䴦������ֱ����һ�����Ų�����俪ʼ���Ż��յ�end */
                            statementdo(nxtlev, ptx, lev);

                            while (inset(sym, statbegsys) || sym==semicolon)
                            {
                                if (sym == semicolon)
                                {
                                    getsymdo;
                                }
                                else
                                {
                                    error(10);  /* ȱ�ٷֺ� */
                                }
                                statementdo(nxtlev, ptx, lev);
                            }
                            if(sym == endsym)
                            {
                                getsymdo;
                            }
                            else
                            {
                                error(17);  /* ȱ��end��ֺ� */
                            }
                        }
                        else
                        {
                            if (sym == whilesym)    /* ׼������while��䴦�� */
                            {
                                loopLevel++;
                                if (loopLevel>loopmax)
                                    error(50);        // ѭ�������涨������
                                cx1 = cx;   /* �����ж�����������λ�� */
                                getsymdo;
                                memcpy(nxtlev, fsys, sizeof(bool)*symnum);
                                nxtlev[dosym] = true;   /* �������Ϊdo */
                                conditiondo(nxtlev, ptx, lev);  /* ������������ */
                                cx2 = cx;   /* ����ѭ����Ľ�������һ��λ�� */
                                // ����Ҳ�� jpc ������ת����ջ��Ϊ0(����������)����������һ��ȥ�������AΪ0������ҲҪ��
                                gendo(jpc, 0, 0, 0);   /* ����������ת��������ѭ���ĵ�ַδ֪ */
                                if (sym == dosym)
                                {
                                    getsymdo;
                                }
                                else
                                {
                                    error(18);  /* ȱ��do */
                                }
                                statementdo(fsys, ptx, lev);    /* ѭ���� */
                                // ��������ת��ȥ�ٴ��ж�while�����Ƿ�����Ĵ���
                                gendo(jmp, 0, cx1, 0); /* ��ͷ�����ж����� */
                                // �����ٸģ���ʱ��cx����jmp �����һ�������code����Ȼ��������ʲô����һ�仹��֪������δ����
                                code[cx2].a = cx;   /* ��������ѭ���ĵ�ַ����if���ƣ�����������޸��ϱ�jpc 0 0 �Ǹ�Aֵ */
                                
                                // ���ÿһ��break
                                while (breakCount)
                                {
                                    // ���breakList�����break��level�͵�ǰloopLevel�����, ˵����ǰ��û��break
                                    if (breakList[breakCount - 1].level == loopLevel)
                                    {
                                        breakCount--;
                                        // �������break����ת��ַ
                                        code[breakList[breakCount].cx].a = cx;
                                    }
                                    else
                                        break;
                                }

                                loopLevel--;
                            }
                            else
                            {
                                if (sym == forsym){  /* ׼������for��䴦�� */
                                    loopLevel++;
                                    if (loopLevel>loopmax)
                                        error(50);          // ѭ�������涨������
                                    getsymdo;
                                    if(sym == ident)    
                                    {
                                        i = position(id,*ptx);
                                        // ��ʶ��δ����
                                        if(i==0)  error(11);
                                        // �ҵ���ʶ��
                                        else{
                                            if(table[i].kind!=variable){
                                                error(12);      // ��ֵ���ӦΪ����
                                                i=0;
                                            } 
											else
											{
                                                getsymdo;
                                                if(sym==becomes) {
                                                    getsymdo; 
                                                }
												else
												{
													error(13);    // ӦΪ��ֵ��
												} 
                                                expressiondo(nxtlev,ptx,lev); // ������ʽ��ֵ
                                                // ��ʵ������ж��Ƕ����
                                                if(i!=0){
                                                    // ֱ�Ӱ����ʹ洢
                                                    gendo(sto, lev-table[i].level, table[i].adr, 0);
                                                }
                                                cx1 = cx;
                                                if (sym == stepsym) 
                                                {
                                                    getsymdo;
                                                    expressiondo(nxtlev,ptx,lev); // ������ʽ(������ֵ)�����ŵ�ջ��
                                                    // �������step��code���ɣ�������opr(����ȡ��)��lit(��������)��lod(��ʶ������)
                                                    which.f = code[cx-1].f;
                                                    which.l = code[cx-1].l;
                                                    which.a = code[cx-1].a;
                                                }else error(42); // ȱstep����
                                                if(sym==untilsym)
                                                {
                                                    getsymdo;
                                                    // ��ѭ������ȡ��ջ������ʱstep��ֵ�����棬����ѱ���ȡ������Ϊ���жϱȽ�
                                                    gendo(lod, lev-table[i].level, table[i].adr, 0);
                                                    expressiondo(nxtlev,ptx,lev);   // ������ʽ��ֵ��Ҳ����until���ֵ
                                                    // ������жϷ��������, opr(����ȡ��)��lit(��������)��lod(��ʶ������)
                                                    if (which.f==lit || which.f==lod){
                                                        // ������ñ�ʶ������ֻ֧������
                                                        // �� ��ջ��(����) �� ջ��(until��ֵ)��1���ڴ�ջ����top-1
                                                        gendo(opr, 0, 13, 0); 
                                                    }else if(which.f==opr){
                                                        // �� ��ջ��(����) �� ջ��(until��ֵ)��1���ڴ�ջ����top-1
                                                        gendo(opr, 0, 11, 0);
                                                    }
                                                    cx2 = cx;    // �����ʱ��cx(��jpc��Ҫ����ĵط�)�����ڻ���jpc
                                                    // �������㣬����ת��A
                                                    gendo(jpc, 0, 0, 0);
                                                }
												else 
												{
													error(43);    // ȱuntil����
												}
                                                if(sym==dosym)  
                                                {
                                                    getsymdo;
                                                    statementdo(fsys,ptx,lev);//���ִ��
                                                    // ��ѭ������ȡ��ջ������ʱstep��ֵ�����棬����ȡ������Ϊ�˼Ӳ���
                                                    gendo(lod, lev-table[i].level, table[i].adr, 0);
                                                    gendo(opr, 0, 2, 0);  //�Ӳ���
                                                    // ���ȥ
                                                    gendo(sto, lev-table[i].level, table[i].adr, 0);
                                                    gendo(jmp, 0, cx1, 0);      // ���رȽϴ�
                                                    code[cx2].a = cx; // ����jpc�жϵ���Ծ��ַ
                                                }else error(44); // ȱdo����
                                            }
                                        }
                                    }
									else
                                    {
                                        error(41);      // for��ӦΪ��ʶ��
									}
                                    // ���ÿһ��break
                                    while (breakCount)
                                    {
                                        // ���breakList�����break��level�͵�ǰloopLevel�����, ˵����ǰ��û��break
                                        if (breakList[breakCount - 1].level == loopLevel)
                                        {
                                            breakCount--;
                                            // �������break����ת��ַ
                                            code[breakList[breakCount].cx].a = cx;
                                        }
                                        else
                                            break;
                                    }

                                    loopLevel--;
                                }
                                else
                                {
                                    if (sym == breaksym)
									{
                                        // ֻ�е�loopLevelΪ0ʱ, �Żᱨ��˵������ѭ���ڵĵط�ʹ����break���
                                        if (!loopLevel)
                                            error(49);          // ����: break����ѭ�������
                                        else
                                        {
                                            getsym();
                                            breakList[breakCount].cx = cx;
                                            breakList[breakCount].level = loopLevel;
                                            breakCount++;
                                            // ������jmp����for�������
                                            gen(jmp, 0, 0, 0);
                                        }
                                    }
                                    else
                                    {
										if(sym == casesym)   /* ׼������case��䴦�� */
										{
											casej=0;
											getsymdo;
											gendo(inte,0,1,0);  /*����һ����ַ�ռ䱣��"���ʽ"����Ľ��*/
											memcpy(nxtlev,fsys,sizeof(bool)*symnum);
											nxtlev[ofsym]=true;    /*case����ı��ʽ�������Ϊof*/
											expressiondo(nxtlev,ptx,lev);
											ptx2=*ptx;
											while(table[ptx2].kind !=variable)  /*Ѱ�����һ������������ж���ı���*/
											{
												ptx2--;
											}
									        gendo(sto,0,table[ptx2].adr+1,0);  /*��"���ʽ"����������¿��ٵĿռ�,��֮ǰ���һ����������һ����ַ*/
											if(sym == ofsym)
											{
												getsymdo;
											}
											else
											{
												error(8);  /*ȱ����of*/
											}
											while(sym != endsym) /*case��end��Ϊ������*/
											{
												casei=0;
												while(sym == number)  /*caseֵ����һЩ�ö��ŷֿ��ĳ���*/
												{
													gendo(lod,0,table[ptx2].adr+1,0);
													gendo(lit,0,num,0);
													gendo(opr,0,9,0);   /*�ж�ֵ���еĳ����Ƿ���֮ǰ"���ʽ"����Ľ�����*/
													cx5[casei++]=cx;  /* ��¼��Ҫ����ĵ�ַ�����*/
													gendo(jpc,0,0,0);  /*������,����ת����ֵ������������ִ��,�������������Ƚ�Щֵ�����ĳ���*/
													getsymdo;
													if(sym==comma)
														getsymdo;
												}
												if(sym==colon)
												{
													cx1=cx;
													gendo(jmp,0,0,0);  /*�����ֵ���ֵ��������"���ʽ"�Ľ��,����ת����һ��case���*/
													for(k=0;k<casei;k++)  
														code[cx5[k]].a=cx;  /*�ѿ�ʼִ��ֵ��������ĵ�һ��λ�û���*/
													getsymdo;
												}
												statementdo(nxtlev,ptx,lev);  /*ÿ��case�������ѭ����*/
												cx6[casej++]=cx;   /*��¼��Ҫ����ĵ�ַ���*/
												gendo(jmp,0,0,0);  /*ִ�д�case����,��Ҫ��ת��caseѭ��*/
												code[cx1].a=cx; /*�������������case������һ��case����λ��*/
												if(sym==semicolon)
												{
													getsymdo;
												}
											}
											if(sym==endsym)  
											{
												getsymdo;
												for(k=0;k<casej;k++)
													code[cx6[k]].a=cx;  /* ��caseѭ�����������һ��λ�û���*/
											}
										}
										else
										{
											if (sym == exitsym){
	                                    		// ֱ������endָ��
	                                    		gendo(end, 0, 0, 0);
	                                    		getsym();
	                                    	}
											else
		                                    {
		                                        memset(nxtlev, 0, sizeof(bool)*symnum); /* �������޲��ȼ��� */
		                                        // ***************************����Ҳ��������ģ�����ȫ����
		                                        testdo(fsys, nxtlev, 19);   /* �������������ȷ�� */
		                                    }	
										}
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/*
* ���ʽ����: <���ʽ> ::= [+|-]<��>{<�Ӽ������><��>}
*/
int expression(bool* fsys, int* ptx, int lev)
{
    enum symbol addop;  /* ���ڱ��������� */
    bool nxtlev[symnum];

    if(sym==plus || sym==minus) /* ��ͷ�������ţ���ʱ��ǰ���ʽ������һ�����Ļ򸺵��� */
    {
        addop = sym;    /* ���濪ͷ�������� */
        getsymdo;
        memcpy(nxtlev, fsys, sizeof(bool)*symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termdo(nxtlev, ptx, lev);   /* ������ */
        if (addop == minus)
        {
            // ��ջ��Ԫ�ص��෴�������ֵ����ջ�������ǰѵ�һ��<��>�ı����
            gendo(opr, 0, 1, 0);
        }
    }
    else    /* ��ʱ���ʽ��������ļӼ� */
    {
        memcpy(nxtlev, fsys, sizeof(bool)*symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termdo(nxtlev, ptx, lev);   /* ������ */
    }
    while (sym==plus || sym==minus)
    {
        addop = sym;
        getsymdo;
        memcpy(nxtlev, fsys, sizeof(bool)*symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        termdo(nxtlev, ptx, lev);   /* ������ */
        if (addop == plus)
        {
            // opr 0 2 ��ջ����ջ����ӣ�����Ŵ�ջ����top-1
            gendo(opr, 0, 2, 0);   /* ���ɼӷ�ָ�� */
        }
        else
        {
            // opr 0 3 ��ջ��-ջ��������Ŵ�ջ����top-1
            gendo(opr, 0, 3, 0);   /* ���ɼ���ָ�� */
        }
    }
    return 0;
}

/*
* ��� <��> ::= <����>{<�˳������><����>}
*/
int term(bool* fsys, int* ptx, int lev)
{
    // ���ڱ���˳�������
    enum symbol mulop;
    bool nxtlev[symnum];

    memcpy(nxtlev, fsys, sizeof(bool)*symnum);
    nxtlev[times] = true;
    nxtlev[slash] = true;
    // ��������
    factordo(nxtlev, ptx, lev);
    while(sym==times || sym==slash)
    {
        mulop = sym;
        getsymdo;
        factordo(nxtlev, ptx, lev);
        if(mulop == times)
        {
            // ��ջ����ջ����������ջ����top-1
            gendo(opr, 0, 4, 0);   /* ���ɳ˷�ָ�� */
        }
        else
        {
            // ��ջ����ջ����������ջ����top-1
            gendo(opr, 0, 5, 0);   /* ���ɳ���ָ�� */
        }
    }
    return 0;
}

/*
* ���Ӵ���: <����> ::= <id>|<integer>|'('<���ʽ>')'
*/
int factor(bool* fsys, int* ptx, int lev)
{
    int i;
    bool nxtlev[symnum];
    // ********************FIRSTû���⣬fsysͬ��Ҫ��
    testdo(facbegsys, fsys, 24);    /* ������ӵĿ�ʼ���� */
    /* while(inset(sym, facbegsys)) */  /* ѭ��ֱ���������ӿ�ʼ���� */
    if(inset(sym,facbegsys))
    {
        if(sym == ident)    /* ����Ϊ��������� */
        {
            i = position(id, *ptx); /* �������� */
            if (i == 0)
            {
                error(11);  /* ��ʶ��δ���� */
            }
            else
            {
                switch (table[i].kind)
                {
                case constant:  /* ����Ϊ���� */
                    // lit 0 A ��A�浽ջ����top+1
                    if (table[i].type=='i'){
                        gendo(lit, 0, table[i].val, 0);    /* ֱ�Ӱѳ�����ֵ��ջ */
                    }
                    else if(table[i].type=='f'){
                        gendo(lit, 1, 0, table[i].valf);    /* ֱ�Ӱѳ�����ֵ��ջ */
                    }
                    break;
                case variable:  /* ����Ϊ���� */
                    // ȡ��Ե�ǰ���� ���ΪL ����ƫ����ΪA �Ĵ洢��Ԫ��ֵ��ջ��, top+1
                    gendo(lod, lev-table[i].level, table[i].adr, 0);   /* �ҵ�������ַ������ֵ��ջ */
                    break;
                case procedur:  /* ����Ϊ���� */
                    error(21);  /* ����Ϊ���� */
                    break;
                case array:
                    getsymdo;
                    if (sym == lbrakt) /* �����������ڵı��ʽ */
                    {
                        getsymdo;
                        memcpy(nxtlev, fsys, sizeof(bool)*symnum);
                        nxtlev[rbrakt] = true;
                        expressiondo(nxtlev, ptx, lev);
                        if (sym == rbrakt)
                        {
                        	// ȡ��Ե�ǰ���� ���ΪL ����ƫ����ΪA+ջ����ֵ �Ĵ洢��Ԫ��ֵ���ǵ�ǰջ��, t����
                            gendo(lda, lev-table[i].level, table[i].adr, 0);
                        }
                        else
                        {
                            error(48);  // �������ȱ��������
                        }
                    }
                    else
                    {
                        error(47);  /* ������ʴ��� */
                    }
                    break;
                }
            }
            getsymdo;
        }
        else
        {
            if(sym == number)   /* ����Ϊ�� */
            {
                // num��ֵ��amax�ǵ�ַ�Ͻ磬�����**************
                if (num > amax)
                {
                    error(31);
                    num = 0;
                }
                // Ҳ����һ��������ֱ�ӷ���ջ��
                gendo(lit, 0, num, 0);
                getsymdo;
            }
            // ����Ǹ����� real
			else if(sym == real)
	        {
	        	// Ҳ����һ��������ֱ�ӷ���ջ��
                gendo(lit, 1, 0, float_);
                printf("%d %f",num,float_);
                getsymdo;
			}
            else
            {
                if (sym == lparen)  /* ����Ϊ���ʽ */
                {
                    getsymdo;
                    memcpy(nxtlev, fsys, sizeof(bool)*symnum);
                    nxtlev[rparen] = true;
                    expressiondo(nxtlev, ptx, lev);
                    if (sym == rparen)
                    {
                        getsymdo;
                    }
                    else
                    {
                        error(22);  /* ȱ�������� */
                    }
                }
                // *****************��
                testdo(fsys, facbegsys, 23);    /* ���Ӻ��зǷ����� */
            }
        }
    }
    return 0;
}

/*
* ��������: <����> ::= <���ʽ><��ϵ�����><���ʽ>|odd <���ʽ>
*/
int condition(bool* fsys, int* ptx, int lev)
{
    enum symbol relop;
    bool nxtlev[symnum];

    if(sym == oddsym)   /* ׼������odd���㴦�� */
    {
        getsymdo;
        expressiondo(fsys, ptx, lev);
        // ջ����ֵ�������������Ϊ1��ż�����Ϊ0
        gendo(opr, 0, 6, 0);   /* ����oddָ�� */
    }
    else
    {
        /* �߼����ʽ���� */
        memcpy(nxtlev, fsys, sizeof(bool)*symnum);
        nxtlev[eql] = true;
        nxtlev[neq] = true;
        nxtlev[lss] = true;
        nxtlev[leq] = true;
        nxtlev[gtr] = true;
        nxtlev[geq] = true;
        expressiondo(nxtlev, ptx, lev);
        if (sym!=eql && sym!=neq && sym!=lss && sym!=leq && sym!=gtr && sym!=geq)
        {
            error(20);
        }
        else
        {
            relop = sym;
            getsymdo;
            expressiondo(fsys, ptx, lev);
            // ��Ԫ�Ƚ�ָ��
            switch (relop)
            {
            case eql:
                gendo(opr, 0, 8, 0);       // ����ô�ջ��Ϊ1, top-1
                break;
            case neq:
                gendo(opr, 0, 9, 0);       // ������ô�ջ��Ϊ1, top-1
                break;
            case lss:
                gendo(opr, 0, 10, 0);      // С�����ô�ջ��Ϊ1, top-1
                break;
            case geq:
                gendo(opr, 0, 11, 0);      // �� ���ô�ջ��Ϊ1, top-1
                break;
            case gtr:
                gendo(opr, 0, 12, 0);      // �������ô�ջ��Ϊ1, top-1
                break;
            case leq:
                gendo(opr, 0, 13, 0);      // �� ���ô�ջ��Ϊ1, top-1
                break;
            }
        }
    }
    return 0;
}

// end 

