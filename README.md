# AI Storage 一个月面试冲刺教程

> 面向 Dell ECS/ObjectScale 工程师：Java 为复制/迁移/CRR 功能开发主力；Go 用于运行数据 telemetry 采集与统计，Python 曾用于工作。
> 面试主线修订：2026-09-25 · 中文教程，保留英文术语与英文短答。
>
> 目标：用约一个月准备 KV Cache × GPU Data Path × S3 over RDMA 交汇处的存储岗位，并在准备过程中开始针对性投递。

## 从这里开始

| 顺序 | 主教程 | 读完应能做什么 |
|---|---|---|
| 1 | [AI Storage & KV Cache Interview Crash Course](_docs/01_AI_Storage_KV_Cache.md) | 区分活跃 Decode、P/D 交接和冷 Prefix 复用；从容量、热度与重算成本决定是否存 S3 |
| 2 | [GPU Data Path: CUDA, RDMA, GPUDirect and S3](_docs/02_GPU_Data_Path.md) | 沿一次冷 KV Range GET 解释服务端读取、TCP/Host 或 RDMA/GPU 路径、buffer 寿命与完成条件 |
| 3 | [AI Storage System Design & Interview Guide](_docs/03_System_Design_Interview_Demo.md) | 把 1 GiB 冷 Prefix 恢复到 GPU Ready 的设计、预算、故障和个人 ECS/ObjectScale 项目回答串起来 |
| 配套 | [AI Storage 闭卷训练册](_docs/00_Interview_Drills.md) | Day 0 诊断、八条追问链、随机算题、英文口述与错题复测 |

**这三份正文就是教程。** 官方链接用于核对事实与版本，不要求另读完整 CUDA/RDMA 文档才能理解。没有重写 ECS/ObjectScale 架构，也没有把任务扩成完整 AI Infra 课程。

文档采用 Markdown，便于搜索、改写、做笔记和交给 Codex CLI。图用 Mermaid，表格和公式可直接阅读；在支持 Mermaid 的预览中显示为架构/时序图。三份主教程合计含 **22 组 Interview Check、17 张 Mermaid 图、30 道必答题**。Demo 只有设计与接口契约，没有完整实现代码。训练册与正文分开，第一次作答时不会直接看到标准答案。

**贯穿案例：**假设一个可复用的 8K Prefix 有 1 GiB KV payload，聚合为 16 个 64 MiB 逻辑块。先决定恢复还是重算，再区分 S3→Host→GPU 基线与双方支持时的 S3 控制请求 + RDMA→GPU 路径；最后证明完整性、布局与设备可见，才发布 GPU Ready。Prefill→Decode 的即时交接优先另行比较直接网络传输，活跃 Decode 不能默认逐 token 从对象层取 KV。

