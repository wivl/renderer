# renderer

Me learning computer graphics by cloning [ssloy/tinyrenderer](https://github.com/ssloy/tinyrenderer)

## TODOS

|items|status|
|-----|------|
|Rewrite codebase in C++|W|
|try conan packge manager|W|
|switch tinytga to stb|W|
|~~Add ambient occlusion~~|N|

## Dependancies

* [eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)
* [stb](https://github.com/nothings/stb)

## setup

Using conan package manager.

```
pip3 install conan
```

```
mkdir build
conan profile detect --force
conan install . --output-folder=build --build=missing
```

Windows

```
cd build
# assuming Visual Studio 15 2017 is your VS version and that it matches your default profile
cmake .. -G "Visual Studio 15 2017" -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
cmake --build . --config Release
```

Linux, macOS

```
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cmake --build .
```

## structure

| class | description | status |
|-------|-------------|--------|
|object|objects|D|
|camera|cameras|D|
|shader|shaders|D|
|light|lights|TODO|
|scene|contains objects, cameras, lights, shaders(in cameras)|TODO|

