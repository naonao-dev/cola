/**
 * @FilePath     : /cola/tutorial/08/test_param_node.cpp
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-11-27 14:24:45
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-27 15:36:56
 * @Copyright (c) 2024 by G, All Rights Reserved.
 **/
#include "../Common/TestNode/MyReadParamNode.h"
#include "../Common/TestNode/MyWriteParamNode.h"
#include "../Common/common.h"
// ! 本例主要演示，参数传递功能
void tutorial_param()
{
    nao::DPipelinePtr pipeline = nao::DPipelineFactory::create();
    NStatus           status;
    nao::DElementPtr  a, b, c, d, e, f = nullptr;

    status += pipeline->registerDElement<MyReadParamNode>(&a, {}, "readNodeA");   // 读取param中的信息，不做修改
    status += pipeline->registerDElement<MyReadParamNode>(&b, {a}, "readNodeB");
    status += pipeline->registerDElement<MyWriteParamNode>(&c, {a}, "writeNodeC");
    status += pipeline->registerDElement<MyWriteParamNode>(&d, {a}, "writeNodeD", 2);   // 对param中的iValue值+1，循环执行2次
    status += pipeline->registerDElement<MyReadParamNode>(&e, {a}, "readNodeE");
    status += pipeline->registerDElement<MyWriteParamNode>(&f, {b, c, d, e}, "writeNodeF");
    if (!status.isOK()) {
        return;   // 使用时，请对所有CGraph接口的返回值做判定。本例子中省略
    }
    status += pipeline->init();

    for (int i = 0; i < 3; i++) {
        status += pipeline->run();
        std::cout << "[DAG] tutorial_param, loop : " << i + 1 << ", and run status = " << status.getCode() << std::endl;
    }
    /**
    auto param = pipeline->getGParam<MyParam>("param1");    // 可以获取pipeline内部的参数内容。但是需要考虑param的setup()和reset()的时机
    std::cout << "[DAG] get param from pipeline, iCount = " << param->iCount << std::endl;
    */
    status += pipeline->destroy();
    nao::DPipelineFactory::remove(pipeline);
}

#include "../Common/TestCondition/MyCondition.h"
#include "../Common/TestCondition/MyParamCondition.h"
#include "../Common/TestNode/HelloDagNode.h"
// !本例主要演示，根据条件进行逻辑判断的功能
void tutorial_condition()
{
    NStatus           status;
    nao::DPipelinePtr pipeline = nao::DPipelineFactory::create();
    nao::DElementPtr  a, b_condition, c, d_condition = nullptr;

    b_condition =
        pipeline->createDGroup<MyCondition>({pipeline->createDNode<MyNode1>(nao::DNodeInfo("conditionNodeB0", 1)),
                                             pipeline->createDNode<MyNode2>(nao::DNodeInfo("conditionNodeB1", 1)),
                                             pipeline->createDNode<MyNode1>(nao::DNodeInfo("conditionNodeB2", 1))});   // 生成 b_condition。执行的时候，根据choose()的返回值，在B0,B1,B2中选择一个执行

    d_condition = pipeline->createDGroup<MyParamCondition>({pipeline->createDNode<MyNode1>(nao::DNodeInfo("paramConditionNodeD0", 1)),
                                                            pipeline->createDNode<MyNode1>(nao::DNodeInfo("paramConditionNodeD1", 1)),
                                                            pipeline->createDNode<MyNode1>(nao::DNodeInfo("paramConditionNodeD2", 1))});

    status += pipeline->registerDElement<MyWriteParamNode>(&a, {}, "writeNodeA", 1);
    status += pipeline->registerDElement<MyCondition>(&b_condition, {a}, "conditionB", 1);
    status += pipeline->registerDElement<MyReadParamNode>(&c, {b_condition}, "readNodeC", 1);
    status += pipeline->registerDElement<MyParamCondition>(&d_condition, {c}, "conditionD", 1);
    if (!status.isOK()) {
        return;
    }
    status = pipeline->init();
    for (int i = 0; i < 3; i++) {
        status += pipeline->run();
        std::cout << "[CGraph] tutorial_condition, loop : " << i + 1 << ", and run status = " << status.getCode() << std::endl;
    }
    status = pipeline->destroy();
    nao::DPipelineFactory::remove(pipeline);
}


