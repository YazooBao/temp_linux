#!/bin/sh  
<<<<<<< HEAD
Version="app version: 1.0"
Time=\""compile time: "$(date '+%Y-%m-%d %H:%M:%S')\"  
Output="#define BUILD_TIME     $Time\n#define BUILD_VERSION  \"$Version\""
echo "#define PRJ_NAME       \"linux demo app!\"" > version.h
echo "$Output" >> version.h
=======
Version="版本号: 1.0"
Time=\""编译时间: "$(date '+%Y-%m-%d %H:%M:%S')\"  
Output="#define BUILD_TIME     $Time\n#define BUILD_VERSION  \"$Version\""  
echo "#define PRJ_NAME       \"标动对讲服务程序\"" > version.h
echo -e "$Output" >> version.h
>>>>>>> master

## save svn revision.
svn_revision=`svn info | grep "Revision: " | sed 's/Revision: //'`
if [ ${#svn_revision} -ne 0 ]
then
    echo "#define SVN_VERSION    $svn_revision" >> version.h
else
    echo "#define SVN_VERSION    0" >> version.h
fi
