/*! 
 *  \file hmi_driver.h
 *  \brief 串口屏驱动文件
 *  \version 1.0
 *  \date 2012-2015
 *  \copyright 广州大彩光电科技有限公司
 */

#ifndef _HMI_DRIVER_
#define _HMI_DRIVER_

#define CRC16_ENABLE 1         /*!< 如果需要CRC16校验功能，修改此宏为1(此时需要在VisualTFT工程中配CRC校验)*/
#define CMD_MAX_SIZE 20        /*!<单条指令大小，根据需要调整，尽量设置大一些*/
#define QUEUE_MAX_SIZE 80   /*!< 指令接收缓冲区大小，根据需要调整，尽量设置大一些*/


#include "data_type.h"
/*! 
 *  \brief  检查数据是否符合CRC16校验
 *  \param buffer 待校验的数据，末尾存储CRC16
 *  \param n 数据长度，包含CRC16
 *  \return 校验通过返回1，否则返回0
 */
uint16 CheckCRC16(uint8 *buffer,uint16 n);

/*! 
 *  \brief  延时
 *  \param  n 延时时间(毫秒单位)
 */
void DelayMS(unsigned int n);

/*! 
 *  \brief  锁定设备配置，锁定之后需要解锁，才能修改波特率、触摸屏、蜂鸣器工作方式
 */
void LockDeviceConfig(void);

/*! 
 *  \brief  解锁设备配置
 */
void UnlockDeviceConfig(void);

/*! 
 *  \brief     修改串口屏的波特率
 *  \details  波特率选项范围[0~14]，对应实际波特率
                   {1200,2400,4800,9600,19200,38400,57600,115200,1000000,2000000,218750,437500,875000,921800,2500000}
 *  \param  option 波特率选项
 */
void SetCommBps(uint8 option);

/*! 
 *  \brief  发送握手命令
 */
void SetHandShake(void);

/*! 
 *  \brief  设置前景色
 *  \param  color 前景色
 */
void SetFcolor(uint16 color);

/*! 
 *  \brief  设置背景色
 *  \param  color 背景色
 */
void SetBcolor(uint16 color);

/*! 
 *  \brief  清除画面
 */
void GUI_CleanScreen(void);

/*! 
 *  \brief  设置文字间隔
 *  \param  x_w 横向间隔
  *  \param  y_w 纵向间隔
 */
void SetTextSpace(uint8 x_w, uint8 y_w);

/*! 
 *  \brief  设置文字显示限制
 *  \param  enable 是否启用限制
 *  \param  width 宽度
 *  \param  height 高度
 */
void SetFont_Region(uint8 enable,uint16 width,uint16 height );

/*! 
 *  \brief  设置过滤色
 *  \param  fillcolor_dwon 颜色下界
 *  \param  fillcolor_up 颜色上界
 */
void SetFilterColor(uint16 fillcolor_dwon, uint16 fillcolor_up);

/*! 
 *  \brief  设置过滤色
 *  \param  x 位置X坐标
 *  \param  y 位置Y坐标
 *  \param  back 颜色上界
 *  \param  font 字体
 *  \param  strings 字符串内容
 */
void DisText(uint16 x, uint16 y,uint8 back,uint8 font,uchar *strings );

/*! 
 *  \brief    显示光标
 *  \param  enable 是否显示
 *  \param  x 位置X坐标
 *  \param  y 位置Y坐标
 *  \param  width 宽度
 *  \param  height 高度
 */
void DisCursor(uint8 enable,uint16 x, uint16 y,uint8 width,uint8 height );

/*! 
 *  \brief      显示全屏图片
 *  \param  image_id 图片索引
 *  \param  masken 是否启用透明掩码
 */
void DisFull_Image(uint16 image_id,uint8 masken);

/*! 
 *  \brief      指定位置显示图片
 *  \param  x 位置X坐标
 *  \param  y 位置Y坐标
 *  \param  image_id 图片索引
 *  \param  masken 是否启用透明掩码
 */
void DisArea_Image(uint16 x,uint16 y,uint16 image_id,uint8 masken);

/*! 
 *  \brief      显示裁剪图片
 *  \param  x 位置X坐标
 *  \param  y 位置Y坐标
 *  \param  image_id 图片索引
 *  \param  image_x 图片裁剪位置X坐标
 *  \param  image_y 图片裁剪位置Y坐标
 *  \param  image_l 图片裁剪长度
 *  \param  image_w 图片裁剪高度
 *  \param  masken 是否启用透明掩码
 */
void DisCut_Image(uint16 x,uint16 y,uint16 image_id,uint16 image_x,uint16 image_y,
                   uint16 image_l, uint16 image_w,uint8 masken);

