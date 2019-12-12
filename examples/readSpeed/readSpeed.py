import hoverboard_api
import time

SENS_HALL = 0x02
SENS_ELECTRICAL = 0x08



ctrl = hoverboard_api.HoverboardAPI(b"/dev/ttyTHS1")

while True:
	ctrl.sendPWM(100, 0)
	ctrl.requestRead(SENS_HALL);
	ctrl.protocolTask()
	time.sleep(0.03)
	print("speed {}".format(ctrl.getSpeed_kmh()))
