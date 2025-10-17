make: makeFinal

makeFinal:
	g++ *.cpp -o chess_engine -O3 -DNDEBUG -march=native -flto -static -fno-exceptions -fno-rtti
	./chess_engine
	