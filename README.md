# BBSS 是 宝宝Shared Screen 的简写。
本程序使用qt开发。<br \> 
BBSS文件夹内部是源码。<br \>
	包括客户端源码和服务器端源码。<br \>

BBSSServer  是打包好的服务器端程序。<br \>
BBSSClient  是客户端程序。<br \>

要求客户端和服务器端必须是在同一个局域网中。<br \>

#简单说一下实现原理：
#服务器端
##第一步
先截屏，截取屏幕后保存到bitmap。
##第二步
把bitmap宽高都切成10份，一共100块的bitmap。<br \>
把切割后的图片和上次发送的图片比较，如果不一样，然后准备发送<br \>
把需要发送的bitmap转换为jpeg图片。<br \>
把jpeg图片序列号为byte。<br \>
##第三步
把序列号的图片，通过udp发送到指定的端口。<br \>
<br \>
#客户端
监听指定的端口，然后接受数据。<br \>
把接受到的数据，反序列化为jpeg图片。<br \>
把jpeg图片，转换为bitmap。<br \>
把小的bitmap合并成大的bitmap。<br \>
然后再客户端显示。<br \>


