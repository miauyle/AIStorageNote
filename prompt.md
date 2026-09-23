# 任务：为有对象存储经验的工程师生成一套「AI Storage 一个月面试冲刺教程」

我要在大约 **1 个月内开始投递 AI Storage / GPU Data Path / AI Infrastructure Storage / 高性能对象存储相关岗位**。

请不要给我一个“学习路线、书单、课程推荐”然后让我自己去学。

**你输出的内容本身就必须是教程。**

也就是说：

- 直接解释概念；
- 直接给例子；
- 直接画数据路径和架构图；
- 直接比较不同技术；
- 直接告诉我面试需要理解到什么深度；
- 直接给典型面试问题和答案；
- 必要时引用当前官方文档；
- 不要把主要篇幅浪费在推荐书籍、博客、视频和学习资源上。

---

# 一、我的背景

我是有多年经验的软件开发工程师，之前主要做 Dell EMC ECS / ObjectScale 分布式对象存储。

已有比较扎实的背景包括：

- Distributed Object Storage
- S3
- object / chunk
- metadata
- data placement
- replication
- Geo replication
- CRR
- recovery
- rebalance
- data migration
- distributed background tasks
- failure handling
- large-scale data movement
- distributed systems
- production issue/debugging

ECS / ObjectScale 本身的架构、team 内部模块、线上问题、项目经验等，我已经有完整资料。

**不要重新给我写 ECS / ObjectScale 教程。**

只需要假设：

> I already have a strong distributed object storage background.

需要的时候，可以用传统对象存储概念来帮助我理解 AI Storage，但不要重新介绍 ECS/ObjectScale 产品细节。

我的主要开发语言过去是 Java。

C++ 不是我的主要语言。

---

# 二、目标岗位和学习目标

我的目标不是成为：

- Machine Learning Researcher
- LLM Algorithm Engineer
- CUDA Kernel Performance Engineer
- C++ Language Expert
- RDMA Driver Developer

我的目标是：

> 从一个有多年 Object Storage / Distributed Storage 背景的工程师，快速迁移到 AI Storage / GPU Data Path / GPU-aware Object Storage 方向。

我希望一个月后能够：

1. 看懂 AI Storage 相关 Job Description。
2. 在面试中解释 AI workload 为什么需要新的 storage architecture。
3. 理解 LLM training / inference 对 storage 和 memory hierarchy 的要求。
4. 深入理解 KV Cache。
5. 理解 GPU memory、HBM、CPU DRAM、NVMe、remote storage 的关系。
6. 理解 CUDA 中和 data movement 相关的核心知识。
7. 理解 RDMA。
8. 理解 GPUDirect RDMA / GPUDirect Storage。
9. 理解 S3-compatible Object Storage 如何与 GPU / RDMA data path 结合。
10. 能够进行 AI Storage system design。
11. 能够连续回答面试官 2～3 层技术追问。
12. 后续能够基于这些知识，用 Codex CLI 开发一个：

KV Cache + GPU Data Path + S3/RDMA 模拟 Demo。

---

# 三、最重要的原则：面试导向，而不是百科全书

整个教程必须严格控制范围。

目标是：

> 1 个月冲击面试。

而不是：

> 半年系统学习整个 AI Infrastructure。

所有内容请标记：

- MUST KNOW
- SHOULD KNOW
- NICE TO KNOW
- SKIP FOR NOW

其中：

MUST KNOW：
必须能在面试中解释，并能回答至少 2～3 层追问。

SHOULD KNOW：
知道工作原理，能够参与讨论。

NICE TO KNOW：
知道是什么即可。

SKIP FOR NOW：
即使重要，但一个月准备期内投入产出比太低。

不要因为某个主题很大，就无限展开。

例如：

不要因为介绍 Transformer，就写成机器学习教材。

不要因为介绍 CUDA，就写成 CUDA Programming Guide 重制版。

不要因为介绍 RDMA，就深入 Linux kernel / RNIC driver。

不要因为介绍 C++，就写成 C++ Primer。

---

# 四、整个教程只围绕三条主线

## 主线 1：LLM / AI Storage

重点回答：

> AI workload 到底为什么需要 Storage？

必须包括：

### LLM 最小知识

只讲 Storage Engineer 真正需要知道的：

- token
- embedding
- Transformer 最小结构
- Attention
- Q / K / V
- 为什么 K/V 可以缓存
- MHA
- MQA
- GQA

