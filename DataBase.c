#include <stdio.h>
#include <io.h>
#include <string.h>
#include <direct.h>
#define N 4  //�ֶ�����������
//�����ֶ����ͽṹ��
typedef struct FieldTyperange
{
	char fieldType[50];  //�ֶ�����
	int minLength;		//�ֶ���С����
	int maxLength;		//�ֶ���󳤶�
	int isFixed;         //�ֶγ����Ƿ�̶�
} fieldType;

//�����ֶνṹ�壬�������ֶ���Ϣ
typedef struct Table
{
	char tableName[50]; //����
	char fieldName[50];  //�ֶ���
	char fieldType[50];  //�ֶ�����
	char fieldLength[10];//�ֶγ���
	char constraint[50];//Լ��
} table;

//����ṹ�屣������������Ϣ
typedef struct TableName
{
	char tableName[50]; //����
	char fieldCount[50];  //�ֶ���
	char location[50];  //����λ��
} tableName;

//Լ��
typedef struct Constraint
{
	char Constraint[50];  //Լ������
} constraint;

//��ʼ����ر���
fieldType field[N];
table tab[100];
constraint constr[20];
char workLocation[100];
FILE *fp_TS = NULL;
FILE *fp_DS = NULL;

//��������
void initializeFieldType();
void showMenu();
void replaceTo(char * ch);
void operateMenu();
void CreateTable();
void addRecord(char *tableName,char * tableLocation);
void showRecord(char *tableName,char * tableLocation);
void findLine(FILE *fp,char* ch1, char* ch2);
void operateRecord();
int judgeFieldType(char * fieldType);
int judgeConstraint(char * constraint);
void readTableStr(char *tableName);
void JieQu(char* ch1, int count , char* ch2);
void QueryTable();
void QueryTableInDatabase();
int TableIsReappear(char *tableName);
int isDate(char * ch);
int isInt(char * ch);
int isInputPass(char * input, char * type);
/*ʵ��4*/
int isAccordPrimarykey(char * value,char *tableName,char * tableLocation);
int isAccordConstraint(char * value,char * constraint,char *tableName,char * tableLocation);

int main()
{
	int ch;
	strcpy(workLocation,getcwd(NULL, NULL));
	if(access("structure",0) !=  -1)
	{
		//�ļ��д���
		//chdir("/structure");
		if(access("./structure/��ṹ��Ϣ.txt",0) !=  -1)
		{
			//�ļ�����
			fp_TS = fopen("./structure/��ṹ��Ϣ.txt","a+");
		}else
		{
			//�ļ�������
			fp_TS = fopen("./structure/��ṹ��Ϣ.txt","w+");
		}

		if(access("./structure/���ݿ����Ϣ.txt",0) !=  -1)
		{
			//�ļ�����
			fp_DS = fopen("./structure/���ݿ����Ϣ.txt","a+");
		}else
		{
			//�ļ�������
			fp_DS = fopen("./structure/���ݿ����Ϣ.txt","w+");
		}
	}else
	{
		//�ļ��в�����
		mkdir("structure");
		fp_TS = fopen("./structure/��ṹ��Ϣ.txt","w+");
		fp_DS = fopen("./structure/���ݿ����Ϣ.txt","w+");
	}
	
	if(access("Date",0) !=  -1)
	{
	}else
	{
		//�ļ��в�����
		mkdir("Date");
	}

	
	initializeFieldType();
	while(1)
	{
		showMenu();
		scanf("%d",&ch);
		system("cls");
		fp_TS = fopen("./structure/��ṹ��Ϣ.txt","a+");
		fp_DS = fopen("./structure/���ݿ����Ϣ.txt","a+");
		switch(ch)
		{
			case 1:
				CreateTable();
				break;
			case 2:
				QueryTable();
				break;
			case 3:
				QueryTableInDatabase();
				break;
			case 4:
				operateRecord();
				break;
			case 0:
				fclose(fp_TS);
				exit();
				break;
			default :
				printf("û�д�ѡ�\n");
				break;
		}
		fclose(fp_TS);
		fclose(fp_DS);
	}
	fclose(fp_TS);
	fclose(fp_DS);
}

