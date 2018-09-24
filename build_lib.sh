EIGEN=/usr/include/eigen3

cd ./bin
for file in ../src/*cpp; do
	g++-8 -std=c++17 -fPIC -c -Wall -Wextra -I$EIGEN -I../include -O3 $file
done
ar crv libmarkets.a *.o
cd ..


