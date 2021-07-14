/*
 * PL/0 complier program for win32 platform (implemented in C)
 *
 * The program has been test on Visual C++ 6.0,  Visual C++.NET and
 * Visual C++.NET 2003,  on Win98,  WinNT,  Win2000,  WinXP and Win2003
 *
 */

typedef enum {
    false, true
} bool;


#define norw 22     /* �ؼ��ָ��� */
#define txmax 100   /* ���ֱ����� */
#define nmax 14     /* number�����λ�� */
#define al 10       /* ���ŵ���󳤶� */
#define amax 2047   /* ��ַ�Ͻ�*/
#define levmax 3    /* ����������Ƕ���������� [0,  levmax]*/
#define cxmax 500   /* ��������������� */
#define strmax 100  // �ַ�����󳤶�, ֻ��Ϊż��
#define loopmax 10  // ѭ��������
#define readnum 10  // readһ�����������

/* ���� */
// ����ö�����ͣ���һ��ö�ٳ�Ա��Ĭ��ֵΪ���͵� 0������ö�ٳ�Ա��ֵ��ǰһ����Ա�ϼ� 1���Դ�����
enum symbol {
	/*����ʶ������(��ʶ��ident)����������number*/
    nul,/*0unknown*/ident,/*1*/     number,/*2*/    

	/*11���ַ�(������>��<) + 5˫�ַ�(>,>=,<,<=,:=) + odd������*/
    plus,/*3*/      minus,/*4*/
    times,/*5*/     slash,/*6 /*/   oddsym,/*7*/    eql,/*8*/       neq,/*9*/
    lss,/*10 <*/    leq,/*11 <=*/   gtr,/*12 >*/    geq,/*13 >=*/   lparen,/*14 (*/
    rparen,/*15 )*/ comma,/*16*/    semicolon,/*17*/period,/*18*/   becomes,/*19 ��ֵ:=*/
    
	/*12������*/
    beginsym,/*20*/ endsym,/*21*/   ifsym,/*22*/    thensym,/*23*/  whilesym,/*24*/
    writesym,/*25*/ readsym,/*26*/  dosym,/*27*/    callsym,/*28*/  constsym,/*29*/
    varsym,/*30*/   procsym,/*31*/	

	/*���ӵ�: 1����������1�����ַ�ð�ţ�һ����������������, 1���ַ���, ����forѭ��������, 2�������ŵ��ַ�, break������,
            +=/-+����˫�ַ�, һ��exit��䱣����, ����caseѡ�����֣� һ��else������*/
    real,/*32*/     colon,/*: 33*/  realsym,/*34*/  str,/*35*/      forsym,/*36*/
    stepsym,/*37*/  untilsym,/*38*/ lbrakt,/*[39*/  rbrakt,/*]40*/  breaksym,/*41*/
    pluseq,/*42+=*/ minuseq,/*43-=*/exitsym,/*44*/  casesym,/*45*/  ofsym,/*46*/
    elsesym,/*47*/ 
};
/* ���Ź�48�� */ 
#define symnum 48

/* ���ֱ��е����� */
enum object {
    constant,
    variable,
    procedur,
    array       //add
};

/* ��������� */
enum fct {
    lit,     opr,     lod,
    sto,     cal,     inte,
    jmp,     jpc,     nan,
    lda,     sta,     lpl,
    lmi,     end,     mov,
};
#define fctnum 15

/* ���������ṹ */
struct instruction
{
    enum fct f; /* ���������ָ�� */
    int l;      /* ���ò���������Ĳ�β� */
    int a;      /* ����f�Ĳ�ͬ����ͬ */
    float real;     // �����漰������������ʱʹ��
    char str[strmax];   // ���漰�ַ�������ʽ��readʱʹ��
};

struct BREAKLIST
{
    int cx;         // ��ŵ�ǰbreak���ɵ�jmp��code�е�λ�ã����ڻ�д
    int level;      // ��ŵ�ǰbreak����ѭ������
};