/*! 
 *  \brief      显示GIF动画
 *  \param  x 位置X坐标
 *  \param  y 位置Y坐标
 *  \param  flashimage_id 图片索引
 *  \param  enable 是否显示
 *  \param  playnum 播放次数
 */
void DisFlashImage(uint16 x,uint16 y,uint16 flashimage_id,uint8 enable,uint8 playnum);

/*! 
 *  \brief      画点
 *  \param  x 位置X坐标
 *  \param  y 位置Y坐标
 */
void GUI_Dot(uint16 x,uint16 y);

/*! 
 *  \brief      画线
 *  \param  x0 起始位置X坐标
 *  \param  y0 起始位置Y坐标
 *  \param  x1 结束位置X坐标
 *  \param  y1 结束位置Y坐标
 */
void GUI_Line(uint16 x0, uint16 y0, uint16 x1, uint16 y1);

/*! 
 *  \brief      画折线
 *  \param  mode 模式
 *  \param  dot 数据点
 *  \param  dot_cnt 点数
 */
void GUI_ConDots(uint8 mode,uint16 *dot,uint16 dot_cnt);

/*! 
 *  \brief      画空心圆
 *  \param  x0 圆心位置X坐标
 *  \param  y0 圆心位置Y坐标
 *  \param  r 半径
 */
void GUI_Circle(uint16 x0, uint16 y0, uint16 r);

/*! 
 *  \brief      画实心圆
 *  \param  x0 圆心位置X坐标
 *  \param  y0 圆心位置Y坐标
 *  \param  r 半径
 */
void GUI_CircleFill(uint16 x0, uint16 y0, uint16 r);

/*! 
 *  \brief      画弧线
 *  \param  x0 圆心位置X坐标
 *  \param  y0 圆心位置Y坐标
 *  \param  r 半径
 *  \param  sa 起始角度
 *  \param  ea 终止角度
 */
void GUI_Arc(uint16 x,uint16 y, uint16 r,uint16 sa, uint16 ea);

/*! 
 *  \brief      画空心矩形
 *  \param  x0 起始位置X坐标
 *  \param  y0 起始位置Y坐标
 *  \param  x1 结束位置X坐标
 *  \param  y1 结束位置Y坐标
 */
void GUI_Rectangle(uint16 x0, uint16 y0, uint16 x1,uint16 y1 );

/*! 
 *  \brief      画实心矩形
 *  \param  x0 起始位置X坐标
 *  \param  y0 起始位置Y坐标
 *  \param  x1 结束位置X坐标
 *  \param  y1 结束位置Y坐标
 */
void GUI_RectangleFill(uint16 x0, uint16 y0, uint16 x1,uint16 y1 );

/*! 
 *  \brief      画空心椭圆
 *  \param  x0 起始位置X坐标
 *  \param  y0 起始位置Y坐标
 *  \param  x1 结束位置X坐标
 *  \param  y1 结束位置Y坐标
 */
void GUI_Ellipse (uint16 x0, uint16 y0, uint16 x1,uint16 y1 );

/*! 
 *  \brief      画实心椭圆
 *  \param  x0 起始位置X坐标
 *  \param  y0 起始位置Y坐标
 *  \param  x1 结束位置X坐标
 *  \param  y1 结束位置Y坐标
 */
void GUI_EllipseFill (uint16 x0, uint16 y0, uint16 x1,uint16 y1 );

/*! 
 *  \brief      画线
 *  \param  x0 起始位置X坐标
 *  \param  y0 起始位置Y坐标
 *  \param  x1 结束位置X坐标
 *  \param  y1 结束位置Y坐标
 */
void SetBackLight(uint8 light_level);

/*! 
 *  \brief   蜂鸣器设置
 *  \time  time 持续时间(毫秒单位)
 */
void SetBuzzer(uint8 time);

/*! 
 *  \brief   触摸屏设置
 *  \param enable 触摸使能
 *  \param beep_on 触摸蜂鸣器
 *  \param work_mode 触摸工作模式：0按下就上传，1松开才上传，2不断上传坐标值，3按下和松开均上传数据
 *  \param press_calibration 连续点击触摸屏20下校准触摸屏：0禁用，1启用
 */
void SetTouchPaneOption(uint8 enbale,uint8 beep_on,uint8 work_mode,uint8 press_calibration);

/*! 
 *  \brief   校准触摸屏
 */
void	CalibrateTouchPane(void);

/*! 
 *  \brief  触摸屏测试
 */
void TestTouchPane(void);

