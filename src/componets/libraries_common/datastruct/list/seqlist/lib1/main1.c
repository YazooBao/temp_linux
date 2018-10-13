#include <stdio.h>
#include <string.h>

#include "seqlist.h"

#define BUFSIZE 32
#define TELNUM	13

enum {INSERT=1, DELETE, SEARCH, SCAN, QUIT};
enum {ID=10, NAME, TEL};

struct stu_st {
	int id;
	char *name;
	char tel[TELNUM];
};

static void print_stu(const void *data)
{
	const struct stu_st *d = data;

	printf("%-3d%-20s%s\n", d->id, d->name, d->tel);
}

static int id_cmp(const void *data, const void *key)
{
	const struct stu_st *d = data;
	const int *k = key;

	return d->id - *k;
}

static int name_cmp(const void *data, const void *key)
{
	const struct stu_st *d = data;
	const char *k = key;

	return strcmp(d->name, k);
}

static int tel_cmp(const void *data, const void *key)
{
	const struct stu_st *d = data;
	const char *k = key;

	return strcmp(d->tel, k);
}

static void buf_clear(void)
{
	while (getchar() != '\n')
		;
}

int main(void)
{
	struct slist_st *stulist;
	struct stu_st tmp, *serh;
	char buf[BUFSIZE];
	int choose;
	int delchs, delid;

	stulist = seqlist_init(sizeof(struct stu_st));
	if (NULL == stulist) {
		fprintf(stderr, "seqlist_init() create failed\n");
		goto ERROR;
	}

	while (1) {
		system("clear");
		printf("       欢迎使用uplooking学生管理系统\n");
		printf("*****************************************\n");
		printf("*        1.插入信息	2.删除信息      *\n");
		printf("*        3.查找信息	4.显示信息      *\n");
		printf("*        5.退出系统                     *\n");
		printf("*****************************************\n");
		scanf("%d", &choose);
		switch (choose) {
			case INSERT:
				printf("输入要插入元素的id:\n");
				scanf("%d", &tmp.id);
				buf_clear();
				printf("name:\n");
				fgets(buf, BUFSIZE, stdin);
				if (buf[strlen(buf)-1] != '\n') {
					buf_clear();
				}
				buf[strlen(buf)-1] = '\0';
				tmp.name = strdup(buf);
				printf("telephone:");
				fgets(tmp.tel, TELNUM, stdin);
				if ((tmp.tel)[strlen(tmp.tel)-1] != '\n')
					buf_clear();
				(tmp.tel)[strlen(tmp.tel)-1] = 0;
				seqlist_insert(stulist, &tmp);	
				break;
			case DELETE:
				printf("10.学号 11.名字 12.电话号\n");			
				printf("请选择删除方式\n");
				scanf("%d", &delchs);
				buf_clear();
				switch (delchs) {
					case ID:
						printf("输入要删除学生的id\n");
						scanf("%d", &delid);
						seqlist_delete(stulist, &delid, id_cmp);
						break;
					case NAME:
						printf("输入要删除学生的name\n");
						fgets(buf, BUFSIZE, stdin);
						if (buf[strlen(buf)-1] != '\n') {
							buf_clear();
						}
						buf[strlen(buf)-1] = '\0';
						seqlist_delete(stulist, buf, name_cmp);
						break;
					case TEL:
						printf("输入要删除学生的tel\n");
						fgets(buf, TELNUM, stdin);
						if (buf[strlen(buf)-1] != '\n') {
							buf_clear();
						}
						buf[strlen(buf)-1] = '\0';
						seqlist_delete(stulist, buf, tel_cmp);
						break;
					default:
						break;
				}

				break;
			case SEARCH:
				serh = (void *)seqlist_search(stulist, "hello", name_cmp);			
				if (serh == NULL) {
					printf("没有你要找的成员\n");
				} else {
					printf("找到了，信息：%d %s %s\n", serh->id, serh->name, serh->tel);
				}
				sleep(3);
				break;
			case SCAN:
				seqlist_traval(stulist, print_stu);
				sleep(3);
				break;
			case QUIT:
				seqlist_destroy(stulist);
				goto QUITSY;
			default:
				fprintf(stderr, "请输入正确的选择\n");
				break;
		}
	}
QUITSY:
	printf("bye-bye\n");

	return 0;
ERROR:
	return 1;
}