//���˵�
void showMenu()
{
	printf("\n-----------------------\n");
	printf("   1�������ṹ\n");  
	printf("   2����ѯ��ṹ��Ϣ\n");
	printf("   3����ʾ���б�\n");
	printf("   4��������\n");
	printf("   0���˳�\n");
	printf("-----------------------\n");
	printf("���������\n");
}

//�Ա������ݽ��еĲ���
void operateMenu()
{
	printf("\n---------------------\n");
	printf("   1����Ӽ�¼\n");  
	printf("   2����ӡ��¼\n");
	printf("   3���޸ļ�¼\n");
	printf("   4��ɾ����¼\n");
	printf("   0������\n");
	printf("-----------------------\n");
	printf("���������\n");
}

//��ѯ��ǰ����Щ��,�����
void QueryTableInDatabase()
{
	//�������ݿ����Ϣ.txt
	char ch,c[100];
	char temp[20];
	fp_DS = fopen("./structure/���ݿ����Ϣ.txt","a+");
	printf("-----------------------\n");
	printf("          �鿴��       \n");
	printf("-----------------------\n");
	if(fgetc(fp_DS) == EOF)
	{
		printf("û���κα�\n");
	}else{
		while(ch != EOF)//ѭ������ļ���ÿһ��
		{
			//���ж�ȡ�ı�,��ÿһ�����ݽ�ȡ�󱣴浽�ṹ����
			fseek(fp_DS,-1,SEEK_CUR);
			fscanf(fp_DS,"%[^\n]", c);
			JieQu(c, 1 , temp);
			printf("	%s\n",temp);
			if(fgetc(fp_DS) == '\n')//�������з�������
			ch = fgetc(fp_DS);		
		}
	}
	printf("-----------------------\n");
	fclose(fp_DS);
}

//ͨ��������ѯ���ж�ָ�����Ƿ��Ѿ����ڣ�1��ʾ���ڣ�0��ʾ������
int TableIsReappear(char *tableName)
{
	//�������ݿ����Ϣ.txt
	char ch,c[100];
	char temp[20];
	fp_DS = fopen("./structure/���ݿ����Ϣ.txt","a+");//�з����ļ��Ĳ���ʱ�����ǰ�򿪣��ú�رգ���ֹǰ������������ͬһ���ļ�ʱ����
	if(fgetc(fp_DS) == EOF)
	{
		//û���κα�
	}else{
		while(ch != EOF)//ѭ������ļ���ÿһ��
		{
			//���ж�ȡ�ı�,��ÿһ�����ݽ�ȡ�󱣴浽�ṹ����
			fseek(fp_DS,-1,SEEK_CUR);
			fscanf(fp_DS,"%[^\n]", c);
			JieQu(c, 1 , temp);
			
			if(strcmp(tableName,temp) == 0)
			{
				fclose(fp_DS);
				return 1;
			}
			
			if(fgetc(fp_DS) == '\n')//�������з�������
			ch = fgetc(fp_DS);		
		}
	}
	fclose(fp_DS);
	return 0;
}



//ɾ����

//�޸ı�ṹ

//�Ա��е����ݽ��в���
void operateRecord()
{
	char tableName[50];
	char tableNameCopy[50];   //findLine()���������ı�ԭ�в�����ֵ��ʹ�ñ���
	int ch;
	char line[50];
	char tableLocation[50];
	//FILE * fp = NULL;
	QueryTableInDatabase();
	printf("��������Ҫ�����ı���\n");
	scanf("%s",tableName);
	strcpy(tableNameCopy,tableName);
	if(!TableIsReappear(tableName))
	{
		printf("��������ȷ�ı���\n");
	}
	else
	{
		readTableStr(tableName);
		findLine("./structure/���ݿ����Ϣ.txt",tableNameCopy, line);
		JieQu(line, 3 , tableLocation);
		printf("����λ�ã�[%s]\n",tableLocation);
		while(1)
		{
			operateMenu();
			scanf("%d",&ch);
			system("cls");
		//	fp = fopen(tableLocation,"a+");
			switch(ch)
			{
				case 1:
					addRecord(tableName,tableLocation);
					break;
				case 2:
					showRecord(tableName,tableLocation);
					break;
				case 3:
					printf("��δ���ţ������ڴ���\n");
					break;
				case 4:
					printf("��δ���ţ������ڴ���\n");
					break;
				case 0:
				//	fclose(fp);
					return;
					break;
				default :
					printf("û�д�ѡ�\n");
					break;
			}
			//fclose(fp);
		}
	}
	//fclose(fp);
}

