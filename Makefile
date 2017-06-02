
packetRobotMain.exe: packetRobotMain.o PacketReader.o LogCSV.o LogFile.o roboBrain.o
	g++ --std=c++14 packetRobotMain.o roboBrain.o PacketReader.o LogCSV.o LogFile.o -o packetRobotMain.exe

roboBrain.o: roboBrain.cpp
	g++ --std=c++14 -c -o roboBrain.o roboBrain.cpp

LogCSV.o: LogCSV.cpp
	g++ --std=c++14 -c -o LogCSV.o LogCSV.cpp
	
PacketReader.o: PacketReader.cpp
	g++ --std=c++14 -c -o PacketReader.o PacketReader.cpp
	
packetRobotMain.o: packetRobotMain.cpp
	g++ --std=c++14 -c -o packetRobotMain.o packetRobotMain.cpp
	
LogFile.o: LogFile.cpp
	g++ --std=c++14 -c -o LogFile.o LogFile.cpp
	

