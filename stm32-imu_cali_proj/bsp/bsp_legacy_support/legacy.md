# legacy bsp

这些bsp实现将在v0.1删除,因为他们实际上都是用pwm实现的,应当放在module层,以彻底隔离bsp和CubeMX的初始化代码.现已提供了bsp_pwm的实现.

之后在修改CubeMX的初始化配置之后就不再需要修改bsp的内容了,所有的修改都会通过app层的初始化配置`xxx_Init_Config_s`来实现.