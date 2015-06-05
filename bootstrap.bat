cd ..
git clone https://bitbucket.org/rafzi/hacklib.git
mv gw2dps hacklib/src
cd hacklib/src
git clone https://bitbucket.org/rafzi/hacklib_gw2.git
cd ..
cmake .
rm src/gw2dps/bootstrap.bat