先看[从 ECS/ObjectScale 到 AI Storage 的前置地图](_docs/01_AI_Storage_KV_Cache.md#ecs-to-ai-storage)，明确 Java 主线、Go telemetry 与 GPU/RDMA 新知识的边界；不把复制、在线迁移或 CRR 直接当成 GPU 数据面经验。

如果已经掌握部分内容，不要从头顺序重读：先做训练册的 Day 0 诊断，按 0 分题回到对应章节。若全部达到 2 分，直接练八条追问链和目标 JD，而不是继续扩大阅读范围。

## 深度规则

| 标记 | 本月要求 |
|---|---|
| MUST KNOW | 能口述、画路径、完成算例，并接住 2～3 层追问 |
| SHOULD KNOW | 理解机制与取舍，能参与工程讨论 |
| NICE TO KNOW | 知道名字对应什么、改变了哪条假设 |
| SKIP FOR NOW | 本月不投入，除非目标 JD 明确需要 |

第二篇必要 C++ 中，**pointer/ownership/lifetime/RAII 与异步资源回收应按 MUST KNOW 对待**。面向 S3 over RDMA / GPU Data Path 岗位，若选择实现 Demo，另练 C++17/20 + CMake 的最小可编译 S3→host 链路；不把一个月花成完整 C++ 语言进修。

**MUST KNOW 约束的是回答能力，不是实现完整度。** 第三篇的长 schema、状态名和接口签名供阅读理解，不需要背诵或全量实现。各章“2 分钟回答”作为骨架，补一个本章算例/时序，再接一个条件变化即可；不是背稿计时任务。

| 深度 | 表现 | 本月安排 |
|---|---|---|
| 定义 | 能说明名词，容易停在一句话 | NICE TO KNOW 可停在这里 |
| 机制 | 能解释数据依赖、资源和完成顺序 | SHOULD KNOW 到这里 |
| 决策 | 能算数量级；输入或故障改变后会修改方案 | 核心 MUST KNOW 到这里 |
| 完整实现 | 驱动/API 细节、所有异常、全框架兼容和性能调优 | 除 JD 明确要求外，本月不追求 |

## 这次补深的内容，优先读哪里

| 原先容易停在结论的地方 | 现在可直接练的内容 |
|---|---|
| Prefix hit 就更快 | 第一篇 §3.5：6K 命中 + 2K 新输入，逐项计算 TTFT 与没省掉的读取 |
| KV 公式算完就结束 | 第一篇 §4.8：输出增长、token budget、为什么初始放得下仍会 OOM |
| PagedAttention 是非连续分配 | 第一篇 §5.5：两请求共享、增长、结束与物理页回收全过程 |
| Offload 取决于 tradeoff | 第一篇 §6.8：算临界带宽，并比较保存 KV 与保存重算输入 |
| RAII / Async 名字都认识 | 第二篇 §2.7、§3.7：看代码找 lifetime 错误，再走双缓冲时间轴 |
| QP/CQ/MR 各背一句 | 第二篇 §5.9：把 source、target、lkey、rkey、WR 与 CQ 对应起来 |
| GPU-direct 自动得到可用 KV | 第二篇 §7.8、§8.2：布局转换与有证据的性能排障 |
| KV、S3 和 GPU 各自会说却串不起来 | 第一篇 §7.4 → 第二篇 §7.9 → 第三篇 §1.6：沿同一冷 Prefix 走到 GPU Ready |
| 架构图画完就算设计 | 第三篇 §1.5、§3.6、§5.6：四 worker 容量、目录粒度、重算风暴预算 |
| 三个独立问题当三层追问 | 第三篇 §7.2、§7.5：改变数字/条件，沿同一问题继续判断 |

以上案例的时间、成本和小模型配置均明确为教学输入；不把推演结果当厂商实测。已有 30 道答案卡仍作索引，深度练习集中到八条知识链，见第三篇 §7.6。

## 按目标 JD 调整最后一周的重点

下面是基于职责的阅读映射，不代表当前职位数量或招聘趋势；实际要求以你拿到的 JD 为准。前两周仍共用 KV 与数据路径基础。

| JD 主要强调的职责 | 加深重点 | 本月可以降低优先级 |
|---|---|---|
| Dataset、checkpoint、分布式存储、对象读写吞吐 | 第一篇 §2；第二篇 §7～8；已有 recovery/rebalance 项目 | 高级 KV policy、完整推理引擎集成 |
| KV serving、prefix reuse、inference storage | 第一篇 §3～7；第三篇容量、调度、失败与系统设计 | verbs 建链参数、CUDA kernel 实现 |
| KV 冷层 + GPU Data Path + S3/RDMA 对象路径 | 第一篇 §7.4、第二篇 §7.9、第三篇 §1.6/§7.4/§8.12；闭卷链 H | 泛训练平台、完整模型源码与自写 RNIC driver |
| GPU data movement、RDMA、C++ systems | 第二篇 ownership、Stream/Event、MR/QP/CQ、GPUDirect；短代码阅读 | 更多模型架构、复杂冷层缓存策略 |

如果 JD 明确把现代 C++、CUDA 或 verbs 实操列为核心考核，本教程只能补齐机制与系统判断，仍需专门编码练习。将重点落在真实要求上，比试图同时达到所有岗位的实现深度更适合一个月准备期。

## 30 天怎么使用这些正文

按每天约 3 小时安排，共约 90 小时；**第二周起可小批量投递匹配 JD，并用实际追问修正学习重点**，无需等第 30 天。开始前先用训练册做 Day 0 诊断。每天建议约 90 分钟读正文、60 分钟闭卷画图/算题/口述、30 分钟纠错；错题按 D+1/D+3/D+7 复测。官方资料主要用于核对特定边界，避免阅读时间不断挤占输出练习。

| 日期 | 直接学习的章节 | 当日/阶段可检查的输出 |
|---|---|---|
| Day 1～3 | 第一篇 §1～3 | 画 token→attention→KV；讲清训练/推理、Prefill/Decode |
| Day 4～5 | 第一篇 §4 | 手算 FP16/FP8、MHA/GQA/MQA、10/100 并发与共享收益 |
| Day 6～7 | 第一篇 §5～7；训练册 Gate Day 7 | 解释 paging、prefix 与 offload；用 §7.4 选择冷 KV 恢复/重算，完成追问链 A/B |
| Day 8～10 | 第二篇 §1～2 | 画拓扑；把 Java 引用思维切换到 buffer ownership/lifetime |
| Day 11～12 | 第二篇 §3～4 | 解释 pinned/Async/Stream/Event 与 A/B/C 三条路径 |
| Day 13～14 | 第二篇 §5；训练册 Gate Day 14 | 从 PD/MR/QP/CQ 讲完一次 READ/WRITE；闭卷完成追问链 C/D |
| Day 15～16 | 第二篇 §6～8 | 区分 GDR/GDS/cuObject；按 §7.9 画两种 S3 GET 并标注 GPU Ready 的证据 |
| Day 17～19 | 第三篇 §1～3 | 沿 §1.6 设计 key、目录与一次 1 GiB 冷 Prefix 恢复，解释 READY 和 lease |
| Day 20～21 | 第三篇 §4～5；训练册 Gate Day 21 | 讲清缓存策略、late DMA；闭卷完成追问链 E/F |
| Day 22～23 | 第三篇 §6～7 | 完成 64 MB 计算和一次 45 分钟 system design 演练 |
| Day 24～26 | 第三篇 §7.2、§7.4、§8.11～8.12 | 连续追问、项目卡和计时设计；若选择实作，从 C++ M0 的真实 S3→host 基线开始 |
| Day 27～28 | 训练册八条知识链 + 已有项目材料 | 重点闭卷链 H；用真实材料准备 ECS 复制/迁移与 ObjectScale CRR 项目卡 |
| Day 29～30 | 训练册 Gate Day 29、第三篇 §9、已投 JD 的反馈 | 随机十题与英文口述；只根据错题补课 |

若只有 60 小时：优先 MUST KNOW，跳过 NICE TO KNOW；Demo 不做真实硬件扩展，也不以实现完成作为投递前提。目标 JD 若明确考 C++ 编码，留出时间写最小 C++ S3 Range GET/校验，而不是只读语法表。不能删掉的内容是 **KV 计算、恢复 vs 重算、buffer lifetime、RDMA completion、S3 语义与失效路径**。

Demo 默认先读设计；若选择实现，核心语言统一为 **C++17/20 + CMake**：M0 真实 S3→host Range GET/校验与耗时分解，M1 可选 C++ lease/失败状态机模拟。普通 C++ S3 SDK 不会自动实现 RDMA；真实 GPU/RDMA 路径需要支持的客户端、服务端和硬件。三类证据不得混成同一组实测结果，见[第三篇 §8.1](_docs/03_System_Design_Interview_Demo.md#cpp-demo-scope)。

## 三个贯穿算例

1. 假设模型 `L=32, Hkv=8, D=128, FP16`：每 token 128 KiB；8,192 tokens 为 1 GiB/request；100 个未共享请求为 100 GiB payload。正文中的 `8K/2K/32K tokens` 教学简称分别按 `8,192/2,048/32,768` 计算，除非另行注明。
2. 16-token page：全层共 2 MiB；聚合 32 pages 为 64 MiB transfer chunk。逻辑聚合不保证物理连续。
3. 64 MB 在 100/200/400 Gbps 上的纯传输下界：5.12/2.56/1.28 ms。64 MiB 要用另一组二进制大小计算，实际 load-to-ready 更高。

这些都是正文推导，不是厂商实测。始终区分 GB/GiB、Gbps/GB/s、单向/双向、理论/有效。

## 技术状态与证据口径

下表保留原版 2026-09-19 的核实口径。2026-09-20 修订重点复核 vLLM prefix caching、Transformers 缓存解释、CUDA 同步/异步、libibverbs MR/post-send、cuObject 与 LMCache 兼容性页面；其他条目不冒充当日全量复核。

| 主题 | 本次核对与分类 | 阅读时的边界 |
|---|---|---|
| S3 | 广泛兼容的对象 API 体系 | S3-compatible 不自动等于支持 RDMA 扩展 |
| PCIe、InfiniBand、RoCE | 标准/规范与相应硬件生态 | 不同层次；不能把某一套实现约束说成所有系统规则 |
| libibverbs | 用户态 RDMA 接口与开源生态 | 不是 wire protocol；操作能力取决于 transport/provider |
| CUDA / GPUDirect RDMA | NVIDIA 技术，在线参考显示 13.4 | 本文核对的是文档快照；安装时核对硬件/驱动/runtime |
| GDS | NVIDIA 技术与存储栈集成 | 功能与直达能力依文件系统、平台、版本；有 fallback |
| cuObject | NVIDIA 官方 client/server libraries | 2026-09-17 页面仍有效；client 从 Toolkit 13.1.1 起提供，当前文档要求 DC；不推定任意 S3 服务可用 |
| vLLM Paged Attention 页面 | 官方历史设计说明 | 页面明确不再代表全部当前实现；本教程学习原理 |
| vLLM prefix / offloading | 框架具体实现 | 后端、版本和布局有关，不把某篇旧 benchmark 当通用结果 |
| LMCache | 开源 KV 管理/复用层 | 当前文档有独立进程与 legacy 模式；兼容性是版本组合问题 |
| 本教程系统/Demo | Architecture proposal / simulation design | 不包装成行业标准、既有产品或已测性能 |

官方依据：[CUDA Runtime](https://docs.nvidia.com/cuda/cuda-runtime-api/api-sync-behavior.html)、[GPUDirect RDMA](https://docs.nvidia.com/cuda/gpudirect-rdma/index.html)、[GDS](https://docs.nvidia.com/gpudirect-storage/overview-guide/index.html)、[cuObject](https://docs.nvidia.com/gpudirect-storage/cuobject/index.html)、[vLLM 历史说明页](https://docs.vllm.ai/en/latest/design/paged_attention/)、[LMCache 兼容性](https://docs.lmcache.ai/getting_started/compatibility.html)。各正文还在相关事实附近给出直接来源。

在线文档会变化。后续实现时记录真实软件版本、GPU/NIC 型号、拓扑和配置；不要把本文件的核实日期当成对所有未来版本的承诺。

高变化技术的来源、核实边界与实验环境模板集中在 [技术来源与版本台账](SOURCES.md)。引用 `latest` 页面是为了便于查当前状态；需要复现实验时应另记 release/tag/commit。

## 最容易答错的十句话

| 错误表述 | 应纠正为 |
|---|---|
| KV 是模型权重 | KV 是输入相关的 attention 中间状态 |
| 有 KV 就不读历史 | 每步仍读取历史 K/V，只是避免重复构建 |
| PagedAttention 就是磁盘分页 | 它首先解决非连续 KV 块的分配与访问 |
| 对象层命中就更快 | 比较完整恢复到 GPU 的成本与重算 |
| Pinned memory 是更快的一种 DRAM | 是 page-locked 属性，作用在 DMA 与异步路径 |
| Async 返回说明已传完 | 提交、完成与设备可见是不同事件 |
| RDMA 完全无 CPU | CPU 通常仍处理控制、提交、完成与错误 |
| GPU pointer 可以直接发给 NIC | 需要受支持的注册、映射、权限与寿命 |
| S3 GET 对应 RDMA READ | 对象方向和 verbs 发起者方向不同 |
| 超时就能复用 target | 旧 DMA 可能迟到，必须安全 drain/撤销/隔离 |

## 可以开始投递的标准

能完成第三篇 §9 的闭卷验收，就可以开始针对相关 JD 投递。真实 RDMA、GPU-direct 和 cuObject 环境是后续扩展，不应成为本月无限延期的理由。职业表述保持清楚：已有对象存储生产经验是底座，新知识和 Demo 是迁移证据，未验证的硬件能力不要写成已落地成果。
