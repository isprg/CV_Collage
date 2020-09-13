FROM debian

ENV OPENCV_VERSION=4.4.0
ENV DEBIAN_FRONTEND=noninteractive
ENV OPENCV_CONTRIB='YES' 
#https://github.com/opencv/opencv/archive/3.4.7.zip

RUN apt update && apt upgrade -y

RUN apt install sudo emacs pkg-config wget sed git -y --fix-missing
RUN apt install -y unzip \
    		   build-essential \
                   libatlas-base-dev \
                   libavcodec-dev \
                   libavformat-dev \
		   qt5-default \
                   libgtk2.0-dev \
                   libswscale-dev \
		   libdc1394-22-dev \
                   libjpeg-dev \
                   libpng-dev \
                   libtbb2 \
                   libtbb-dev \
                   libtiff-dev \
                   libv4l-dev \
                   libvtk6-dev \
		   gcc \
		   g++ \
		   make \
		   clang \
		   qt5-qmake \
		   zlib1g-dev \
		   libcurl4-gnutls-dev \
		   python-dev \
		   python-tk \
		   pylint \
		   python-numpy  \
                   python3-dev \
		   python3-tk \
		   pylint3 \
		   python3-numpy \
		   flake8 \
		   cmake
		   
#RUN cd /tmp && git clone https://github.com/Kitware/CMake.git && cd CMake && ./configure --system-curl --system-zlib && make && make install

RUN apt install -y zlib1g-dev libjpeg-dev libwebp-dev libpng-dev libtiff5-dev libopenexr-dev libgdal-dev
RUN apt install -y libdc1394-22-dev libavcodec-dev libavformat-dev libswscale-dev \
                        libtheora-dev libvorbis-dev libxvidcore-dev libx264-dev yasm \
                        libopencore-amrnb-dev libopencore-amrwb-dev libv4l-dev libxine2-dev
RUN apt install -y libtbb-dev libeigen3-dev
RUN wget https://github.com/opencv/opencv/archive/${OPENCV_VERSION}.zip && unzip ${OPENCV_VERSION}.zip && rm ${OPENCV_VERSION}.zip && mv opencv-${OPENCV_VERSION} /OpenCV
RUN wget https://github.com/opencv/opencv_contrib/archive/${OPENCV_VERSION}.zip &&  unzip ${OPENCV_VERSION}.zip && rm ${OPENCV_VERSION}.zip
RUN ls -al *
RUN mv opencv_contrib-${OPENCV_VERSION} opencv_contrib &&  mv opencv_contrib /OpenCV
RUN ls -al *
RUN cd /OpenCV && mkdir build && cd build && cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local -DWITH_QT=ON -DWITH_OPENGL=ON -DFORCE_VTK=ON -DWITH_TBB=ON -DWITH_GDAL=ON -DWITH_XINE=ON -DENABLE_PRECOMPILED_HEADERS=OFF -D OPENCV_GENERATE_PKGCONFIG=YES -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules .. && make -j4 && make install && sh -c 'echo "/usr/local/lib" >> /etc/ld.so.conf.d/opencv.conf' && ldconfig
RUN sed -i "s/includedir_old\=\${prefix}\/include\/opencv4\/opencv/includedir_old\=\${prefix}\/include\/opencv4\/opencv2/" /OpenCV/build/unix-install/opencv4.pc
RUN cp /OpenCV/build/unix-install/opencv4.pc /usr/local/lib/pkgconfig/


#RUN wget https://raw.githubusercontent.com/milq/milq/master/scripts/bash/install-opencv.sh
#RUN sed -i -e '8,9d' ./install-opencv.sh
#RUN sed -i -e '56,56d' ./install-opencv.sh
#RUN sed -i "91i-D CMAKE_BUILD_TYPE=RELEASE \-D CMAKE_INSTALL_PREFIX=/usr/local \-D INSTALL_C_EXAMPLES=ON \-D OPENCV_ENABLE_NONFREE=ON \-D BUILD_EXAMPLES=ON \-D WITH_OPENMP=ON \       -D WITH_IPP=ON \       -D WITH_CSTRIPES=ON \-D OPENCV_GENERATE_PKGCONFIG=ON" ./install-opencv.sh
#sed -i "s/sudo//" ./install-opencv.sh
#RUN chmod +x install-opencv.sh
#RUN cat install-opencv.sh
#RUN ./install-opencv.sh


RUN groupadd --gid 1000 developer && \
    useradd  --uid 1000 --gid 1000 --groups sudo --create-home --shell /bin/bash developer && \
    echo 'developer:P@ssw0rd' | chpasswd
    
RUN git clone https://github.com/spmallick/learnopencv.git /home/developer/learnopencv
RUN echo 'PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig \nexport PKG_CONFIG_PATH' >> /home/developer/.bashrc
RUN ln -s /mnt/docker /home/developer/docker
USER developer
WORKDIR /home/developer
CMD /bin/bash