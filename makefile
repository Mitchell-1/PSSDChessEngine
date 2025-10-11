make: makeV3

makeV1:
	./v1-init

makeV2:
	
	./v2-algChanges

makeV3:
	g++ *.cpp -o v3-legal -g -O1
	./v3-legal

makeV4:
	g++ *.cpp -o v4-UCI -g -O1
	./v4-UCI
	