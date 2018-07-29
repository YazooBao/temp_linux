/************************************************************************************
 *@file name  : config.c
 *@brief      :
 *@author     : yazoo.bao
 *@mail       : 1027004184@qq.com
 *@creat time: 2018年 07月 27日 星期五 14:57:24 CST
************************************************************************************/
/*includes-------------------------------------------------------------------------*/
#include "mxml.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elog.h"
/*typedef--------------------------------------------------------------------------*/
struct _p_attr{
    int cab;
    int id;
};

struct p_attr_t{
    struct _p_attr cc1;
    struct _p_attr cc2;
    struct _p_attr master;
    struct _p_attr machinist;
    int alarm_report;
};
/*define---------------------------------------------------------------------------*/
/*macro----------------------------------------------------------------------------*/
/*variables------------------------------------------------------------------------*/
struct p_attr_t p_attr;
int alarm_report;
/*function prototype---------------------------------------------------------------*/
/*variables------------------------------------------------------------------------*/

/*! \brief config
 *
 *  config project
 *
 * \param char *path 配置文件路径
 * \return null
 */
void pr_config(char *path)
{
    FILE *fp;
    mxml_node_t *tree, *call, *elm;
    char *elm_attr;
    int elm_i;

    fp = fopen(path, "r+");

    if(fp == NULL){//文件打开失败
        elog_w("warning", "xml open failed!\n");
        elog_i("info", "creat xml...\n");

        // tree = mxmlNewXML("1.0");

        // call = mxmlNewElement(tree, "call");

        // elm = mxmlNewElement(call, "cc1");
        // mxmlElementSetAttr(elm, "cab", "1");
        // mxmlElementSetAttr(elm, "id", "1");

        // elm = mxmlNewElement(call, "cc2");
        // mxmlElementSetAttr(elm, "cab", "8");
        // mxmlElementSetAttr(elm, "id", "1");

        // elm = mxmlNewElement(call, "master");
        // mxmlElementSetAttr(elm, "cab", "5");
        // mxmlElementSetAttr(elm, "id", "1");

        // elm = mxmlNewElement(call, "machinist");
        // mxmlElementSetAttr(elm, "cab", "5");
        // mxmlElementSetAttr(elm, "id", "2");

        // elm = mxmlNewElement(call, "alarm_report");
        // mxmlElementSetAttr(elm, "value", "1");

        if((fp == fopen(path,"w+")) == NULL){
            elog_w("warning", "创建文件失败!");
            exit(1);
        }

        // if(mxmlSaveFile(tree, fp, MXML_NO_CALLBACK) < 0){
        //     elog_w("warning", "save file failed!");
        //     exit(1);
        // }
        fclose(fp);
        // mxmlDelete(tree);
    }
    else{//文件打开成功
        elog_d("debug", "xml open success!\n");

        tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
        elog_d("debug", "xml load...");

        call = mxmlFindElement(tree, tree, "call", NULL, NULL, MXML_DESCEND);

        elm = mxmlFindElement(call, tree, "cc1", NULL, NULL, MXML_DESCEND);
        p_attr.cc1.cab = atoi(mxmlElementGetAttr(elm, "cab"));
        p_attr.cc1.id = atoi(mxmlElementGetAttr(elm, "id"));

        elm = mxmlFindElement(call, tree, "cc2", NULL, NULL, MXML_DESCEND);
        p_attr.cc2.cab = atoi(mxmlElementGetAttr(elm, "cab"));
        p_attr.cc2.id = atoi(mxmlElementGetAttr(elm, "id"));

        elm = mxmlFindElement(call, tree, "master", NULL, NULL, MXML_DESCEND);
        p_attr.master.cab = atoi(mxmlElementGetAttr(elm, "cab"));
        p_attr.master.id = atoi(mxmlElementGetAttr(elm, "id"));

        elm = mxmlFindElement(call, tree, "machinist", NULL, NULL, MXML_DESCEND);
        p_attr.machinist.cab = atoi(mxmlElementGetAttr(elm, "cab"));
        p_attr.machinist.id = atoi(mxmlElementGetAttr(elm, "id"));

        elm = mxmlFindElement(call, tree, "alarm_report", NULL, NULL, MXML_DESCEND);
        p_attr.alarm_report = atoi(mxmlElementGetAttr(elm, "value"));

        elog_i("info", "cc1.cab:%d\n", p_attr.cc1.cab);
        elog_i("info", "cc1.id:%d\n", p_attr.cc1.id);

        elog_i("info", "cc2.cab:%d\n", p_attr.cc2.cab);
        elog_i("info", "cc2.id:%d\n", p_attr.cc2.id);

        elog_i("info", "master.cab:%d\n", p_attr.master.cab);
        elog_i("info", "master.id:%d\n", p_attr.master.id);

        elog_i("info", "machinist.cab:%d\n", p_attr.machinist.cab);
        elog_i("info", "machinist.id:%d\n", p_attr.machinist.id);

        elog_i("info", "alarm_report-value:%d\n", p_attr.alarm_report);
        
        fclose(fp);
        mxmlDelete(tree);
    }
}
