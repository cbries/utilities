all: l1
l1:
	g++ -O3 -o ResourceLoad ResourceLoad.cpp ResourceBlock.cpp \
		`Magick++-config --cppflags --cxxflags --ldflags --libs` \
		-I. -I./boost/include -L./boost/lib -lboost_thread
run-all: run-m1a run-m1b run-m1c run-m1d 
run-m1a:
	LD_LIBRARY_PATH=./boost/lib ./ResourceLoad
run-m1b:
	LD_LIBRARY_PATH=./boost/lib ./ResourceLoad -bt
run-m1c:
	LD_LIBRARY_PATH=./boost/lib ./ResourceLoad -rt
run-m1d:
	LD_LIBRARY_PATH=./boost/lib ./ResourceLoad -bt -rt
