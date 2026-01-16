# 拔刀斩（代号：幻想导数）
🚧 *开发中 | In Development*

基于 UE5 的快节奏砍杀动作游戏 Demo，以"拔刀斩"为核心机制，支持俯视角 (Top-down) 与越肩视角 (Over-the-shoulder) 双视角实时切换。

https://github.com/user-attachments/assets/79e52ef4-9658-4105-b58f-25102e97cd7c

## 核心玩法

- 拔刀斩三阶段：蓄力 → 斩击 → 收刀
- 能量机制：收刀回蓝，首斩强化
- 双视角战斗：俯视角（割草，爽，水果忍者精神续作）/ 越肩（博弈，魂，FPS但枪是角色）

## 技术实现

### TD↔TP 视角切换

https://github.com/user-attachments/assets/82e144c6-aef7-4152-99c7-21e3ba8e48d7

通过逆向几何实时计算Camera Offset，在BlendNode外层进行覆盖旋转值，使用`OnBlendResults` 中的二阶Blend结果，消除镜头跳变。

[查看开发文档](https://mike31nb-cuhk.github.io/Quartz-build-4-Obsidian/%E5%BC%80%E5%8F%91%E6%97%A5%E5%BF%97/2025.12.10-LookAtBlend/25.12.10-25.12.24-%E6%B6%88%E9%99%A4%E8%A7%86%E8%A7%92%E8%B7%B3%E5%8F%98%EF%BC%9A%E5%9F%BA%E4%BA%8E%E9%80%86%E5%90%91%E5%87%A0%E4%BD%95%E6%8E%A8%E5%AF%BC%E4%B8%8E%E4%BA%8C%E9%98%B6%E6%B7%B7%E5%90%88%E4%BF%AE%E6%AD%A3%E7%9A%84-LookAtBlend-%E7%AE%97%E6%B3%95)



### 视角状态管理

https://github.com/user-attachments/assets/67431590-6ea0-453a-b8db-45cfbad791f9

拒绝 Subsystem 过度设计，采用 GameplayTag + BlueprintFunctionLibrary 方案，零运行时实例开销实现视角状态同步。

[查看开发文档](https://mike31nb-cuhk.github.io/Quartz-build-4-Obsidian/%E5%BC%80%E5%8F%91%E6%97%A5%E5%BF%97/2025.12.25-ViewModeTagSync/25.12.25---26.1.07-%E6%8B%92%E7%BB%9D%E8%BF%87%E5%BA%A6%E8%AE%BE%E8%AE%A1%EF%BC%9A%E4%B8%BA%E4%BD%95%E6%88%91%E6%94%BE%E5%BC%83-Subsystem-%E8%80%8C%E9%80%89%E7%94%A8GameplayTag-+-BlueprintFunctionLibrary%E7%AE%A1%E7%90%86%E8%A7%86%E8%A7%92%E7%8A%B6%E6%80%81)



## 开发进度

- [x] Motion Matching
- [x] 拔刀斩 GA
- [x] 视角切换
- [x] 敌人检测与锁定
- [ ] 弹反机制
- [ ] 伤害系统

## 技术栈

UE5 源码 | GAS | Gameplay Camera System | Unreal C++