/*! 
 *  \brief      设置当前写入图层
 *  \details  一般用于实现双缓存效果(绘图时避免闪烁)：
 *  \details  uint8 layer = 0;
 *  \details  WriteLayer(layer);    //设置写入层
 *  \details  ClearLayer(layer);    //使图层变透明
 *  \details  //添加一系列绘图指令
 *  \details  //DisText(100,100,0,4,"hello hmi!!!");
 *  \details  DisplyLayer(layer);  //切换显示层
 *  \details  layer = (layer+1)%2;  //双缓存切换
 *  \see DisplyLayer
 *  \see ClearLayer
 *  \param  layer 图层编号
 */
void WriteLayer(uint8 layer);

/*! 
 *  \brief      设置当前显示图层
 *  \param  layer 图层编号
 */
void DisplyLayer(uint8 layer);

/*! 
 *  \brief      清除图层，使图层变成透明
 *  \param  layer 图层编号
 */
void ClearLayer(uint8 layer);

/*! 
 *  \brief  写数据到串口屏用户存储区
 *  \param  startAddress 起始地址
 *  \param  length 字节数
 *  \param  _data 待写入的数据
 */
void WriteUserFlash(uint32 startAddress,uint16 length,uint8 *_data);

/*! 
 *  \brief  从串口屏用户存储区读取数据
 *  \param  startAddress 起始地址
 *  \param  length 字节数
 */
void ReadUserFlash(uint32 startAddress,uint16 length);

/*! 
 *  \brief      拷贝图层
 *  \param  src_layer 原始图层
 *  \param  dest_layer 目标图层
 */
void CopyLayer(uint8 src_layer,uint8 dest_layer);

/*! 
 *  \brief      设置当前画面
 *  \param  screen_id 画面ID
 */
void SetScreen(uint16 screen_id);

/*! 
 *  \brief      获取当前画面
 */
void GetScreen(void);

/*! 
 *  \brief     禁用\启用画面更新
 *  \details 禁用\启用一般成对使用，用于避免闪烁、提高刷新速度
 *  \details 用法：
 *	\details SetScreenUpdateEnable(0);//禁止更新
 *	\details 一系列更新画面的指令
 *	\details SetScreenUpdateEnable(1);//立即更新
 *  \param  enable 0禁用，1启用
 */
void SetScreenUpdateEnable(uint8 enable);

/*! 
 *  \brief     设置控件输入焦点
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  focus 是否具有输入焦点
 */
void SetControlFocus(uint16 screen_id,uint16 control_id,uint8 focus);

/*! 
 *  \brief     显示\隐藏控件
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  visible 是否显示
 */
void SetControlVisiable(uint16 screen_id,uint16 control_id,uint8 visible);

/*! 
 *  \brief     设置触摸控件使能
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  enable 控件是否使能
 */
void SetControlEnable(uint16 screen_id,uint16 control_id,uint8 enable);

/*! 
 *  \brief     获取控件值
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 */
void GetControlValue(uint16 screen_id,uint16 control_id);

/*! 
 *  \brief     设置按钮状态
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  value 按钮状态
 */
void SetButtonValue(uint16 screen_id,uint16 control_id,uchar value);

/*! 
 *  \brief     设置文本值
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  str 文本值
 */
void SetTextValue(uint16 screen_id,uint16 control_id,uchar *str);

/*! 
 *  \brief      设置进度值
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  value 数值
 */
void SetProgressValue(uint16 screen_id,uint16 control_id,uint32 value);

/*! 
 *  \brief     设置仪表值
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  value 数值
 */
void SetMeterValue(uint16 screen_id,uint16 control_id,uint32 value);

/*! 
 *  \brief      设置滑动条
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  value 数值
 */
void SetSliderValue(uint16 screen_id,uint16 control_id,uint32 value);

/*! 
 *  \brief      设置选择控件
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  item 当前选项
 */
void SetSelectorValue(uint16 screen_id,uint16 control_id,uint8 item);

/*! 
 *  \brief      开始播放动画
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 */
void AnimationStart(uint16 screen_id,uint16 control_id);

/*! 
 *  \brief      停止播放动画
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 */
void AnimationStop(uint16 screen_id,uint16 control_id);

/*! 
 *  \brief      暂停播放动画
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 */
void AnimationPause(uint16 screen_id,uint16 control_id);

/*! 
 *  \brief     播放制定帧
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  frame_id 帧ID
 */
void AnimationPlayFrame(uint16 screen_id,uint16 control_id,uint8 frame_id);

/*! 
 *  \brief     播放上一帧
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 */
void AnimationPlayPrev(uint16 screen_id,uint16 control_id);

/*! 
 *  \brief     播放下一帧
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 */
void AnimationPlayNext(uint16 screen_id,uint16 control_id);

