# H264_Compression
->The project requires CMake version 3.24
->Install CMake 3.24
    Step-1. Uninstall the default version provided by Ubuntu's package manager using : 

        sudo apt remove --purge --auto-remove cmake

    Step-2. Go to the official CMake webpage, then download and install the latest .sh version in opt/cmake. Update the version and build variables in the following command to get the desired version:

        version=3.24
        build=1
        ## don't modify from here
        limit=3.20
        result=$(echo "$version >= $limit" | bc -l)
        os=$([ "$result" == 1 ] && echo "linux" || echo "Linux")
        mkdir ~/temp
        cd ~/temp
        wget https://cmake.org/files/v$version/cmake-$version.$build-$os-x86_64.sh 
        sudo mkdir /opt/cmake
        sudo sh cmake-$version.$build-$os-x86_64.sh --prefix=/opt/cmake

    Step-3. Add the installed binary link to /usr/local/bin/cmake by running this:

        sudo ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake

    Step-4. Test your new cmake version using:

        cmake --version

->Install dependency libraries( libavcodec, libavutil, libavformat, libswscale, libswresample) : 
    Run this command :

        sudo apt install libavcodec-dev && 
        sudo apt install libavformat-dev && 
        sudo apt install libavutil-dev && 
        sudo apt install libswscale-dev && 
        sudo apt install libswresample-dev  

-> For generating and running the executable :
    Inside H264_COMPRESSION/
    1.Create build folder :
        mkdir build
    2.Move inside build folder : 
        cd build
    3.Do cmake : 
        cmake ..
    4.Do make :
        make   
    5.Run the executable : 
        ./H264_Compression
    6.You can find the output mp4 file in build folder.

