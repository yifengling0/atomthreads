clear
::openocd -f openocd.cfg -c init -c "reset halt" 
::一些配置以及烧录语句,根据情况使用:
openocd -f openocd.cfg -c init -c "reset halt" -c "flash write_image erase %1% 0x8000000" -c "reset run" -c shutdown
 

