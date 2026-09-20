# AI Storage 一个月面试冲刺教程

> 面向有多年分布式对象存储经验、主要使用 Java 的工程师。  
> 面试版修订：2026-09-20 · 中文教程，保留英文术语与英文短答。
>
> 目标：约一个月后开始投递 AI Storage / GPU Data Path / 高性能对象存储相关岗位。

## 从这里开始

| 顺序 | 主教程 | 读完应能做什么 |
|---|---|---|
| 1 | [AI Storage & KV Cache Interview Crash Course](01_AI_Storage_KV_Cache.md) | 从 LLM workload 推导 KV 容量、热度、分页、复用与层级选择 |
| 2 | [GPU Data Path: CUDA, RDMA, GPUDirect and S3](02_GPU_Data_Path.md) | 解释 Storage→GPU 的每一步、copy/DMA、ownership 与完成顺序 |
| 3 | [AI Storage System Design & Interview Guide](03_System_Design_Interview_Demo.md) | 完成分布式 KV 系统设计，回答 30 个必答题，并拿到 Demo 实现规格 |

**这三份正文就是教程。** 官方链接用于核对事实与版本，不要求另读完整 CUDA/RDMA 文档才能理解。没有重写 ECS/ObjectScale 架构，也没有把任务扩成完整 AI Infra 课程。

文档采用 Markdown，便于搜索、改写、做笔记和交给 Codex CLI。图用 Mermaid，表格和公式可直接阅读；在支持 Mermaid 的预览中显示为架构/时序图。三份主教程合计含 **22 组 Interview Check、17 张 Mermaid 图、30 道必答题**。Demo 只有设计与接口契约，没有完整实现代码。

## 深度规则

| 标记 | 本月要求 |
|---|---|
| MUST KNOW | 能口述、画路径、完成算例，并接住 2～3 层追问 |
| SHOULD KNOW | 理解机制与取舍，能参与工程讨论 |
| NICE TO KNOW | 知道名字对应什么、改变了哪条假设 |
| SKIP FOR NOW | 本月不投入，除非目标 JD 明确需要 |

第二篇必要 C++ 中，**pointer/ownership/lifetime/RAII 与异步资源回收应按 MUST KNOW 对待**；其余语法只学到能读 CUDA/RDMA 示例。不要把一个月花成 C++ 语言进修。

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
| 架构图画完就算设计 | 第三篇 §1.5、§3.6、§5.6：四 worker 容量、目录粒度、重算风暴预算 |
| 三个独立问题当三层追问 | 第三篇 §7.2、§7.5：改变数字/条件，沿同一问题继续判断 |

以上案例的时间、成本和小模型配置均明确为教学输入；不把推演结果当厂商实测。已有 30 道答案卡仍作索引，深度练习集中到七条知识链，见第三篇 §7.6。

## 按目标 JD 调整最后一周的重点

下面是基于职责的阅读映射，不代表当前职位数量或招聘趋势；实际要求以你拿到的 JD 为准。前两周仍共用 KV 与数据路径基础。

| JD 主要强调的职责 | 加深重点 | 本月可以降低优先级 |
|---|---|---|
| Dataset、checkpoint、分布式存储、对象读写吞吐 | 第一篇 §2；第二篇 §7～8；已有 recovery/rebalance 项目 | 高级 KV policy、完整推理引擎集成 |
| KV serving、prefix reuse、inference storage | 第一篇 §3～7；第三篇容量、调度、失败与系统设计 | verbs 建链参数、CUDA kernel 实现 |
| GPU data movement、RDMA、C++ systems | 第二篇 ownership、Stream/Event、MR/QP/CQ、GPUDirect；短代码阅读 | 更多模型架构、复杂冷层缓存策略 |

如果 JD 明确把现代 C++、CUDA 或 verbs 实操列为核心考核，本教程只能补齐机制与系统判断，仍需专门编码练习。将重点落在真实要求上，比试图同时达到所有岗位的实现深度更适合一个月准备期。

## 30 天怎么使用这些正文

按每天约 3 小时安排，共约 90 小时；可与投递并行。每天建议约 90 分钟读正文、60 分钟闭卷画图/算题/口述、30 分钟纠错。官方资料主要用于核对特定边界，避免阅读时间不断挤占输出练习。

| 日期 | 直接学习的章节 | 当日/阶段可检查的输出 |
|---|---|---|
| Day 1～3 | 第一篇 §1～3 | 画 token→attention→KV；讲清训练/推理、Prefill/Decode |
| Day 4～5 | 第一篇 §4 | 手算 FP16/FP8、MHA/GQA/MQA、10/100 并发与共享收益 |
| Day 6～7 | 第一篇 §5～7 | 解释 paging、prefix 与 offload；给出对象层适合/不适合的情景 |
| Day 8～10 | 第二篇 §1～2 | 画拓扑；把 Java 引用思维切换到 buffer ownership/lifetime |
| Day 11～12 | 第二篇 §3～4 | 解释 pinned/Async/Stream/Event 与 A/B/C 三条路径 |
| Day 13～14 | 第二篇 §5 | 从 PD/MR/QP/CQ 讲完一次 READ/WRITE 与 completion |
| Day 15～16 | 第二篇 §6～8 | 区分 GDR/GDS/cuObject；画两种 S3 GET；解释如何证实路径 |
| Day 17～19 | 第三篇 §1～3 | 设计 key、目录与一次恢复，解释 READY 和 lease |
| Day 20～21 | 第三篇 §4～5 | 讲清缓存策略、prefetch、late DMA 与故障降级 |
| Day 22～23 | 第三篇 §6～7 | 完成 64 MB 计算和一次 45 分钟 system design 演练 |
| Day 24～26 | 第三篇 §7.2、§7.5、§8.11 | 连续追问、两次计时设计；读懂三个 Demo 实验，额外有余力再做 M0 |
| Day 27～28 | 第三篇 §7.6 的七条知识链 + 已有项目材料 | 随机改数字重算；准备迁移/恢复、性能排障、partial failure 三张项目卡 |
| Day 29～30 | 第三篇 §9、目标 JD、简历表述、投递 | 根据闭卷弱项补课，明确已有经验与模拟/实机证据边界 |

若只有 60 小时：优先 MUST KNOW；C++ 语法表只读懂；Demo 不做真实硬件扩展；跳过 NICE TO KNOW。不能删掉的内容是 **KV 计算、恢复 vs 重算、buffer lifetime、RDMA completion、S3 语义与失效路径**。

Demo 默认只读设计。若选择实现，优先用熟悉的 Java 完成三个小实验，C++/CUDA/RDMA 后端后续单独验证；不同时开展语言迁移、推理框架集成和硬件部署。

## 三个贯穿算例

1. 假设模型 `L=32, Hkv=8, D=128, FP16`：每 token 128 KiB；8,192 tokens 为 1 GiB/request；100 个未共享请求为 100 GiB payload。
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
