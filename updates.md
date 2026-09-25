---
layout: page
title: 全部更新
description: AI Storage 面试教程的内容更新归档。每条链接指向当前版本中对应的章节。
permalink: /updates/
---

这里记录已发布的**教程内容更新**，按北京时间排列。每条链接指向当前教程的对应位置；旧章节可能已被后来修订。站点样式、按钮和构建修复不计入内容更新。

## 2026-09-25 · KV Cache × S3 over RDMA × GPU Data Path 贯穿案例

- **冷 Prefix 的存储决策：**[如何区分活跃 Decode、P/D 即时交接与 S3 冷层复用]({{ site.baseurl }}/docs/01_AI_Storage_KV_Cache/#kv-cold-prefix-decision)，比较恢复与重算的完整代价。
- **S3→GPU 数据路径：**[一次冷 KV Range GET 到 GPU Ready]({{ site.baseurl }}/docs/02_GPU_Data_Path/#s3-range-get-gpu-ready)，涵盖服务端读取/EC、TCP→Host→GPU 与双方支持时的 RDMA→GPU 路径、buffer 生命周期、校验和设备可见。
- **系统设计与项目表达：**[1 GiB 冷 Prefix 恢复请求]({{ site.baseurl }}/docs/03_System_Design_Interview_Demo/#kv-cold-prefix-request)、[ECS/ObjectScale 经验如何回答追问]({{ site.baseurl }}/docs/03_System_Design_Interview_Demo/#ecs-objectscale-experience)、[真实 S3→Host Range GET probe 规格]({{ site.baseurl }}/docs/03_System_Design_Interview_Demo/#s3-host-probe)。
- **配套训练：**[闭卷链 H：冷 KV 怎样经 S3 进入 GPU]({{ site.baseurl }}/docs/00_Interview_Drills/#cold-kv-drill)。

## 2026-09-23 · 闭卷训练与来源台账

- **闭卷训练册：**新增 [Day 0 诊断]({{ site.baseurl }}/docs/00_Interview_Drills/#day0-diagnosis)、[按知识链组织的连续追问]({{ site.baseurl }}/docs/00_Interview_Drills/#drill-chains)、随机算题、英文口述和错题复测；9 月 25 日又补入链 H。
- **技术证据：**新增[技术来源与版本台账]({{ site.baseurl }}/SOURCES.html)，集中说明高变化技术的引用边界和实验环境记录方法。

## 2026-09-20 · 主教程补深

- **KV Cache：**增加 [6K Prefix 命中与 TTFT 算例]({{ site.baseurl }}/docs/01_AI_Storage_KV_Cache/#kv-prefix-hit-ttft)、[增长后 OOM 的容量推导]({{ site.baseurl }}/docs/01_AI_Storage_KV_Cache/#kv-capacity-growth)、[block table 共享与回收]({{ site.baseurl }}/docs/01_AI_Storage_KV_Cache/#kv-block-lifecycle)、[恢复与重算的带宽临界判断]({{ site.baseurl }}/docs/01_AI_Storage_KV_Cache/#kv-restore-recompute-budget)。
- **GPU Data Path：**增加 [C++ 异步 buffer 生命周期代码题]({{ site.baseurl }}/docs/02_GPU_Data_Path/#gpu-buffer-lifetime)、[双缓冲时间轴]({{ site.baseurl }}/docs/02_GPU_Data_Path/#gpu-double-buffer)、[verbs 工作过程]({{ site.baseurl }}/docs/02_GPU_Data_Path/#rdma-verbs-lifecycle)、[GPU 内 KV 布局转换]({{ site.baseurl }}/docs/02_GPU_Data_Path/#gpu-kv-layout)、[性能证据追问]({{ site.baseurl }}/docs/02_GPU_Data_Path/#gpu-path-performance)。
- **系统设计：**增加 [可计算的部署规模]({{ site.baseurl }}/docs/03_System_Design_Interview_Demo/#kv-system-capacity)、[目录粒度]({{ site.baseurl }}/docs/03_System_Design_Interview_Demo/#kv-directory-granularity)、[重算风暴预算]({{ site.baseurl }}/docs/03_System_Design_Interview_Demo/#kv-recompute-storm)、[故障反例]({{ site.baseurl }}/docs/03_System_Design_Interview_Demo/#kv-system-diagnostics)与[最小可讲 Demo 实验]({{ site.baseurl }}/docs/03_System_Design_Interview_Demo/#kv-demo-experiments)。

## 2026-09-20 · 三篇主教程首次发布

- [AI Storage 与 KV Cache]({{ site.baseurl }}/docs/01_AI_Storage_KV_Cache/)：LLM 工作负载、KV 容量、分页、复用与冷层取舍。
- [GPU Data Path]({{ site.baseurl }}/docs/02_GPU_Data_Path/)：CUDA 数据搬运、RDMA、GPUDirect 与 S3 对象路径。
- [系统设计与面试指南]({{ site.baseurl }}/docs/03_System_Design_Interview_Demo/)：KV 分层系统设计、失败处理、性能模型和 Demo 规格。

---

[返回首页]({{ site.baseurl }}/)
