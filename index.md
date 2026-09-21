---
layout: home
title: AI Storage Notes
description: 面向存储工程师的 AI Storage、KV Cache 与 GPU Data Path 面试教程。
hero:
  eyebrow: AI Storage 面试教程
  title: 从存储系统走向 AI Infrastructure
  lead: >-
    用三份循序渐进的教程掌握 KV Cache、GPU 数据路径与分布式 AI Storage 系统设计。
  buttons:
    - { label: "开始学习", url: "/docs/01_AI_Storage_KV_Cache/", style: "btn--primary", icon: "fa-solid fa-rocket" }
    - { label: "查看 GitHub", url: "https://github.com/miauyle/ai-storage-notes", style: "btn--ghost", icon: "fa-brands fa-github" }
features:
  - icon: "fa-solid fa-brain"
    title: KV Cache 基础
    text: 从 LLM workload 推导容量、复用、分页与缓存层级。
    url: "/docs/01_AI_Storage_KV_Cache/"
  - icon: "fa-solid fa-microchip"
    title: GPU Data Path
    text: 理解 CUDA、RDMA、GPUDirect、S3 与异步资源生命周期。
    url: "/docs/02_GPU_Data_Path/"
  - icon: "fa-solid fa-diagram-project"
    title: 系统设计
    text: 完成 GPU-aware 分布式 KV Cache 系统设计与面试演练。
    url: "/docs/03_System_Design_Interview_Demo/"
sections:
  - title: 推荐学习顺序
    text: 先建立模型与容量基础，再理解数据路径，最后完成系统设计。
    links:
      - { label: "1. AI Storage 与 KV Cache", url: "/docs/01_AI_Storage_KV_Cache/", icon: "fa-solid fa-book-open", text: "模型、容量、分页、复用与分层" }
      - { label: "2. GPU Data Path", url: "/docs/02_GPU_Data_Path/", icon: "fa-solid fa-arrow-right-arrow-left", text: "CUDA、RDMA、GPUDirect 与 S3" }
      - { label: "3. 系统设计与面试", url: "/docs/03_System_Design_Interview_Demo/", icon: "fa-solid fa-sitemap", text: "架构、故障、性能模型与 Demo" }
---