//��Ӽ�¼
void addRecord(char *tableName,char * tableLocation)
{
	FILE * fp = NULL;
	char line[100];
	char inputLine[200];
	char temp[50];
	char n[10];
	int fNum = 0;
	int recNum = 0;
	int i = 0;
	int j = 0;
	char tableNameCopy[50];   //findLine()���������ı�ԭ�в�����ֵ��ʹ�ñ���
	strcpy(tableNameCopy,tableName);
	findLine("./structure/���ݿ����Ϣ.txt",tableNameCopy, line); //tableLocation
	//ͨ����������ѯ����ṹ���м����ֶΣ�
	//1.��ѯ���м����ֶΣ���ÿ���ֶε���Ϣ����ṹ��
	JieQu(line, 2 , n);
	fNum = atoi(n);

	printf("�����������¼������\n");
	scanf("%d",&recNum);
	system("cls");
	printf("%s\n",tableLocation);
	fp = fopen(tableLocation,"a+");
	for(i = 0;i < recNum; i++)
	{
		strcpy(inputLine," ");
		printf("�����%d����¼��\n",i+1);
		for(j = 0; j < fNum; j++)
		{
			printf("����%s[%s(%s)]��\n",tab[j].fieldName,tab[j].fieldType,tab[j].fieldLength);
			scanf("%s",temp);
			while(isInputPass(temp, tab[j].fieldType) != 1){
				//���벻�Ϸ���
				if(strcmp(tab[j].fieldType,"varchar") == 0)
				{
					if(strlen(temp)<=atoi(tab[j].fieldLength))
					{
						break;
					}
				}
				printf("���벻�Ϸ�������������%s[%s(%s)]��\n",tab[j].fieldName,tab[j].fieldType,tab[j].fieldLength);
				scanf("%s",temp);
			}
			
			//�ж������Ƿ����Լ��
			while(isAccordConstraint(&temp,tab[j].constraint,tableName,tableLocation) != 1)
			{
				//������Լ����
				printf("���벻����Լ��������������%s[%s(%s)--%s]��\n",tab[j].fieldName,tab[j].fieldType,tab[j].fieldLength,tab[j].constraint);
				scanf("%s",temp);
			}

			fprintf(fp,"%s ",temp);
			if(j != fNum-1)
			{
				fprintf(fp," ");
			}			
		}
		printf("-----------------------\n");
		fprintf(fp,"\n");
	}
	fclose(fp);
}

//ɾ����¼

//��ѯ��¼
void showRecord(char *tableName,char * tableLocation)
{
	FILE * fp = NULL;
	char * p,*s;
	char line[100];
	char outputLine[200];
	char temp[50];
	char n[10];
	char ch,c[100];
	int fNum = 0;  //�м����ֶ�
	int recNum = 0;
	int i = 0,j = 0;
	char tableNameCopy[50];   //findLine()���������ı�ԭ�в�����ֵ��ʹ�ñ���
	
	strcpy(tableNameCopy,tableName);
	findLine("./structure/���ݿ����Ϣ.txt",tableName, line); //tableLocation
	//ͨ����������ѯ����ṹ���м����ֶΣ�
	//1.��ѯ���м����ֶΣ���ÿ���ֶε���Ϣ����ṹ��
	JieQu(line, 2 , n);
	fNum = atoi(n);
	printf("%s\n",tableLocation);
	printf("---------------------------------------------------------------------------------------\n");
	fp = fopen(tableLocation,"a+");
	if(fgetc(fp) == EOF)
	{
		printf("û�д˼�¼\n");
	}else{
		//printf("---%s\n",tab[0].fieldName);
		for(j = 0; j < fNum; j++)
		{
			printf("   %s\t\t",tab[j].fieldName);
			//strcat(outputLine, tab[i].fieldName); 
			//strcat(outputLine, '\t'); 
		}
		printf("\n");
	//	printf("---%s\n",tab[0].fieldName);
		while(ch != EOF)//ѭ������ļ���ÿһ��
		{
			//���ж�ȡ�ı�,��ÿһ�����ݽ�ȡ�󱣴浽�ṹ����
			fseek(fp,-1,SEEK_CUR);
			fscanf(fp,"%[^\n]", c);
			p = outputLine;
			for(i = 0; i < fNum; i++)
			{
				s = temp;
				JieQu(c, i+1 , &temp);
				
				while(*s != '\0')
				{
					*p++ = *s++;
				}
				*p++ = '\t'; 
				*p++ = '\t';
				//*p++ = '\t';
			}
			*p = '\0';
			printf("%s\n",outputLine);
			if(fgetc(fp) == '\n')//�������з�������
			ch = fgetc(fp);		
		}
	}
	printf("---------------------------------------------------------------------------------------\n");
	fclose(fp);
}