FILE* virCode;       /* ������������ */
char ch;            /* ��ȡ�ַ��Ļ�������getch ʹ�� */
// ����ö�ٱ������������е����ͽ��и�ֵ, ��ŵ�ǰ���ŵı�ʶ
enum symbol sym;    /* ��ǰ�ķ��� */
enum symbol sym_op;    /* +=/-=���ݴ���� */
// ��ű�ʶ��
char id[al+1];      /* ��ǰident, �����һ���ֽ����ڴ��0 */
int num;            /* ��ǰnumber */
float float_;       // ��ǰreal
char str_[strmax];  // ��ŵ�ǰ�ַ���
char format_[strmax/2];     // ��ŵ�ǰ�ַ����и�ʽ������
float tempfloat[readnum];   // ��ʱ��Ž�������read scanf����������
char tempstr[strmax] = "";  // �ڽ��������ݴ��ַ���
int index_temp = 0;         // ר���ڸ� tempstr��ֵ
// char str_segs[strmax][strmax];      // ��Ÿ�ʽ���и��ֶε�����ֱ��������ַ�
int arrSize = 0;    // ���ڴ�ŵ�ǰarray��size
int loopLevel = 0;  // ���ڼ�¼��ǰ����for��while��ѭ���ڼ���, 0�ʹ���û����
struct BREAKLIST breakList[loopmax];        // ����ѭ����break����Ϣ
int breakCount = 0; // ��¼�ڽ��뵱ǰѭ���ĵ�һ��֮��, �Ѿ��ۼ��ж��ٸ�break��
int ReadNum = 0;    // ��¼��ʽ�����������ж��ٸ���ʶ��
int ReadFormat = 0; // ��¼��ʽ��������ĸ�ʽ�����Ÿ���
int save_dx;        // ���̴��β���ʹ�õĸ�������, ���֮ǰ��dxֵ
int param_num = 0;   // ��¼���β�������
int cc, ll;          /* getchʹ�õļ�������cc��ʾ��ǰ�ַ�(ch)��λ�� */
int hs;              /*  ��ȡ�ĵ�ǰ���� */ 
int cx;             /* ���������ָ��, ȡֵ��Χ[0, cxmax-1]*/
int cx5[10],cx6[10],casei,casej;        /* ���ڴ��case�����jmp��jpc��Ҫ�������Щ��������*/
char line[81];      /* ��ȡ�л����� */
// ��Ŷ�ȡ��һ����
char a[al+1];       /* ��ʱ����, �����һ���ֽ����ڴ��0 */
struct instruction code[cxmax]; /* ����������������� */
// norw: �����ֵĸ���Ϊ13��al: ���ŵ���󳤶�Ϊ10
char word[norw][al];        /* ������ */
// ����ö�ٱ������������е����ͽ��и�ֵ
enum symbol wsym[norw];     /* �����ֶ�Ӧ�ķ���ֵ */
enum symbol ssym[256];      /* ���ַ��ķ���ֵ */
// ��P_code����ָ�8��
char mnemonic[fctnum][5];   /* ���������ָ������ */
bool declbegsys[symnum];    /* ��ʾ������ʼ�ķ��ż��� */
bool statbegsys[symnum];    /* ��ʾ��俪ʼ�ķ��ż��� */
bool facbegsys[symnum];     /* ��ʾ���ӿ�ʼ�ķ��ż��� */

/* ���ֱ�ṹ */
struct tablestruct
{
    char name[al];      /* ���� */
    enum object kind;   /* ���ͣ�const, var, array or procedure */
    int val;            /* ��ֵ��constʹ�ü�¼ֵ��procedure��¼�������� */
    float valf;         // ���㣬��constʹ��
    int level;          /* �����㣬��const��ʹ�� */
    int adr;            /* ��ַ����const��ʹ�ã���Ϊ������ֵval��ֱ�Ӵ�������������A���� */
    int size;           /* ��Ҫ������������ռ�, procedure��arrayʹ�� */
	char type;          /* ����type, i��ʾ����, f��ʾ������ */
};

struct tablestruct table[txmax]; /* ���ֱ� */

FILE* Pl0_SourceCode; // �洢����ȡ�ļ��� �ļ�ָ��, ��ָ��ָ����ļ�
// �����ѷ��ֵĴ������
int err; /* ��������� */

/* �������лᷢ��fatal errorʱ������-1��֪�������ĺ����������˳����� */
// �궨�庯��������ʱ��ʶ��define���Իس���Ϊ������
#define getsymdo                      if(-1 == getsym()) return -1
#define getchdo                       if(-1 == getch()) return -1
#define testdo(a, b, c)               if(-1 == test(a, b, c)) return -1
#define gendo(a, b, c, d)             if(-1 == gen(a, b, c, d)) return -1
#define expressiondo(a, b, c)         if(-1 == expression(a, b, c)) return -1
#define factordo(a, b, c)             if(-1 == factor(a, b, c)) return -1
#define termdo(a, b, c)               if(-1 == term(a, b, c)) return -1
#define conditiondo(a, b, c)          if(-1 == condition(a, b, c)) return -1
#define statementdo(a, b, c)          if(-1 == statement(a, b, c)) return -1
#define arraydeclarationdo(a, b, c)   if(-1 == arraydeclaration(a, b, c)) return -1
#define constdeclarationdo(a, b, c)   if(-1 == constdeclaration(a, b, c)) return -1
#define vardeclarationdo(a, b, c)     if(-1 == vardeclaration(a, b, c)) return -1

/*
    error: �����Ӧ����
    30: ���ֹ���
    9: �����β���� .(period), ��û����������
*/
void error(int n);
int getsym();
int getch();
void init();
int gen(enum fct x, int y, int z, float real);
int test(bool* s1, bool* s2, int n);
int inset(int e, bool* s);
int addset(bool* sr, bool* s1, bool* s2, int n);
int subset(bool* sr, bool* s1, bool* s2, int n);
int mulset(bool* sr, bool* s1, bool* s2, int n);
int block(int lev, int tx, bool* fsys, int thisParamNum);
void interpret();
int factor(bool* fsys, int* ptx, int lev);
int term(bool* fsys, int* ptx, int lev);
int condition(bool* fsys, int* ptx, int lev);
int expression(bool* fsys, int* ptx, int lev);
int statement(bool* fsys, int* ptx, int lev);
void listcode(int cx0);
int arraydeclaration(int* ptx, int lev, int* pdx);
int vardeclaration(int* ptx, int lev, int* pdx);
int constdeclaration(int* ptx, int lev, int* pdx);
int position(char* idt, int tx);
void enter(enum object k, int* ptx, int lev, int* pdx);
int base(int l, float* s, int b);
void StoreToFile();
void print_nameTable(int tx0, int tx);

// end 
