---
layout: home
title: AI Storage Notes
description: 面向对象存储工程师的 KV Cache、GPU Data Path 与 S3 over RDMA 面试教程。
hero:
  eyebrow: AI Storage 面试教程
  title: 从对象存储走向 AI 存储数据路径
  lead: >-
    沿一段可复用 KV 的完整链路，掌握容量与重算取舍、S3 冷层、GPU 数据搬运、失败恢复与系统设计。
  buttons:
    - { label: "开始学习", url: "/docs/01_AI_Storage_KV_Cache/", style: "btn--primary", icon: "fa-solid fa-rocket" }
    - { label: "先做诊断", url: "/docs/00_Interview_Drills/", style: "btn--ghost", icon: "fa-solid fa-pen-to-square" }
    - { label: "查看 GitHub", url: "https://github.com/miauyle/ai-storage-notes", style: "btn--ghost", icon: "fa-brands fa-github" }
features:
  - icon: "fa-solid fa-brain"
    title: KV Cache 基础
    text: 从 LLM workload 推导容量、复用、分页，以及冷 Prefix 何时值得保存和恢复。
    url: "/docs/01_AI_Storage_KV_Cache/"
  - icon: "fa-solid fa-microchip"
    title: GPU Data Path
    text: 沿 S3→Host/GPU 路径理解 CUDA、RDMA、GPUDirect 与异步资源生命周期。
    url: "/docs/02_GPU_Data_Path/"
  - icon: "fa-solid fa-diagram-project"
    title: 系统设计
    text: 完成冷 KV 从 S3 恢复到 GPU Ready 的设计、预算、故障与项目面试演练。
    url: "/docs/03_System_Design_Interview_Demo/"
  - icon: "fa-solid fa-pen-to-square"
    title: 闭卷训练
    text: 用诊断、追问链、随机算题和错题复测检验真实掌握程度。
    url: "/docs/00_Interview_Drills/"
sections:
  - title: 推荐学习顺序
    text: 不确定基础时先做 Day 0 诊断；再按弱项学习模型、数据路径与系统设计。
    links:
      - { label: "0. Day 0 诊断", url: "/docs/00_Interview_Drills/", icon: "fa-solid fa-pen-to-square", text: "先闭卷作答，再按弱项回到正文" }
      - { label: "1. AI Storage 与 KV Cache", url: "/docs/01_AI_Storage_KV_Cache/", icon: "fa-solid fa-book-open", text: "模型、容量、分页、复用与分层" }
      - { label: "2. GPU Data Path", url: "/docs/02_GPU_Data_Path/", icon: "fa-solid fa-arrow-right-arrow-left", text: "CUDA、RDMA、GPUDirect 与 S3" }
      - { label: "3. 系统设计与面试", url: "/docs/03_System_Design_Interview_Demo/", icon: "fa-solid fa-sitemap", text: "架构、故障、性能模型与 Demo" }
  - title: 最近更新
    text: 精选近期有实质内容变化的教程入口；按日期查看完整归档。
    links:
      - { label: "2026-09-25 · 从 ECS 到 AI Storage", url: "/docs/01_AI_Storage_KV_Cache/#ecs-to-ai-storage", icon: "fa-solid fa-link", text: "按 Java 功能开发和真实项目经验补齐 GPU 数据路径前置" }
      - { label: "2026-09-25 · 对象服务端 Range GET", url: "/docs/02_GPU_Data_Path/#s3-server-read-prereq", icon: "fa-solid fa-server", text: "从对象定位、后端读取走到 host buffer，再比较 RDMA" }
      - { label: "2026-09-25 · C++ Demo 规格", url: "/docs/03_System_Design_Interview_Demo/#cpp-demo-scope", icon: "fa-solid fa-code", text: "真实 S3→host 基线、模拟故障和硬件直达分开验证" }
      - { label: "2026-09-25 · 冷 Prefix 存储决策", url: "/docs/01_AI_Storage_KV_Cache/#kv-cold-prefix-decision", icon: "fa-solid fa-brain", text: "何时把可复用 KV 放进 S3，何时恢复或重算" }
      - { label: "2026-09-25 · 1 GiB 冷 KV 系统设计", url: "/docs/03_System_Design_Interview_Demo/#kv-cold-prefix-request", icon: "fa-solid fa-diagram-project", text: "贯穿容量预算、失败处理与 GPU Ready" }
      - { label: "查看全部更新 →", url: "/updates/", icon: "fa-solid fa-clock-rotate-left", text: "按日期浏览全部教程内容更新及对应章节" }
---