//�޸ļ�¼

//�����ṹ�ĺ���������
void CreateTable()
{
	int i = 0;
	char i2[2];
	int j = 0;
	int k = 0;
	char absLocation[100];
	strcpy(absLocation,workLocation);
	printf("�����������\n");
	scanf("%s",tab[0].tableName);
	if(TableIsReappear(&tab[0].tableName))
	{
		printf("�ñ��Ѵ��ڣ�\n");
		return -1;
	}
	printf("����ı�ṹ���ֶθ���\n");
	scanf("%d",&i);
	strcpy(i2,itoa(i));
	for(; i >= 1 ; i--, j++)
	{
		strcpy(tab[j].tableName,tab[0].tableName);
		printf("�������%d���ֶ�����\n",j+1);
		scanf("%s",tab[j].fieldName);
		printf("�������%d���ֶ�����[%s %s %s]��\n",j+1,field[0].fieldType,field[1].fieldType,field[2].fieldType);
		scanf("%s",tab[j].fieldType);
		while((k = judgeFieldType(&tab[j].fieldType)) == -1)
		{
			printf("�Ƿ����룬�����������%d���ֶ����ͣ�\n",j+1);
			scanf("%s",tab[j].fieldType);
		}
		
		if(field[k].isFixed == 1)
		{
			strcpy(tab[j].fieldLength,itoa(field[k].minLength));
		}
		else
		{
			printf("�������%d���ֶγ���[%s(%d-%d)]��\n",j+1,field[k].fieldType,field[k].minLength,field[k].maxLength);
			scanf("%s",&tab[j].fieldLength);
			while((atoi(tab[j].fieldLength) < field[k].minLength) || (atoi(tab[j].fieldLength) > field[k].maxLength))
			{
				printf("�ֶγ��Ȳ�����Ҫ�������������%d���ֶγ��ȣ�\n",j+1);
				scanf("%s",&tab[j].fieldLength);
			}
		}
		printf("Ϊ��%d���ֶ����Լ��[%s %s %s %s]������0Ϊ������κ�Լ������\n",j+1,constr[1].Constraint,constr[2].Constraint,constr[3].Constraint,constr[4].Constraint);
		scanf("%s",&tab[j].constraint);
		while((k = judgeConstraint(&tab[j].constraint) == -1))
		{
			printf("�Ƿ����룬�����������%d���ֶ�Լ��[%s %s %s %s]��\n",j+1,constr[1].Constraint,constr[2].Constraint,constr[3].Constraint,constr[4].Constraint);
			scanf("%s",&tab[j].constraint);
		}
		fprintf(fp_TS,"%s %s %s %s %s\n",tab[j].tableName,tab[j].fieldName,tab[j].fieldType,tab[j].fieldLength,tab[j].constraint);
	}
	strcat(absLocation, "\\Date\\");
	strcat(absLocation, tab[0].tableName);
	strcat(absLocation, ".txt");
	replaceTo(absLocation);
//	creat(absLocation,0);
	fopen(absLocation,"w+");
	printf("-----------------------------------\n");
	printf("�����ɹ���\n�ļ�����λ�ã�[%s]\n",absLocation);
	fp_DS = fopen("./structure/���ݿ����Ϣ.txt","a+");
	fprintf(fp_DS,"%s %s %s\n",tab[0].tableName,i2,absLocation);
	fclose(fp_DS);
}

