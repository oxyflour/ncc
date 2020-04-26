## To build occt
```bash
# git clone <this repo> --recursive
cd deps/occt
sudo apt-get install tcllib tklib tcl-dev tk-dev libfreetype6-dev libxt-dev libxmu-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libfreeimage-dev libtbb-dev libgl2ps-dev
cmake -DBUILD_LIBRARY_TYPE=Static
cd ../../
mkdir -p deps/occt-7.4
cp deps/occt/src/**/*.hxx deps/occt-7.4/include/
cp deps/occt/src/**/*.lxx deps/occt-7.4/include/
cp deps/occt/src/**/*.gxx deps/occt-7.4/include/
cp deps/occt/src/**/*.h   deps/occt-7.4/include/
cp deps/occt/lin64/gcc/lib/*.* deps/occt-7.4/lib/
```
