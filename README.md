# 可见光信息传输项目日志
## 2020/3/8 - 18:50
项目一 v1.0 Release 发布，实现了44.7kB/s的传输速率。
## How to use
1. 准备Eclipse,JDK,Visual Studio,OpenCV,FFMPEG。
2. 准备输出目录(也可以直接将Github中的。NET-Program-Output放到E盘下)。
3. 在Encoder/settings/Options.java与Decoder/settings.h中配置输出目录。
4. 在Options.java与settings.h中配置FFMPEG目录。
5. Java项目是独立的，不需要额外的库；C++项目需要引入OpenCV运行库，可自行上网检索。
6. 把待编码文件放入输出目录中，并命名为NetIn.G24(建议小于200kB)，运行Java项目获得EncResult/Enc00Out.mp4文件
7. 使用PotPlayer播放(注意关闭左上角的消息)，建议手机先开始录制再播放。
8. 将录制好的视频重命名为VIDEO.mp4放在输出目录中
9. 运行C++项目，(运气好解码成功的话)得到NetOut.G24
10. 使用好压的MD5校验比较两个文件NetIn.G24、NetOut.G24的相似度。
## 提示
可以在Decoder/Decode.cpp/decodeSingleImg()中修改颜色阈值，获得更好的解码效果。
## Group 24 目标
### 当前的主要目标
- [ ] 无
### 杂项
- [ ] 多线程读取图片
- [ ] 图形界面
### 已完成
- [x] 10px 的30x30方块轮廓识别(v0.1.00)
- [x] 广角变形方块校正(v0.1.10)
- [x] 单个方块读取(v0.1.20)
- [x] 实现单帧校验
- [x] **Encoder** Java实现
- [x] 15FPS 动画化(v0.2.00)
- [x] 实现白天读取
- [x] 降帧率至10FPS
- [x] **Decoder** C++实现(v1.0 Beta 版本)
- [x] 写一下适配，修正代码