//��ѯָ����Ľṹ��Ϣ
void QueryTable()
{
	char tableName[50];
	char ch,c[100];
	int i = 0,j = 0;
	QueryTableInDatabase();
	printf("��������Ҫ��ѯ�ı�����\n");
	scanf("%s",tableName);
	if(TableIsReappear(tableName) == 0)
	{
		printf("--------------------------------------------------------------------\n");
		printf("	�ñ����ڣ�\n");
		printf("--------------------------------------------------------------------\n");
		return -1;
	}
	while(ch != EOF)//ѭ������ļ���ÿһ��
	{
		//���ж�ȡ�ı�,��ÿһ�����ݽ�ȡ�󱣴浽�ṹ����
		fseek(fp_TS,-1,SEEK_CUR);
		fscanf(fp_TS,"%[^\n]", c);
		JieQu(c, 1 , &tab[i].tableName);
		
		if(strcmp(tableName,tab[i].tableName) == 0)
		{
			JieQu(c, 2 , &tab[i].fieldName);
			JieQu(c, 3 , &tab[i].fieldType);
			JieQu(c, 4 , &tab[i].fieldLength);
			JieQu(c, 5 , &tab[i].constraint);
			i++;
		}
		
		if(fgetc(fp_TS) == '\n')//�������з�������
		ch = fgetc(fp_TS);		
	}
	printf("------------------------------------------------------------------------------------\n");
	printf("�ֶ���\t\t\t��������\t\t�ֶγ���\t\tԼ������\n");
	for(j = 0; j < i; j++)
	{
		printf("%s\t\t\t%s\t\t\t%s\t\t\t%s\n",tab[j].fieldName,tab[j].fieldType,tab[j].fieldLength,tab[j].constraint);
	}
	printf("------------------------------------------------------------------------------------\n");
}

//�ж�������ֶ������Ƿ����Ҫ��,������Ҫ�󣬷��ظ��ֶε��±꣬��������Ҫ�󣬷���-1
int judgeFieldType(char * fieldType)
{
	int i = 0;	
	for(i = 0; i <= N; i++ )
	{
		if(strcmp(fieldType,field[i].fieldType) == 0)
		{
			return i;
		}
	}
	return -1;
}

//��ʼ���ֶ�����
void initializeFieldType()
{
	strcpy(field[0].fieldType,"varchar");
	field[0].minLength = 0;
	field[0].maxLength = 5000;
	field[0].isFixed = 0;

	strcpy(field[1].fieldType,"int");
	field[1].minLength = 4;
	field[1].maxLength = 4;
	field[1].isFixed = 1;

	strcpy(field[2].fieldType,"date");
	field[2].minLength = 8;
	field[2].maxLength = 8;
	field[2].isFixed = 1;

	strcpy(constr[0].Constraint,"0");
	strcpy(constr[1].Constraint,"PRIMARYKEY");
	strcpy(constr[2].Constraint,"NOTNULL");
	strcpy(constr[3].Constraint,"CHECK(gender)");
	strcpy(constr[4].Constraint,"CHECK(date)");

}

//����������ñ�Ľṹ����ṹ����
void readTableStr(char *tableName)
{
	char ch,c[100];
	int i = 0;
	if(!TableIsReappear(tableName))
	{
		printf("--------------------------------------------------------------------\n");
		printf("	�ñ�����\n");
		printf("--------------------------------------------------------------------\n");
		return -1;
	}
	while(ch != EOF)//ѭ������ļ���ÿһ��
	{
		//���ж�ȡ�ı�,��ÿһ�����ݽ�ȡ�󱣴浽�ṹ����
		fseek(fp_TS,-1,SEEK_CUR);
		fscanf(fp_TS,"%[^\n]", c);
		JieQu(c, 1 , &tab[i].tableName);
		
		if(strcmp(tableName,tab[i].tableName) == 0)
		{
			JieQu(c, 2 , &tab[i].fieldName);
			JieQu(c, 3 , &tab[i].fieldType);
			JieQu(c, 4 , &tab[i].fieldLength);
			JieQu(c, 5 , &tab[i].constraint);
			i++;
		}
		
		if(fgetc(fp_TS) == '\n')//�������з�������
		ch = fgetc(fp_TS);		
	}
}

