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
    mxml_node_t *xml = NULL, *call = NULL, *elm = NULL, *cc1 = NULL;
    char *elm_attr;
    int elm_i;
    char temp[10] = {0};

    fp = fopen(path, "r+");

    if(fp == NULL){//文件打开失败
        elog_w("warning", "xml open failed!\n");

        // mxml_node_t *new_xml = mxmlNewXML("1.0");
        // if(new_xml == NULL){
        //     elog_i("debug", "new_xml = null");
        //     return;
        // }

        // call = mxmlNewElement(new_xml, "call");
        // if(call==NULL){
        //     elog_i("debug", "call = null");
        //     return;
        // }

        // elm = mxmlNewElement(call, "cc1");
        // if(elm ==  NULL){
        //     elog_w("warning", "new cc1 failed");
        //     return;
        // }
        // mxmlElementSetAttr(elm, "cab", "1");
        // mxmlElementSetAttr(elm, "id", "1");

        // elm = mxmlNewElement(call, "cc2");
        // if(elm ==  NULL){
        //     elog_w("warning", "new cc2 failed");
        //     return;
        // }
        // mxmlElementSetAttr(elm, "cab", "8");
        // mxmlElementSetAttr(elm, "id", "1");

        // elm = mxmlNewElement(call, "master");{
        //     if(elm ==  NULL){
        //     elog_w("warning", "new master failed");
        //     return;
        // }
        // }
        // mxmlElementSetAttr(elm, "cab", "5");
        // mxmlElementSetAttr(elm, "id", "1");

        // elm = mxmlNewElement(call, "machinist");
        // if(elm ==  NULL){
        //     elog_w("warning", "new machinist failed");
        //     return;
        // }
        // mxmlElementSetAttr(elm, "cab", "5");
        // mxmlElementSetAttr(elm, "id", "2");

        // elm = mxmlNewElement(call, "alarm_report");
        // if(elm ==  NULL){
        //     elog_w("warning", "new alarm_report failed");
        //     return;
        // }
        // mxmlElementSetAttr(elm, "value", "1");

        // if((fp == fopen(path,"wb")) == NULL){
        //     elog_w("warning", "创建文件失败!");
        //     exit(1);
        // }

        // if(mxmlSaveFile(xml, fp, MXML_NO_CALLBACK) < 0){
        //     elog_w("warning", "save file failed!");
        //     exit(1);
        // }
        // fclose(fp);
        // mxmlDelete(xml);
    }
    else{//文件打开成功
        elog_d("debug", "xml open success!\n");

        if((xml = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK)) == NULL){
            elog_w("warning", "xml load failed!");
            return;
        }
        
        elog_d("debug", "xml load...");

        if((call = mxmlFindElement(xml, xml, "call", NULL, NULL, MXML_DESCEND)) == NULL){
            elog_w("warning","cant find element call");
            return;
        }

        if((elm = mxmlFindElement(call, xml, "cc1", NULL, NULL, MXML_DESCEND)) == NULL){
            elog_w("warning","cant find element cc1");
            return;
        }
        p_attr.cc1.cab = atoi(mxmlElementGetAttr(elm, "cab"));
        p_attr.cc1.id = atoi(mxmlElementGetAttr(elm, "id"));

        if((elm = mxmlFindElement(call, xml, "cc2", NULL, NULL, MXML_DESCEND)) == NULL){
            elog_w("warning","cant find element cc2");
            return;
        }
        p_attr.cc2.cab = atoi(mxmlElementGetAttr(elm, "cab"));
        p_attr.cc2.id = atoi(mxmlElementGetAttr(elm, "id"));

        if((elm = mxmlFindElement(call, xml, "master", NULL, NULL, MXML_DESCEND)) == NULL){
            elog_w("warning","cant find element master");
            return;
        }
        p_attr.master.cab = atoi(mxmlElementGetAttr(elm, "cab"));
        p_attr.master.id = atoi(mxmlElementGetAttr(elm, "id"));

        if((elm = mxmlFindElement(call, xml, "machinist", NULL, NULL, MXML_DESCEND)) == NULL){
            elog_w("warning","cant find element machinist");
            return;
        }
        p_attr.machinist.cab = atoi(mxmlElementGetAttr(elm, "cab"));
        p_attr.machinist.id = atoi(mxmlElementGetAttr(elm, "id"));

        if((elm = mxmlFindElement(call, xml, "alarm_report", NULL, NULL, MXML_DESCEND)) == NULL){
            elog_w("warning","cant find element alarm_report");
            return;
        }
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
        mxmlDelete(xml);
    }
}
