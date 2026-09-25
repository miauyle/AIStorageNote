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
---
