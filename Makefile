
packetRobotMain.exe: packetRobotMain.o PacketReader.o LogCSV.o LogFile.o roboBrain.o
	g++ --std=c++14 -g $^ -o $@

%.o: %.cpp
	g++ --std=c++14 -c -g -o $@ $^
	
clean:
	rm *.o *.exe
