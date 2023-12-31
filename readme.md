## To build occt
```bash
git submodule update --init --recursive
cd deps/occt
# checkout branch
sudo apt-get install tcllib tklib tcl-dev tk-dev libfreetype6-dev libxt-dev libxmu-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libfreeimage-dev libtbb-dev libgl2ps-dev
cmake -DBUILD_LIBRARY_TYPE=Static
cd ../../
export OCCT_DIR=deps/occt-7.7
mkdir -p $OCCT_DIR
cp deps/occt/src/**/*.hxx $OCCT_DIR/include/
cp deps/occt/src/**/*.lxx $OCCT_DIR/include/
cp deps/occt/src/**/*.gxx $OCCT_DIR/include/
cp deps/occt/src/**/*.h   $OCCT_DIR/include/
cp deps/occt/lin64/gcc/lib/*.* $OCCT_DIR/lib/
```