数学推导控制在最低程度。

重点不是公式，而是：

> 这些设计为什么影响 memory usage 和 storage workload？

---

### Training vs Inference

必须理解：

Training：

- dataset
- data loader
- model weights
- gradients
- optimizer state
- checkpoint
- distributed training
- checkpoint read/write
- dataset access pattern

Inference：

- model loading
- Prefill
- Decode
- KV Cache
- batching
- long context
- prefix reuse

重点解释：

> training 和 inference 为什么产生完全不同的 storage / memory workload。

---

### KV Cache

这是整个教程最重要的主题之一。

必须详细但紧凑地讲清楚：

- KV Cache 是什么
- 为什么存在
- 为什么 Q 不需要像 K/V 一样长期缓存
- Prefill 如何产生 KV Cache
- Decode 如何读取 KV Cache
- KV Cache lifetime
- KV Cache layout
- KV Cache block
- KV Cache 大小如何估算
- num_layers
- num_kv_heads
- head_dim
- token count
- datatype
- MHA / GQA / MQA 对 KV Cache 大小的影响

必须给出实际计算例子。

例如：

给一个假设模型：

- layers
- KV heads
- head dimension
- context length
- FP16 / FP8

算出：

> 一个 request 的 KV Cache 大约多大。

然后进一步计算：

> 10 / 100 个并发请求会占多少 GPU HBM。

让我真正建立 capacity intuition。

---

### KV Cache management

必须理解：

- allocation
- fragmentation
- blocks
- paging
- eviction
- reuse
- prefix caching
- offloading
- prefetch
- promotion
- tiering

重点介绍：

- vLLM PagedAttention
- KV blocks
- block table
- prefix caching
- KV Cache offloading

不要深入 vLLM 全部源码。

目标是能够解释：

> 为什么 PagedAttention 对 KV Cache memory management 有帮助？

---

### KV Cache tiering

重点讨论：

GPU HBM

↓

CPU DRAM / pinned memory

↓

Local NVMe

↓

Remote cache / remote storage

↓

Object Storage

分析不同 tier：

- latency
- bandwidth
- capacity
- cost
- persistence
- sharing
- failure characteristics

重点讨论：

> Object Storage 是否适合作为 KV Cache 的某一级？

不要默认答案一定是“适合”。

需要分析 tradeoff。

例如：

- latency 是否太高
- block size
- prefetch
- reuse probability
- remote sharing
- cold KV Cache
- long-lived prefix
- disaggregated inference

---

# 五、主线 2：GPU Data Path

整个部分的核心问题是：

> Data 到底怎么进入 GPU？

请从最简单路径逐渐演进。

例如：

Storage

↓

CPU / Kernel

↓

Host DRAM

↓

PCIe

↓

GPU HBM

然后不断分析：

> 哪些 copy 可以减少？

---

## GPU memory hierarchy

必须讲：

- GPU
- HBM
- CPU DRAM
- PCIe
- NVLink
- host memory
- device memory

重点理解：

> bandwidth、latency、capacity 的数量级差异。

尽量给现代 GPU / PCIe / NIC 的合理数量级例子。

涉及具体硬件数字时，必须注明型号和来源。

---

## CUDA：只讲 AI Storage 真正需要的

我的 CUDA 学习目标不是 kernel optimization。

重点是：

- host vs device
- kernel 的基本概念
- thread
- block
- grid

但重点放在：

- cudaMalloc
- cudaFree
- cudaMemcpy
- cudaMemcpyAsync
- CUDA Stream
- CUDA Event
- pageable memory
- pinned memory
- cudaMallocHost / cudaHostAlloc
- DMA
- asynchronous transfer
- memory synchronization
- Unified Virtual Addressing
- Unified Memory 基础

重点解释下面几个 data path：

### Path A

pageable CPU memory

→ staging / pinned buffer

→ DMA

→ GPU

### Path B

pinned CPU memory

→ DMA

→ GPU

### Path C

NIC / Storage device

→ direct DMA

→ GPU memory

每一步说明：

- CPU 做什么
- kernel 做什么
- NIC / GPU / DMA engine 做什么
- 是否产生 memory copy
- latency/bandwidth bottleneck 在哪里

---

# 六、C++：只作为 CUDA / RDMA / systems programming 的工具

不要单独写一本完整 C++ 教程。

假设我是 Java Developer。