/*! 
 *  \brief     曲线控件-添加通道
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  channel 通道号
 *  \param  color 颜色
 */
void GraphChannelAdd(uint16 screen_id,uint16 control_id,uint8 channel,uint16 color);

/*! 
 *  \brief     曲线控件-删除通道
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  channel 通道号
 */
void GraphChannelDel(uint16 screen_id,uint16 control_id,uint8 channel);

/*! 
 *  \brief     曲线控件-添加数据
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  channel 通道号
 *  \param  pData 曲线数据
 *  \param  nDataLen 数据个数
 */
void GraphChannelDataAdd(uint16 screen_id,uint16 control_id,uint8 channel,uint8 *pData,uint16 nDataLen);

/*! 
 *  \brief     曲线控件-清除数据
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  channel 通道号
 */
void GraphChannelDataClear(uint16 screen_id,uint16 control_id,uint8 channel);

/*! 
 *  \brief     曲线控件-设置视图窗口
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  x_offset 水平偏移
 *  \param  x_mul 水平缩放系数
 *  \param  y_offset 垂直偏移
 *  \param  y_mul 垂直缩放系数
 */
void GraphSetViewport(uint16 screen_id,uint16 control_id,int16 x_offset,uint16 x_mul,int16 y_offset,uint16 y_mul);

/*! 
 *  \brief     开始批量更新
 *  \param  screen_id 画面ID
 */
void BatchBegin(uint16 screen_id);

/*! 
 *  \brief     批量更新按钮控件
 *  \param  control_id 控件ID
 *  \param  value 数值
 */
void BatchSetButtonValue(uint16 control_id,uint8 state);

/*! 
 *  \brief     批量更新进度条控件
 *  \param  control_id 控件ID
 *  \param  value 数值
 */
void BatchSetProgressValue(uint16 control_id,uint32 value);

/*! 
 *  \brief     批量更新滑动条控件
 *  \param  control_id 控件ID
 *  \param  value 数值
 */
void BatchSetSliderValue(uint16 control_id,uint32 value);

/*! 
 *  \brief     批量更新仪表控件
 *  \param  control_id 控件ID
 *  \param  value 数值
 */
void BatchSetMeterValue(uint16 control_id,uint32 value);

/*! 
 *  \brief     批量更新文本控件
 *  \param  control_id 控件ID
 *  \param  strings 字符串
 */
void BatchSetText(uint16 control_id,uchar *strings);

/*! 
 *  \brief     批量更新动画\图标控件
 *  \param  control_id 控件ID
 *  \param  frame_id 帧ID
 */
void BatchSetFrame(uint16 control_id,uint16 frame_id);

/*! 
 *  \brief    结束批量更新
 */
void BatchEnd(void);

/*! 
 *  \brief     设置倒计时控件
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  timeout 倒计时(秒)
 */
void SeTimer(uint16 screen_id,uint16 control_id,uint32 timeout);

/*! 
 *  \brief     开启倒计时控件
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 */
void StartTimer(uint16 screen_id,uint16 control_id);

/*! 
 *  \brief     停止倒计时控件
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 */
void StopTimer(uint16 screen_id,uint16 control_id);

/*! 
 *  \brief     暂停倒计时控件
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 */
void PauseTimer(uint16 screen_id,uint16 control_id);

/*! 
 *  \brief     设置控件背景色
 *  \details  支持控件：进度条、文本
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  color 背景色
 */
void SetControlBackColor(uint16 screen_id,uint16 control_id,uint16 color);

/*! 
 *  \brief     设置控件前景色
  * \details  支持控件：进度条
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  color 前景色
 */
void SetControlForeColor(uint16 screen_id,uint16 control_id,uint16 color);

/*! 
 *  \brief     显示\隐藏弹出菜单控件
 *  \param  screen_id 画面ID
 *  \param  control_id 控件ID
 *  \param  show 是否显示，为0时focus_control_id无效
 *  \param  focus_control_id 关联的文本控件(菜单控件的内容输出到文本控件)
 */
void ShowPopupMenu(uint16 screen_id,uint16 control_id,uint8 show,uint16 focus_control_id);

/*! 
 *  \brief     显示\隐藏系统键盘
 *  \param  show 0隐藏，1显示
 *  \param  x 键盘显示位置X坐标
 *  \param  y 键盘显示位置Y坐标
 *  \param  type 0小键盘，1全键盘
 *  \param  option 0正常字符，1密码，2时间设置
 *  \param  max_len 键盘录入字符长度限制
 */
void ShowKeyboard(uint8 show,uint16 x,uint16 y,uint8 type,uint8 option,uint8 max_len);

#endif
