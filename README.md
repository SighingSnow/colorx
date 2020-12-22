# colorx

课程项目：简单室内场景三维建模及真实感绘制（总分40分）（以4人（或少于4人）为一组实现，教师以给平均分的形式打分，如果组内不平均分配，则需小组提供组内所有同学都同意的分配方案，报助教或老师同意，组内分差不能大于10分）

### 基本要求：（共35分，每实现一个项目满分得5分）
1. 具有基本体素（立方体、球、圆柱、圆锥、多面棱柱、多面棱台）的建模表达能力；
2. 具有基本三维网格导入导出功能（建议OBJ格式）；
3. 具有基本材质、纹理的显示和编辑能力；
4. 具有基本几何变换功能（旋转、平移、缩放等）；
5. 基本光照明模型要求，并实现基本的光源编辑（如调整光源的位置，光强等参数）；
6. 能对建模后场景进行漫游如Zoom In/Out， Pan, Orbit, Zoom To Fit等观察功能。
7.  能够提供屏幕截取/保存功能。


### 额外要求： （每增加一个额外项目的效果，额外增加分数，但每项满分为5分，严格要求，直至加满至40分）
1. 漫游时可实时碰撞检测
2. 光照明模型细化，可任选实现实时阴影、caustic、全局光照明（光子跟踪）、辐射度等
3. 采用HTML5/IOS/Android移动平台实现
4. 构建了基于此引擎的完整三维游戏，具有可玩性。
5. 与增强现实应用结合
6. 具有一定的对象表达能力，能够表达门、窗、墙等；

### 说明
首先通过colox中的init进行初始化
之后获取到了SceneManager
SceneManager中的drawall对场景节点进行绘制，场景节点为假删除。
SceneManager中包括光照，相机，普通模型如立方体，以及obj等带网格模型的绘制

> 此次架构设计采用MVC模式
> ScenManager为View层，用户通过EventHandler实现交互，EventHandler为Control层，
> Model层为相机、网格、光照等基础部分组成。

### 安装运行
在根目录下分别输入以下两条命令即可编译得到相应文件。

```cmake
cmake .
make
```

