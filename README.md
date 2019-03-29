# LearnCG

主要使用OpenGL图形库。方便自己上面复现论文、实现图形学效果、以及其它的demo。

复现的论文：
- 《MARCHING CUBES：A HIGH RESOLUTION 3D SURFACE CONSTRUCTION ALGORITHM》

效果：
- 雾化效果
- 粒子系统
- 简单的水面效果



# Renderer3D
C++光栅化渲染器，计划在[skywind3000/mini3d](https://github.com/skywind3000/mini3d)程序基础上完成@skywind3000 布置的基础作业、进阶作业，然后做一些课后习题。
- 基础作业
    - [ ] 增加背面剔除
    - [ ] 增加简单光照
    - [ ] 提供更多渲染模式
    - [ ] 实现二次线性差值的纹理读取

- 进阶作业
    - [ ] 推导并证明程序中用到的所有几何知识
    - [ ] 优化顶点计算性能 
    - [ ] 优化 draw_scanline 性能
    - [ ] 从 BMP/TGA 文件加载纹理
    - [ ] 载入 BSP 场景并实现漫游

- 课外习题
    - [ ] 左手坐标系→右手坐标系


# FixedPipeline
固定管线



# OpenGL_Lib
一些用到的库，比如glfw、glad、glm、assimp