只讲后续 CUDA / RDMA / GPU Data Path 会真正遇到的 C++。

必须包括：

- pointer
- reference
- const
- stack
- heap
- object lifetime
- ownership
- RAII
- destructor
- unique_ptr
- shared_ptr
- move semantics
- vector
- unordered_map
- class / struct
- enum
- lambda
- template basic
- thread
- mutex
- atomic
- alignment

重点使用：

Java → C++ 对比。

例如：

Java：

Foo foo = new Foo();

解释对应到 C++ 为什么可能出现：

Foo foo;

Foo\* foo;

Foo& foo;

std::unique_ptr

std::shared_ptr

Foo&&

不要只讲语法。

重点讲：

> ownership、lifetime 和 memory。

并且每个 C++ 特性都要说明：

> 为什么 CUDA / RDMA / storage systems code 会用到它？

---

# 七、RDMA

目标不是成为 RDMA driver developer。

目标是能够完整解释一个 RDMA data path。

必须讲清楚：

- InfiniBand
- RoCEv2
- RNIC / HCA
- userspace verbs
- kernel bypass 的准确含义
- memory registration
- pinned memory

核心对象：

- PD
- MR
- QP
- CQ
- WR
- WQE
- SGE
- lkey
- rkey

核心 operation：

- SEND / RECV
- RDMA READ
- RDMA WRITE

必须重点解释：

### 为什么 RDMA 快？

不要简单回答：

> 因为 bypass CPU。

请具体解释：

- fewer copies
- reduced kernel involvement
- RNIC DMA
- registered memory
- one-sided operations
- reduced software stack overhead

并说明：

> CPU 并不是完全不存在。

---

## TCP vs RDMA

需要画出：

TCP data path

和

RDMA data path

并比较：

- data copies
- kernel involvement
- CPU utilization
- latency
- throughput
- connection/state management
- failure/debug complexity

---

# 八、GPUDirect

必须明确区分：

- GPUDirect P2P
- GPUDirect RDMA
- GPUDirect Storage

不要混成一个概念。

重点讲：

## GPUDirect RDMA

NIC

↓

PCIe

↓

GPU HBM

解释：

- GPU memory registration
- peer memory
- RNIC 如何 DMA 到 GPU memory
- CPU 是否仍参与 control path
- data path 和 control path 区别

---

## GPUDirect Storage

重点解释：

传统：

Storage

→ CPU bounce buffer

→ GPU

和：

Storage

→ DMA

→ GPU

有什么区别。

同时说明：

GPUDirect RDMA 和 GPUDirect Storage：

- 相同点
- 不同点
- 使用场景

---

# 九、S3 / Object Storage + RDMA + GPU

这是整个教程最贴近我过去 Object Storage 背景的部分。

不要重新介绍基础 S3。

重点从传统 S3 GET 开始。

例如：

Application

↓

HTTP / S3

↓

TCP/IP

↓

Object Storage

↓

response payload

↓

CPU memory

↓

GPU copy

↓

GPU

然后分析：

> 如果需要高带宽 GPU data path，这里面哪里是瓶颈？

---

## S3 + RDMA

请非常严谨。

不要简单声称：

> S3 over RDMA 是一个统一标准协议。

必须区分：

- S3 API / semantics
- HTTP control path
- data plane
- RDMA transport
- vendor implementation
- architecture design

重点讨论：

> S3 control plane + RDMA data plane

这种 architecture。

---

## NVIDIA cuObject

如果当前官方 NVIDIA 文档中 cuObject 仍是有效技术，请使用最新官方资料介绍。

重点讲：

- cuObject 解决什么问题
- 为什么 Object Storage 需要 GPU-direct path
- S3 GET
- S3 PUT
- RANGE GET
- multipart
- control plane
- RDMA data plane
- GPU buffer
- RDMA descriptor
- RDMA WRITE
- RDMA READ

必须画：

### Traditional S3 GET

和

### RDMA / GPU-aware S3 GET

的数据路径对比。

并解释：

> S3 semantics 为什么仍然有价值？

> 为什么不是直接把整个 S3 替换成一个 RDMA protocol？

---

# 十、主线 3：AI Storage System Design

完成前面知识后，把所有东西串起来。

设计一个概念系统：

LLM Inference

↓

KV Cache Manager

↓

GPU HBM Tier

↓

CPU DRAM Tier

↓

NVMe Tier

