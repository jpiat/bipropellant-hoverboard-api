import hoverboard_api
import time



ctrl = hoverboard_api.HoverboardAPI(b"/dev/ttyTHS1")

#ctrl.sendSpeedData(2.0, 0.0, 500, 0)
while True:
	#ctrl.sendSpeedData(1.0, 1.0, 500, 0)
	ctrl.sendPWM(100, 0)
	ctrl.requestRead(hoverboard_api.HoverboardAPI.sensHall);
	ctrl.protocolTask()
	time.sleep(0.03)
	print("speed {} {}".format(ctrl.getSpeed0_kmh(), ctrl.getSpeed1_kmh()))
