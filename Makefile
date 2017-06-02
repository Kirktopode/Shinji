
packetRobotMain.exe: packetRobotMain.o PacketReader.o LogCSV.o LogFile.o
	g++ --std=c++14 packetRobotMain.o PacketReader.o LogCSV.o LogFile.o -o packetRobotMain.exe

LogCSV.o: LogCSV.cpp
	g++ --std=c++14 -c -o LogCSV.o LogCSV.cpp
	
PacketReader.o: PacketReader.cpp
	g++ --std=c++14 -c -o PacketReader.o PacketReader.cpp
	
packetRobotMain.o: packetRobotMain.cpp
	g++ --std=c++14 -c -o packetRobotMain.o packetRobotMain.cpp
	
LogFile.o: LogFile.cpp
	g++ --std=c++14 -c -o LogFile.o LogFile.cpp
	

