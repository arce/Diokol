gcc src/mainPi.c -lluajit -lbcm_host -lpthread -lbrcmOpenVG -lbrcmGLESv2 -lbrcmEGL -o bin/pi/Diököl -I./include -I/opt/vc/include -I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/include/interface/vcos/pthreads -I.. -L./lib/pi -w -Wl,-R./lib -L/opt/vc/lib