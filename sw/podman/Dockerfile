
FROM debian:testing

# arguments
ARG user
ARG uid
ARG gid

# user
RUN groupadd --gid $gid $user 2>&1 > /tmp/docker-build.log || echo "Could add user $user to group $gid"

# add user with password "jackson"
RUN useradd -m -u $uid -g $gid -p BsUHWbFZO93Ks -s /bin/bash $user 2>&1 > /tmp/docker-build.log || echo "Could not create user $user"; \
	adduser $user sudo; \
	chown $user:$user /opt/

# environment variables
ENV prefixNative="x86_64-linux-gnu" \
	prefixNativeDir="/usr/local" \
	prefixMinGw="x86_64-w64-mingw32" \
	prefixRpi="arm-linux-gnueabihf"

ENV prefixMinGwDir="/usr/$prefixMinGw" \
	prefixRpiDir="/usr/$prefixRpi"

ENV versLibz="v1.2.11" \
	versLibssl="1_1_1j" \
	versLibcurl="7.80.0" \
	versLibjsoncpp="1.9.4" \
	versLibGlfw="3.2.1" \
	versLibGlm="0.9.9.8" \
	versLibGlew="2.2.0" \
	versLibFreetype="2-10-4" \
	versGraphviz="2.50.0"

ENV toolDir="/opt/tools" \
	buildDir="/opt/dependencies-build"

ENV PATH="$toolDir:$PATH"

# directories
RUN mkdir /home/$user/build; \
	mkdir ${buildDir}; \
	mkdir ${buildDir}/zlib; \
	mkdir ${buildDir}/zlib/build; \
	mkdir ${buildDir}/zlib/build/native; \
	mkdir ${buildDir}/zlib/build/win64; \
	mkdir ${buildDir}/zlib/build/rpi; \
	mkdir ${buildDir}/openssl; \
	mkdir ${buildDir}/openssl/build; \
	mkdir ${buildDir}/openssl/build/native; \
	mkdir ${buildDir}/openssl/build/win64; \
	mkdir ${buildDir}/openssl/build/rpi; \
	mkdir ${buildDir}/curl; \
	mkdir ${buildDir}/curl/build; \
	mkdir ${buildDir}/curl/build/native; \
	mkdir ${buildDir}/curl/build/win64; \
	mkdir ${buildDir}/curl/build/rpi; \
	mkdir ${buildDir}/jsoncpp; \
	mkdir ${buildDir}/jsoncpp/build; \
	mkdir ${buildDir}/jsoncpp/build/native; \
	mkdir ${buildDir}/jsoncpp/build/win64; \
	mkdir ${buildDir}/jsoncpp/build/rpi; \
	mkdir ${buildDir}/glfw; \
	mkdir ${buildDir}/glfw/build; \
	mkdir ${buildDir}/glfw/build/native; \
	mkdir ${buildDir}/glfw/build/win64; \
	mkdir ${buildDir}/glfw/build/rpi; \
	mkdir ${buildDir}/glm; \
	mkdir ${buildDir}/glew; \
	mkdir ${buildDir}/graphviz; \
	mkdir ${buildDir}/freetype; \
	mkdir ${buildDir}/freetype/build; \
	mkdir ${buildDir}/freetype/build/native; \
	mkdir ${buildDir}/freetype/build/win64

# update
RUN apt update; apt -y upgrade; \
	apt update --fix-missing

# app install
RUN apt -y install sudo openssh-client \
				git \
				vim \
				tree \
				ack \
				wget \
				bash-completion \
				fortune cowsay \
				build-essential \
				autoconf \
				libtool \
				pkg-config \
				python3 \
				python3-pip \
				mingw-w64 \
				g++-$prefixRpi \
				libtclap-dev \
				libgl1-mesa-glx \
				libgles2-mesa-dev \
				libegl1-mesa-dev \
				cmake \
				extra-cmake-modules \
				libxrandr-dev \
				libxinerama-dev \
				libxcursor-dev \
				libxi-dev \
				wayland-protocols \
				libwayland-dev \
				bison \
				flex \
				libgd-dev \
				libwebp-dev \
				libxkbcommon-dev; \
	pip3 install \
				meson \
				ninja

# tools
ADD tools $toolDir

WORKDIR $toolDir

RUN g++ bin2cpp.cpp -o bin2cpp

# dependencies
ADD dependencies /opt/dependencies

WORKDIR ${buildDir}

RUN update-alternatives --set $prefixMinGw-g++ /usr/bin/$prefixMinGw-g++-posix; \
	ln -sr /usr/bin/python3 /usr/bin/python

RUN patch /usr/share/pkg-config-crosswrapper /opt/dependencies/build/pkg-config-crosswrapper.patch; \
	ln -sr /usr/share/pkg-config-crosswrapper /usr/bin/$prefixMinGw-pkg-config; \
	ln -sr /usr/share/pkg-config-crosswrapper /usr/bin/$prefixRpi-pkg-config

# zlib
WORKDIR zlib

RUN git clone https://github.com/DSP-Crowd/zlib.git zlib-${versLibz}
WORKDIR zlib-${versLibz}
RUN git checkout ${versLibz}

