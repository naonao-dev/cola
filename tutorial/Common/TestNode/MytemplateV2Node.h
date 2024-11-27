/**
 * @FilePath     : /cola/tutorial/Common/TestNode/MytemplateV2Node.h
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-11-27 14:59:52
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-27 14:59:53
 * @Copyright (c) 2024 by G, All Rights Reserved.
 **/
#ifndef NAO_MYTEMPLATEV2NODE_H
#define NAO_MYTEMPLATEV2NODE_H

#include "../common.h"

template<int TID>
class MyTemplateV2Node : public nao::DNode
{
public:
    NStatus run() override
    {
        nao::NAO_ECHO("[MyTemplateV2Node] template id = [%d] ", TID);
        return NStatus();
    }
};

#endif   // NAO_MYTEMPLATEV2NODE_H