↓

Remote/Object Storage Tier

对象存储支持两类 data path：

1. traditional S3/TCP
2. RDMA / GPU-direct optimized path

---

## 必须讨论以下 system design 问题

### KV Cache key

如何唯一表示一块 KV Cache？

考虑：

- model
- model version
- layer
- token range
- prefix
- tenant
- precision
- hash

不需要设计成生产级标准，但要让我理解问题。

---

### Cache policy

讨论：

- hit
- miss
- eviction
- LRU 类策略
- frequency
- recency
- prefix popularity
- capacity
- TTL

---

### Prefetch

什么时候应该提前拉 KV block？

错误 prefetch 有什么成本？

---

### Promotion / Demotion

例如：

Object Storage

→ NVMe

→ CPU

→ GPU

以及：

GPU

→ CPU

→ Object Storage

---

### Failure

讨论：

- GPU failure
- node failure
- network failure
- object store failure
- stale KV
- partial transfer
- retry
- idempotency

可以利用 distributed storage 基础，但不要重新教学分布式系统。

---

### Performance model

必须能够粗略估算：

- KV block size
- transfer size
- network bandwidth
- transfer latency
- HBM bandwidth
- PCIe bandwidth

例如：

一个 64 MB KV block：

在 100Gbps / 200Gbps / 400Gbps 网络上理论传输时间是多少？

然后说明：

实际时间为什么高于理论值？

---

# 十一、面试导向输出格式

每个重要章节最后增加：

## Interview Check

包含：

### 30 秒回答

如果面试官问：

“What is KV Cache?”

如何在 30 秒内回答。

---

### 2 分钟回答

更完整解释。

---

### Deep Dive

面试官继续追问时：

Q1

Q2

Q3

分别应该怎么回答。

---

### Common Trap

列出容易答错的地方。

例如：

错误：

“RDMA 完全不需要 CPU。”

正确：

说明 control path / setup / completion 等仍然存在 CPU participation。

---

# 十二、必须重点回答的面试问题

至少包括：

1. What is KV Cache?
2. Why does LLM inference need KV Cache?
3. Prefill and Decode 有什么区别？
4. KV Cache 为什么会成为 HBM capacity bottleneck？
5. 如何计算 KV Cache 大小？
6. MHA、MQA、GQA 对 KV Cache 有什么影响？
7. PagedAttention 解决什么问题？
8. Prefix caching 是什么？
9. 为什么需要 KV Cache offloading？
10. GPU HBM、CPU DRAM、NVMe、Object Storage 如何组成 cache hierarchy？
11. Object Storage 适合存 KV Cache 吗？
12. CUDA pinned memory 为什么重要？
13. cudaMemcpy 和 cudaMemcpyAsync 有什么区别？
14. DMA 是什么？
15. PCIe 在 GPU data path 中扮演什么角色？
16. RDMA 为什么快？
17. RDMA READ 和 RDMA WRITE 有什么区别？
18. Memory Registration 为什么是 RDMA 的核心？
19. QP / CQ / MR 分别是什么？
20. TCP 与 RDMA data path 有什么区别？
21. GPUDirect RDMA 是什么？
22. GPUDirect Storage 是什么？
23. GPUDirect RDMA 和 GDS 有什么区别？
24. 为什么 Storage → CPU → GPU 可能成为瓶颈？
25. 什么叫 zero-copy / copy avoidance？
26. S3 如何与 RDMA data plane 结合？
27. control plane / data plane 为什么可以分离？
28. 为什么保留 S3 semantics？
29. 如何设计一个 distributed KV Cache storage system？
30. 如果要求支持 GPU-direct object storage，你会怎么设计？

---

# 十三、教程深度限制

这是非常重要的要求。

我只有大约 1 个月。

因此：

如果某知识：

- 面试极有可能遇到；
- 是理解后续技术的 prerequisite；
- 能帮助我发挥 Object Storage 背景优势；

就详细解释。

否则压缩。

特别不要深入：

- Transformer 数学推导
- backpropagation
- optimizer algorithm
- CUDA kernel 极限优化
- PTX
- SASS
- CUTLASS
- Tensor Core programming
- C++ template metaprogramming
- C++ ABI
- coroutine internals
- Linux RDMA kernel driver internals
- NIC firmware
- PCIe protocol electrical/link-layer details
- 大量机器学习算法

除非为了理解当前内容必须涉及。

---

