#!/usr/bin/env python
import rospy
from dji_sdk.srv import SetHardSync
import time

if __name__ == "__main__":
    print("Start hardSyc helper")

    rospy.wait_for_service('/dji_sdk_1/dji_sdk/set_hardsyc')
    call_hardsyc = rospy.ServiceProxy('/dji_sdk_1/dji_sdk/set_hardsyc', SetHardSync)

    ret = 0
    while ret == 0:
        ret = call_hardsyc(25, 0)
        time.sleep(0.1)

    print("Successful called hardsyc")
    print("exiting hardsyc")
