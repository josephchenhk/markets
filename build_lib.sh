EIGEN=/usr/include/eigen3



cd ./bin
for file in ../src/*cpp; do
	g++ -std=c++11 -fPIC -c -I$EIGEN -I../include -O3 $file; 
done
ar crv libmarkets.a *.o
cd ..