WORKDIR ../build

WORKDIR native
RUN ../../zlib-${versLibz}/configure --static; \
	make -j; make install
WORKDIR ..

WORKDIR win64
RUN CC=$prefixMinGw-gcc AR=$prefixMinGw-ar RANLIB=$prefixMinGw-ranlib \
	../../zlib-${versLibz}/configure --static --prefix=$prefixMinGwDir; \
	make -j; make install
WORKDIR ..

WORKDIR rpi
RUN CC=$prefixRpi-gcc AR=$prefixRpi-ar RANLIB=$prefixRpi-ranlib \
	../../zlib-${versLibz}/configure --static --prefix=$prefixRpiDir; \
	make -j; make install
WORKDIR ..

WORKDIR ../..

# openssl
WORKDIR openssl

RUN git clone https://github.com/DSP-Crowd/openssl.git OpenSSL_${versLibssl}
WORKDIR OpenSSL_${versLibssl}
RUN git checkout OpenSSL_${versLibssl}

WORKDIR ../build

WORKDIR native
RUN ../../OpenSSL_${versLibssl}/Configure linux-x86_64 no-ssl2 no-shared --prefix=$prefixNativeDir; \
	make -j; make install_sw
WORKDIR ..

WORKDIR win64
RUN ../../OpenSSL_${versLibssl}/Configure mingw64 no-ssl2 no-shared --prefix=$prefixMinGwDir --cross-compile-prefix=$prefixMinGw-; \
	make -j; make install_sw
WORKDIR ..

WORKDIR rpi
RUN ../../OpenSSL_${versLibssl}/Configure linux-generic32 no-ssl2 no-shared --prefix=$prefixRpiDir --cross-compile-prefix=$prefixRpi-; \
	make -j; make install_sw
WORKDIR ..

WORKDIR ../..

# curl
WORKDIR curl

RUN git clone https://github.com/DSP-Crowd/curl.git curl-${versLibcurl}
WORKDIR curl-${versLibcurl}
RUN git checkout curl-$(echo "$versLibcurl" | sed "s:\.:_:g"); \
	autoreconf -fi

WORKDIR ../build

ENV cross="$prefixNative" \
	crossDir="$prefixNativeDir"

WORKDIR native
RUN \
	CC="$cross-gcc" \
	AS="$cross-as" \
	LD="$cross-ld" \
	NM="$cross-nm" \
	AR="$cross-ar" \
	RANLIB="$cross-ranlib" \
	LDFLAGS="-static" \
	CFLAGS="-static-libgcc" \
	CXXFLAGS="-static-libstdc++" \
	PKG_CONFIG="$cross-pkg-config --static" \
	../../curl-${versLibcurl}/configure \
	--enable-static \
	--enable-http-auth \
	--disable-shared \
	--without-brotli \
	--build=x86_64-linux-gnu \
	--host="$cross" \
	--prefix="$crossDir"; \
	make -j; make install
WORKDIR ..

ENV cross="$prefixMinGw" \
	crossDir="$prefixMinGwDir"

WORKDIR win64
RUN \
	CC="$cross-gcc" \
	AS="$cross-as" \
	LD="$cross-ld" \
	NM="$cross-nm" \
	AR="$cross-ar" \
	RANLIB="$cross-ranlib" \
	LDFLAGS="-static" \
	CFLAGS="-static-libgcc" \
	CXXFLAGS="-static-libstdc++" \
	PKG_CONFIG="$cross-pkg-config --static" \
	LIBS="-lcrypt32" \
	../../curl-${versLibcurl}/configure \
	--enable-static \
	--enable-http-auth \
	--disable-shared \
	--without-brotli \
	--build=x86_64-linux-gnu \
	--host="$cross" \
	--prefix="$crossDir" \
	--with-schannel \
	--with-default-ssl-backend=schannel; \
	make -j; make install
WORKDIR ..

ENV cross="$prefixRpi" \
	crossDir="$prefixRpiDir"

WORKDIR rpi
RUN \
	CC="$cross-gcc" \
	AS="$cross-as" \
	LD="$cross-ld" \
	NM="$cross-nm" \
	AR="$cross-ar" \
	RANLIB="$cross-ranlib" \
	LDFLAGS="-static" \
	CFLAGS="-static-libgcc" \
	CXXFLAGS="-static-libstdc++" \
	PKG_CONFIG="$cross-pkg-config --static" \
	../../curl-${versLibcurl}/configure \
	--enable-static \
	--enable-http-auth \
	--disable-shared \
	--without-brotli \
	--build=x86_64-linux-gnu \
	--host="$cross" \
	--prefix="$crossDir"; \
	make -j; make install
WORKDIR ..

WORKDIR ../..

# tclap
RUN cp -r /usr/include/tclap/ $prefixMinGwDir/include/.; \
	cp /usr/lib/x86_64-linux-gnu/pkgconfig/tclap.pc $prefixMinGwDir/lib/pkgconfig/.; \
	sed -i "s:usr:usr/$prefixMinGw:g" $prefixMinGwDir/lib/pkgconfig/tclap.pc

