import hoverboard_api
import time

ctrl = hoverboard_api.HoverboardAPI(b"/dev/ttyTHS1")
ctrl.scheduleRead(hoverboard_api.HoverboardAPI.sensHall, -1, 100);
ctrl.scheduleRead(hoverboard_api.HoverboardAPI.sensElectrical, -1, 1000);

while True :
	ctrl.protocolTask()
	print("speed = {} mm/s, vbat = {} volts".format(ctrl.getSpeed_mms(), ctrl.getBatteryVoltage()))
	time.sleep(0.1)