/*
//ʹjudgeFieldType(char * fieldType)�����ֶ����ͽṹ�������Ӧ�������±꣬���·����Ͳ���Ҫʵ����
//�ж�������ֶ����ͳ����Ƿ�̶������̶������ֶγ��ȣ������̶�������-1
int isFixedOf()
{
}

//�ֶ����ͳ��Ȳ��̶�ʱ�������ֶγ�����Сֵ
int minLengthOf()
{
}

//�ֶ����ͳ��Ȳ��̶�ʱ�������ֶγ������ֵ
int maxLengthOf()
{
}
*/
//��ȡch1��count�����ݣ���ֵ��ch2
//ѧ���� ѧ�� string 20
void JieQu(char* ch1, int count , char* ch2)
{
	int i = 0;
	const char s[2] = " ";
    char *token;
	char copch1[100];
	strcpy(copch1,ch1);
	/* ��ȡ��һ�����ַ��� */
	token = strtok(copch1, s);
	i++;
	/* ������ȡ���������ַ��� */
	
	while( token != NULL ) 
	{
		if(i == count)
			break;
		token = strtok(NULL,s);
		i++;
	}
	strcpy(ch2,token);
}
//ͨ����һ��������(�������Ψһ)���ҵ�ָ���ļ���һ�����ݲ����������ݸ�ֵ��ch2�����ݲ���ʱ�����ļ���ִ�н�����ú������Զ��ر��ļ�
//����ʱ�봫�뱸�ݣ�����
void findLine(char *fileLocation,char* ch1, char* ch2)
{
	FILE *fp = NULL;
	char ch,c[100];
	char temp[50];
	char ch1Temp[50];
	strcpy(ch1Temp,ch1);
//	printf(".............................%s\n",ch1);
	if(TableIsReappear(ch1Temp) != 1)
	{
		printf("������ڣ�\n");
		return -1;
	}
	//printf("--------------------------------%s\n",ch1);
	fp = fopen(fileLocation,"a+");
	if(fgetc(fp) == EOF)
	{
		printf("������Ϊ�գ�\n");
	}else{
		while(ch != EOF)//ѭ������ļ���ÿһ��
		{
			
			//���ж�ȡ�ı�,��ÿһ�����ݽ�ȡ�󱣴浽�ṹ����
			fseek(fp,-1,SEEK_CUR);
			fscanf(fp,"%[^\n]", c);
			JieQu(c, 1 , temp);
			
			if(strcmp(ch1Temp,temp) == 0)
			{
				//printf("�ҵ���8888888888888888888\n");
				strcpy(ch2,c);
				//printf("--------------------------------%s\n",ch1);
			}
			
			if(fgetc(fp) == '\n')//�������з�������
			ch = fgetc(fp);		
		}
	}
//	strcpy(ch1,ch1Temp);//ch1���б仯��
//	printf("--------------------------------%s\n",ch1);
//	printf("ch1Temp--------------------------------%s\n",ch1Temp);
	fclose(fp);
}

//��·���е�\ת��Ϊ/��ֹ�ڳ�����ת���ַ���Ӱ��
void replaceTo(char * ch)
{
	char * f = NULL;
	f = ch;
	while(*f)
	{
		if(*f == '\\')
			*f = '/';
		f++;
	}
}


//����������ַ������ж��Ƿ�Ϊint�ͣ�����atoi(name)��itoa(num,ch2,10)������Ҫ#include <stdlib.h>
int isInt(char * ch)
{
	int num;
	int lenth1;
	int lenth2;
	char temp[50];

	lenth1 = strlen(ch);
	num = atoi(ch);
	itoa(num,temp,10);
	lenth2 = strlen(temp);

	if(lenth1 == lenth2)
	{
		return 1;
	}else if(lenth1-lenth2 == 1){
		return -1;
	}else{
		return 0;
	}
}