# 十四、资料来源要求

涉及当前技术状态时，请搜索和使用最新官方资料。

优先级：

1. NVIDIA 官方文档
2. CUDA 官方文档
3. vLLM 官方文档 / GitHub
4. LMCache 官方文档 / GitHub
5. Linux / RDMA 官方或权威资料
6. PCI-SIG / InfiniBand / RoCE 权威资料
7. 主流厂商正式技术文档

特别注意：

CUDA、GPUDirect、GDS、cuObject、vLLM、LMCache 等发展很快。

不要根据旧知识直接写。

如果某项技术：

- experimental
- preview
- version-specific
- hardware-specific

必须明确注明。

区分：

- industry standard
- NVIDIA technology
- vendor implementation
- research idea
- architecture proposal

不要把它们混为一谈。

---

# 十五、最终生成的文档结构

请生成三份主教程。

---

# Document 1

# AI Storage & KV Cache Interview Crash Course

目标：

让我快速建立：

LLM workload → KV Cache → AI Storage

完整 mental model。

重点：

- training vs inference
- Prefill / Decode
- KV Cache
- PagedAttention
- Prefix Cache
- Offloading
- Tiering
- HBM / DRAM / NVMe / Object Storage
- AI Storage workload

这是主教程。

---

# Document 2

# GPU Data Path: CUDA, RDMA, GPUDirect and S3

目标：

让我理解：

Storage → GPU

的数据是怎么移动的。

重点：

- necessary C++
- CUDA memory
- pinned memory
- DMA
- PCIe
- RDMA
- GPUDirect RDMA
- GPUDirect Storage
- S3 control plane + RDMA data plane
- cuObject（如果当前仍有效）

不要写成完整 C++ 或 CUDA 教科书。

---

# Document 3

# AI Storage System Design & Interview Guide

目标：

把：

KV Cache

-

GPU

-

RDMA

-

S3

-

Object Storage

全部串起来。

大量使用：

- architecture diagram
- data path
- tradeoff
- interview question
- 30-second answer
- 2-minute answer
- follow-up questions

最终让我能够完成：

> Design a GPU-aware distributed KV Cache storage system.

这类面试 System Design。

---

# 十六、Demo 只做设计，不写完整代码

最后附一章：

# Demo Design

设计一个后续可以交给 Codex CLI 实现的小项目：

GPU KV Cache Object Store

大致结构：

KV Cache Manager

↓

GPU Tier

↓

CPU Tier

↓

Object Store Tier

Object Store 支持：

- S3DataPath
- MockRdmaDataPath

定义类似：

IDataPath

IKVCacheTier

模拟：

- lookup
- cache miss
- eviction
- offload
- prefetch
- promotion

第一版不要求真实 RDMA hardware。

后续可以扩展：

Mock RDMA

↓

libibverbs

↓

GPUDirect RDMA

↓

cuObject / GPU-aware Object Storage

只需要给：

- architecture
- components
- interfaces
- data flow
- development milestones
- benchmark plan

不要现在生成大量代码。

---

# 十七、写作风格

我是有经验的工程师。

不要使用：

“什么是电脑”“什么是网络”这种初级教学口吻。

风格应该是：

technical

direct

engineering-oriented

interview-oriented

用大量：

- diagram
- table
- data path
- comparison
- realistic examples

少写没有信息量的长篇叙述。

第一次出现一个概念时解释清楚。

之后直接使用。

当一个概念特别容易混淆时，用：

A vs B

的方式解释。

尽量建立完整 mental model，而不是堆名词。

---

# 十八、最终目标

整个教程最终应该让我形成这样一条完整知识链：

Object Storage

↓

AI Workload

↓

LLM Inference

↓

Prefill / Decode

↓

KV Cache

↓

GPU HBM

↓

KV Cache Tiering

↓

CPU DRAM / NVMe / Remote Storage

↓

CUDA Data Movement

↓

DMA / PCIe

↓

RDMA

↓

GPUDirect

↓

S3 + RDMA Data Path

↓

GPU-aware Object Storage

↓

AI Storage System Design

所有内容都围绕这个目标展开。

不要偏离成：

泛 AI 教程、泛 CUDA 教程或泛 C++ 教程。

记住最终目的：

**一个有多年 Object Storage 经验的软件工程师，在约 1 个月内形成参加 AI Storage / GPU Data Path 面试所需要的知识体系。**

