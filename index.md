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
    - { label: "如何开始", url: "/docs/00_Study_Guide/", style: "btn--primary", icon: "fa-solid fa-rocket" }
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
    text: 先用使用指南确定预算；按诊断弱项阅读，C++ 实验计入原有学习时间。
    links:
      - { label: "使用指南与时间预算", url: "/docs/00_Study_Guide/", icon: "fa-solid fa-compass", text: "90/60 小时安排、30 天输出与 JD 分流" }
      - { label: "0. Day 0 诊断", url: "/docs/00_Interview_Drills/", icon: "fa-solid fa-pen-to-square", text: "先闭卷作答，再按弱项回到正文" }
      - { label: "1. AI Storage 与 KV Cache", url: "/docs/01_AI_Storage_KV_Cache/", icon: "fa-solid fa-book-open", text: "模型、容量、分页、复用与分层" }
      - { label: "2. GPU Data Path", url: "/docs/02_GPU_Data_Path/", icon: "fa-solid fa-arrow-right-arrow-left", text: "CUDA、RDMA、GPUDirect 与 S3" }
      - { label: "3. 系统设计与面试", url: "/docs/03_System_Design_Interview_Demo/", icon: "fa-solid fa-sitemap", text: "架构、故障、性能模型与 Demo" }
  - title: 最近更新
    text: 精选近期有实质内容变化的教程入口；按日期查看完整归档。
    links:
      - { label: "2026-09-26 · 如何使用本教程", url: "/docs/00_Study_Guide/#time-budget", icon: "fa-solid fa-compass", text: "网站内直接查看 90/60 小时预算、30 天安排和停止线" }
      - { label: "2026-09-26 · 可运行 C++ 实验", url: "/docs/04_CPP_Labs/", icon: "fa-solid fa-code", text: "ownership、异步保活、Range 校验与错误修复练习" }
      - { label: "2026-09-26 · 真实项目追问链", url: "/docs/03_System_Design_Interview_Demo/#ecs-project-drills", icon: "fa-solid fa-comments", text: "复制、在线迁移和 CRR：先讲本人工作，再连接 AI" }
      - { label: "2026-09-26 · Demo 分阶段验收", url: "/docs/03_System_Design_Interview_Demo/#demo-stage-acceptance", icon: "fa-solid fa-list-check", text: "入门切片、真实 M0、模拟 M1 的交付与证据分别列清" }
      - { label: "2026-09-25 · 1 GiB 冷 KV 系统设计", url: "/docs/03_System_Design_Interview_Demo/#kv-cold-prefix-request", icon: "fa-solid fa-diagram-project", text: "贯穿容量预算、失败处理与 GPU Ready" }
      - { label: "查看全部更新 →", url: "/updates/", icon: "fa-solid fa-clock-rotate-left", text: "按日期浏览全部教程内容更新及对应章节" }
---