RUN cp -r /usr/include/tclap/ $prefixRpiDir/include/.; \
	cp /usr/lib/x86_64-linux-gnu/pkgconfig/tclap.pc $prefixRpiDir/lib/pkgconfig/.; \
	sed -i "s:usr:usr/$prefixRpi:g" $prefixRpiDir/lib/pkgconfig/tclap.pc

# jsoncpp
WORKDIR jsoncpp

RUN git clone https://github.com/DSP-Crowd/jsoncpp.git jsoncpp-${versLibjsoncpp}
WORKDIR jsoncpp-${versLibjsoncpp}
RUN git checkout ${versLibjsoncpp}

RUN meson ../build/native --default-library static --libdir=lib --includedir=include/jsoncpp --prefix=$prefixNativeDir; \
	meson ../build/win64 --default-library static --libdir=lib --includedir=include/jsoncpp --prefix=$prefixMinGwDir --cross-file /opt/dependencies/build/crosscompile-win64.txt; \
	meson ../build/rpi --default-library static --libdir=lib --includedir=include/jsoncpp --prefix=$prefixRpiDir --cross-file /opt/dependencies/build/crosscompile-rpi.txt

WORKDIR ../build

WORKDIR native
RUN ninja; ninja install
WORKDIR ..

WORKDIR win64
RUN ninja; ninja install
WORKDIR ..

WORKDIR rpi
RUN ninja; ninja install
WORKDIR ..

WORKDIR ../..

# glfw
WORKDIR glfw

RUN git clone https://github.com/DSP-Crowd/glfw glfw-${versLibGlfw}
WORKDIR glfw-${versLibGlfw}
RUN git checkout ${versLibGlfw}

WORKDIR ../build

WORKDIR native
RUN cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX:PATH=$prefixNativeDir -DGLFW_USE_WAYLAND=ON ../../glfw-${versLibGlfw}/.; \
	make; \
	make install
WORKDIR ..

WORKDIR win64
RUN cmake -DCMAKE_INSTALL_PREFIX:PATH=$prefixMinGwDir -DCMAKE_TOOLCHAIN_FILE=CMake/x86_64-w64-mingw32.cmake ../../glfw-${versLibGlfw}/.; \
	make; \
	make install
WORKDIR ..

#WORKDIR rpi
#RUN make; make install
#WORKDIR ..

WORKDIR ../..

# glm
WORKDIR glm

RUN git clone https://github.com/DSP-Crowd/glm glm-${versLibGlm}
WORKDIR glm-${versLibGlm}
RUN git checkout ${versLibGlm}

RUN cp -r glm $prefixNativeDir/include/.; \
	cp -r glm $prefixMinGwDir/include/.; \
	cp -r glm $prefixRpiDir/include/.

WORKDIR ../..

# glew
WORKDIR glew

RUN git clone https://github.com/DSP-Crowd/glew.git glew-${versLibGlew}_native; \
	git clone https://github.com/DSP-Crowd/glew.git glew-${versLibGlew}_win64

ENV cross="$prefixNative" \
	crossDir="$prefixNativeDir"

WORKDIR glew-${versLibGlew}_native/auto
RUN git checkout glew-${versLibGlew}; \
	make
WORKDIR ..
RUN GLEW_PREFIX="$crossDir" GLEW_DEST="$crossDir" GLEW_NO_GLU="-DGLEW_NO_GLU" SYSTEM=linux make install
WORKDIR ..

ENV cross="$prefixMinGw" \
	crossDir="$prefixMinGwDir"

WORKDIR glew-${versLibGlew}_win64/auto
RUN git checkout glew-${versLibGlew}; \
	make
WORKDIR ..
RUN GLEW_PREFIX="$crossDir" GLEW_DEST="$crossDir" GLEW_NO_GLU="-DGLEW_NO_GLU" SYSTEM=linux-mingw64 make install
WORKDIR ..

WORKDIR ..

# freetype
WORKDIR freetype

RUN git clone https://github.com/DSP-Crowd/freetype.git freetype-${versLibFreetype}

WORKDIR freetype-${versLibFreetype}
RUN git checkout VER-${versLibFreetype}; \
	./autogen.sh

WORKDIR ../build

ENV cross="$prefixNative" \
	crossDir="$prefixNativeDir"

WORKDIR native
RUN \
	PKG_CONFIG="$cross-pkg-config --static" \
	../../freetype-${versLibFreetype}/configure \
	--enable-static \
	--disable-shared \
	--build=x86_64-linux-gnu \
	--host="$cross" \
	--prefix="$crossDir" \
	--without-brotli \
	--without-png; \
	make -j; make install
WORKDIR ..

ENV cross="$prefixMinGw" \
	crossDir="$prefixMinGwDir"

WORKDIR win64
RUN \
	PKG_CONFIG="$cross-pkg-config --static" \
	../../freetype-${versLibFreetype}/configure \
	--enable-static \
	--disable-shared \
	--build=x86_64-linux-gnu \
	--host="$cross" \
	--prefix="$crossDir" \
	--without-brotli \
	--without-png; \
	make -j; make install
WORKDIR ..

WORKDIR ../..
