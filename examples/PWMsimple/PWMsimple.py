

import hoverboard_api
import time

ctrl = hoverboard_api.HoverboardAPI(b"/dev/ttyTHS1")


while True:
	ctrl.sendPWM(100, 0)
	time.sleep(0.03)
