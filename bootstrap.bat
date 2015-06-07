@echo off
cd ..
git clone https://bitbucket.org/rafzi/hacklib.git
cd hacklib/src
git clone https://bitbucket.org/rafzi/hacklib_gw2.git
cd ../..
mv "gw2dps" "hacklib/src"