//! 本例主要演示，多个pipeline同时执行的情况
void tutorial_pipeline_1(nao::DPipelinePtr pipeline_1)
{
    if (nullptr == pipeline_1) {
        return;
    }
    nao::DElementPtr node1A, node1B, node1C = nullptr;
    pipeline_1->registerDElement<MyNode1>(&node1A, {}, "node1A");
    pipeline_1->registerDElement<MyNode1>(&node1B, {node1A}, "node1B");
    pipeline_1->registerDElement<MyNode1>(&node1C, {node1B}, "node1C");
    pipeline_1->process(5);   // 执行n次，本例中 n=5
}

void tutorial_pipeline_2(nao::DPipelinePtr pipeline_2)
{
    if (nullptr == pipeline_2) {
        return;
    }
    nao::DElementPtr node2A, node2B, node2C = nullptr;
    pipeline_2->registerDElement<MyNode2>(&node2A, {}, "node2A");
    pipeline_2->registerDElement<MyNode2>(&node2B, {node2A}, "node2B");
    pipeline_2->registerDElement<MyNode2>(&node2C, {node2A}, "node2C");
    pipeline_2->process(3);
}

void tutorial_pipeline_3(nao::DPipelinePtr pipeline_3)
{
    if (nullptr == pipeline_3) {
        return;
    }
    NStatus          status;
    nao::DElementPtr node3A, node3B, node3C, node3D = nullptr;
    nao::DElementPtr region = nullptr;
    node3A                  = pipeline_3->createDNode<MyNode1>(nao::DNodeInfo({}, "node3A", 1));
    node3B                  = pipeline_3->createDNode<MyNode2>(nao::DNodeInfo({node3A}, "node3B", 1));
    node3C                  = pipeline_3->createDNode<MyNode1>(nao::DNodeInfo({node3A}, "node3C", 1));
    node3D                  = pipeline_3->createDNode<MyNode1>(nao::DNodeInfo({node3B, node3C}, "node3D", 1));
    region                  = pipeline_3->createDGroup<nao::DRegion>({node3A, node3B, node3C, node3D});
    pipeline_3->registerDElement<nao::DRegion>(&region);
    pipeline_3->process(2);
}

void tutorial_multi_pipeline()
{
    nao::DPipelinePtr pipeline_1 = nao::DPipelineFactory::create();
    nao::DPipelinePtr pipeline_2 = nao::DPipelineFactory::create();
    nao::DPipelinePtr pipeline_3 = nao::DPipelineFactory::create();
    /**
     * 设置个别pipeline的内部 thread pool 资源信息，用以减少整体资源占用（可选）
     * 这里主要是为了说明，多个pipeline一起运行的时候，可以通过接口，针对个别pipeline进行调度资源的设置
     */
    nao::UThreadPoolConfig config;
    config.default_thread_size_ = 4;
    config.max_thread_size_     = 4;
    config.monitor_enable_      = false;
    nao::UThreadPool pool(true, config);   // 开辟一个4个线程的线程池，直接 init，并且参数设置为 config
    /**
     * 本例中，pipeline1 和 pipeline2 的并发逻辑相对简单
     * 通过如下接口，将这两个pipeline中的调度资源，修改为同一个线程池。
     * ps：注意，必须在 pipeline init之前，先init传入的线程池
     */
    pipeline_1->setSharedThreadPool(&pool);
    pipeline_2->setSharedThreadPool(&pool);
    /**
     * 经过上述的设置，pipeline1 和 pipeline2 共享同一个线程池，去调度其中的dag逻辑
     * pipeline3 没有设定，故使用自带的默认线程池完成自己的调度逻辑
     */
    std::thread thd1 = std::thread(tutorial_pipeline_1, pipeline_1);
    std::thread thd2 = std::thread(tutorial_pipeline_2, pipeline_2);
    std::thread thd3 = std::thread(tutorial_pipeline_3, pipeline_3);
    thd1.join();
    thd2.join();
    thd3.join();
    nao::DPipelineFactory::clear();
}