//����һ���ַ����ж��Ƿ����Ҫ�����ڸ�ʽ20000819��������atoi(name)��isInt(ch)������Ҫ#include <stdlib.h>
int isDate(char * ch)
{
	//1900�꣬��ƽ�ꣻ2000�������ꣻ 2008��������
	char temp[5];
	char * p = NULL;
	char * s = NULL;
	int year = 0, mounth = 0, day = 0;
	int i = 0;
	if((isInt(ch) == 1) && (strlen(ch) == 8))
	{
		p = ch;
		s = temp;
		for(i = 0; i < 4; i++)
		{
			*s = *p;
			s++;
			p++;
		}
		year = atoi(temp);
		s = temp;
		for(i = 0; i < 2; i++)
		{
			*s = *p;
			s++;
			p++;
		}
		*s = '\0';
		mounth = atoi(temp);
		s = temp;
		for(i = 0; i < 2; i++)
		{
			*s = *p;
			s++;
			p++;
		}
		*s = '\0';
		day = atoi(temp);
		if(year < 1900 || year > 2022 || mounth < 1 || mounth > 12 || day < 1 || day > 31)
		{//
			return 0;
		}else{
			if((mounth == 4 || mounth == 6 || mounth == 9 || mounth == 11) && (day > 30))
			{
				//4��6��9��11����30��
				return 0;
			}
			if(((year%4 != 0) || ((year%100 == 0) && (year%400 != 0))) && (mounth == 2) && (day > 28))
			{//ƽ��2��28��
				return 0;
			}else if(((year%400 == 0) || ((year%4 == 0) && (year%100 != 0))) && (mounth == 2) && (day > 29))
			{//����2��29��
				return 0;
			}
		}
		return 1;
	}else{
		//printf("����������\n");
		return 0;
	}
}

//������������ݼ�Ӧ������ʲô���ͣ��ж������Ƿ�Ϸ�
int isInputPass(char * input, char * type)
{
	if(strcmp("int",type) == 0)
	{
		return isInt(input);
	}
	else if(strcmp("date",type) == 0)
	{
		return isDate(input);
	}
	else if(strcmp("varchar",type) == 0)
	{
		return 0;
	}
	else{
		printf("���Ͳ����ڣ�����������\n");
		return 0;
	}
}

/*ʵ��4*/
//�ж������Լ�������Ƿ����Ҫ�󣬷��ظ����͵��±꣬��������Ҫ�󣬷���-1
int judgeConstraint(char * constraint)
{
	int i = 0;	
	for(i = 0; i <= 5; i++ )
	{
		if(strcmp(constraint,constr[i].Constraint) == 0)
		{
			return i;
		}
	}
	return -1;
}

//�ж������Ƿ����Լ��
int isAccordConstraint(char * value,char * constraint,char *tableName,char * tableLocation)
{
	if(strcmp("0",constraint) == 0)
	{
		return 1;
	}
	else if(strcmp("PRIMARYKEY",constraint) == 0)
	{
		return isAccordPrimarykey(value,tableName,tableLocation);
	}
	else if(strcmp("NOTNULL",constraint) == 0)
	{
		if(strcmp("null",value) != 0)
			return 1;
	}
	else if(strcmp("CHECK(gender)",constraint) == 0)
	{
		if(strcmp("��",value) == 0 || strcmp("Ů",value) == 0)
			return 1;
	}
	else if(strcmp("CHECK(date)",constraint) == 0)
	{
		return isDate(value);
	}
	return 0;
}
//�ж��Ƿ����PRIMARYKEYԼ��
int isAccordPrimarykey(char * value,char *tableName,char * tableLocation)
{
	FILE * fp = NULL;
	char * p,*s;
	char line[100];
	char outputLine[200];
	char temp[50];
	char n[10];
	char ch,c[100];
	int fNum = 0;  //�м����ֶ�
	int recNum = 0;
	int i = 0,j = 0;
	char tableNameCopy[50];   //findLine()���������ı�ԭ�в�����ֵ��ʹ�ñ���
	
	strcpy(tableNameCopy,tableName);
	findLine("./structure/���ݿ����Ϣ.txt",tableName, line); //tableLocation
	//ͨ����������ѯ����ṹ���м����ֶΣ�
	//1.��ѯ���м����ֶΣ���ÿ���ֶε���Ϣ����ṹ��
	JieQu(line, 2 , n);
	fNum = atoi(n);
	fp = fopen(tableLocation,"a+");
	if(fgetc(fp) == EOF)
	{
		fclose(fp);
		return 1;
	}else{
		while(ch != EOF)//ѭ������ļ���ÿһ��
		{
			fseek(fp,-1,SEEK_CUR);
			fscanf(fp,"%[^\n]", c);
			JieQu(c, 1, n);
			if(strcmp(n,value) == 0)
			{
				fclose(fp);
				return 0;
			}
			if(fgetc(fp) == '\n')//�������з�������
			ch = fgetc(fp);		
		}
	}
	fclose(fp);
	return 1;
}