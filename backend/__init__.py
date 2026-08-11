from app import systeminfo

monitor = systeminfo("../src_c/sys_info.c")

system_data = monitor.get_system_data()
if system_data:
    print(system_data)

else:
    print("Error : Occured while getting system data")