//! 本例主要演示，注册节点的时候，传入模板节点类型的情况
#include "../Common/TestNode/MyTemplateNode.h"
#include "../Common/TestNode/MyTemplateV2Node.h"
void tutorial_template()
{
    nao::DPipelinePtr                 pipeline = nao::DPipelineFactory::create();
    nao::DTemplateNodePtr<int, float> a        = nullptr;
    nao::DTemplateNodePtr<int, float> b        = nullptr;
    nao::DTemplateNodePtr<int>        c        = nullptr;
    nao::DElementPtr                  d        = nullptr;
    /**
     * 注册几个模板节点
     * 可以根据 MyTemplateNode 构造函数的不同，而实现不同的构造方式
     * 也可以参考 MyTemplateV2Node 的方式进行构造
     */
    pipeline->registerDElement<MyTemplateNode<int, float>>(&a, {}, 3, 3.5f);
    pipeline->registerDElement<MyTemplateNode<int, float>>(&b, {a}, 5, 3.75f);
    pipeline->registerDElement<MyTemplateNode<int>>(&c, {b}, 8);
    pipeline->registerDElement<MyTemplateV2Node<4>>(&d, {c});   // 也可以通过模板，传递参数数据

    pipeline->process();   // 运行pipeline
    nao::DPipelineFactory::remove(pipeline);
}

//! 本例主要演示，在element中添加切面的逻辑
#include "../Common/TestAspect/MyTemplateAspect.h"
#include "../Common/TestAspect/MyTimerAspect.h"
#include "../Common/TestAspect/MyTraceAspect.h"
void tutorial_aspect() {
    nao::DPipelinePtr pipeline = nao::DPipelineFactory::create();
    nao::DElementPtr a, b_region, c = nullptr;
    b_region = pipeline->createDGroup<nao::DRegion>({
        pipeline->createDNode<MyNode1>(nao::DNodeInfo({}, "nodeB1", 1)),
        pipeline->createDNode<MyNode2>(nao::DNodeInfo({}, "nodeB2", 2))
    });
    pipeline->registerDElement<MyNode1>(&a, {}, "nodeA", 1);
    pipeline->registerDElement<nao::DRegion>(&b_region, {a}, "regionB", 1);
    pipeline->registerDElement<MyNode1>(&c, {b_region}, "nodeC", 1);

    /** 针对node类型，添加 MyTraceAspect 切面逻辑 */
    a->addDAspect<MyTraceAspect>();
    /** 针对node类型，添加可变参的MyTemplateAspect 切面逻辑 */
    a->addDAspect<MyTemplateAspect<int, double>>(20, 7.0);
    /** 针对group类型，添加 MyTimerAspect 切面逻辑 */
    b_region->addDAspect<MyTimerAspect>();
    /**
     * 给特定的element，统一添加 MyTraceAspect 类型的切面
     * 不传参数，表示对pipeline内部所有的节点，添加该切面
     * */
    pipeline->addDAspect<MyTraceAspect>({b_region, c});
    pipeline->process();    // 运行pipeline
    nao::DPipelineFactory::remove(pipeline);
}

//!本例主要演示，在切面流程中加入自定义参数的逻辑
#include "../Common/TestAspect/MyConnAspect.h"
#include "../Common/TestParams/MyConnParam.h"
#include "../Common/TestAspect/MyPipelineParamAspect.h"
void tutorial_aspect_param() {
    nao::DPipelinePtr pipeline = nao::DPipelineFactory::create();
    MyConnParam paramA;
    paramA.ip_ = "127.0.0.1";
    paramA.port_ = 6666;
    MyConnParam paramB;
    paramB.ip_ = "255.255.255.255";
    paramB.port_ = 9999;
    nao::DElementPtr a, b, c = nullptr;
    pipeline->registerDElement<MyNode1>(&a, {}, "nodeA", 1);
    pipeline->registerDElement<MyNode2>(&b, {a}, "nodeB", 1);
    pipeline->registerDElement<MyWriteParamNode>(&c, {b}, "nodeC", 2);
    /** 给a节点添加 MyConnAspect 切面的逻辑，并且传入 paramA 相关参数 */
    a->addDAspect<MyConnAspect, MyConnParam>(&paramA);
    /** 给b节点添加多个切面，有的传递参数，有的不传递 */
    b->addDAspect<MyConnAspect, MyConnParam>(&paramB)->addDAspect<MyTimerAspect>();
    /** 在切面中，获取pipeline中的参数，并且进行对应处理 */
    c->addDAspect<MyPipelineParamAspect>();
    pipeline->process();
    nao::DPipelineFactory::clear();
}

int main()
{
    // tutorial_param();
    // tutorial_condition();
    // tutorial_multi_pipeline();
    //tutorial_template();
    //tutorial_aspect();
    tutorial_aspect_param();
    return 0;
}