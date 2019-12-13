import hoverboard_api
import time



ctrl = hoverboard_api.HoverboardAPI(b"/dev/ttyTHS1")

while True:
	ctrl.sendPWM(100, 0)
	ctrl.requestRead(hoverboard_api.HoverboardAPI.sensHall);
	ctrl.protocolTask()
	time.sleep(0.03)
	print("speed {}".format(ctrl.getSpeed_kmh()))
